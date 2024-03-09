#include "dapch.h"
#include "vulkan_graphics_api.h"
#include "logger.h"
#include "core/containers.h"

#include <set>
#include <algorithm>
#include <vector>
#include <fstream>
#include "core/maths/maths.h"

#if defined(DA_GRAPHICS_VULKAN) && defined(DA_WINDOW_GLFW)
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
#include <unordered_map>
#include <backends/imgui_impl_vulkan.h>

#include "vulkan_graphics_texture2d.h"

#include "core/graphics/graphics_smesh.h"
#include "vulkan_graphics_pipeline.h"

namespace da::platform {

#if DA_DEBUG || DA_RELEASE
#define VK_CHECK(x, y) ASSERT(x == y)
#endif

#if DA_FINAL 
#define VK_CHECK(x, y) x
#endif


#ifdef NDEBUG
	const bool enableValidationLayers = true;
#else
	const bool enableValidationLayers = true;
#endif

	void* vkAllocate(void* pUserData,
		size_t                                      size,
		size_t                                      alignment,
		VkSystemAllocationScope                     allocationScope)
	{
		return malloc(size);
	}

	void* vkRealloc(void* pUserData,
		void* pOriginal,
		size_t                                      size,
		size_t                                      alignment,
		VkSystemAllocationScope                     allocationScope)
	{
		return realloc(pOriginal, size);
	}

