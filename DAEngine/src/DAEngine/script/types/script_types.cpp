#include "dapch.h"
#include "script_types.h"
#include <sol/sol.hpp>
#include "script/script_engine.h"

#define SCRIPT_TYPE(type, name, solType) FScriptType(#name, offsetOf(&type::name), solType, CHashString())
#define SCRIPT_TYPE_OBJECT(type, name, solType, objType)  FScriptType(#name, offsetOf(&type::name), solType, HASHSTR(#objType))
#define REGISTER_SCRIPT_TYPE(type, ...) registerSolType<type>(HASHSTR(#type), { __VA_ARGS__ })

namespace da::script
{

	std::unordered_map<CHashString, FScriptTypeFunc> CScriptTypes::ms_types;
	template<typename T>
	static void registerSolType(CHashString name, std::vector<FScriptType> solTypes);
	template<typename T, typename U>
	static constexpr size_t offsetOf(U T::* member);

	void CScriptTypes::registerTypes()
	{
		REGISTER_SCRIPT_TYPE(FScriptGuid, SCRIPT_TYPE(FScriptGuid, Index, EScriptDataType::String));
		REGISTER_SCRIPT_TYPE(FScriptVector3,
			SCRIPT_TYPE(FScriptVector3, x, EScriptDataType::Float),
			SCRIPT_TYPE(FScriptVector3, y, EScriptDataType::Float),
			SCRIPT_TYPE(FScriptVector3, z, EScriptDataType::Float));
		REGISTER_SCRIPT_TYPE(FScriptVector2,
			SCRIPT_TYPE(FScriptVector2, x, EScriptDataType::Float),
			SCRIPT_TYPE(FScriptVector2, y, EScriptDataType::Float));
	}

	std::unordered_map<CHashString, FScriptTypeFunc>& CScriptTypes::getTypes()
	{
		return ms_types;
	}

	template<typename T, typename U>
	static constexpr size_t offsetOf(U T::* member)
	{
		return (char*)&((T*)nullptr->*member) - (char*)nullptr;
	}

	template<typename T>
	static void getFunc(sol::object o, void* out, std::vector<FScriptType> solTypes, CHashString typeName)
	{
		T obj;
		sol::type oType = o.get_type();
		LOG_SASSERT((oType != sol::type::lua_nil), o.lua_state(), "Type is nil when converting to type: %s", typeName.c_str());
		for (size_t i = 0; i < solTypes.size(); i++) {
			FScriptType type = solTypes[i];

			sol::object o1 = o.as<sol::table>()[type.Name];
			sol::type tt = o1.get_type();
			switch (tt)
			{
				case sol::type::number: {
					char* ptr = &((char*)&obj)[type.Offset];
					if (type.DataType == EScriptDataType::Int) {
						*(int*)ptr = o1.as<int>();
					}
					else {
						*(float*)ptr = o1.as<float>();
					}
					break;
				}
				case sol::type::string:
				{
					char* ptr = &((char*)&obj)[type.Offset];
					std::string val = o1.as<std::string>();
					*(std::string*)ptr = val;
					break;
				}
				case sol::type::table:
				{
					char* ptr = &((char*)&obj)[type.Offset];
					CScriptTypes::getTypes()[type.TypeName].GetFunc(o1, ptr);
					break;
				}
				default:
				{
					LOG_SASSERT(false, o.lua_state(), "Type is %d when converting to type: %s", (int)tt, typeName.c_str());
					break;
				}
			}
		}
		*(T*)out = obj;
	}

	template<typename T>
	static sol::object createType(std::vector<FScriptType> solTypes, sol::state_view* view, void* object, CHashString name) {
		sol::table table = view->create_named_table("Vector3");
		for (size_t i = 0; i < solTypes.size(); i++) {
			FScriptType type = solTypes[i];
			char* ptr = &((char*)object)[type.Offset];
			switch (type.DataType)
			{
			case EScriptDataType::Int:
				table[type.Name] = *(int*)ptr;
				break;
			case EScriptDataType::Float: {
				table[type.Name] = *(float*)ptr;
				break;
			}
			case EScriptDataType::String:
				table[type.Name] = *(std::string*)ptr;
				break;
			case EScriptDataType::Table:
				table[type.Name] = CScriptTypes::getTypes()[type.TypeName].CreateFunc(view, ptr);
				break;
			default:
				LOG_SASSERT(false, view->lua_state(), "Type is %d when creating type: %s", (int)type.DataType, name.c_str());
				break;
			}
		}

		return table;
	}

	template<typename T>
	static void registerSolType(CHashString name, std::vector<FScriptType> solTypes)
	{
		FScriptTypeFunc funcs;

		funcs.GetFunc = [solTypes, name] (sol::object o, void* out) {
			getFunc<T>(o, out, solTypes, name);
		};

		funcs.CreateFunc = [solTypes, name](sol::state_view* view, void* object) {
			return createType<T>(solTypes, view, object, name);
		};

		CScriptTypes::getTypes()[name] = funcs;
	}

}