#pragma once
#include <daengine/core.h>
#include "level.h"

#define INVALID_LEVEL_ID 0xFFFFFFFF

class CLevelSelector {
public:
	inline CLevelSelector(da::modules::CWindowModule& window) : m_window(window) {}
	void initialize();
	uint32_t addLevel(ILevel* level);
	void startLevel(uint32_t id);
	const std::vector<ILevel*> getLevels() const;
	void update(float dt);
	void shutdown();


private:
	std::vector<ILevel*> m_levels;
	uint32_t m_level = INVALID_LEVEL_ID;
	da::modules::CWindowModule& m_window;
#ifdef DA_REVIEW
	bool m_levelDebug = false;
#endif

};