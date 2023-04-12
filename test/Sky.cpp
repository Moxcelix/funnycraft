#include "Sky.h"

Vector3 Sky::getSkyColor(bool day_state) {
	return day_state ? day_color : night_color;
}