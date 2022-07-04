#pragma once
#include "core/core.h"
#include "core/containers.h"
#include "core/utility.h"

#include <stdio.h>
#include <fstream>

namespace da {
#define NAMEOF(x) #x

#define LOG_INFO(ELogChannel, msg, ...) CLogger::LogInfo(ELogChannel, CString("[%s] ") + msg, __FUNCTION__  , ##__VA_ARGS__)
#define LOG_DEBUG(ELogChannel, msg, ...) CLogger::LogDebug(ELogChannel, CString("[%s] ") + msg, __FUNCTION__  , ##__VA_ARGS__)
#define LOG_WARN(ELogChannel, msg, ...) CLogger::LogWarning(ELogChannel, CString("[%s] ") + msg __FUNCTION__  , ##__VA_ARGS__)
#define LOG_ERROR(ELogChannel, msg, ...) CLogger::LogError(ELogChannel, CString("[%s] ") + msg, __FUNCTION__  , ##__VA_ARGS__)
#define LOG_ASSERT(x, ELogChannel, msg, ...) CLogger::LogAssert(x, ELogChannel, CString("[%s] ") + msg, __FUNCTION__  , ##__VA_ARGS__)

#define __LOGFUNC__(x, y) template <typename ...Args> \
inline static void x(const ELogChannel& channel, const CString& message, Args ... args) {\
CLogger::log(y, channel, message, args...); }

#define __LOGFUNC_EMPTY__(x, y) template <typename ...Args> \
inline static void x(const ELogChannel& channel, const CString& message, Args ... args) {}

	enum class ELogChannel : uint8_t {
		Core,
		Container,
		Maths,
		Modules,
		Platform,
		Application,
		Graphics,
		Window
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
			char buffer[5096];
			
#ifdef DA_COLORED_OUT
			const CString msg = "%s[%llu] %s: [%s] " + message + "\n%s";
#if DA_PLATFORM_WINDOWS
			sprintf_s(buffer, sizeof(buffer), msg.cstr(), colorTypeMap[(uint8_t)type], utility::GetTimeUS(), logTypeMap[(uint8_t)type], logChannelMap[(uint8_t)channel], args..., colorTypeMap[(uint8_t)ELogColor::Default]);
#else
            snprintf(buffer, sizeof(buffer), msg.cstr(), colorTypeMap[(uint8_t)type], utility::GetTimeUS(), logTypeMap[(uint8_t)type], logChannelMap[(uint8_t)channel], args..., colorTypeMap[(uint8_t)ELogColor::Default]);
#endif
			logInternal(CString(buffer));
#else
			const CString msg = "[%llu] %s: [%s] " + message + "\n";
#if DA_PLATFORM_WINDOWS
			sprintf_s(buffer, sizeof(buffer), msg.cstr(), utility::GetTimeUS(), logTypeMap[(uint8_t)type], logChannelMap[(uint8_t)channel], args...);
#else
			snprintf(buffer, sizeof(buffer), msg.cstr(), utility::GetTimeUS(), logTypeMap[(uint8_t)type], logChannelMap[(uint8_t)channel], args...);
#endif
			logInternal(CString(buffer));
#endif

			
		}
		static void logInternal(CString&& message);

		inline static const char* logChannelMap[] = { "Core", "Container", "Maths", "Modules", "Platform", "Application", "Graphics", "Window"};
		inline static const char* logTypeMap[] = { "Info", "Debug", "Warning", "Error", "Assert" };
#ifdef DA_COLORED_OUT
		inline static const char* colorTypeMap[] = { "\033[39m", "\033[36m", "\033[33m", "\033[31m", "\033[35m", "\033[39m" };
#endif

		static CLogger* s_logger;
		FILE* m_file;
	};


}
