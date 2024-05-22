#pragma once
#include "Vector3.h"

class Sky {
private:
	const Vector3 night_color = { 0.f, 0.f, 0.1f };
	const Vector3 twilight_color = {0.4f, 0.3f, 0.5f };
	const Vector3 day_color = { 0.6f, 0.8f, 1.f };
public:
	Vector3 get_sky_color(float time_normal);
};

