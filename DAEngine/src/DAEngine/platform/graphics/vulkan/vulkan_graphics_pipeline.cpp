
#include "vulkan_graphics_pipeline.h"
#ifdef DA_GRAPHICS_VULKAN
#include <fstream>
#include "core/logger.h"
#include "vulkan_graphics_material.h"
#include "graphics/factory/factory_graphics_material.h"

namespace da {
	static std::vector<char> readFile(const std::string& filename) {
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			throw std::runtime_error("failed to open file!");
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}


	CVulkanGraphicsPipeline::CVulkanGraphicsPipeline(da::CGraphicsApi& graphicsApi, const std::string& vertexShader, const std::string& fragShader, da::FVertexBindingDescription vertexBinding,
		const std::vector<da::FVertexInputAttributeDescription>& inputAttribDesc) :
		da::CGraphicsPipeline(graphicsApi, vertexShader, fragShader, vertexBinding, inputAttribDesc), m_vulkanGraphicsApi(*static_cast<CVulkanGraphicsApi*>(&m_graphicsApi))
	{
		
	}

	void CVulkanGraphicsPipeline::create()
	{
		if (m_initialized) return;
		createDescriptorSets();
		createGraphicsPipeline();
		m_initialized = true;
	}

	void CVulkanGraphicsPipeline::destroy()
	{
		if (!m_initialized) return;
		vkDestroyPipeline(m_vulkanGraphicsApi.getDevice(), m_graphicsPipeline, &m_vulkanGraphicsApi.getAllocCallbacks());
		vkDestroyPipelineLayout(m_vulkanGraphicsApi.getDevice(), m_pipelineLayout, &m_vulkanGraphicsApi.getAllocCallbacks());
		vkDestroyDescriptorSetLayout(m_vulkanGraphicsApi.getDevice(), m_descriptorSetLayout, &m_vulkanGraphicsApi.getAllocCallbacks());
		m_initialized = false;
	}

	VkShaderModule CVulkanGraphicsPipeline::createShaderModule(const std::vector<char>& code, VkDevice device) {
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VkShaderModule shaderModule{};
		auto result = vkCreateShaderModule(device, &createInfo, &m_vulkanGraphicsApi.getAllocCallbacks(), &shaderModule);
		LOG_ASSERT(result == VK_SUCCESS, ELogChannel::Graphics, "Failed to create ShaderModule");
		return shaderModule;
	}


	void CVulkanGraphicsPipeline::createDescriptorSets()
	{
		VkDescriptorSetLayoutBinding uboLayoutBinding{};
		VkDescriptorSetLayoutBinding samplerLayoutBinding{};
		

		std::vector<VkDescriptorSetLayoutBinding> bindings = addDescriptorSets();
		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		layoutInfo.pBindings = bindings.data();

		auto result = vkCreateDescriptorSetLayout(m_vulkanGraphicsApi.getDevice(), &layoutInfo, &m_vulkanGraphicsApi.getAllocCallbacks(), &m_descriptorSetLayout);

		LOG_ASSERT(result == VK_SUCCESS, ELogChannel::Graphics, "Failed to create DescriptorSetLayout");
	}

	std::vector<VkDescriptorSetLayoutBinding> CVulkanGraphicsPipeline::addDescriptorSets()
	{

		VkDescriptorSetLayoutBinding uboLayoutBinding = {};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

		VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
		samplerLayoutBinding.binding = 1;
		samplerLayoutBinding.descriptorCount = 5;
		samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding.pImmutableSamplers = nullptr;
		samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		return { uboLayoutBinding, samplerLayoutBinding };
	}

