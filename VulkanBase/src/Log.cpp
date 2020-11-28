#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> Log::m_logger;

void Log::Intialize(spdlog::level::level_enum level)
{
	m_logger = spdlog::stdout_color_mt("app");
	m_logger->set_level(level);
	m_logger->set_pattern("%^[%T] %n: %v%$");
}
