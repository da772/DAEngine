#pragma once

#include "component.h"
#include "daengine/asset/asset.h"

namespace da::core
{
	class CStaticMesh;
	class CMaterial;

	class CSmeshComponent {
#ifdef DA_DEBUG
		COMPONENT_H_DEBUG(CSmeshComponent);
#else
		COMPONENT_H(CSmeshComponent);
#endif

	public:
		CSmeshComponent(const std::string& meshPath, CMaterial* material, CEntity& parent);

		void onInitialize();
		void onUpdate(float dt);
		void onShutdown();
		CStaticMesh* getStaticMesh() const;
		CMaterial* getMaterial() const;
#ifdef DA_DEBUG
		void onDebugRender();
#endif

	private:
		CStaticMesh* m_staticMesh;
		CMaterial* m_material;

	};
}