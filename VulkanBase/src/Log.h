#ifndef LOG_H
#define LOG_H

#include <memory.h>
#include "spdlog/spdlog.h"


class Log
{
private:
	static std::shared_ptr<spdlog::logger> m_logger;
public:
	static void Intialize(spdlog::level::level_enum level);
	inline static std::shared_ptr<spdlog::logger> GetHandle() { return m_logger; }
};

#define CORE_ERROR(...) Log::GetHandle()->error(__VA_ARGS__);
#define CORE_WARN(...) Log::GetHandle()->warn(__VA_ARGS__);
#define CORE_INFO(...) Log::GetHandle()->info(__VA_ARGS__);
#define CORE_TRACE(...) Log::GetHandle()->trace(__VA_ARGS__);

#endif
