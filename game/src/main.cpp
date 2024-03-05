#include <DAEngine/core/entry.h>
#include "game/game.h"

da::CApp* da::createApp(int argc, const char** argv)
{
	return new CGame(argc, argv);
}