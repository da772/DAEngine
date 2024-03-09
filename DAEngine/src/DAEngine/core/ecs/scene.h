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
				T* c = (T*) &m_components[i];
				if (c->getId() == guid) {
					return c;
				}
			}

			return nullptr;
		}

		template <typename T>
		size_t findComponentIndex(const CGuid& guid) const {
			for (size_t i = 0; i < m_components.size(); i += sizeof(T)) {
				T* c = (T*)&m_components[i];
				if (c->getId() == guid) {
					return i;
				}
			}

			return -1;
		}

#ifdef DA_DEBUG
		void* findComponent(const CGuid& guid) const {
			for (size_t i = 0; i < m_count; i ++) {
				for (size_t j = 0; j < m_size; j++) {
					bool found = true;
					for (size_t z = 0; z < sizeof(uint128_t); z++) {
						if ((i*m_size)+z+j >= m_components.size() || (unsigned char)m_components[(i*m_size) + z + j] != guid.data()[z]) {
							found = false;
							break;
						}
					}
					if (found) return (void*)&m_components[i*m_size];
				}
			}

			return nullptr;
		}
#endif

		template <typename T>
		bool removeComponent(const CGuid& guid) {
			size_t index = findComponentIndex<T>(guid);

			if (index == -1) return false;

			T* component = (T*)m_components[index];
			component->~T();
			m_components.erase(m_components.begin()+index, m_components.begin() + index + sizeof(T));
			m_count--;
			return true;
		}

		template <typename T>
		bool removeComponentIndex(size_t index) {
			if (index == -1) return false;

			T* component = (T*)&m_components[index];
			component->~T();
			m_components.erase(m_components.begin() + index, m_components.begin() + index + sizeof(T));
			m_count--;
			return true;
		}

		const void* getComponentAtIndex(size_t index) const {
			return &m_components.at(index * m_size);
		}

		const void* getComponentAtIndexUnscaled(size_t index) const {
			return &m_components.at(index);
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
#ifdef DA_DEBUG
		void (*debugRender)(void*) = nullptr;
#endif
	};

	class CScene {
	public:
		CScene(const CGuid& guid);
		~CScene();
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
			FComponentContainer& container = m_components[T::getTypeHash()];
			size_t index = container.findComponentIndex<T>(guid);

			if (index == -1) {
				return false;
			}

			if (m_initialized) {
				m_componentLifeCycle[T::getTypeHash()].shutdown((void*)container.getComponentAtIndexUnscaled(index));
			}

			return container.removeComponentIndex<T>(index);
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

		inline const FComponentContainer& getComponents(const CHashString& typeHash) {
			return m_components[typeHash];
		}

		inline const FECSLifeCycle& getComponentLifeCycle(const CHashString& typeHash) const {
			return m_componentLifeCycle[typeHash];
		}

		const std::vector<CEntity*>& getEntities() const {
			return m_entities;
		}

	private:
		std::vector<CEntity*> m_entities;
		std::unordered_map<CHashString, FComponentContainer> m_components;
		CGuid m_guid;
		static std::unordered_map<CHashString, FECSLifeCycle> m_componentLifeCycle;
		bool m_initialized = false;
	};

}
