#pragma once

#include <spdlog/fmt/fmt.h>


#ifdef DEVELOPMENT
#define ONLY_DEBUG(x) x;
#else
#define ONLY_DEBUG(x)
#endif // DEVELOPMENT


namespace logger
{
	void info([[maybe_unused]] const char* message) noexcept;

	void warning([[maybe_unused]] const char* message) noexcept;

	void error([[maybe_unused]] const char* message) noexcept;

	template<class... Args>
	void info([[maybe_unused]] Args&&... args) noexcept { ONLY_DEBUG(info(fmt::format(std::forward<Args>(args)...).c_str())) }

	template<class... Args>
	void warning([[maybe_unused]] Args&&... args) noexcept { ONLY_DEBUG(warning(fmt::format(std::forward<Args>(args)...).c_str())) }

	template<class... Args>
	void error([[maybe_unused]] Args&&... args) noexcept { ONLY_DEBUG(error(fmt::format(std::forward<Args>(args)...).c_str())) }

	template<class... Args>
	void info([[maybe_unused]] bool success, [[maybe_unused]] Args&&... args) noexcept { ONLY_DEBUG(if (!success) { info(std::forward<Args>(args)...); }) }

	template<class... Args>
	void warning([[maybe_unused]] bool success, [[maybe_unused]] Args&&... args) noexcept { ONLY_DEBUG(if (!success) { warning(std::forward<Args>(args)...); }) }

	template<class... Args>
	void error([[maybe_unused]] bool success, [[maybe_unused]] Args&&... args) noexcept { ONLY_DEBUG(if (!success) { error(std::forward<Args>(args)...); }) }
}