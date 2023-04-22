#pragma once
#include "Vector3.h"

const Vector3 DAY_LIGHT = {1.f, 1.f, 1.f};
const Vector3 NIGHT_LIGHT = {1.f, 1.f, 1.f};

inline auto LIGHT_SUM(Vector3 a, Vector3  b, Vector3  c, Vector3 d, Vector3 e) {
	return Vector3::clrm((a + b + c + d) * 0.25, e);
}

inline auto LIGHT_LERP(char light) {
	return 0;
}
