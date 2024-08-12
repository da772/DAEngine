#include <core/entry.h>
#include <platform/app/app.h>
#include "game/game.h"

da::CApp* da::createApp(int argc, const char** argv)
{
	return new CGame(argc, argv);
}