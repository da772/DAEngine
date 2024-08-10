#pragma once
#include <sol/sol.hpp>
#include "script/script_engine.h"

#include "graphics/graphics_material_data.h"

#define GET_SCRIPT_TYPE(type, obj) da::script::getSolType<type>(obj, HASHSTR(#type))
#define CREATE_SCRIPT_TYPE(view, type, obj) da::script::createSolType<type>(view, obj, HASHSTR(#type))
#define REGISTER_SCRIPT_TYPE(type, ...) da::script::registerSolType<type>(HASHSTR(#type), { __VA_ARGS__ })

#define SCRIPT_TYPE(type, name, solType) da::script::FScriptType(#name, da::script::CScriptTypes::offsetOf(&type::name), solType, CHashString())
#define SCRIPT_ARR_TYPE(type, name, solType) da::script::FScriptType(#name, da::script::CScriptTypes::offsetOf(&type::name),  da::script::EScriptDataType::Vector, CHashString(), solType)
#define SCRIPT_TYPE_OBJECT(type, name, objType)  da::script::FScriptType(#name, da::script::CScriptTypes::offsetOf(&type::name), da::script::EScriptDataType::Object, HASHSTR(#objType))
#define SCRIPT_ARR_TYPE_OBJECT(type, name, objType)  da::script::FScriptType(#name, da::script::CScriptTypes::offsetOf(&type::name), da::script::EScriptDataType::Vector, HASHSTR(#objType), da::script::EScriptDataType::Object, sizeof(objType))


namespace da::script
{
	enum class EScriptDataType {
		Invalid,
		Int,
		Float,
		Ptr,
		Bool,
		String,
		Object,
		Vector
	};

	struct FScriptGuid {
		std::string Index;
	};

	struct FScriptType {
		std::string Name;
		size_t Offset;
		EScriptDataType DataType;
		EScriptDataType ChildDataType;
		size_t ChildSize;
		CHashString TypeName = {};

		FScriptType(std::string name, size_t offset, EScriptDataType dataType, CHashString typeName, EScriptDataType childType = EScriptDataType::Invalid, size_t childSize = 0) {
			Name = name;
			Offset = offset;
			TypeName = typeName;
			DataType = dataType;
			ChildDataType = childType;
			ChildSize = childSize;
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
		template<typename T, typename U>
		static constexpr size_t offsetOf(U T::* member)
		{
			return (char*)&((T*)nullptr->*member) - (char*)nullptr;
		}
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
	static void registerSolType(CHashString name, std::vector<FScriptType> solTypes)
	{
		FScriptTypeFunc funcs;

		funcs.GetFunc = [solTypes, name](sol::object o, void* out) {
			getFunc<T>(o, out, solTypes, name);
			};

		funcs.CreateFunc = [solTypes, name](sol::state_view* view, void* object) {
			return createType<T>(solTypes, view, object, name);
			};

		CScriptTypes::getTypes()[name] = funcs;
	}

	template<typename T>
	static sol::object createSolType(sol::state_view* view, T* obj, CHashString typeName) {
		return CScriptTypes::getTypes()[typeName].CreateFunc(view, obj);
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
			case sol::type::boolean:
			{
				char* ptr = &((char*)&obj)[type.Offset];
				*(bool*)ptr = o1.as<bool>();
				break;
			}
			case sol::type::number: {
				char* ptr = &((char*)&obj)[type.Offset];
				if (type.DataType == EScriptDataType::Int) {
					*(int*)ptr = o1.as<int>();
				}
				else if (type.DataType == EScriptDataType::Float) {
					*(float*)ptr = o1.as<float>();
				}
				else if (type.DataType == EScriptDataType::Ptr) {
					*(size_t*)ptr = o1.as<size_t>();
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
				if (type.DataType == EScriptDataType::Object)
				{
					CScriptTypes::getTypes()[type.TypeName].GetFunc(o1, ptr);
					break;
				}
				
				if (type.DataType == EScriptDataType::Vector) {
					sol::table t = o1.as<sol::table>();

					switch (type.ChildDataType)
					{
						case EScriptDataType::Object: {
							std::vector<char> cvec;
							cvec.resize(t.size() * type.ChildSize);
							for (const auto& key_value_pair : t) {
								int ii = key_value_pair.first.as<int>() - 1;
								sol::object value = key_value_pair.second;
								CScriptTypes::getTypes()[type.TypeName].GetFunc(value, &cvec[ii * type.ChildSize]);
							}
							*(std::vector<char>*)ptr = cvec;
							break;
						}
						case EScriptDataType::String: {
							std::vector<std::string> cvec;
							cvec.resize(t.size());
							for (const auto& key_value_pair : t) {
								int ii = key_value_pair.first.as<int>() - 1;
								cvec[ii] = key_value_pair.second.as<std::string>();
							}
							*(std::vector<std::string>*)ptr = cvec;
							break;
						}
						case EScriptDataType::Ptr:
						{
							std::vector<char> cvec;
							cvec.resize(t.size() * sizeof(size_t));
							for (const auto& key_value_pair : t) {
								int ii = key_value_pair.first.as<int>() - 1;
								sol::object value = key_value_pair.second;
								*(size_t*)&cvec[ii * sizeof(size_t)] = value.as<size_t>();
							}
							*(std::vector<char>*)ptr = cvec;
							break;
						}
						case EScriptDataType::Float:
						case EScriptDataType::Int:
						{
							std::vector<char> cvec;
							cvec.resize(t.size() * sizeof(int));
							for (const auto& key_value_pair : t) {
								int ii = key_value_pair.first.as<int>() - 1;
								sol::object value = key_value_pair.second;
								*(int*)&cvec[ii * sizeof(int)] = value.as<int>();
							}
							*(std::vector<char>*)ptr = cvec;
							break;
						}
					}
					
				}

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
		sol::table table = view->create_table();
		for (size_t i = 0; i < solTypes.size(); i++) {
			FScriptType type = solTypes[i];
			char* ptr = &((char*)object)[type.Offset];
			switch (type.DataType)
			{
			case EScriptDataType::Bool:
				table[type.Name] = *(bool*)ptr;
				break;
			case EScriptDataType::Int:
				table[type.Name] = *(int*)ptr;
				break;
			case EScriptDataType::Float: {
				table[type.Name] = *(float*)ptr;
				break;
			}
			case EScriptDataType::Ptr: {
				table[type.Name] = *(size_t*)ptr;
				break;
			}
			case EScriptDataType::String:
				table[type.Name] = *(std::string*)ptr;
				break;
			case EScriptDataType::Object:
				table[type.Name] = CScriptTypes::getTypes()[type.TypeName].CreateFunc(view, ptr);
				break;
			default:
				LOG_SASSERT(false, view->lua_state(), "Type is %d when creating type: %s", (int)type.DataType, name.c_str());
				break;
			}
		}

		return table;
	}
}