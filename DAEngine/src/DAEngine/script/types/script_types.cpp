
#include "script_types.h"

namespace da
{

	std::unordered_map<CHashString, FScriptTypeFunc> CScriptTypes::ms_types;

	void CScriptTypes::registerTypes()
	{
		REGISTER_SCRIPT_TYPE(FScriptGuid, SCRIPT_TYPE(FScriptGuid, Index, EScriptDataType::String));
		REGISTER_SCRIPT_TYPE(glm::vec4,
			SCRIPT_TYPE(glm::vec4, x, EScriptDataType::Float),
			SCRIPT_TYPE(glm::vec4, y, EScriptDataType::Float),
			SCRIPT_TYPE(glm::vec4, z, EScriptDataType::Float),
			SCRIPT_TYPE(glm::vec4, w, EScriptDataType::Float));
		REGISTER_SCRIPT_TYPE(glm::vec3,
			SCRIPT_TYPE(glm::vec3, x, EScriptDataType::Float),
			SCRIPT_TYPE(glm::vec3, y, EScriptDataType::Float),
			SCRIPT_TYPE(glm::vec3, z, EScriptDataType::Float));
		REGISTER_SCRIPT_TYPE(glm::vec2,
			SCRIPT_TYPE(glm::vec2, x, EScriptDataType::Float),
			SCRIPT_TYPE(glm::vec2, y, EScriptDataType::Float));

		REGISTER_SCRIPT_TYPE(da::FMaterialData,
			SCRIPT_TYPE_OBJECT(da::FMaterialData, baseColorFactor, glm::vec4),
			SCRIPT_TYPE_OBJECT(da::FMaterialData, uvScale, glm::vec2),
			SCRIPT_TYPE_OBJECT(da::FMaterialData, emissiveFactor, glm::vec3),
			SCRIPT_TYPE(da::FMaterialData, roughnessFactor, EScriptDataType::Float),
			SCRIPT_TYPE(da::FMaterialData, metallicFactor, EScriptDataType::Float),
			SCRIPT_TYPE(da::FMaterialData, normalScale, EScriptDataType::Float),
			SCRIPT_TYPE(da::FMaterialData, occlusionStrength, EScriptDataType::Float),
			SCRIPT_TYPE(da::FMaterialData, m_baseColorTexture, EScriptDataType::Ptr),
			SCRIPT_TYPE(da::FMaterialData, m_emissiveTexture, EScriptDataType::Ptr),
			SCRIPT_TYPE(da::FMaterialData, m_occlusionTexture, EScriptDataType::Ptr),
			SCRIPT_TYPE(da::FMaterialData, m_normalTexture, EScriptDataType::Ptr),
			SCRIPT_TYPE(da::FMaterialData, m_metallicRoughnessTexture, EScriptDataType::Ptr),
			);
	}

	std::unordered_map<CHashString, FScriptTypeFunc>& CScriptTypes::getTypes()
	{
		return ms_types;
	}
}