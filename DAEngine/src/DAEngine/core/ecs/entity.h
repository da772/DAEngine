#pragma once

#include "scene.h"
#include "daengine/core/maths/transform.h"

#ifdef DA_DEBUG
#include <imgui.h>
#endif

namespace da::core
{
	class CScene;

	template<typename T>
	struct FComponentRef {
		FComponentRef(const CGuid& guid, CScene* scene) : m_guid(guid), m_scene(scene){

		}

		FComponentRef() {

		}

		bool isValid() {
			return m_scene && m_guid.isValid();
		}

		T* operator->() const {

			if (!m_scene || !m_guid.isValid()) return nullptr;
			return m_scene->getComponent<T>(m_guid);
		}

	private:
		CGuid m_guid;
		CScene* m_scene;
	};

	class CEntity {
	public:
		CEntity(const CGuid& guid, CScene* scene);
		CEntity(CScene* scene);

	public:
		const CGuid& getId() const;
		CHashString getTag() const;
		void setTag(CHashString tag);
		const da::maths::CTransform& getTransform() const;
		da::maths::CTransform& getTransform();
		
		template <typename T, typename... Args>
		FComponentRef<T> addComponent(Args&&... args)
		{
			if (T* comp = m_scene->createComponent<T>(std::forward<Args>(args)..., *this)) {

				m_components[T::getTypeHash()] = comp->getId();
				return FComponentRef<T>(comp->getId(), m_scene);
			}

			return FComponentRef<T>();
		}

		template <typename T>
		FComponentRef<T> getComponent() {
			const auto& it = m_components.find(T::getTypeHash());

			if (it == m_components.end()) {
				return FComponentRef<T>();
			}

			if (T* c = m_scene->getComponent<T>(it->second)) {
				return FComponentRef<T>(c->getId(), m_scene);
			}

			return FComponentRef<T>();
		}

		template <typename T>
		bool removeComponent(const CGuid& guid) {
			bool removed = m_scene->destroyComponent<T>(guid);

			if (removed) {
				m_components.erase(m_components.find(T::getId()));
			}

			return removed;

		}
		template <typename T>
		bool removeComponent(const FComponentRef<T>& c) {
			bool removed = m_scene->destroyComponent<T>(c->getId());


			if (removed) {
				m_components.erase(m_components.find(T::getId()));
			}

			return removed;
		}

#ifdef DA_DEBUG
		void debugRender();
#endif

	private:
		std::unordered_map<CHashString, CGuid> m_components;
		CGuid m_guid;
		CScene* m_scene;
		da::maths::CTransform m_transform;
		CHashString m_tag;

			
	};
}