#include "daengine.h"
#include "app.h"


da::CApp* da::createApp()
{
	return new ProtoTypeApp();
}