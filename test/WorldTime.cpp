#include "WorldTime.h"

auto WorldTime::get_time()->w_time {
	return time;
}

auto WorldTime::get_accumulator_value()->float {
	return accumulator / static_cast<float>(cycle);
}

auto WorldTime::get_time_normal()->float {
	const auto t = time % cycle;

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

auto WorldTime::IsChangesTime() -> bool {
	const auto t = time % cycle;

	return (t >= day && t < day + sunrise) ||
		(t >= day + sunrise + night && t < cycle);
}

void WorldTime::Shift(float delta_time) {
	accumulator += delta_time * speed;

	if (accumulator > 1) {
		time += static_cast<int>(accumulator);
		accumulator = 0;
	}
}

void WorldTime::set(w_time time) {
	this->time = time;
}