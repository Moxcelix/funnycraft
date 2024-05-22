#include "ChunkLoader.h"

unsigned int ChunkLoader::render_distance = 4;

void ChunkLoader::Update() {
	UpdatePoint();
	LoadTerrain();
	UnloadTerrain();

	world->render_distance = render_distance;
}

void ChunkLoader::UpdatePoint() {
	int_position = {
		static_cast<int>(player->pos.x),
		static_cast<int>(player->pos.y),
		static_cast<int>(player->pos.z)
	};
}

void ChunkLoader::LoadTerrain() {
	if (!world) return;

	if (world->chunks_loaded < World::max_chunks_count - 1) {
		for (const auto& i : sphere) {
			int x = int_position.x + i.x * Chunk::size;
			int y = int_position.y + i.y * Chunk::size;
			int z = int_position.z + i.z * Chunk::size;

			world->AddToCreate({ x, y, z });
		}
	}
}

void ChunkLoader::UnloadTerrain() {
	if (!world) return;

	for (int i = 0; i < world->chunks_loaded; i++) {
		const auto distance =
			Vector3Int::Distance(
				world->chunks[i]->get_pos(),
				int_position);

		if (distance > (render_distance + 1) * Chunk::size) {
			world->AddToDelete(world->chunks[i]->get_pos());
		}
	}
}

void ChunkLoader::GenereateSphere() {
	sphere.clear();
	int render_distance = ChunkLoader::render_distance - 1;

	for (int i = -render_distance; i <= render_distance; i++) {
		for (int j = -render_distance; j <= render_distance; j++) {
			for (int k = -render_distance; k <= render_distance; k++) {
				if (i * i + j * j + k * k < render_distance * render_distance) {
					sphere.push_back({ i, j, k });
				}
			}
		}
	}

	for (int i = 0; i < sphere.size(); i++) {
		for (int j = 0; j < sphere.size() - 1; j++) {
			if (sphere[j] > sphere[j + 1]) {
				std::swap(sphere[j], sphere[j + 1]);
			}
		}
	}

	positions_count = sphere.size();
}