	void CVulkanGraphicsPipeline::createGraphicsPipeline()
	{
		auto vertShaderCode = readFile(m_vertexShaderPath.c_str());
		auto fragShaderCode = readFile(m_fragShaderPath.c_str());

		VkShaderModule vertShaderModule = createShaderModule(vertShaderCode, m_vulkanGraphicsApi.getDevice());
		VkShaderModule fragShaderModule = createShaderModule(fragShaderCode, m_vulkanGraphicsApi.getDevice());

		VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
		vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertShaderStageInfo.module = vertShaderModule;
		vertShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
		fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragShaderStageInfo.module = fragShaderModule;
		fragShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.pVertexBindingDescriptions = (VkVertexInputBindingDescription*)&m_bindingDesc;
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(m_inputAttrbDesc.size());
		vertexInputInfo.pVertexAttributeDescriptions = (const VkVertexInputAttributeDescription*)m_inputAttrbDesc.data(); // Optional

		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)m_vulkanGraphicsApi.getSwapChainExt().width;
		viewport.height = (float)m_vulkanGraphicsApi.getSwapChainExt().height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = m_vulkanGraphicsApi.getSwapChainExt();

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;

		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;

		rasterizer.rasterizerDiscardEnable = VK_FALSE;

		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;

		rasterizer.lineWidth = 1.0f;

		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

		rasterizer.depthBiasEnable = VK_FALSE;
		rasterizer.depthBiasConstantFactor = 0.0f; // Optional
		rasterizer.depthBiasClamp = 0.0f; // Optional
		rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

		updateRasterizer(rasterizer);

		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.rasterizationSamples = m_vulkanGraphicsApi.getMaxUsableSampleCount();
		multisampling.pSampleMask = nullptr; // Optional
		multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
		multisampling.alphaToOneEnable = VK_FALSE; // Optional
		multisampling.sampleShadingEnable = VK_TRUE; // enable sample shading in the pipeline
		multisampling.minSampleShading = .2f;

		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_TRUE;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f; // Optional
		colorBlending.blendConstants[1] = 0.0f; // Optional
		colorBlending.blendConstants[2] = 0.0f; // Optional
		colorBlending.blendConstants[3] = 0.0f; // Optional

