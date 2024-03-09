#pragma once
#include "DAEngine/core/core.h"

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

		friend class CMaterialFactory;

	};

	class CMaterialFactory
	{
	public:
		static CMaterial* create(const std::string& vsPath, const std::string& fsPath);
		static CMaterial* create(const std::string& csPath);

		static void remove(CMaterial* material);

#if defined(DA_DEBUG) || defined(DA_RELEASE)
		static void reloadShaders();
#endif

	private:
#if defined(DA_DEBUG) || defined(DA_RELEASE)
		static bool m_debug;
#endif

	private:
		static std::unordered_map<CHashString, FMaterialInfo> ms_materials;
	};
}