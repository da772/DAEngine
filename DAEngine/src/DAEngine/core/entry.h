#pragma once
#include "daengine.h"

#if defined(DA_PLATFORM_WINDOWS) && defined(DA_FINAL) || defined(DA_RELEASE)
#include <windows.h>
#endif
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
	
	{
		da::CApp* app = da::createApp(__argc, (const char**)__argv);
		app->initalize();
		app->update();
		app->shutdown();

		delete app;
		app = nullptr;
	}
	
	return 0;
}
