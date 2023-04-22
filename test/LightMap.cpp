#include "LightMap.h"

#include <fstream>
#include <iostream>

LightMap::LightMap() {
	std::ifstream stream{ file_path, std::ios::binary };

	if (!stream.is_open()) {
		std::cout << "warning: [light map] file \"" << file_path << "\" not found!";
		return;
	}

	unsigned char info[file_header_size]{};
	unsigned char data[width * height * 3]{};

	stream.read(reinterpret_cast<char*>(info), file_header_size);

	auto width_sus = *reinterpret_cast<int*>(&info[file_width_byte_number]);
	auto height_sus = *reinterpret_cast<int*>(&info[file_height_byte_number]);

	if (width_sus != width || height_sus != height) {
		std::cout << "warning: [light map] file \"" << file_path << "\" is illegal!";
	}

	stream.read(reinterpret_cast<char*>(data), width * height * 3);
	// funny *.bmp -> (b, g, r) 
	for (auto i = 0; i < width * height; i++) {
		const float max_color_value = 0xFF;
		color_map[i] = { 
			data[i * 3 + 2] / max_color_value,
			data[i * 3 + 1] / max_color_value,
			data[i * 3] / max_color_value };
	}
}
// time must be in range of [0, 1]
// 0 - day
// 1 - night
auto LightMap::get_color(Light light, double time) -> Vector3 {
	// start corner is left-down gap
	const auto light_coord = (height - light.sky - 1) * width + light.block;
	const auto& day_light_color = color_map[light_coord];
	const auto& night_light_color = color_map[light_coord + height];

	return Vector3::lerp(day_light_color, night_light_color, time);
}