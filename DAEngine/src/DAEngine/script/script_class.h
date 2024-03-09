#pragma once

namespace da::script
{
	class CScriptClass {
	public:
		CScriptClass(const std::string& path, const std::string& classType, const std::string& objName);

		void setup(const da::core::CGuid& parent, const da::core::CGuid& guid);
		void cleanup(bool keepObj = false);

		void classInitialize();
		void classUpdate(float dt);
		void classShutdown();

		inline const std::string& getPath() const { return m_path; }
		inline const std::string& getClass() const { return m_classType; }
		inline const std::string& getObjName() const { return m_objName; }

	private:
		std::string m_path, m_classType, m_objName;
		int m_baseRef = 0, m_classRef = 0, m_funcRef = 0, m_objRef = 0;
		int m_initRef = 0, m_updateRef = 0, m_shutdownRef = 0;
		int m_getIdRef = 0, m_setIdRef = 0, m_setEntityRef = 0, m_getEntityRef = 0;

		void* m_state = nullptr;

	};
}
