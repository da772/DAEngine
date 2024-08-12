#pragma once

#include "core/factory.h"
#include "dastd.h"
#include "graphics/graphics_material.h"
#include "dacommon.h"

using namespace da::graphics;

namespace da::factory
{
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