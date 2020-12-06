#ifndef LOG_H
#define LOG_H

#include <memory.h>
#include "spdlog/spdlog.h"

class Log
{
private:
	static std::shared_ptr<spdlog::logger> mLogger;
public:
	static void Intialize(spdlog::level::level_enum level);
	inline static std::shared_ptr<spdlog::logger> GetHandle() { return mLogger; }
};

//#define CORE_ERROR(...) Log::GetHandle()->error(__FILE__ __LINE__##__VA_ARGS__);
//#define CORE_WARN(...) Log::GetHandle()->warn(__FILE__ __LINE__##__VA_ARGS__);
//#define CORE_INFO(...) Log::GetHandle()->info(__FILE__ __LINE__##__VA_ARGS__);

#define CORE_ERROR(...) Log::GetHandle()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::level_enum::err, __VA_ARGS__);
#define CORE_WARN(...) Log::GetHandle()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::level_enum::warn, __VA_ARGS__);
#define CORE_INFO(...) Log::GetHandle()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::level_enum::info, __VA_ARGS__);
#endif
