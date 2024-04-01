#include "dapch.h"

#include "script_engine.h"
#include "asset/asset.h"
#include <chrono>
#include <algorithm>
#include "natives/script_native.h"
#include "types/script_types.h"


extern "C" {
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

#ifdef DA_PLATFORM_WINDOWS
#define SOL_EXCEPTIONS_SAFE_PROPAGATION 1
#define SOL_LUAJIT 1
#endif
#include <sol/sol.hpp>

namespace da::script
{
	CScriptEngine* CScriptEngine::s_instance = nullptr;
	std::unordered_map<uint32_t, int> CScriptEngine::s_scriptMap;

	extern "C" static int lua_requires(lua_State * L)
	{
		char buffer[1024] = "scripts/build/";
		const char* path = luaL_checkstring(L, 1);
		size_t pathLen = strlen(path);
		memcpy(&buffer[14], path, pathLen);
        for (size_t i = 14; i < pathLen+14; i++) {
            if (buffer[i] == '.') {
                buffer[i] = '/';
                break;
            }
        }
		char ext[] = ".lua";
		memcpy(&buffer[pathLen + 14], ext, sizeof(ext));
		buffer[pathLen + 14 + sizeof(ext)] = '\0';


		//std::string filePath = "scripts/" + std::string(path) + ".lua";

		//LOG_INFO(ELogChannel::Script, "Attempting to require script: %s", filePath.c_str());
		
		int ref = CScriptEngine::getScript(buffer, true);
		lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
		//lua_call(L, 0, 1);
		return 1;
	}

	extern "C" static int lua_print(lua_State * L)
	{
		int top = lua_gettop(L);

		std::string str;

		if (top == 0)
			return 0;

		for (unsigned index = 1; index <= top; index++)
		{
			int type = lua_type(L, index);
			switch (type)
			{
				// booleans
			case LUA_TBOOLEAN:
				str = str + (lua_toboolean(L, index) ? "true" : "false");
				break;

				// numbers
			case LUA_TNUMBER:
				str = str + std::to_string(lua_tonumber(L, index));
				break;

				// strings
			case LUA_TSTRING:
				str = str + lua_tostring(L, index);
				break;

				// other
			default:
				//str = str + lua_typename(L, type);
				break;
			}
		}

		LOG_DEBUG(ELogChannel::Script, "%s", str.c_str());
		return 0;
	}

	extern "C" int lua_break(lua_State * L)
	{
		sol::state_view lua = sol::state_view(L);
		auto result = lua.script(
			"if os.getenv('LOCAL_LUA_DEBUGGER_VSCODE') == '1' then \
			require('lldebugger').start() \
			end\
			");
		return 0;
	}


	struct FTestSubType {
		int var1 = 5;
		int var2 = 10;
	};

	struct FTestType {
		int var1 = 0;
		std::string var2 = {};
		FTestSubType var3 = {};
	};

	void my_panic(sol::optional<std::string> maybe_msg) {
		std::cerr << "Lua is in a panic state and will now abort() the application" << std::endl;
		if (maybe_msg) {
			const std::string& msg = maybe_msg.value();
			std::cerr << "\terror message: " << msg << std::endl;
		}
		// When this function exits, Lua will exhibit default behavior and abort()
	}

	int my_exception_handler(lua_State* L, sol::optional<const std::exception&> maybe_exception, sol::string_view description) {
		// L is the lua state, which you can wrap in a state_view if necessary
		// maybe_exception will contain exception, if it exists
		// description will either be the what() of the exception or a description saying that we hit the general-case catch(...)
		std::cout << "An exception occurred in a function, here's what it says ";
		if (maybe_exception) {
			std::cout << "(straight from the exception): ";
			const std::exception& ex = *maybe_exception;
			std::cout << ex.what() << std::endl;
		}
		else {
			std::cout << "(from the description parameter): ";
			std::cout.write(description.data(), static_cast<std::streamsize>(description.size()));
			std::cout << std::endl;
		}

		// you must push 1 element onto the stack to be
		// transported through as the error object in Lua
		// note that Lua -- and 99.5% of all Lua users and libraries -- expects a string
		// so we push a single string (in our case, the description of the error)
		return sol::stack::push(L, description);
	}

	static sol::function s_debugger;

	void CScriptEngine::initialize()
	{
		if (s_instance) return;
		s_instance = new CScriptEngine();

		s_instance->m_state = lua_open();
		luaL_openlibs(s_instance->m_state);

#ifdef DA_PLATFORM_WINDOWS
		lua_pushcfunction(s_instance->m_state, luaopen_jit);
		lua_pushstring(s_instance->m_state, LUA_JITLIBNAME);
		lua_call(s_instance->m_state, 1, 0);
#endif
		s_instance->m_stateView = new sol::state_view(s_instance->m_state);
		s_instance->m_stateView->set_panic(sol::c_call<decltype(&my_panic), &my_panic>);
		s_instance->m_stateView->open_libraries(sol::lib::base, sol::lib::debug, sol::lib::package, sol::lib::jit, sol::lib::os);
		s_instance->m_stateView->set_exception_handler(&my_exception_handler);

		da::script::CScriptTypes::registerTypes();
		registerFunctions();
		registerNatives(s_instance->m_state, s_instance->m_stateView);
	}

	void CScriptEngine::registerFunctions()
	{
		lua_register(s_instance->m_state, "require", lua_requires);
		lua_register(s_instance->m_state, "print", lua_print);
		lua_register(s_instance->m_state, "_break", lua_break);
	}

	bool CScriptEngine::hasScript(const CHashString& hash)
	{
		const std::unordered_map<uint32_t, int>::iterator it = s_scriptMap.find(hash.hash());

		return it != s_scriptMap.end();
	}

	int CScriptEngine::getScript(const char* path, bool cacheResult)
	{
		uint32_t hash = HASH(path);
		const std::unordered_map<uint32_t, int>::iterator it = s_scriptMap.find(hash);

		if (it == s_scriptMap.end()) {
			CAsset asset(path);
			int load_stat = luaL_loadbuffer(s_instance->m_state, asset.data(), asset.size(), path);

			if (cacheResult) {
				lua_call(s_instance->m_state, 0, 1);
			}

			s_scriptMap[hash] = luaL_ref(s_instance->m_state, LUA_REGISTRYINDEX);
			return s_scriptMap[hash];
		}

		return it->second;
	}

    void CScriptEngine::clearAll() {
        s_scriptMap.clear();
    }


	void CScriptEngine::readTable(int idx) {
		if (lua_istable(s_instance->m_state, idx))
		{
			lua_pushnil(s_instance->m_state);
			while (lua_next(s_instance->m_state, idx - 1)) {
				const char* key = lua_tostring(s_instance->m_state, idx - 1);
				int type = lua_type(s_instance->m_state, idx);
				switch (type)
				{
					// booleans
				case LUA_TBOOLEAN:
					LOG_ERROR(ELogChannel::Script, "bool");
					break;

					// numbers
				case LUA_TNUMBER:
					LOG_ERROR(ELogChannel::Script, "num");
					break;

					// strings
				case LUA_TSTRING:
					LOG_ERROR(ELogChannel::Script, "str");
					break;

				case LUA_TFUNCTION:
					LOG_ERROR(ELogChannel::Script, "func");
					break;
					// other
				case LUA_TTABLE:
					//read_table(idx);
					break;
				default:
					//str = str + lua_typename(L, type);
					break;
				}
				lua_pop(s_instance->m_state, 1);
			}

			lua_pop(s_instance->m_state, 1);
		}
	}

	void* CScriptEngine::loadScript(const char* path)
	{
		int ref = CScriptEngine::getScript(path, false);
		lua_rawgeti(s_instance->m_state, LUA_REGISTRYINDEX, ref);
		if (lua_pcall(s_instance->m_state, 0, 1, 0) != LUA_OK) {
			LOG_ERROR(ELogChannel::Script, "%s failed: %s", path, lua_tostring(s_instance->m_state, -1));
		}

		return s_instance->m_state;
	}

	void CScriptEngine::unloadScript(const CHashString& hash)
	{
		const auto& it = s_scriptMap.find(hash.hash());
		if (it == s_scriptMap.end()) return;
		luaL_unref(s_instance->m_state, LUA_REGISTRYINDEX, it->second);
		s_scriptMap.erase(it);
	}

	void CScriptEngine::shutdown()
	{
		if (!s_instance) return;

		lua_close(s_instance->m_state);
		s_instance->clearAll();
		s_instance = nullptr;
	}

	lua_State* CScriptEngine::getState()
	{
		ASSERT(s_instance);
		return s_instance->m_state;
	}

	sol::state_view* CScriptEngine::getStateView()
	{
		return s_instance->m_stateView;
	}

	std::string CScriptEngine::getCallstack(lua_State* L)
	{
		sol::state_view lua(L);
		sol::function traceFunc = lua["debug"]["traceback"];
		
		sol::function_result traceResult = traceFunc();

		std::string trace = traceResult.get<std::string>();

		return trace;
	}

	}
