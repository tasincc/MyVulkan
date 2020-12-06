#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> Log::mLogger;

void Log::Intialize(spdlog::level::level_enum level)
{
	mLogger = spdlog::stdout_color_mt("Console");
	mLogger->set_level(level);
	//m_logger->set_pattern("%^[%T] %n: %v%$");
	mLogger->set_pattern("%^[%T][%@,%!]: %v%$");
}
