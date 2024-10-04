#pragma once

#include "DAEngine/core.h"

class ILevel {
public:
	inline ILevel(CHashString name, const da::CGraphicsModule& graphicsModule, const da::CWindowModule& windowModule) 
		: m_name(name), m_graphicsModule(graphicsModule), m_windowModule(windowModule) {}

	virtual void initialize() = 0;
	virtual void update(float dt) = 0;
	inline virtual void lateUpdate(float dt) {};
	virtual void shutdown() = 0;

	inline CHashString getName() const { return m_name; }

protected:
	const da::CGraphicsModule& m_graphicsModule;
	const da::CWindowModule& m_windowModule;

private:
	CHashString m_name;
};