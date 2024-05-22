#pragma once
#include <array>

#include "Vector3.h"

struct Light {
	unsigned char sky	: 4;
	unsigned char block	: 4;
};

class LightMap {
private:
	constexpr static const char* file_path = "resources/effects/light_color_map.bmp";
	constexpr static const auto file_header_size = 54;
	constexpr static const auto file_width_byte_number = 18;
	constexpr static const auto file_height_byte_number = 22;

	constexpr static std::size_t height = 8;
	constexpr static std::size_t width = 16;
	// formatter, are you OK? 💀
	std::array<Vector3, height* width> color_map;

	LightMap();
public:
	constexpr static auto light_sampling = 8;
	constexpr static auto max_light = light_sampling - 1;

	[[nodiscard]] static auto instance() -> LightMap& {
		static LightMap lightMap;
		return lightMap;
	}
	
	auto get_color(Light, double) -> Vector3;

	LightMap(LightMap const&) noexcept = delete;
	LightMap(LightMap const&&) noexcept = delete;
	LightMap& operator=(LightMap const&) noexcept = delete;
	LightMap& operator=(LightMap&&) noexcept = delete;

	~LightMap() = default;
};