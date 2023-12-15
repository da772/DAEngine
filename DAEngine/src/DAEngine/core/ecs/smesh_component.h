#pragma once

#include "component.h"
#include "daengine/asset/asset.h"

namespace da::core
{
	class CStaticMesh;
	class CMaterial;

	class CSmeshComponent {
		COMPONENT_H(CSmeshComponent);

	public:
		CSmeshComponent(const std::string& meshPath, CMaterial* material, CEntity& parent);

		void onInitialize();
		void onUpdate(float dt);
		void onShutdown();
		CStaticMesh* getStaticMesh() const;
		CMaterial* getMaterial() const;

	private:
		CStaticMesh* m_staticMesh;
		CMaterial* m_material;

	};
}