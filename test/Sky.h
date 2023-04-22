#pragma once
#include "Vector3.h"

class Sky {
private:
	const Vector3 night_color = { 0, 0, 0.1 };
	const Vector3 twilight_color = {0.4, 0.1, 0.5 };
	const Vector3 day_color = { 0.6, 0.8, 1 };
public:
	Vector3 getSkyColor(bool);
};

