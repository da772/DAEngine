#pragma once
#include "daengine.h"

#if defined(DA_PLATFORM_WINDOWS) && defined(DA_RELEASE) || defined(DA_FINAL)
#include <windows.h>
#endif

extern da::App* da::CreateApp();
#if defined(DA_PLATFORM_WINDOWS) && defined(DA_RELEASE) || defined(DA_FINAL)
int WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
#else
int main(int argc, char** argv)
#endif
{
	da::App* app = da::CreateApp();
	app->Begin();
	app->Run();
	app->End();

	delete app;

	return 0;
}