		std::vector<VkDynamicState> dynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_LINE_WIDTH
		};

		VkPipelineDynamicStateCreateInfo dynamicState{};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
		dynamicState.pDynamicStates = dynamicStates.data();

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 1;
		pipelineLayoutInfo.pSetLayouts = &m_descriptorSetLayout;


		auto result = vkCreatePipelineLayout(m_vulkanGraphicsApi.getDevice(), &pipelineLayoutInfo, &m_vulkanGraphicsApi.getAllocCallbacks(), &m_pipelineLayout);

		LOG_ASSERT(result == VK_SUCCESS, ELogChannel::Graphics, "Failed to create PipelineSetLayout");

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = nullptr; // Optional
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = nullptr; // Optional

		pipelineInfo.layout = m_pipelineLayout;
		pipelineInfo.renderPass = m_vulkanGraphicsApi.getRenderPass();
		pipelineInfo.subpass = 0;

		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
		pipelineInfo.basePipelineIndex = -1; // Optional

		VkPipelineDepthStencilStateCreateInfo depthStencil{};
		depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencil.depthTestEnable = VK_TRUE;
		depthStencil.depthWriteEnable = VK_TRUE;
		depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
		depthStencil.depthBoundsTestEnable = VK_FALSE;
		depthStencil.minDepthBounds = 0.0f; // Optional
		depthStencil.maxDepthBounds = 1.0f; // Optional
		depthStencil.stencilTestEnable = VK_FALSE;
		depthStencil.front = {}; // Optional
		depthStencil.back = {}; // Optional

		updateDepthStencil(depthStencil);

		pipelineInfo.pDepthStencilState = &depthStencil;

		result = vkCreateGraphicsPipelines(m_vulkanGraphicsApi.getDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, &m_vulkanGraphicsApi.getAllocCallbacks(), &m_graphicsPipeline);

		LOG_ASSERT(result == VK_SUCCESS, ELogChannel::Graphics, "Failed to create GraphicsPipeline");

		vkDestroyShaderModule(m_vulkanGraphicsApi.getDevice(), fragShaderModule, &m_vulkanGraphicsApi.getAllocCallbacks());
		vkDestroyShaderModule(m_vulkanGraphicsApi.getDevice(), vertShaderModule, &m_vulkanGraphicsApi.getAllocCallbacks());
	}

	void CVulkanGraphicsPipeline::render(VkCommandBuffer& commandBuffer, int frame)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, getPipeline());
		for (const FVulkanMeshData& data : m_renderables)
		{
			VkBuffer vertexBuffers[] = { data.VertexBuffer };
			VkDeviceSize offsets[] = { 0 };
			vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

			vkCmdBindIndexBuffer(commandBuffer, data.IndexBuffer, 0, VK_INDEX_TYPE_UINT32);

			CVulkanGraphicsMaterial* material = dynamic_cast<CVulkanGraphicsMaterial*>(data.Material);

			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, getPipelineLayout(), 0, 1, &material->getDescriptorSets()[frame], 0, nullptr);

			vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(data.Renderable->getIndices().size()), 1, 0, 0, 0);
		}
	}

	da::FVulkanMeshData CVulkanGraphicsPipeline::createMeshData(da::IRenderable* renderable, da::CMaterial* material) const
	{
		FVulkanMeshData meshData;
		ASSERT(renderable);
		ASSERT(material);
		meshData.Material = material;
		meshData.Renderable = renderable;

		{
			VkDeviceSize bufferSize = sizeof(da::FVertexBase) * renderable->getVertices().size();

			VkBuffer stagingBuffer;
			VkDeviceMemory stagingBufferMemory;
			m_vulkanGraphicsApi.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
			void* data;
			vkMapMemory(m_vulkanGraphicsApi.getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
			memcpy(data, renderable->getVertices().data(), (size_t)bufferSize);
			vkUnmapMemory(m_vulkanGraphicsApi.getDevice(), stagingBufferMemory);

			m_vulkanGraphicsApi.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, meshData.VertexBuffer, meshData.VertexMemory);

			m_vulkanGraphicsApi.copyBuffer(stagingBuffer, meshData.VertexBuffer, bufferSize);

			vkDestroyBuffer(m_vulkanGraphicsApi.getDevice(), stagingBuffer, &m_vulkanGraphicsApi.getAllocCallbacks());
			vkFreeMemory(m_vulkanGraphicsApi.getDevice(), stagingBufferMemory, &m_vulkanGraphicsApi.getAllocCallbacks());
		}
		{
			VkDeviceSize bufferSize = sizeof(uint32_t) * meshData.Renderable->getIndices().size();

			VkBuffer stagingBuffer;
			VkDeviceMemory stagingBufferMemory;
			m_vulkanGraphicsApi.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

			void* data;
			vkMapMemory(m_vulkanGraphicsApi.getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
			memcpy(data, meshData.Renderable->getIndices().data(), (size_t)bufferSize);
			vkUnmapMemory(m_vulkanGraphicsApi.getDevice(), stagingBufferMemory);

			m_vulkanGraphicsApi.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, meshData.IndexBuffer, meshData.IndexMemory);

			m_vulkanGraphicsApi.copyBuffer(stagingBuffer, meshData.IndexBuffer, bufferSize);

			vkDestroyBuffer(m_vulkanGraphicsApi.getDevice(), stagingBuffer, &m_vulkanGraphicsApi.getAllocCallbacks());
			vkFreeMemory(m_vulkanGraphicsApi.getDevice(), stagingBufferMemory, &m_vulkanGraphicsApi.getAllocCallbacks());
		}

		

		return meshData;
	}

	void CVulkanGraphicsPipeline::update(int frame)
	{
		
	}

	void CVulkanGraphicsPipeline::clean()
	{
		for (const FVulkanMeshData& data : m_renderables)
		{
			vkDestroyBuffer(m_vulkanGraphicsApi.getDevice(), data.VertexBuffer, &m_vulkanGraphicsApi.getAllocCallbacks());
			vkFreeMemory(m_vulkanGraphicsApi.getDevice(), data.VertexMemory, &m_vulkanGraphicsApi.getAllocCallbacks());

			vkDestroyBuffer(m_vulkanGraphicsApi.getDevice(), data.IndexBuffer, &m_vulkanGraphicsApi.getAllocCallbacks());
			vkFreeMemory(m_vulkanGraphicsApi.getDevice(), data.IndexMemory, &m_vulkanGraphicsApi.getAllocCallbacks());

			da::CMaterialFactory::remove(data.Material);
		}

		m_renderables.clear();
	}

}


#endif
