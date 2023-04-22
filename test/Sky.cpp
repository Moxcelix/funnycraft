#include "Sky.h"

Vector3 Sky::get_sky_color(float time_normal) {
	if (time_normal < 0.5) {
		return Vector3::lerp(day_color, twilight_color, time_normal * 2);
	}

	return Vector3::lerp(twilight_color, night_color, (time_normal - 0.5) * 2);
}