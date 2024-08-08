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
	struct FInstance
	{
		glm::mat4 Transform;
		uint32_t Id;
	};

	class CSmeshComponent {
#ifdef DA_REVIEW
		COMPONENT_H_NO_UPDATE_DEBUG(CSmeshComponent);
#else
		COMPONENT_H_NO_UPDATE(CSmeshComponent);
#endif

	public:
		CSmeshComponent(const std::string& meshPath, CEntity& parent);
		CSmeshComponent(da::graphics::CStaticMesh* meshPath, CEntity& parent);
		CSmeshComponent(const std::string& meshPath, bool inverseNormals, CEntity& parent);

		void onInitialize();
		void onShutdown();
		uint32_t addInstance(const glm::mat4& transform);
		void removeInstance(uint32_t id);
		const std::vector<FInstance>& getInstances() const;
		da::graphics::CStaticMesh* getStaticMesh() const;
#ifdef DA_REVIEW
		void onDebugRender();
#endif

	private:
		da::graphics::CStaticMesh* m_staticMesh;
		bool m_inverseNormals : 1;
		std::vector<FInstance> m_instances;
		uint32_t m_instanceCounter = 0;

	};
}