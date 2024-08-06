#pragma once

namespace da::core {

	class CEntity;

#define COMPONENT_DEFAULT_ALLOC 25

	struct FComponentContainer {
	public:
		template <typename T, typename... Args>
		T* createComponent(Args&&... args) {

			if (m_components.empty()) {
				m_components.reserve(sizeof(T) * COMPONENT_DEFAULT_ALLOC);
			}

			m_size = sizeof(T);
			m_components.insert(m_components.end(), sizeof(T), 0);
			LOG_ASSERT( (m_components.capacity() <= (size_t)(sizeof(T) * COMPONENT_DEFAULT_ALLOC)), ELogChannel::Core, "Component Arena Full %s", T::getTypeHash().c_str());
			char* ptr = &m_components[m_count * sizeof(T)];
			new ((T*)ptr)T (std::forward<Args>(args)...);
#ifdef DA_REVIEW
			LOG_ASSERT(((T*)ptr)->getId().isValid(), ELogChannel::Core, "Creating component with invalid id");
#endif
			CHashString hash((const char*)((T*)ptr)->getId().data(), sizeof(uint128_t));
			m_componentCache[hash] = (uint32_t)m_count;
			m_count++;
			return (T*)ptr;
		}


		template <typename T>
		T* findComponent(const CGuid& guid) {

			CHashString hash((const char*)guid.data(), sizeof(uint128_t));
			const std::unordered_map<CHashString, uint32_t>::const_iterator& it = m_componentCache.find(hash);

			if (it != m_componentCache.end())
			{
				return (T*)&m_components[it->second * sizeof(T)];
			}

			for (size_t i = 0; i < m_components.size(); i += sizeof(T)) {
				T* c = (T*) &m_components[i];
				if (c->getId() == guid) {
					m_componentCache[hash] = (uint32_t)i/sizeof(T);
					return c;
				}
			}

			return nullptr;
		}

		template <typename T>
		size_t findComponentIndex(const CGuid& guid) {

			CHashString hash((const char*)guid.data(), sizeof(uint128_t));
			const std::unordered_map<CHashString, uint32_t>::const_iterator& it = m_componentCache.find(hash);

			if (it != m_componentCache.end())
			{
				return it->second * sizeof(T);
			}

			for (size_t i = 0; i < m_components.size(); i += sizeof(T)) {
				T* c = (T*)&m_components[i];
				if (c->getId() == guid) {
					m_componentCache[hash] = (uint32_t)i/sizeof(T);
					return i;
				}
			}

			return -1;
		}

#ifdef DA_REVIEW
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
			dirty();
			return true;
		}

		template <typename T>
		bool removeComponentIndex(size_t index) {
			if (index == -1) return false;

			T* component = (T*)&m_components[index];
			component->~T();
			m_components.erase(m_components.begin() + index, m_components.begin() + index + sizeof(T));
			m_count--;

			dirty();
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

		inline size_t getComponentSize() const {
			return m_size;
		}

	private:
		void dirty();


	private:
		std::vector<char> m_components;
		size_t m_count;
		size_t m_size;
		std::unordered_map<CHashString, uint32_t> m_componentCache;
	};

	struct FECSLifeCycle {
		void (*init)(void*);
		void (*update)(void*, float);
		void (*shutdown)(void*);
#ifdef DA_REVIEW
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
			T* component = m_components[typeHash].createComponent<T>(std::forward<Args>(args)...);
			if (m_initialized) {
				getComponentLifeCycle<T>().init((void*)component);
			}

			return component;
		}

		template <typename T>
		bool destroyComponent(const CGuid& guid) {
			FComponentContainer& container = m_components[T::getTypeHash()];
			size_t index = container.findComponentIndex<T>(guid);

			if (index == -1) {
				return false;
			}

			if (m_initialized) {
				getComponentLifeCycle<T>().shutdown((void*)container.getComponentAtIndexUnscaled(index));
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
			m_componentLifeCycle.push_back({ T::getTypeHash() , lifeCycle });
		}
        
		template <typename T>
		bool hasComponents() {
			const CHashString& typeHash = T::getTypeHash();
			return m_components.find(typeHash) != m_components.end();
		}

        template <typename T>
        const FComponentContainer& getComponents() {
            const CHashString& typeHash = T::getTypeHash();
            return m_components[typeHash];
        }

		inline const FComponentContainer& getComponents(const CHashString& typeHash) {
			return m_components[typeHash];
		}

		template <typename T>
		inline const FECSLifeCycle& getComponentLifeCycle() const {
			const CHashString& typeHash = T::getTypeHash();
			for (const std::pair<CHashString, FECSLifeCycle>& it : m_componentLifeCycle) {
				if (it.first == typeHash) {
					return it.second;
				}
			}

			ASSERT(false);
			return m_componentLifeCycle[-1].second;
		}

		inline const FECSLifeCycle& getComponentLifeCycle(const CHashString& typeHash) const {
			for (const std::pair<CHashString, FECSLifeCycle>& it : m_componentLifeCycle) {
				if (it.first == typeHash) {
					return it.second;
				}
			}

			ASSERT(false);
			return m_componentLifeCycle[-1].second;
		}

		const std::vector<CEntity*>& getEntities() const {
			return m_entities;
		}

		const CEntity* getEntityFromTag(CHashString tag) const;

#ifdef DA_REVIEW
		inline const std::unordered_map<CHashString, FComponentContainer>& getComponentContainers() const { return m_components; }
#endif

	private:
		std::vector<CEntity*> m_entities;
		std::unordered_map<CHashString, FComponentContainer> m_components;
		CGuid m_guid;
		static std::vector<std::pair<CHashString, FECSLifeCycle>> m_componentLifeCycle;
		bool m_initialized = false;
	};

}
