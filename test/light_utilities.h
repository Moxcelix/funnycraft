#pragma once
#include "Vector3.h"

inline auto LIGHT_SUM(Vector3 a, Vector3  b, Vector3  c, Vector3 d, Vector3 e) {
	return Vector3::clrm((a + b + c + d) * 0.25, e);
}
