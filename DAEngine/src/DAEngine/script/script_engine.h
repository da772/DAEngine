#pragma once

extern "C" {
	typedef struct lua_State lua_State;
}

namespace da::script
{

	class CScriptEngine {

	public:
		static void initialize();
		static void shutdown();
		static void load_script(const char* path);

	private:
		static void register_functions();

	private:
		lua_State* m_state;


	private:
		static CScriptEngine* s_instance;


	};

}
