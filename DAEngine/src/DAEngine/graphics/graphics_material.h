#pragma once
#include "core/core.h"
#include "core/factory.h"
#include "dacommon.h"

namespace da::graphics
{
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
}