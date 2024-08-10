#include "entity.h"
#include "scene.h"

namespace da::core {

	CEntity::CEntity(const CGuid& guid, CScene* scene) : m_guid(guid), m_scene(scene)
	{

	}

	CEntity::CEntity(CScene* scene) : m_guid(CGuid::Generate()), m_scene(scene)
	{

	}

	const da::core::CGuid& CEntity::getId() const
	{
		return m_guid;
	}

	const da::maths::CTransform& CEntity::getTransform() const
	{
		return m_transform;
	}

	da::maths::CTransform& CEntity::getTransform()
	{
		return m_transform;
	}

	CHashString CEntity::getTag() const
	{
		return m_tag;
	}

	void CEntity::setTag(CHashString tag)
	{
		m_tag = tag;
	}

#ifdef DA_REVIEW
	void CEntity::debugRender()
	{
		if (ImGui::CollapsingHeader("Components")) {
			ImGui::Indent();
			for (const std::pair<CHashString, FComponentInfo>& component : m_components)
			{
				char compNameBuffer[1024];
				sprintf_s(compNameBuffer, sizeof(compNameBuffer), "%s (%s)", component.first.c_str(), component.second.Guid.c_str());

				const FComponentContainer& container = m_scene->getComponents(component.first);
				const FECSLifeCycle& lifeCycle = m_scene->getComponentLifeCycle(component.first);

				if (ImGui::CollapsingHeader(compNameBuffer, !lifeCycle.debugRender ? ImGuiTreeNodeFlags_Leaf : 0)) {
					ImGui::Indent();
					if (void* ptr = container.findComponent(component.second.Guid))
					{
						
						if (lifeCycle.debugRender) lifeCycle.debugRender(ptr);
					}
					ImGui::Unindent();
				}
				
			}
			ImGui::Unindent();
		}
	}
#endif

	CEntity::~CEntity()
	{
		for (const std::pair<CHashString, FComponentInfo>& kv : m_components) {
			kv.second.Delete();
		}

		m_components = {};
	}

}