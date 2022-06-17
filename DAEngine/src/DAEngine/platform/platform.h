#pragma once
#if (defined(DA_PLATFORM_MACOSX) || defined(DA_PLATFORM_WINDOWS) || defined(DA_PLATFORM_LINUX)) && !defined(DA_TEST)
#define DA_WINDOW_GLFW
#define DA_GRAPHICS_VULKAN
#endif
