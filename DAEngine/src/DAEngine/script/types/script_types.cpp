#include "dapch.h"
#include "script_types.h"

namespace da::script
{

	std::unordered_map<CHashString, FScriptTypeFunc> CScriptTypes::ms_types;

	void CScriptTypes::registerTypes()
	{
		REGISTER_SCRIPT_TYPE(FScriptGuid, SCRIPT_TYPE(FScriptGuid, Index, EScriptDataType::String));
		REGISTER_SCRIPT_TYPE(glm::vec3,
			SCRIPT_TYPE(glm::vec3, x, EScriptDataType::Float),
			SCRIPT_TYPE(glm::vec3, y, EScriptDataType::Float),
			SCRIPT_TYPE(glm::vec3, z, EScriptDataType::Float));
		REGISTER_SCRIPT_TYPE(glm::vec2,
			SCRIPT_TYPE(glm::vec2, x, EScriptDataType::Float),
			SCRIPT_TYPE(glm::vec2, y, EScriptDataType::Float));
	}

	std::unordered_map<CHashString, FScriptTypeFunc>& CScriptTypes::getTypes()
	{
		return ms_types;
	}
}