	void vkFree(void* pUserData,
		void* pMemory)
	{
		free(pMemory);
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) {

		if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
			CLogger::LogError(ELogChannel::Graphics, "Vulkan Validation Layer: %s", pCallbackData->pMessage);
		}

		
		return VK_FALSE;
	}

	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, debugMessenger, pAllocator);
		}
	}

	CVulkanGraphicsApi::CVulkanGraphicsApi(core::CWindow* windowModule) : graphics::CGraphicsApi(windowModule)
	{
		m_allocCallbacks = {
	   nullptr,
	   vkAllocate,
	   vkRealloc,
	   vkFree
		};
		createInstance();
		setupDebugCallback();
	}


	CVulkanGraphicsApi::~CVulkanGraphicsApi()
	{

	}

	void CVulkanGraphicsApi::initialize()
	{
		createSurface();
		selectPhysicalDevice();
		createLogicalDevice();
		createSwapChain();
		createImageViews();
		createRenderPass();
		prepareOffscreenFramebuffer();
		createCommandPool();
		createColorResources();
		createDepthResources();
		createFramebuffers();

		createCommandBuffers();
		createSyncObjects();
	}

	void CVulkanGraphicsApi::update()
	{
		bool framebufferResized = false;
		vkWaitForFences(m_device, 1, &m_inFlightFences[m_currentFrame], VK_TRUE, UINT64_MAX);

        
		
		VkResult result = vkAcquireNextImageKHR(m_device, m_swapChain, UINT64_MAX, m_imageAvailableSemaphores[m_currentFrame], VK_NULL_HANDLE, &m_imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
			framebufferResized = false;
			recreateSwapChain();
			return;
		}

		ASSERT(result == VK_SUCCESS);

		for (size_t i = 0; i < m_pipelines.size(); i++) {
			m_pipelines[i]->update(m_currentFrame);
		}
		vkResetFences(m_device, 1, &m_inFlightFences[m_currentFrame]);

		// Shadow Renderpass
		{
			beginRecordingShadowCommandBuffer(m_commandBuffers[m_currentFrame], m_shadowPass.frameBuffer, m_shadowPass.renderPass);

			stopRecordingCommandBuffer(m_commandBuffers[m_currentFrame]);
		}

		vkResetCommandBuffer(m_commandBuffers[m_currentFrame], 0);
		// Geometry RenderPass
		{
			beginRecordingCommandBuffer(m_commandBuffers[m_currentFrame], m_swapChainFramebuffers[m_imageIndex], m_renderPass);
			for (size_t i = 0; i < m_pipelines.size(); i++) {
				m_pipelines[i]->render(m_commandBuffers[m_currentFrame], m_currentFrame);
			}
			for (size_t i = 0; i < m_renderFunctions.size(); i++) {
				m_renderFunctions[i]->operator()(m_commandBuffers[m_currentFrame]);
			}
			stopRecordingCommandBuffer(m_commandBuffers[m_currentFrame]);
		}
		
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { m_imageAvailableSemaphores[m_currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_commandBuffers[m_currentFrame];

		VkSemaphore signalSemaphores[] = { m_renderFinishedSemaphores[m_currentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

        
        result = vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, m_inFlightFences[m_currentFrame]);
        
		VK_CHECK(result, VK_SUCCESS);

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { m_swapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &m_imageIndex;
		presentInfo.pResults = nullptr; // Optional
		result = vkQueuePresentKHR(m_presentQueue, &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
            framebufferResized) {
            recreateSwapChain();
        }
    
		m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	void CVulkanGraphicsApi::cleanupSwapChain()
	{
		vkDestroyImageView(m_device, m_depthImageView, &m_allocCallbacks);
		vkDestroyImage(m_device, m_depthImage, &m_allocCallbacks);
		vkFreeMemory(m_device, m_depthImageMemory, &m_allocCallbacks);

		vkDestroyImageView(m_device, m_colorImageView, &m_allocCallbacks);
		vkDestroyImage(m_device, m_colorImage, &m_allocCallbacks);
		vkFreeMemory(m_device, m_colorImageMemory, &m_allocCallbacks);

		for (size_t i = 0; i < m_swapChainFramebuffers.size(); i++) {
			vkDestroyFramebuffer(m_device, m_swapChainFramebuffers[i], &m_allocCallbacks);
		}

		for (size_t i = 0; i < m_pipelines.size(); i++) {
			m_pipelines[i]->destroy();
		}

		vkDestroySampler(m_device, m_shadowPass.depthSampler, nullptr);

		// Depth attachment
		vkDestroyImageView(m_device, m_shadowPass.depth.view, nullptr);
		vkDestroyImage(m_device, m_shadowPass.depth.image, nullptr);
		vkFreeMemory(m_device, m_shadowPass.depth.mem, nullptr);
		vkDestroyFramebuffer(m_device, m_shadowPass.frameBuffer, nullptr);
		vkDestroyRenderPass(m_device, m_shadowPass.renderPass, nullptr);

		vkDestroyRenderPass(m_device, m_renderPass, &m_allocCallbacks);
		

		for (size_t i = 0; i < m_swapChainImageViews.size(); i++) {
			vkDestroyImageView(m_device, m_swapChainImageViews[i], &m_allocCallbacks);
		}

		vkDestroySwapchainKHR(m_device, m_swapChain, &m_allocCallbacks);
	}

	void CVulkanGraphicsApi::shutdown()
	{
		vkDeviceWaitIdle(m_device);

		cleanupSwapChain();



		for (size_t i = 0; i < m_pipelines.size(); i++)
		{
			m_pipelines[i]->clean();
		}

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroySemaphore(m_device, m_renderFinishedSemaphores[i], &m_allocCallbacks);
			vkDestroySemaphore(m_device, m_imageAvailableSemaphores[i], &m_allocCallbacks);
			vkDestroyFence(m_device, m_inFlightFences[i], &m_allocCallbacks);
		}

		vkDestroyCommandPool(m_device, m_commandPool, &m_allocCallbacks);
		vkDestroyDevice(m_device, &m_allocCallbacks);
	
		if (enableValidationLayers) {
			DestroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger, &m_allocCallbacks);
		}
		
		LOG_INFO(ELogChannel::Graphics, "Destroying Surface");
		vkDestroySurfaceKHR(m_instance, m_surface, &m_allocCallbacks);
		LOG_INFO(ELogChannel::Graphics, "Destroying Instance");
		vkDestroyInstance(m_instance, &m_allocCallbacks);
	}

	void CVulkanGraphicsApi::createInstance()
	{
        m_validationLayers.push_back("VK_LAYER_KHRONOS_validation");
		if (!enableValidationLayers) {
			VK_CHECK(checkValidationLayerSupport(m_validationLayers), true);
		}
		
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		// TODO: Change app/engine name
		appInfo.pApplicationName = "ProtoType";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "DAEngine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_3;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
#if DA_PLATFORM_MACOSX
        createInfo.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR; // MoltenVK
#endif


		auto extensions = getRequiredExtensions();
#if DA_PLATFORM_MACOSX
		extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        extensions.push_back("VK_KHR_get_physical_device_properties2"); // MoltenVK
#endif
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();
        

		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(m_validationLayers.size());
			createInfo.ppEnabledLayerNames = m_validationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}

		LOG_INFO(ELogChannel::Graphics, "Create Instance");
		VkResult result = vkCreateInstance(&createInfo, &m_allocCallbacks, &m_instance);
		ASSERT(result == VK_SUCCESS);
	}

	bool CVulkanGraphicsApi::checkValidationLayerSupport(const std::vector<const char*>& validationLayers) {

		const uint32_t WIDTH = 800;
		const uint32_t HEIGHT = 600;

		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : validationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				return false;
			}
		}

		return true;
	}

	std::vector<const char*> CVulkanGraphicsApi::getRequiredExtensions()
	{

		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions;
		for (uint32_t i = 0; i < glfwExtensionCount; i++)
		{
			extensions.push_back(glfwExtensions[i]);
		}

		if (enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	void CVulkanGraphicsApi::setupDebugCallback()
	{
		if (!enableValidationLayers) return;

		VkDebugUtilsMessengerCreateInfoEXT createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
		createInfo.pUserData = nullptr;

        auto result = CreateDebugUtilsMessengerEXT(m_instance, &createInfo, &m_allocCallbacks, &m_debugMessenger);
        
		VK_CHECK(result, VK_SUCCESS);
	}

	bool checkDeviceExtensionSupport(VkPhysicalDevice device) {

		std::vector<const char*> deviceExtensions;
		deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}



	VkPhysicalDevice CVulkanGraphicsApi::findDevices(const std::vector<VkPhysicalDevice>& devices)
	{
		int score = -3;
		VkPhysicalDevice result = VK_NULL_HANDLE;
		VkPhysicalDeviceProperties rs_deviceProperties;

		for (const VkPhysicalDevice& device : devices) {
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(device, &deviceProperties);

			VkPhysicalDeviceFeatures deviceFeatures;
			vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

			int s = deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ? 1000 : 0;
			s += deviceProperties.limits.maxImageDimension2D;

			bool extensionsSupported = checkDeviceExtensionSupport(device);

			bool swapChainAdequate = false;
			if (extensionsSupported)
			{
				SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device, m_surface);
				swapChainAdequate = swapChainSupport.formats.size() > 0 && swapChainSupport.presentModes.size() > 0;
			}

			VkPhysicalDeviceFeatures supportedFeatures;
			vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

			if (!deviceFeatures.geometryShader || !deviceFeatures.fillModeNonSolid || !findQueueFamilies(device, std::nullopt).graphicsFamily.has_value() || !checkDeviceExtensionSupport(device) || !swapChainAdequate || !supportedFeatures.samplerAnisotropy) s = -2;

			if (s > score) {
				score = s;
				result = device;
				rs_deviceProperties = deviceProperties;
			}

			LOG_INFO(ELogChannel::Graphics, "Found Device: %s Score: %d", deviceProperties.deviceName, s);
		}

		LOG_INFO(ELogChannel::Graphics, "Selected Device: %s", rs_deviceProperties.deviceName);

		ASSERT(result != VK_NULL_HANDLE);

		return result;
	}

	void CVulkanGraphicsApi::selectPhysicalDevice()
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
		ASSERT(deviceCount);
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

		m_physicalDevice = findDevices(devices);
		m_msaaSamples = getMaxUsableSampleCount();

		ASSERT(m_physicalDevice != VK_NULL_HANDLE);
	}

	void CVulkanGraphicsApi::createLogicalDevice()
	{
		QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice, m_surface);

		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
		queueCreateInfo.queueCount = 1;
		float queuePriority = 1.0f;
		queueCreateInfo.pQueuePriorities = &queuePriority;

		VkPhysicalDeviceFeatures deviceFeatures{};
		deviceFeatures.samplerAnisotropy = VK_TRUE;
		//deviceFeatures.fillModeNonSolid = VK_TRUE;
		deviceFeatures.sampleRateShading = VK_TRUE;

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		createInfo.pQueueCreateInfos = &queueCreateInfo;
		createInfo.queueCreateInfoCount = 1;

		createInfo.pEnabledFeatures = &deviceFeatures;

		createInfo.enabledExtensionCount = 0;

		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(m_validationLayers.size());
			createInfo.ppEnabledLayerNames = m_validationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		for (uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();

		std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
#ifdef DA_PLATFORM_MACOSX
			,"VK_KHR_portability_subset" // Molten
#endif
		};

		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        auto result = vkCreateDevice(m_physicalDevice, &createInfo, &m_allocCallbacks, &m_device);
        
		VK_CHECK(result, VK_SUCCESS);

		vkGetDeviceQueue(m_device, indices.graphicsFamily.value(), 0, &m_graphicsQueue);
		vkGetDeviceQueue(m_device, indices.presentFamily.value(), 0, &m_presentQueue);

	}

	void CVulkanGraphicsApi::createSurface()
	{
		LOG_INFO(ELogChannel::Graphics, "Vulkan creating GLFW Surface");
        auto result =glfwCreateWindowSurface(m_instance, (GLFWwindow*)m_nativeWindow->getNativeWindow(), &m_allocCallbacks, &m_surface);
		VK_CHECK(result, VK_SUCCESS);
        
	}

	VkExtent2D CVulkanGraphicsApi::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
			return capabilities.currentExtent;
		}
		else {
			int width, height;
			glfwGetFramebufferSize((GLFWwindow*)m_nativeWindow->getNativeWindow(), &width, &height);

			VkExtent2D actualExtent = {
				static_cast<uint32_t>(width),
				static_cast<uint32_t>(height)
			};

			actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

			return actualExtent;
		}
	}

	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return availablePresentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}

		return availableFormats[0];
	}

	SwapChainSupportDetails CVulkanGraphicsApi::querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
		SwapChainSupportDetails details;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);


		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

		if (formatCount != 0) {
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	void CVulkanGraphicsApi::createSwapChain()
	{
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(m_physicalDevice, m_surface);

		VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);


		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = m_surface;

		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice, m_surface);
		m_queueFamilyIndices[0] = indices.graphicsFamily.value();
		m_queueFamilyIndices[1] = indices.presentFamily.value();

		if (indices.graphicsFamily != indices.presentFamily) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = m_queueFamilyIndices;
		}
		else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0; // Optional
			createInfo.pQueueFamilyIndices = nullptr; // Optional
		}

		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;

		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

        auto res =vkCreateSwapchainKHR(m_device, &createInfo, &m_allocCallbacks, &m_swapChain);
		VK_CHECK(res, VK_SUCCESS);

		vkGetSwapchainImagesKHR(m_device, m_swapChain, &imageCount, nullptr);
		m_swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(m_device, m_swapChain, &imageCount, m_swapChainImages.data());

		m_swapChainImageFormat = surfaceFormat.format;
		m_swapChainExtent = extent;
	}

	void CVulkanGraphicsApi::createImageViews()
	{
		m_swapChainImageViews.resize(m_swapChainImages.size());

		for (uint32_t i = 0; i < m_swapChainImages.size(); i++) {
			m_swapChainImageViews[i] = createImageView(m_swapChainImages[i],VK_IMAGE_VIEW_TYPE_2D ,1, m_swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
		}
	}

	void CVulkanGraphicsApi::createRenderPass()
	{
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = m_swapChainImageFormat;
		colorAttachment.samples = m_msaaSamples;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		
		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentDescription depthAttachment{};
		depthAttachment.format = findDepthFormat();
		depthAttachment.samples = m_msaaSamples;
		depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depthAttachmentRef{};
		depthAttachmentRef.attachment = 1;
		depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentReference colorAttachmentResolveRef{};
		colorAttachmentResolveRef.attachment = 2;
		colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;
		subpass.pDepthStencilAttachment = &depthAttachmentRef;
		subpass.pResolveAttachments = &colorAttachmentResolveRef;

		VkAttachmentDescription colorAttachmentResolve{};
		colorAttachmentResolve.format = m_swapChainImageFormat;
		colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		
		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;

		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;

		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		std::vector<VkAttachmentDescription> attachments = { colorAttachment, depthAttachment, colorAttachmentResolve };
		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

        auto result = vkCreateRenderPass(m_device, &renderPassInfo, &m_allocCallbacks, &m_renderPass);

		VK_CHECK(result, VK_SUCCESS);
	}


	#define DEPTH_FORMAT VK_FORMAT_D16_UNORM

	void CVulkanGraphicsApi::prepareOffscreenFramebuffer()
	{
		m_shadowPass.width = 2048;
		m_shadowPass.height = 2048;

		// For shadow mapping we only need a depth attachment
		VkImageCreateInfo image = {};
		image.imageType = VK_IMAGE_TYPE_2D;
		image.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		image.extent.width = m_shadowPass.width;
		image.extent.height = m_shadowPass.height;
		image.extent.depth = 1;
		image.mipLevels = 1;
		image.arrayLayers = 1;
		image.samples = VK_SAMPLE_COUNT_1_BIT;
		image.tiling = VK_IMAGE_TILING_OPTIMAL;
		image.format = DEPTH_FORMAT;																// Depth stencil attachment
		image.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;		// We will sample directly from the depth attachment for the shadow mapping
		auto result = vkCreateImage(m_device, &image, nullptr, &m_shadowPass.depth.image);

		VK_CHECK(result, VK_SUCCESS);

		VkMemoryAllocateInfo memAlloc = {};
		memAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		VkMemoryRequirements memReqs = {};
		vkGetImageMemoryRequirements(m_device, m_shadowPass.depth.image, &memReqs);
		memAlloc.allocationSize = memReqs.size;
		memAlloc.memoryTypeIndex = findMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		result = vkAllocateMemory(m_device, &memAlloc, &getAllocCallbacks(), &m_shadowPass.depth.mem);

		VK_CHECK(result, VK_SUCCESS);

		result = vkBindImageMemory(m_device, m_shadowPass.depth.image, m_shadowPass.depth.mem, 0), VK_SUCCESS;
		VK_CHECK(result, VK_SUCCESS);

		VkImageViewCreateInfo depthStencilView = {};
		depthStencilView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		depthStencilView.viewType = VK_IMAGE_VIEW_TYPE_2D;
		depthStencilView.format = DEPTH_FORMAT;
		depthStencilView.subresourceRange = {};
		depthStencilView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		depthStencilView.subresourceRange.baseMipLevel = 0;
		depthStencilView.subresourceRange.levelCount = 1;
		depthStencilView.subresourceRange.baseArrayLayer = 0;
		depthStencilView.subresourceRange.layerCount = 1;
		depthStencilView.image = m_shadowPass.depth.image;
		result = vkCreateImageView(m_device, &depthStencilView, nullptr, &m_shadowPass.depth.view);

		VK_CHECK(result, VK_SUCCESS);

		// Create sampler to sample from to depth attachment
		// Used to sample in the fragment shader for shadowed rendering
		VkFilter shadowmap_filter = VK_FILTER_LINEAR;
		VkSamplerCreateInfo sampler = {};
		sampler.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		sampler.magFilter = shadowmap_filter;
		sampler.minFilter = shadowmap_filter;
		sampler.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		sampler.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		sampler.addressModeV = sampler.addressModeU;
		sampler.addressModeW = sampler.addressModeU;
		sampler.mipLodBias = 0.0f;
		sampler.maxAnisotropy = 1.0f;
		sampler.minLod = 0.0f;
		sampler.maxLod = 1.0f;
		sampler.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
		result = vkCreateSampler(m_device, &sampler, nullptr, &m_shadowPass.depthSampler);

		VK_CHECK(result, VK_SUCCESS);

		prepareOffscreenRenderpass();

		// Create frame buffer
		VkFramebufferCreateInfo fbufCreateInfo = {};
		fbufCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		fbufCreateInfo.renderPass = m_shadowPass.renderPass;
		fbufCreateInfo.attachmentCount = 1;
		fbufCreateInfo.pAttachments = &m_shadowPass.depth.view;
		fbufCreateInfo.width = m_shadowPass.width;
		fbufCreateInfo.height = m_shadowPass.height;
		fbufCreateInfo.layers = 1;

		result = vkCreateFramebuffer(m_device, &fbufCreateInfo, nullptr, &m_shadowPass.frameBuffer);

		VK_CHECK(result, VK_SUCCESS);
	}

	void CVulkanGraphicsApi::prepareOffscreenRenderpass()
	{
		VkAttachmentDescription attachmentDescription{};
		attachmentDescription.format = DEPTH_FORMAT;
		attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
		attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;							// Clear depth at beginning of the render pass
		attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;						// We will read from depth, so it's important to store the depth attachment results
		attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;					// We don't care about initial layout of the attachment
		attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;// Attachment will be transitioned to shader read at render pass end

		VkAttachmentReference depthReference = {};
		depthReference.attachment = 0;
		depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;			// Attachment will be used as depth/stencil during render pass

		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 0;													// No color attachments
		subpass.pDepthStencilAttachment = &depthReference;									// Reference to our depth attachment

		// Use subpass dependencies for layout transitions
		std::vector<VkSubpassDependency> dependencies(2);

		dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
		dependencies[0].dstSubpass = 0;
		dependencies[0].srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		dependencies[0].dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependencies[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
		dependencies[0].dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

		dependencies[1].srcSubpass = 0;
		dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
		dependencies[1].srcStageMask = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		dependencies[1].srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		dependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

		VkRenderPassCreateInfo renderPassCreateInfo = {};
		renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassCreateInfo.attachmentCount = 1;
		renderPassCreateInfo.pAttachments = &attachmentDescription;
		renderPassCreateInfo.subpassCount = 1;
		renderPassCreateInfo.pSubpasses = &subpass;
		renderPassCreateInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
		renderPassCreateInfo.pDependencies = dependencies.data();

		auto result = vkCreateRenderPass(m_device, &renderPassCreateInfo, nullptr, &m_shadowPass.renderPass);

		VK_CHECK(result, VK_SUCCESS);
	}

	void CVulkanGraphicsApi::createFramebuffers()
	{
		m_swapChainFramebuffers.resize(m_swapChainImageViews.size());

		for (size_t i = 0; i < m_swapChainImageViews.size(); i++) {
			std::vector<VkImageView> attachments = {
				m_colorImageView,
				m_depthImageView,
				m_swapChainImageViews[i],
			};

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = m_renderPass;
			framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			framebufferInfo.pAttachments = attachments.data();
			framebufferInfo.width = m_swapChainExtent.width;
			framebufferInfo.height = m_swapChainExtent.height;
			framebufferInfo.layers = 1;

            auto result = vkCreateFramebuffer(m_device, &framebufferInfo, &m_allocCallbacks, &m_swapChainFramebuffers[i]);
            
			VK_CHECK(result, VK_SUCCESS);

		}
	}

	void CVulkanGraphicsApi::createCommandPool()
	{
		QueueFamilyIndices queueFamilyIndices = findQueueFamilies(m_physicalDevice, m_surface);

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

        auto result = vkCreateCommandPool(m_device, &poolInfo, &m_allocCallbacks, &m_commandPool);
        
		VK_CHECK(result, VK_SUCCESS);
	}

    void CVulkanGraphicsApi::stopRecordingCommandBuffer(VkCommandBuffer commandBuffer)
    {
        vkCmdEndRenderPass(commandBuffer);
        auto result = vkEndCommandBuffer(commandBuffer);
        VK_CHECK(result, VK_SUCCESS);
    }

	void CVulkanGraphicsApi::beginRecordingShadowCommandBuffer(VkCommandBuffer commandBuffer, VkFramebuffer frameBuffer, VkRenderPass renderPass)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

		auto res = vkBeginCommandBuffer(commandBuffer, &beginInfo);

		VK_CHECK(res, VK_SUCCESS);

		VkRenderPassBeginInfo renderPassBeginInfo = {};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.renderPass = m_shadowPass.renderPass;
		renderPassBeginInfo.framebuffer = m_shadowPass.frameBuffer;
		renderPassBeginInfo.renderArea.extent.width = m_shadowPass.width;
		renderPassBeginInfo.renderArea.extent.height = m_shadowPass.height;
		renderPassBeginInfo.clearValueCount = 1;
	
		std::vector<VkClearValue> clearValues(2);
		clearValues[0].depthStencil = { 1.0f, 0 };

		renderPassBeginInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport = {};
		viewport.height = (float)m_shadowPass.height;
		viewport.width = (float)m_shadowPass.width;
		viewport.x = 0.f;
		viewport.y = 1.f;
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

		VkRect2D scissor = {};
		scissor.extent.width = m_shadowPass.width;
		scissor.extent.height = m_shadowPass.height;
		scissor.offset.x = 0;
		scissor.offset.y = 0;

		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);


		vkCmdSetDepthBias(
			commandBuffer,
			1.25f,
			0.0f,
			1.75f);

		//vkCmdBindPipeline(drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelines.offscreen);
		//vkCmdBindDescriptorSets(drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets.offscreen, 0, nullptr);
		//scenes[sceneIndex].draw(drawCmdBuffers[i]);

	}

	void CVulkanGraphicsApi::beginRecordingCommandBuffer(VkCommandBuffer commandBuffer, VkFramebuffer framebuffer, VkRenderPass renderPass) {
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
        
        auto res = vkBeginCommandBuffer(commandBuffer, &beginInfo);
        
		VK_CHECK(res, VK_SUCCESS);

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = renderPass;
		renderPassInfo.framebuffer = framebuffer;
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = m_swapChainExtent;

		std::vector<VkClearValue> clearValues(2);
		clearValues[0].color = { {0.70f, 0.83f, 0.87f, 0.0f} };
		clearValues[1].depthStencil = { 1.0f, 0 };

		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void CVulkanGraphicsApi::createCommandBuffers()
	{
		m_commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = m_commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32_t)m_commandBuffers.size();
        
        auto result = vkAllocateCommandBuffers(m_device, &allocInfo, m_commandBuffers.data());

		VK_CHECK(result, VK_SUCCESS);
	}

	void CVulkanGraphicsApi::createSyncObjects()
	{
		m_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		m_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		m_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            auto result =vkCreateSemaphore(m_device, &semaphoreInfo, &m_allocCallbacks, &m_imageAvailableSemaphores[i]);
			VK_CHECK(result, VK_SUCCESS);
            result = vkCreateSemaphore(m_device, &semaphoreInfo, &m_allocCallbacks, &m_renderFinishedSemaphores[i]);
			VK_CHECK(result, VK_SUCCESS);
            result = vkCreateFence(m_device, &fenceInfo, &m_allocCallbacks, &m_inFlightFences[i]);
			VK_CHECK(result, VK_SUCCESS);
		}
		
	}

	void CVulkanGraphicsApi::recreateSwapChain()
	{
		int width = 0, height = 0;
		while (width == 0 || height == 0) {
			glfwGetFramebufferSize((GLFWwindow*)m_nativeWindow->getNativeWindow(), &width, &height);
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(m_device);
		cleanupSwapChain();
		
		createSwapChain();
		createImageViews();
		createRenderPass();
		prepareOffscreenFramebuffer();
		for (size_t i = 0; i < m_pipelines.size(); i++) {
			m_pipelines[i]->create();
		}
		createDepthResources();
		createColorResources();
		createFramebuffers();
	}

	uint32_t CVulkanGraphicsApi::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const {
	
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memProperties);
		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		return 0;
	}

	void CVulkanGraphicsApi::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
		VkCommandBuffer commandBuffer = beginSingleTimeCommands();

		VkBufferCopy copyRegion{};
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		endSingleTimeCommands(commandBuffer);
	}

	void CVulkanGraphicsApi::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
	{
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(m_device, &bufferInfo, &m_allocCallbacks, &buffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to create buffer!");
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(m_device, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

        auto result = vkAllocateMemory(m_device, &allocInfo, &m_allocCallbacks, &bufferMemory);
        
		VK_CHECK(result, VK_SUCCESS);

		vkBindBufferMemory(m_device, buffer, bufferMemory, 0);
	}

	da::platform::QueueFamilyIndices CVulkanGraphicsApi::findQueueFamilies(VkPhysicalDevice device, std::optional<VkSurfaceKHR> surface)
	{
		QueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}

			if (surface.has_value()) {
				VkBool32 presentSupport = false;
                auto result = vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface.value(), &presentSupport);
				VK_CHECK(result, VK_SUCCESS);
				if (presentSupport) {
					indices.presentFamily = i;
				}
			}

			if (indices.isComplete())
				break;

			i++;
		}

		return indices;
	}

	void CVulkanGraphicsApi::createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSample,VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
		 VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) {
		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = width;
		imageInfo.extent.height = height;
		imageInfo.extent.depth = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = format;
		imageInfo.tiling = tiling;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = usage;
		imageInfo.samples = numSample;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.mipLevels = mipLevels;

        auto result =vkCreateImage(m_device, &imageInfo, &m_allocCallbacks, &image);
        
		VK_CHECK(result, VK_SUCCESS);

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(m_device, image, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

        result = vkAllocateMemory(m_device, &allocInfo, &m_allocCallbacks, &imageMemory);
        
		VK_CHECK(result, VK_SUCCESS);

		vkBindImageMemory(m_device, image, imageMemory, 0);
	}


	VkCommandBuffer CVulkanGraphicsApi::beginSingleTimeCommands() const
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = m_commandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(m_device, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}

    void CVulkanGraphicsApi::immediateSubmit(std::function<void (VkCommandBuffer cmd)>&& func) const
    {
        VkCommandBuffer buff = beginSingleTimeCommands();
        
        func(buff);
        endSingleTimeCommands(buff);
    }

	void CVulkanGraphicsApi::endSingleTimeCommands(VkCommandBuffer commandBuffer) const
	{
		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(m_graphicsQueue);

		vkFreeCommandBuffers(m_device, m_commandPool, 1, &commandBuffer);
	}

	bool hasStencilComponent(VkFormat format) {
		return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
	}

	void CVulkanGraphicsApi::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels)
	{
		VkCommandBuffer commandBuffer = beginSingleTimeCommands();
		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = mipLevels;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

			if (hasStencilComponent(format)) {
				barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
			}
		}
		else {
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		}

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		}
		else {
			ASSERT(false);
		}

		vkCmdPipelineBarrier(
			commandBuffer,
			sourceStage, destinationStage,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);
		endSingleTimeCommands(commandBuffer);

	}

	void CVulkanGraphicsApi::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
	{
		VkCommandBuffer commandBuffer = beginSingleTimeCommands();
		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;

		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;

		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = {
			width,
			height,
			1
		};
		vkCmdCopyBufferToImage(
			commandBuffer,
			buffer,
			image,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1,
			&region
		);
		endSingleTimeCommands(commandBuffer);
	}

	void CVulkanGraphicsApi::createTextureImageView()
	{
		//m_textureImageView = createImageView(m_textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels);
	}

	VkImageView CVulkanGraphicsApi::createImageView(VkImage image, VkImageViewType viewType, uint32_t layerCount, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels)
	{
		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = image;
		viewInfo.viewType = viewType;
		viewInfo.format = format;
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = mipLevels;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = layerCount;
		viewInfo.subresourceRange.aspectMask = aspectFlags;

		VkImageView imageView;
        auto result =vkCreateImageView(m_device, &viewInfo, &m_allocCallbacks, &imageView);
		VK_CHECK(result, VK_SUCCESS);

		return imageView;
	}

	VkFormat CVulkanGraphicsApi::findDepthFormat() {
		return findSupportedFormat(
			{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
			VK_IMAGE_TILING_OPTIMAL,
			VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
		);
	}

	VkFormat CVulkanGraphicsApi::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
		for (VkFormat format : candidates) {
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(m_physicalDevice, format, &props);

			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
				return format;
			}
			else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
				return format;
			}
		}

		ASSERT(false);

		return {};
	}

	void CVulkanGraphicsApi::createDepthResources()
	{
		VkFormat depthFormat = findDepthFormat();

		createImage(m_swapChainExtent.width, m_swapChainExtent.height, 1, m_msaaSamples, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_depthImage, m_depthImageMemory);
		m_depthImageView = createImageView(m_depthImage, VK_IMAGE_VIEW_TYPE_2D, 1, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
	}

	VkSampleCountFlagBits CVulkanGraphicsApi::getMaxUsableSampleCount() const
	{
		VkPhysicalDeviceProperties physicalDeviceProperties;
		vkGetPhysicalDeviceProperties(m_physicalDevice, &physicalDeviceProperties);

		VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;
		if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
		if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
		if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
		if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
		if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
		if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

		return VK_SAMPLE_COUNT_1_BIT;
	}
	void CVulkanGraphicsApi::createColorResources()
	{
		VkFormat colorFormat = m_swapChainImageFormat;

		createImage(m_swapChainExtent.width, m_swapChainExtent.height, 1, m_msaaSamples, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_colorImage, m_colorImageMemory);
		m_colorImageView = createImageView(m_colorImage, VK_IMAGE_VIEW_TYPE_2D, 1, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
	}

	void CVulkanGraphicsApi::submitRenderFunction(std::function<void(VkCommandBuffer cmd)>* func)
	{
		m_renderFunctions.push_back(func);
	}

	void CVulkanGraphicsApi::removeRenderFunction(std::function<void(VkCommandBuffer cmd)>* func)
	{
		const auto& it = std::find(m_renderFunctions.begin(), m_renderFunctions.end(), func);
		if (it != m_renderFunctions.end())
			m_renderFunctions.erase(it);
	}

	void CVulkanGraphicsApi::submitPipeline(da::graphics::CGraphicsPipeline* pipeline)
	{
		CVulkanGraphicsPipeline* vkPipeline = dynamic_cast<CVulkanGraphicsPipeline*>(pipeline);
		LOG_ASSERT(vkPipeline, ELogChannel::Graphics, "Failed to cast CGraphicsPipeline to CVulkangraphicsPipeline");
		m_pipelines.push_back(vkPipeline);
		recreateSwapChain();
	}


}

#endif
