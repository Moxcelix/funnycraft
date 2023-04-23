#include "Terrain.h"

void Terrain::SetTree(Chunk* chunk, int x, int y, int z, int height, 
					  block_id block, block_id leaves) {
	static int rad = 5;
	static float wide = 1.5f;

	for (int i = 0; i < height; i++) {
		SetBlock(chunk, x, y, z + i, block);
	}

	for (int i = -rad; i <= rad; i++) {
		for (int j = -rad; j <= rad; j++) {
			for (int k = -rad; k <= rad; k++)
			{
				if (sqrt(wide * i * i + wide * j * j + k * k) <= rad - rand() % 3)
				{
					if (i != 0 || j != 0 || k >= 0) {
						SetBlock(chunk, x + i, y + j, z + k + height, leaves);
					}
				}
			}
		}
	}
}

int Terrain::GetTerrainNoise(int x, int y, int z) {
	static float main2D;
	static float volume2D;
	static float main3D;
	static float hills3D;
	static float hills2D;
	static float shiftX, shiftY;
	static int prevX = -1;
	static int prevY = -1;

	const auto scale = .05f;
	const auto main2D_scale = .8f * scale;
	const auto hills2D_scale = .4f * scale;
	const auto main3D_scale = .5f * scale;
	const auto hills3D_scale = .4f * scale;
	const auto volume2D_scale = .3f * scale;
	const auto shift_scale = .7f * scale;

	const auto shift = 128;

	shiftX = perlinNoise.noise(x * shift_scale, y * shift_scale, z * shift_scale) * shift;
	shiftY = perlinNoise.noise(y * shift_scale, x * shift_scale, z * shift_scale) * shift;

	if (prevX != x || prevY != y) {
		prevY = y;
		prevX = x;

		if (world->settings.params[1]) {
			main2D = perlinNoise.octave_noise(x * main2D_scale, y * main2D_scale, 2) * .1f;
		} else {
			main2D = 0.05f;
		}

		hills2D = perlinNoise.octave_noise(x * hills2D_scale, y * hills2D_scale, 2) * .5f + .1f;
		volume2D = perlinNoise.octave_noise(y * hills2D_scale, x * hills2D_scale, 2);
		hills2D = pick(hills2D, 0.3, 0.45) + .1f;
		volume2D = pick(volume2D, 0.35, 0.5);
	}

	if (world->settings.params[0]) {
		main3D = perlinNoise.octave_noise((x + shiftX) * main3D_scale, (y + shiftY) * main3D_scale, z * main3D_scale, 2) * .9f;
		hills3D = perlinNoise.octave_noise((x - shiftX) * hills3D_scale, (y - shiftY) * hills3D_scale, z * hills3D_scale, 2);
	}

	hills3D *= hills2D;
	main3D *= main3D * volume2D;

	auto result = main2D;

	if (main3D > main2D) {
		result = main3D;
	}

	result += hills3D;

	auto noise = static_cast<int>(result * land_height) + ground_height;

	return noise;
}

int Terrain::GetNoise(int x, int y, int z, float scale, int size) {
	int noise = int(perlinNoise.noise(x * scale, y * scale, z * scale) * size);

	return noise;
}

float Terrain::pick(float x, float x0, float x1) {
	float m = 1. / (x1 - x0);
	x -= x0;
	x *= m;

	if (x > 1) x = 1;
	if (x < 0) x = 0;

	return x;
}

float Terrain::linear(float x, float x0, float x1, float y0, float y1) {
	float y = ((x - x0) * (y1 - y0) / (x1 - x0)) + y0;

	return y;
}

void Terrain::SetGround(Chunk* chunk, int x, int y, int z) {
	for (auto i = 0; i < ground_length + GetNoise(x, y, z, 0.03f, 4); i++) {
		const auto block = chunk->GetBlock(x, y, z - i);
		if (block == Block::stone || block == Block::null) {
			auto index = i;
			if (index >= ground_length)
				index = ground_length - 1;

			chunk->SetBlock(x, y, z - i, ground_mas[index]);
		}
		else {
			break;
		}
	}
}

