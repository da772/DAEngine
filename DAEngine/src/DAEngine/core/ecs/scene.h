#pragma once

namespace da::core {

	class CEntity;

	struct FComponentContainer {
	public:
		template <typename T, typename... Args>
		T* createComponent(Args&&... args) {
			m_size = sizeof(T);
			m_components.insert(m_components.end(), sizeof(T), 0);
			char* ptr = &m_components[m_count * sizeof(T)];
			new ((T*)ptr)T (std::forward<Args>(args)...);
#if DA_DEBUG || DA_RELEASE
			LOG_ASSERT(((T*)ptr)->getId().isValid(), ELogChannel::Core, "Creating component with invalid id");
#endif
			m_count++;
			return (T*)ptr;
		}

		template <typename T>
		T* findComponent(const CGuid& guid) const {
			for (size_t i = 0; i < m_components.size(); i += sizeof(T)) {
				T* c = (T*) & m_components[i];
				if (c->getId() == guid) {
					return c;
				}
			}

			return nullptr;
		}

		template <typename T>
		bool removeComponent(const CGuid& guid) {
			T* component = findComponent<T>(guid);

			if (!component) return false;

			m_components.erase(component, component + sizeof(T));
			m_count--;
			return true;
		}

		const void* getComponentAtIndex(size_t index) const {
			return &m_components.at(index * m_size);
		}
		
		template <typename T>
		T* getComponentAtIndex(size_t index) const {
			return (T*) & m_components.at(index * m_size);
		}

		size_t getCount() const {
			return m_count;
		}

	private:
		std::vector<char> m_components;
		size_t m_count;
		size_t m_size;
	};

	struct FECSLifeCycle {
		void (*init)(void*);
		void (*update)(void*, float);
		void (*shutdown)(void*);
	};

	class CScene {
	public:
		CScene(const CGuid& guid);
		const CGuid& getId() const;

		void initialize();
		void update(float dt);
		void shutdown();

		template <typename T, typename... Args>
		T* createComponent(Args&&... args) {
			const CHashString& typeHash = T::getTypeHash();

			return m_components[typeHash].createComponent<T>(std::forward<Args>(args)...);
		}

		template <typename T>
		bool destroyComponent(const CGuid& guid) {
			return m_components[T::getTypeHash()].removeComponent(guid);
		}

		template <typename T>
		T* getComponent(const CGuid& guid) {
			const CHashString& typeHash = T::getTypeHash();
			return m_components[typeHash].findComponent<T>(guid);
		}

		CEntity* createEntity(const CGuid& guid = CGuid::Generate());
		bool removeEntity(CEntity* entity);

		template <typename T>
		static void registerComponentLifeCycle(const FECSLifeCycle& lifeCycle) {
			m_componentLifeCycle[T::getTypeHash()] = lifeCycle;
		}
        
        template <typename T>
        const FComponentContainer& getComponents() {
            const CHashString& typeHash = T::getTypeHash();
            return m_components[typeHash];
        }

		const std::vector<CEntity*>& getEntities() const {
			return m_entities;
		}

	private:
		std::vector<CEntity*> m_entities;
		std::unordered_map<CHashString, FComponentContainer> m_components;
		static std::unordered_map<CHashString, FECSLifeCycle> m_componentLifeCycle;
		CGuid m_guid;
	};

}
