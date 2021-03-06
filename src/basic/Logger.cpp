#include "Logger.hpp"

#ifdef DEVELOPMENT
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#endif // DEVELOPMENT

namespace logger
{
	void info(const char* message) noexcept
	{
		ONLY_DEBUG(spdlog::info(message))
	}

	void warning(const char* message) noexcept
	{
		ONLY_DEBUG(spdlog::warn(message))
	}

	void error(const char* message) noexcept 
	{
		ONLY_DEBUG(spdlog::error(message))
	}
}