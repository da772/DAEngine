#pragma once

#include "DAEngine/core.h"

class ILevel {
public:
	inline ILevel(CHashString name, const da::modules::CGraphicsModule& graphicsModule, const da::modules::CWindowModule& windowModule) 
		: m_name(name), m_graphicsModule(graphicsModule), m_windowModule(windowModule) {}

	virtual void initialize() = 0;
	virtual void update(float dt) = 0;
	inline virtual void lateUpdate(float dt) {};
	virtual void shutdown() = 0;

	inline CHashString getName() const { return m_name; }

protected:
	const da::modules::CGraphicsModule& m_graphicsModule;
	const da::modules::CWindowModule& m_windowModule;

private:
	CHashString m_name;
};