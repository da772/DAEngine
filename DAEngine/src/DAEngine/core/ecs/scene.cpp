#include <vector>

#include "scene.h"
#include "entity.h"
#include "component.h"

namespace da {

	std::vector<std::pair<CHashString, da::FECSLifeCycle>> CScene::m_componentLifeCycle;

	CScene::CScene(const CGuid& guid) : m_guid(guid)
	{

	}

	const da::CGuid& CScene::getId() const
	{
		return m_guid;
	}

	da::CEntity* CScene::createEntity(const CGuid& guid /*= CGuid::Generate()*/)
	{
		CEntity* entity = new CEntity(guid, this);
		m_entities.push_back(entity);
		return entity;
	}

	bool CScene::removeEntity(CEntity* entity)
	{
		const auto& it = std::find(m_entities.begin(), m_entities.end(), entity);

		if (it == m_entities.end()) return false;
		delete entity;

		m_entities.erase(it);
		return true;
	}

	void CScene::initialize()
	{
		for (const std::pair<CHashString, FECSLifeCycle>& kv : m_componentLifeCycle) {
			const FComponentContainer& components = getComponents(kv.first);
			for (size_t i = 0; i < components.getCount(); i++) {
				kv.second.init((void*)components.getComponentAtIndex(i));
			}
		}
		m_initialized = true;
	}

	void CScene::update(float dt)
	{
		for (const std::pair<CHashString, FECSLifeCycle>& kv : m_componentLifeCycle) {
			const FComponentContainer& components = getComponents(kv.first);
			if (!kv.second.update) continue;
			for (size_t i = 0; i < components.getCount(); i++) {
				kv.second.update((void*)components.getComponentAtIndex(i), dt);
			}
		}
	}

	void CScene::shutdown()
	{
		for (const std::pair<CHashString, FECSLifeCycle>& kv : m_componentLifeCycle) {
			const FComponentContainer& components = getComponents(kv.first);
			for (size_t i = 0; i < components.getCount(); i++) {
				kv.second.shutdown((void*)components.getComponentAtIndex(i));
			}
		}

		m_initialized = false;
	}

	CScene::~CScene()
	{
		for (int32_t i = m_entities.size()-1; i >= 0; i--) {
			removeEntity(m_entities[i]);
		}
	}

	const da::CEntity* CScene::getEntityFromTag(CHashString tag) const
	{
		for (const CEntity* e : m_entities) {
			if (e->getTag() == tag) {
				return e;
			}
		}

		return nullptr;
	}

	void FComponentContainer::dirty()
	{
		m_componentCache = {};
	}



}
