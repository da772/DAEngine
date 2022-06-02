#include "daengine.h"
#include "app.h"


da::CApp* da::createApp(int argc, const char** argv)
{
	return new ProtoTypeApp(argc, argv);
}