#pragma once

#define LIGHT_SUM(a, b, c, d, e)				\
	Vector3::clrm((a + b + c + d) * 0.25, e)	

enum class LightType {
	FLAT,
	SMOOTH,
};