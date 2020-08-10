#pragma once

#include "Fseconds.hpp"

struct Timer
{
	[[nodiscard]] constexpr bool update(Fseconds delatT) noexcept
	{
		dt += delatT;
		if (dt < duration)
		{
			return false;
		}
		dt -= duration;
		return true;
	}

	Fseconds duration = std::chrono::seconds(1);
	Fseconds dt = Fseconds::zero();
};
