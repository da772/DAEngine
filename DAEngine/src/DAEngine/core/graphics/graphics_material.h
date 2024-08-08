#pragma once
#include "DAEngine/core/core.h"
#include "Daengine/core/factory.h"

namespace da::graphics
{
	class CMaterialFactory;
	class CMaterial;

	struct FMaterialInfo
	{
		CMaterial* Material;
		uint16_t Count;
	};

	class CMaterial
	{
	public:
		virtual uint16_t getHandle() const { return -1; };
		inline virtual CHashString getHash() const { return m_hash; };

	protected:
		virtual void initialize() {};
		virtual void shutdown() {};
		virtual ~CMaterial() {};


	protected:
		CHashString m_hash;

		friend class da::core::CFactory<CMaterial>;
		friend class CMaterialFactory;
	};

	class CMaterialFactory : da::core::CFactory<CMaterial>
	{
	public:
		static CMaterial* create(const std::string& vsPath, const std::string& fsPath);
		static CMaterial* create(const std::string& csPath);

		static void remove(CMaterial* material);

#ifdef DA_REVIEW
		static void reloadShaders();
#endif

	private:
#ifdef DA_REVIEW
		bool m_debug;
		friend class da::core::CFactoryDebug;
#endif

		static CMaterialFactory ms_factory;
	};
}