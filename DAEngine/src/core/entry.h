#pragma once
#include "daengine.h"

extern da::App* da::CreateApp();

int main(int argc, char** argv)
{
	da::App* app = da::CreateApp();
	app->Begin();
	app->Run();
	app->End();

	delete app;

	return 0;
}