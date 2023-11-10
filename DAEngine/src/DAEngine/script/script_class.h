#pragma once

namespace da::script
{
	class CScriptClass {
	public:
		CScriptClass(const std::string& path, const std::string& classType, const std::string& objName);
		CScriptClass();

		void setup();
		void cleanup();

		void classInitialize();
		void classUpdate(float dt);
		void classShutdown();


	private:
		std::string m_path, m_classType, m_objName;
		int m_baseRef = 0, m_classRef = 0, m_funcRef = 0, m_objRef = 0;
		int m_initRef = 0, m_updateRef = 0, m_shutdownRef = 0;

		void* m_state = nullptr;

	};
}