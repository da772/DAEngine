#pragma once
#include "core/core.h"
#include "core/containers.h"
#include "core/utility.h"

#include <stdio.h>
#include <fstream>

namespace da {
#define NAMEOF(x) #x

#define LOG_INFO(ELogChannel, msg, ...) da::CLogger::LogInfo(ELogChannel, std::string("[%s] ") + msg, __FUNCTION__  , ##__VA_ARGS__)
#define LOG_DEBUG(ELogChannel, msg, ...) da::CLogger::LogDebug(ELogChannel, std::string("[%s] ") + msg, __FUNCTION__  , ##__VA_ARGS__)
#define LOG_WARN(ELogChannel, msg, ...) da::CLogger::LogWarning(ELogChannel, std::string("[%s] ") + msg, __FUNCTION__  , ##__VA_ARGS__)
#define LOG_ERROR(ELogChannel, msg, ...) da::CLogger::LogError(ELogChannel, std::string("[%s] ") + msg, __FUNCTION__  , ##__VA_ARGS__)
#define LOG_ASSERT(x, ELogChannel, msg, ...) da::CLogger::LogAssert(x, ELogChannel, std::string("[%s] ") + msg, __FUNCTION__  , ##__VA_ARGS__); if (!x) { ASSERT(x); }
#define LOG_CALLSTACK(ELogType, ELogChannel) da::CLogger::log(ELogType, ELogChannel, ::std::string("[%s] PRINTING CALLSTACK\n") + ::std::to_string(::std::stacktrace::current(2)), __FUNCTION__)

#define __LOGFUNC__(x, y) template <typename ...Args> \
inline static void x(const ELogChannel& channel, const std::string& message, Args ... args) {\
CLogger::log(y, channel, message, args...); }

#define __LOGFUNC_EMPTY__(x, y) template <typename ...Args> \
inline static void x(const ELogChannel& channel, const std::string& message, Args ... args) {}

	enum class ELogChannel : uint8_t {
		Core,
		Container,
		Maths,
		Modules,
		Platform,
		Application,
		Graphics,
		Physics,
		Window,
		Script,
		Net,
		AI
	};

	enum class ELogType : uint8_t
	{
		Info,
		Debug,
		Warning,
		Error,
		Assert
	};
#ifdef DA_COLORED_OUT
	enum class ELogColor : uint8_t
	{
		Green,
		Cyan,
		Yellow,
		Red,
		Pink,
		Default,
		Invalid
	};
#endif

	class CLogger
	{
	public:
#ifdef DA_REVIEW
		__LOGFUNC__(LogDebug, ELogType::Debug);
#else
		__LOGFUNC_EMPTY__(LogDebug, ELogType::Debug);
#endif
		__LOGFUNC__(LogInfo, ELogType::Info);
		__LOGFUNC__(LogWarning, ELogType::Warning);
		__LOGFUNC__(LogError, ELogType::Error);
		template <typename ...Args> 
		inline static void LogAssert(bool assert, ELogChannel channel, const std::string& message, Args ... args) {
			if (assert) return;
			CLogger::log(ELogType::Assert, channel, message, args...);
			fflush(stdout);
			CLogger::logCallstack(ELogType::Assert, channel);
		}

		static void initialize();
		static void shutdown();

		template <typename ...Args>
		inline static void log(ELogType type, ELogChannel channel, const std::string& message, Args ... args)
		{
			char buffer[20384];
			
#ifdef DA_COLORED_OUT
			const std::string msg = "%s[%llu] %s: [%s] " + message + "\n%s";
#if DA_PLATFORM_WINDOWS
			sprintf_s(buffer, sizeof(buffer), msg.c_str(), colorTypeMap[(uint8_t)type], utility::GetTimeUS(), logTypeMap[(uint8_t)type], logChannelMap[(uint8_t)channel], args..., colorTypeMap[(uint8_t)ELogColor::Default]);
#else
            snprintf(buffer, sizeof(buffer), msg.c_str(), colorTypeMap[(uint8_t)type], utility::GetTimeUS(), logTypeMap[(uint8_t)type], logChannelMap[(uint8_t)channel], args..., colorTypeMap[(uint8_t)ELogColor::Default]);
#endif
			logInternal(std::string(buffer));
#else
			const std::string msg = "[%llu] %s: [%s] " + message + "\n";
#if DA_PLATFORM_WINDOWS
			sprintf_s(buffer, sizeof(buffer), msg.c_str(), utility::GetTimeUS(), logTypeMap[(uint8_t)type], logChannelMap[(uint8_t)channel], args...);
#else
            snprintf(buffer, sizeof(buffer), msg.c_str(), utility::GetTimeUS(), logTypeMap[(uint8_t)type], logChannelMap[(uint8_t)channel], args...);
#endif
			logInternal(std::string(buffer));
#endif

			
		}
	private:
		static void logInternal(std::string&& message);
		static void logCallstack(ELogType type, ELogChannel channel);

		inline static const char* logChannelMap[] = { "Core", "Container", "Maths", "Modules", "Platform", "Application", "Graphics", "Physics", "Window", "Script", "Net", "AI"};
		inline static const char* logTypeMap[] = { "Info", "Debug", "Warning", "Error", "Assert" };
#ifdef DA_COLORED_OUT
		inline static const char* colorTypeMap[] = { "\033[39m", "\033[36m", "\033[33m", "\033[31m", "\033[35m", "\033[39m" };
#endif

		static CLogger* s_logger;
		FILE* m_file;
	};


}
