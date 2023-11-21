#pragma once

#include "component.h"
#include "asset/asset.h"

namespace da::core
{
	class CStaticMesh;
	class CMaterial;

	class CSmeshComponent {
		COMPONENT_H(CSmeshComponent);

	public:
		CSmeshComponent(const std::string& meshPath, const std::string& materialPathVS, const std::string& materialPathFS, CEntity& parent);

		void onInitialize();
		void onUpdate(float dt);
		void onShutdown();

	private:
		CStaticMesh* m_staticMesh;
		CMaterial* m_material;

	};
}