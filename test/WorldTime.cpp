#include "WorldTime.h"

auto WorldTime::get_time()->w_time {
	return time;
}
auto WorldTime::get_time_normal()->float {
	auto t = time % cycle;

	if (t >= 0 && t < day) {
		return 0;
	}

	if (t >= day + sunrise && t < day + sunrise + night) {
		return 1;
	}

	if (t >= day && t < day + sunrise) {
		return (t - day) / static_cast<float>(sunrise);
	}

	if (t >= day + sunrise + night && t < cycle) {
		return 1.f - ((t - cycle + sunset) / static_cast<float>(sunset));
	}
}