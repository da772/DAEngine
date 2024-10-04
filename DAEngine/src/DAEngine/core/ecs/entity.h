#pragma once

#include "scene.h"
#include "core/logger.h"
#include "stl/maths/transform.h"

#ifdef DA_REVIEW
#include <imgui.h>
#endif

namespace da
{
	class CScene;

	struct FComponentInfo {
		CGuid Guid;
		std::function<void()> Delete;
	};

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

			ASSERT(m_scene);
			ASSERT(m_guid.isValid());
			if (!m_scene || !m_guid.isValid()) return nullptr;
			T* p = m_scene->getComponent<T>(m_guid);
			ASSERT(p);
			return p;
		}

	private:
		CGuid m_guid;
		CScene* m_scene;
	};

	class CEntity {
	public:
		CEntity(const CGuid& guid, CScene* scene);
		CEntity(CScene* scene);
		~CEntity();

	public:
		const CGuid& getId() const;
		CHashString getTag() const;
		void setTag(CHashString tag);
		const da::CTransform& getTransform() const;
		da::CTransform& getTransform();
		
		template <typename T, typename... Args>
		FComponentRef<T> addComponent(Args&&... args)
		{
			if (T* comp = m_scene->createComponent<T>(std::forward<Args>(args)..., *this)) {
				CGuid guid = comp->getId();
				m_components[T::getTypeHash()] = { guid, [this, guid] { this->removeComponentInternal<T>(guid); } };
				return FComponentRef<T>(guid, m_scene);
			}

			return FComponentRef<T>();
		}

		template <typename T>
		FComponentRef<T> getComponent() const {
			const auto& it = m_components.find(T::getTypeHash());

			if (it == m_components.end()) {
				return FComponentRef<T>();
			}

			if (T* c = m_scene->getComponent<T>(it->second.Guid)) {
				return FComponentRef<T>(c->getId(), m_scene);
			}

			return FComponentRef<T>();
		}

		template <typename T>
		bool removeComponent(const CGuid& guid) {
			bool removed = removeComponentInternal<T>(guid);

			if (removed) {
				m_components.erase(m_components.find(T::getTypeHash()));
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

#ifdef DA_REVIEW
		void debugRender();
#endif
	private:
		template <typename T>
		bool removeComponentInternal(const CGuid& guid) {
			return m_scene->destroyComponent<T>(guid);
		}


	private:
		std::unordered_map<CHashString, FComponentInfo> m_components;
		CGuid m_guid;
		CScene* m_scene;
		da::CTransform m_transform;
		CHashString m_tag;

			
	};
}