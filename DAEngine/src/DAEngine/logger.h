#pragma once
#include "core/core.h"
#include "core/containers.h"
#include "core/utility.h"

#include <stdio.h>

namespace da {
#define __LOGFUNC__(x, y) template <typename ...Args> \
inline static void x(const ELogChannel& channel, const CString& message, Args ... args) {\
CLogger::log(y, channel, message, args...); }

	enum class ELogChannel : uint8_t {
		Core,
		Container,
		Maths,
		Modules,
		Platform,
		External
	};

	enum class ELogType : uint8_t
	{
		Trace,
		Debug,
		Warning,
		Error,
		Assert
	};

	enum class ELogColor : uint8_t
	{
		Green,
		Cyan,
		Yellow,
		Red,
		Default,
	};

	class CLogger
	{
	public:
		__LOGFUNC__(LogDebug, ELogType::Debug);
		__LOGFUNC__(LogTrace, ELogType::Trace);
		__LOGFUNC__(LogWarning, ELogType::Warning);
		__LOGFUNC__(LogError, ELogType::Error);
		template <typename ...Args> 
		inline static void LogAssert(bool assert, const ELogChannel& channel, const CString& message, Args ... args) {
			if (assert) return;
			CLogger::log(ELogType::Assert, channel, message, args...);
			ASSERT(assert);
		}

		static void initialize();
		static void shutdown();

	private:
		template <typename ...Args>
		inline static void log(ELogType type, ELogChannel channel, const CString& message, Args ... args)
		{
			char buffer[4096];
			const CString msg = "%s[%s] %s: [%s] " + message + "\n%s";
			sprintf_s(buffer, sizeof(buffer), msg.cstr(), colorTypeMap[(uint8_t)type], utility::CurrentDateTime().cstr(), logTypeMap[(uint8_t)type], logChannelMap[(uint8_t)channel], args..., colorTypeMap[(uint8_t)ELogColor::Default]);

			logInternal(CString(buffer));
		}
		static void logInternal(const CString& message);

		inline static const char* logChannelMap[] = { "Core", "Container", "Maths", "Modules", "Platform", "External" };
		inline static const char* logTypeMap[] = { "Trace", "Debug", "Warning", "Error", "Assert" };
		inline static const char* colorTypeMap[] = { "\033[32m", "\033[36m", "\033[33m", "\033[31m", "\033[39m" };

		static void* s_outFile;
	};


}