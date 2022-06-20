#pragma once
#include "daengine.h"

#if defined(DA_PLATFORM_WINDOWS) && defined(DA_FINAL)
#include <windows.h>
#endif
#include <iostream>
#include "daengine/core/memory/memory.h"

extern da::CApp* da::createApp(int argc, const char** argv);
#if defined(DA_PLATFORM_WINDOWS) && defined(DA_FINAL)
int WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
#else
int main(int argc, const char** argv)
#endif
{
#if !defined(DA_PLATFORM_WINDOWS)
#define __argc argc
#define __argv argv
#endif

	std::cout << "Memory Allocated: " << da::memory::getMemoryAllocated() << "\n" << std::endl;
	{
		da::memory::push_memory_layer(da::memory::EMemoryLayer::Application);
		da::CApp* app = da::createApp(__argc, (const char**)__argv);
		app->initalize();
		app->update();
		app->shutdown();

		delete app;
		app = nullptr;
		
		da::memory::pop_memory_layer();
	}

	std::cout << "Memory Allocated: " << da::memory::getMemoryAllocated() << "\n" << std::endl;
	for (uint8_t i = 0; i < (uint8_t)da::memory::EMemoryLayer::INVALID; i++) {
		printf("Layer: %s - %llu bytes\n", da::memory::get_memory_layer_name((da::memory::EMemoryLayer)i), da::memory::get_memory_layers()[i]);
	}


	return 0;
}
