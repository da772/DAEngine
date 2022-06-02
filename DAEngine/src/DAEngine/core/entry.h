#pragma once
#include "daengine.h"

#if defined(DA_PLATFORM_WINDOWS) && defined(DA_RELEASE) || defined(DA_FINAL)
#include <windows.h>
#endif

extern da::CApp* da::createApp(int argc, const char** argv);
#if defined(DA_PLATFORM_WINDOWS) && defined(DA_RELEASE) || defined(DA_FINAL)
int WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
#else
int main(int argc, const char** argv)
#endif
{
	da::CApp* app = da::createApp(argc, argv);
	app->initalize();
	app->update();
	app->shutdown();

	delete app;

	return 0;
}