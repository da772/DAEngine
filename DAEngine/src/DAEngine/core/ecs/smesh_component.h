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
		COMPONENT_H_NO_UPDATE_DEBUG(CSmeshComponent);
#else
		COMPONENT_H_NO_UPDATE(CSmeshComponent);
#endif

	public:
		CSmeshComponent(const std::string& meshPath, CEntity& parent);
		CSmeshComponent(const std::string& meshPath, bool inverseNormals, CEntity& parent);

		void onInitialize();
		void onShutdown();
		da::graphics::CStaticMesh* getStaticMesh() const;
#ifdef DA_DEBUG
		void onDebugRender();
#endif

	private:
		da::graphics::CStaticMesh* m_staticMesh;
		bool m_inverseNormals;

	};
}