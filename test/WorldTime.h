#pragma once
#include <functional>

typedef unsigned long long w_time;

class WorldTime {
private:
	w_time time = 0;
	float accumulator = 0;
	float speed = 0.01f;
public:
	constexpr static w_time cycle = 2400;

	constexpr static w_time day = 1000;
	constexpr static w_time sunrise = 200;
	constexpr static w_time night = 1000;
	constexpr static w_time sunset = 200;

	auto get_time()->w_time;
	auto get_time_normal()->float;
	auto get_accumulator_value() -> float;

	void set(w_time time);

	auto IsChangesTime() -> bool;

	void Shift(float delta_time);
};

