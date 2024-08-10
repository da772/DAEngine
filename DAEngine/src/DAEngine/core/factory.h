#pragma once
#include "core.h"
#include "core/logger.h"

namespace da::core
{
#ifdef DA_REVIEW
	class CFactoryDebug;
#endif

	template<typename T>
	class CFactory
	{
	public:
	protected:
		struct FInstanceInfo
		{
			T* Ptr;
			uint32_t Count;
		};

		bool has(CHashString hash);
		T* addInstance(CHashString hash, std::function<T*()> instanceFactory);
		void removeInstance(CHashString hash);
		std::unordered_map<CHashString, FInstanceInfo> m_instances;

#ifdef DA_REVIEW
		bool hasInstances();
		std::vector<std::pair<CHashString, uint32_t>> getInstanceNames();
		friend class CFactoryDebug;
#endif

	};

#ifdef DA_REVIEW
	template<typename T>
	std::vector<std::pair<CHashString, uint32_t>> da::core::CFactory<T>::getInstanceNames()
	{
		std::vector<std::pair<CHashString, uint32_t>> out;
		for (const std::pair<CHashString, FInstanceInfo>& kv : m_instances)
		{
			out.push_back({ kv.first, kv.second.Count });
		}

		return out;
	}
#endif

#ifdef DA_REVIEW
	class CFactoryDebug
	{
	public:
		static void checkInstances();
	private:
		static void printInstances(const std::vector<std::pair<CHashString, uint32_t>>& instances);
	};
#endif


#ifdef DA_REVIEW
	template<typename T>
	bool da::core::CFactory<T>::hasInstances()
	{
		return m_instances.size();
	}
#endif

	template<typename T>
	bool CFactory<T>::has(CHashString hash)
	{
		return m_instances.find(hash) != m_instances.end();
	}


	template<typename T>
	T* CFactory<T>::addInstance(CHashString hash, std::function<T* ()> instanceFactory)
	{
		const std::unordered_map<CHashString, FInstanceInfo>::iterator& it = m_instances.find(hash);

		if (it == m_instances.end())
		{
			T* instance = instanceFactory();
			m_instances[hash] = { instance, 1 };
			return instance;
		}

		it->second.Count++;
		return it->second.Ptr;
	}


	template<typename T>
	void CFactory<T>::removeInstance(CHashString hash)
	{
		const std::unordered_map<CHashString, FInstanceInfo>::iterator& it = m_instances.find(hash);

		if (it == m_instances.end())
		{
			return;
		}
		--it->second.Count;

		if (it->second.Count == 0)
		{
			delete it->second.Ptr;
			m_instances.erase(it);
		}

	}

}