void Terrain::Generate(Chunk* chunk) {
	const auto bedrock = Block::bedrock->id;
	const auto stone = Block::stone->id;

	srand(seed + chunk->get_pos().GetHashCode());

	for (int x = 0; x < Chunk::size; x++) {
		for (int y = 0; y < Chunk::size; y++)
		{
			auto noise0 = -1;
			auto noise1 = -1;

			for (int z = 0; z < Chunk::size; z++) {
				const auto pos = chunk->get_pos();
				const auto global_x = x + pos.x;
				const auto global_y = y + pos.y;
				const auto global_z = z + pos.z;

				if (global_z == 0) {
					SetBlock(chunk, x, y, z, bedrock);

					continue;
				}

				if (noise1 >= 0) {
					noise0 = noise1;
				} else {
					noise0 = GetTerrainNoise(global_x, global_y, global_z);
				}

				noise1 = GetTerrainNoise(global_x, global_y, global_z + 1); 

				if (global_z < noise0) {
					if (chunk->GetBlockID(x, y, z) == 0) {
						SetBlock(chunk, x, y, z, stone);
					}

					if (global_z + 1 >= noise1) {
						SetGround(chunk, x, y, z);
						if (world->settings.params[2]) SetRandomTree(chunk, x, y, z);
						if (world->settings.params[3]) SetRandomPlant(chunk, x, y, z + 1);
					}
				}
			}
		}
	}

	for (int i = 0; i < world->global_buffer.size(); i++) {
		const auto pos = Vector3Int(
			world->global_buffer[i].x, 
			world->global_buffer[i].y,
			world->global_buffer[i].z) - chunk->get_pos();

		if (chunk->InRange(pos.x, pos.y, pos.z)) {
			if (!chunk->GetBlockID(pos.x, pos.y, pos.z)) {
				chunk->SetBlock(pos.x, pos.y, pos.z, world->global_buffer[i].block);
			}

			world->to_remove.push_back(i);
		}
	}
}

void Terrain::SetRandomTree(Chunk* chunk, int x, int y, int z) {
	const auto oak_leaves = Block::leaves->id;
	const auto birch_leaves = Block::birch_leaves->id;
	const auto oak = Block::oak->id;
	const auto birch = Block::birch->id;
	const auto random_value = rand() % 500;

	if (random_value == 1) SetTree(chunk, x, y, z, 5 + rand() % 4, oak, oak_leaves);
	if (random_value == 2) SetTree(chunk, x, y, z, 5 + rand() % 4, birch, birch_leaves);
}

void Terrain::SetRandomPlant(Chunk* chunk, int x, int y, int z) {
	const auto poppy = Block::poppy->id;
	const auto dandelion = Block::dandelion->id;
	const auto tall_grass = Block::tall_grass->id;
	const auto random_value = rand() % 500;

	if (random_value == 3)	SetBlock(chunk, x, y, z, poppy);
	if (random_value == 4)	SetBlock(chunk, x, y, z, dandelion);
	if (random_value > 450) SetBlock(chunk, x, y, z, tall_grass);
}

void Terrain::SetBlock(Chunk* chunk, int x, int y, int z, block_id block) {
	if (z < 0 || z >= world->WorldHeight * Chunk::size)
		return;

	if (chunk->InRange(x, y, z)) {
		const auto id = chunk->GetBlockID(x, y, z);

		if (id == 0 || id == -1) {
			chunk->SetBlock(x, y, z, block);
		}
	} else {
		Vector3Int p = chunk->get_pos();
		world->SetBufferBlock(x + p.x, y + p.y, z + p.z, block);
	}
}

void Terrain::SetBlock(Chunk* chunk, int x, int y, int z, Block* block) {
	SetBlock(chunk, x, y, z, block->id);
}