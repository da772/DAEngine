#pragma once

#include "component.h"
#include "daengine/asset/asset.h"

namespace da::graphics
{
	class CStaticMesh;
	class CMaterial;
}

namespace da::core
{
	

	class CSmeshComponent {
#ifdef DA_DEBUG
		COMPONENT_H_DEBUG(CSmeshComponent);
#else
		COMPONENT_H(CSmeshComponent);
#endif

	public:
		CSmeshComponent(const std::string& meshPath, CEntity& parent);

		void onInitialize();
		void onUpdate(float dt);
		void onShutdown();
		da::graphics::CStaticMesh* getStaticMesh() const;
#ifdef DA_DEBUG
		void onDebugRender();
#endif

	private:
		da::graphics::CStaticMesh* m_staticMesh;

	};
}