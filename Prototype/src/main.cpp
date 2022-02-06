#include "daengine.h"
#include "app.h"


da::App* da::CreateApp()
{
	return new ProtoTypeApp();
}