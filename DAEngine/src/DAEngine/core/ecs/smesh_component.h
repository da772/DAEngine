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
		CSmeshComponent(const std::string& meshPath, da::graphics::CMaterial* material, CEntity& parent);

		void onInitialize();
		void onUpdate(float dt);
		void onShutdown();
		da::graphics::CStaticMesh* getStaticMesh() const;
		da::graphics::CMaterial* getMaterial() const;
#ifdef DA_DEBUG
		void onDebugRender();
#endif

	private:
		da::graphics::CStaticMesh* m_staticMesh;
		da::graphics::CMaterial* m_material;

	};
}