#pragma once

typedef unsigned long long w_time;

class WorldTime {
private:
	w_time time = 1100;
public:
	constexpr static w_time cycle = 2400;

	constexpr static w_time day = 1000;
	constexpr static w_time sunrise = 200;
	constexpr static w_time night = 1000;
	constexpr static w_time sunset = 200;

	auto get_time()->w_time;
	auto get_time_normal()->float;
};

