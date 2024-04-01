#pragma once

#include <DAEngine.h>

class ILevel {
public:
	inline ILevel(CHashString name) : m_name(name) {}

	virtual void initialize() = 0;
	virtual void update(float dt) = 0;
	virtual void shutdown() = 0;

	inline CHashString getName() const { return m_name; }

private:
	CHashString m_name;
};