#pragma once
#include <daengine.h>
#include <daengine/core.h>
#include <daengine/components.h>
#include "level.h"


class CVehicle;

class CTestBed02Level : public ILevel {

public:
	CTestBed02Level(CHashString name, da::modules::CWindowModule& window);
	void initialize();
	void update(float dt);
	void shutdown();

private:
	CVehicle* m_vehicle;
	da::modules::CWindowModule& m_window;

};