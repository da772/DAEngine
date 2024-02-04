#include "dapch.h"

#include "scene.h"
#include "entity.h"
#include "component.h"

#include "test_component.h"

namespace da::core {

	std::unordered_map<CHashString, da::core::FECSLifeCycle> CScene::m_componentLifeCycle;

	CScene::CScene(const CGuid& guid) : m_guid(guid)
	{

	}

	const da::core::CGuid& CScene::getId() const
	{
		return m_guid;
	}

	da::core::CEntity* CScene::createEntity(const CGuid& guid /*= CGuid::Generate()*/)
	{
		CEntity* entity = new CEntity(guid, this);
		m_entities.push_back(entity);
		return entity;
	}

	bool CScene::removeEntity(CEntity* entity)
	{
		const auto& it = std::find(m_entities.begin(), m_entities.end(), entity);

		if (it == m_entities.end()) return false;
		delete* it;

		m_entities.erase(it);
		return true;
	}

	void CScene::initialize()
	{
		for (std::pair<const CHashString, FComponentContainer>& kv : m_components) {
			FECSLifeCycle& f = m_componentLifeCycle[kv.first];
			for (size_t i = 0; i < kv.second.getCount(); i++) {
				f.init((void*)kv.second.getComponentAtIndex(i));
			}
		}
	}

	void CScene::update(float dt)
	{
		for (std::pair<const CHashString, FComponentContainer>& kv : m_components) {
			FECSLifeCycle& f = m_componentLifeCycle[kv.first];
			if (!f.update) continue;
			for (size_t i = 0; i < kv.second.getCount(); i++) {
				f.update((void*)kv.second.getComponentAtIndex(i), dt);
			}
		}
	}

	void CScene::shutdown()
	{
		for (std::pair<const CHashString, FComponentContainer>& kv : m_components) {
			FECSLifeCycle& f = m_componentLifeCycle[kv.first];
			for (size_t i = 0; i < kv.second.getCount(); i++) {
				f.shutdown((void*)kv.second.getComponentAtIndex(i));
			}
		}
	}

}
