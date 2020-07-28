#ifndef LOG_H
#define LOG_H

#include <memory>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

#include "Core.h"

namespace HE
{
    class Log
    {
    public:
        static void Init();

        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger()
        {
            return s_CoreLogger;
        }

        inline static std::shared_ptr<spdlog::logger>& GetClientLogger()
        {
            return s_ClientLogger;
        }
    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;

    };
}


// Core log macros
#define HE_CORE_FATAL(...) ::HE::Log::GetCoreLogger()->fatal(__VA_ARGS__)
#define HE_CORE_ERROR(...) ::HE::Log::GetCoreLogger()->error(__VA_ARGS__)
#define HE_CORE_WARN(...)  ::HE::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define HE_CORE_INFO(...)  ::HE::Log::GetCoreLogger()->info(__VA_ARGS__)
#define HE_CORE_TRACE(...) ::HE::Log::GetCoreLogger()->trace(__VA_ARGS__)

// Client log macros
#define HE_FATAL(...)      ::HE::Log::GetClientLogger()->fatal(__VA_ARGS__)
#define HE_ERROR(...)      ::HE::Log::GetClientLogger()->error(__VA_ARGS__)
#define HE_WARN(...)       ::HE::Log::GetClientLogger()->warn(__VA_ARGS__)
#define HE_INFO(...)       ::HE::Log::GetClientLogger()->info(__VA_ARGS__)
#define HE_TRACE(...)      ::HE::Log::GetClientLogger()->trace(__VA_ARGS__)


// if dist build
#ifdef RELEASE
    #define HE_CORE_INFO
#endif


#endif // LOG_H
