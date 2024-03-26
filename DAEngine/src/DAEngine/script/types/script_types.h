#pragma once
#include <sol/sol.hpp>

namespace da::script
{

#define GET_SCRIPT_TYPE(type, obj) getSolType<type>(obj, HASHSTR(#type))
#define CREATE_SCRIPT_TYPE(view, type, obj) createSolType<type>(view, obj, HASHSTR(#type))

	enum class EScriptDataType {
		Int,
		Float,
		String,
		Table
	};

	struct FScriptGuid {
		std::string Index;
	};

	struct FScriptVector2 {
		inline FScriptVector2() {};
		inline FScriptVector2(glm::vec2 vec) : x(vec.x), y(vec.y){

		}
		float x, y;

		inline glm::vec2 toGlm() { return { x,y }; };
	};

	struct FScriptVector3 {
		inline FScriptVector3() {};
		inline FScriptVector3(glm::vec3 vec) : x(vec.x), y(vec.y), z(vec.z) {

		}
		float x, y, z;
		inline glm::vec3 toGlm() { return { x,y,z }; };
	};

	struct FScriptType {
		std::string Name;
		size_t Offset;
		EScriptDataType DataType;
		CHashString TypeName = {};

		FScriptType(std::string name, size_t offset, EScriptDataType dataType, CHashString typeName) {
			Name = name;
			Offset = offset;
			TypeName = typeName;
			DataType = dataType;
		}
	};

	struct FScriptTypeFunc {
		std::function<void(sol::object, void*)> GetFunc;
		std::function<sol::object(sol::state_view* view, void*)> CreateFunc;
	};

	class CScriptTypes
	{
	public:
		static void registerTypes();
		static std::unordered_map<CHashString, FScriptTypeFunc>& getTypes();
	private:
		static std::unordered_map<CHashString, FScriptTypeFunc> ms_types;
	};

	template<typename T>
	static T getSolType(sol::object obj, CHashString typeName) {
		T res;
		CScriptTypes::getTypes()[typeName].GetFunc(obj, &res);
		return res;
	}

	template<typename T>
	static sol::object createSolType(sol::state_view* view, T* obj, CHashString typeName) {
		return CScriptTypes::getTypes()[typeName].CreateFunc(view, obj);
	}
}