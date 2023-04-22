#include "Chunk.h"
#include "LightMap.h"

float Chunk::brightness = 0.1f;

Chunk::Chunk() {
	pos = { 0, 0, 0 };

	Init();
}

Chunk::~Chunk() {
	SaveChunk();
	world = nullptr;
	data.Clear();
	uv_data.Clear();
}

Chunk::Chunk(int x, int y, int z, World* world) {
	pos = Vector3Int(x, y, z);
	this->world = world;

	Init();
}

block_id Chunk::GetBlockID(int x, int y, int z) {
	if (InRange(x, y, z)) return blocks[x][y][z];

	if (z + pos.z < 0 || z + pos.z >= ChunkSize * world->WorldHeight)
		return 0;

	if (InLightRange(LightMap::light_sampling + x, 
		LightMap::light_sampling + y, LightMap::light_sampling + z)) {
		int _x = (ChunkSize + x) / ChunkSize;
		int _y = (ChunkSize + y) / ChunkSize;
		int _z = (ChunkSize + z) / ChunkSize;

		Chunk* chunk = chunks[_x + _y * 3 + _z * 9];

		if (chunk) {
			_x = pos.x - chunk->pos.x + x;
			_y = pos.y - chunk->pos.y + y;
			_z = pos.z - chunk->pos.z + z;

			if (InRange(_x, _y, _z))
				return chunk->blocks[_x][_y][_z];
		}

		return -1;
	}
	else {
		return world->GetBlockID(x + pos.x, y + pos.y, z + pos.z);
	}
}

void Chunk::ClearCash() {
	data.Clear();
	uv_data.Clear();
}

void Chunk::Init() {
	for (int i = 0; i < ChunkSize; i++) {
		for (int j = 0; j < ChunkSize; j++) {
			for (int k = 0; k < ChunkSize; k++) {
				blocks[i][j][k] = Block::air->id;
			}
		}
	}

	for (int i = 0; i < 27; i++) {
		chunks[i] = nullptr;
	}
}

void Chunk::Generate() {
	if (!LoadChunk()) world->Generate(this);
}

inline Block const* Chunk::GetBlock(block_id ID) {
	return Block::GetBlock(ID);
}

void Chunk::GetMeshData(VertexData* data, UVData* uv, int x, int y, int z, int layer) {
	auto block = GetBlock(GetBlockID(x, y, z));

	if (block->GetRenderLayer() == layer)
		block->GetMeshData(data, uv, x, y, z, this);
}

inline Block const* Chunk::GetBlock(int x, int y, int z) {
	block_id id = GetBlockID(x, y, z);
	return Block::GetBlock(id);
}

inline bool Chunk::InRange(int x) {
	return x >= 0 && x < ChunkSize;
}

inline bool Chunk::InRange(int x, int y, int z) {
	return InRange(x) && InRange(y) && InRange(z);
}

bool Chunk::InLightRange(int x, int y, int z) {
	return (x >= 0 && x < ChunkSize + LightMap::light_sampling * 2) &&
		(y >= 0 && y < ChunkSize + LightMap::light_sampling * 2) &&
		(z >= 0 && z < ChunkSize + LightMap::light_sampling * 2);
}

void Chunk::UpdateMesh() {
	ClearCash();

	for (int x = 0; x < ChunkSize; x++) {
		for (int y = 0; y < ChunkSize; y++) {
			for (int z = 0; z < ChunkSize; z++) {
				GetMeshData(&data, &uv_data, x, y, z, 0);
			}
		}
	}
}

void Chunk::Update()
{
	UpdateLight();
	UpdateMesh();

	ticks = 0;
	world->chunks_updating++;
}

void Chunk::RecalculateSkyLightSolidity() {
	auto b_count = 0;

	for (int x = 0; x < ChunkSize; x++) {
		for (int y = 0; y < ChunkSize; y++) {
			bool detected = false;
			for (int z = 0; z < ChunkSize; z++) {
				if (!detected) {
					if (!GetBlock(x, y, z)->Transparent()) {
						detected = true;
					}
				}
			}

			if (detected) b_count++;
		}
	}

	is_solid = b_count >= ChunkSize * ChunkSize;
}

inline void Chunk::SetBlock(int x, int y, int z, block_id block) {
	if (InRange(x, y, z)) blocks[x][y][z] = block;
	else world->SetBlock(x + pos.x, y + pos.y, z + pos.z, block);
}

void Chunk::SetBlock(int x, int y, int z, Block* block) {
	SetBlock(x, y, z, block->id);
}

void Chunk::SaveChunk()
{
	if (!modified) return;

	std::ofstream stream;
	stream.open(save_folder + World::name + "/" +
		std::to_string(pos.x) + " " +
		std::to_string(pos.y) + " " +
		std::to_string(pos.z) + ".dat");

	for (int x = 0; x < Chunk::ChunkSize; x++) {
		for (int y = 0; y < Chunk::ChunkSize; y++) {
			for (int z = 0; z < Chunk::ChunkSize; z++) {
				stream << blocks[x][y][z];
			}
		}
	}

	stream.close();
}

bool Chunk::LoadChunk() {
	std::ifstream stream;
	stream.open(save_folder + World::name + "/" +
		std::to_string(pos.x) + " " + 
		std::to_string(pos.y) + " " + 
		std::to_string(pos.z) + ".dat");

	if (stream.is_open())
	{
		for (int x = 0; x < Chunk::ChunkSize; x++) {
			for (int y = 0; y < Chunk::ChunkSize; y++) {
				for (int z = 0; z < Chunk::ChunkSize; z++) {
					blocks[x][y][z] = stream.get();
				}
			}
		}

		stream.close();
		return true;
	}

	return false;
}

Vector3 Chunk::GetLigthColor(int x, int y, int z) {
	if (!GetBlock(x, y, z)->Transparent())
		return Vector3{ brightness, brightness, brightness };

	return LightMap::instance().get_color(GetLight(x, y, z), world->time.get_time_normal())
		* (1 - brightness) + Vector3{ brightness, brightness, brightness };
}

void Chunk::UpdateMem() {
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			for (int z = 0; z < 3; z++) {
				int X = (x - 1) * ChunkSize;
				int Y = (y - 1) * ChunkSize;
				int Z = (z - 1) * ChunkSize;

				chunks[x + y * 3 + z * 9] =
					world->GetChunk(pos.x + X, pos.y + Y, pos.z + Z);
			}
		}
	}
}

void Chunk::UpdateLight() {
	for (int x = 0; x < ChunkSize + LightMap::light_sampling * 2; x++) {
		for (int y = 0; y < ChunkSize + LightMap::light_sampling * 2; y++) {
			for (int z = 0; z < ChunkSize + LightMap::light_sampling * 2; z++) {
				light_map[x][y][z] = { 0, 0 };
			}
		}
	}

	for (int x = 0; x < ChunkSize; x++) {
		for (int y = 0; y < ChunkSize; y++) {
			for (int z = 0; z < ChunkSize; z++)
			{
				buffer_light_map[x][y][z] = { 0, 0 };

				auto light = GetBlock(x, y, z)->Luminosity();

				if (light) {
					light_map[x + LightMap::light_sampling]
						[y + LightMap::light_sampling]
					[z + LightMap::light_sampling].block = light;
				}
			}
		}
	}

	Chunk* chunk = chunks[22]; // верхний чанк

	for (int x = 0; x < ChunkSize; x++) {
		for (int y = 0; y < ChunkSize; y++) {
			sky_light[x][y] = true;
			for (int z = ChunkSize - 1; z >= 0; z--) {
				if ((!chunk || chunk->sky_light[x][y]) && GetBlock(x, y, z)->Transparent()) {
					light_map[x + LightMap::light_sampling]
						[y + LightMap::light_sampling]
						[z + LightMap::light_sampling].sky = LightMap::max_light;
				}
				else {
					sky_light[x][y] = false;
					break;
				}
			}
		}
	}
	// alien light
	for (int i = 0; i < 27; i++) {
		if (i == 13) // skip current chunk
			continue;

		if (chunks[i]) {
			auto sx = pos.x - chunks[i]->pos.x;
			auto sy = pos.y - chunks[i]->pos.y;
			auto sz = pos.z - chunks[i]->pos.z;

			for (int x = 0; x < ChunkSize; x++) {
				for (int y = 0; y < ChunkSize; y++) {
					for (int z = 0; z < ChunkSize; z++) {
						if (!InLightRange(x + sx + LightMap::light_sampling,
							y + sy + LightMap::light_sampling,
							z + sz + LightMap::light_sampling)) continue;

						// значение света в соседнем чанке в области этого чанка
						auto l = chunks[i]->CheckLight(x + sx, y + sy, z + sz);

						if (l.sky > buffer_light_map[x][y][z].sky)
							buffer_light_map[x][y][z].sky = l.sky;

						if (l.block > buffer_light_map[x][y][z].block)
							buffer_light_map[x][y][z].block = l.block;
					}
				}
			}
		}
	}

	for (unsigned char l = LightMap::max_light; l > 0; l--) {
		for (int x = 0; x < ChunkSize + LightMap::light_sampling * 2; x++) {
			for (int y = 0; y < ChunkSize + LightMap::light_sampling * 2; y++) {
				for (int z = 0; z < ChunkSize + LightMap::light_sampling * 2; z++) {
					if (light_map[x][y][z].sky == l) {
						SetSkyLight(x + 1 - LightMap::light_sampling, y - LightMap::light_sampling, z - LightMap::light_sampling, light_map[x][y][z].sky - 1);
						SetSkyLight(x - 1 - LightMap::light_sampling, y - LightMap::light_sampling, z - LightMap::light_sampling, light_map[x][y][z].sky - 1);
						SetSkyLight(x - LightMap::light_sampling, y + 1 - LightMap::light_sampling, z - LightMap::light_sampling, light_map[x][y][z].sky - 1);
						SetSkyLight(x - LightMap::light_sampling, y - 1 - LightMap::light_sampling, z - LightMap::light_sampling, light_map[x][y][z].sky - 1);
						SetSkyLight(x - LightMap::light_sampling, y - LightMap::light_sampling, z + 1 - LightMap::light_sampling, light_map[x][y][z].sky - 1);
						SetSkyLight(x - LightMap::light_sampling, y - LightMap::light_sampling, z - 1 - LightMap::light_sampling, light_map[x][y][z].sky - 1);
					}

					if (light_map[x][y][z].block == l) {
						SetBlockLight(x + 1 - LightMap::light_sampling, y - LightMap::light_sampling, z - LightMap::light_sampling, light_map[x][y][z].block - 1);
						SetBlockLight(x - 1 - LightMap::light_sampling, y - LightMap::light_sampling, z - LightMap::light_sampling, light_map[x][y][z].block - 1);
						SetBlockLight(x - LightMap::light_sampling, y + 1 - LightMap::light_sampling, z - LightMap::light_sampling, light_map[x][y][z].block - 1);
						SetBlockLight(x - LightMap::light_sampling, y - 1 - LightMap::light_sampling, z - LightMap::light_sampling, light_map[x][y][z].block - 1);
						SetBlockLight(x - LightMap::light_sampling, y - LightMap::light_sampling, z + 1 - LightMap::light_sampling, light_map[x][y][z].block - 1);
						SetBlockLight(x - LightMap::light_sampling, y - LightMap::light_sampling, z - 1 - LightMap::light_sampling, light_map[x][y][z].block - 1);
					}
				}
			}
		}
	}
}

inline void Chunk::SetSkyLight(int x, int y, int z, unsigned char l) {
	if (!GetBlock(x, y, z)->Transparent()) return;

	if (light_map[x + LightMap::light_sampling][y + LightMap::light_sampling][z + LightMap::light_sampling].sky < l) {
		light_map[x + LightMap::light_sampling][y + LightMap::light_sampling][z + LightMap::light_sampling].sky = l;
	}
}

inline void Chunk::SetBlockLight(int x, int y, int z, unsigned char l) {
	if (!GetBlock(x, y, z)->Transparent()) return;

	if (light_map[x + LightMap::light_sampling][y + LightMap::light_sampling][z + LightMap::light_sampling].block < l) {
		light_map[x + LightMap::light_sampling][y + LightMap::light_sampling][z + LightMap::light_sampling].block = l;
	}
}

inline Light Chunk::GetLight(int x, int y, int z) {
	if (InRange(x, y, z)) {
		auto sky = std::max(light_map[x + LightMap::light_sampling][y + LightMap::light_sampling][z + LightMap::light_sampling].sky,
			buffer_light_map[x][y][z].sky);
		auto block = std::max(light_map[x + LightMap::light_sampling][y + LightMap::light_sampling][z + LightMap::light_sampling].block,
			buffer_light_map[x][y][z].block);

		return { sky, block };
	}
	// for z-bounds
	if (InRange(x) && InRange(y)) {
		if (pos.z + z < 0 || pos.z + z >= world->WorldHeight * ChunkSize) {
			auto light = light_map[x + LightMap::light_sampling]
				[y + LightMap::light_sampling][z + LightMap::light_sampling];

			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++)
				{
					if (i == 1 && j == 1) continue;

					Chunk* chunk = chunks[i + j * 3 + 9];

					if (chunk) {
						const auto _x = x + (2 - i) * ChunkSize;
						const auto _y = y + (2 - j) * ChunkSize;
						const auto _z = z + LightMap::light_sampling;

						if (!InLightRange(_x, _y, _z)) break;

						const auto l = chunk->light_map[_x][_y][_z];

						if (l.sky > light.sky) light.sky = l.sky;
						if (l.block > light.block) light.block = l.block;
					}
				}
			}
			return light;
		}
	}
	return world->GetLight(x + pos.x, y + pos.y, z + pos.z);
}

inline Light Chunk::CheckLight(int x, int y, int z) {
	if (InRange(x, y, z)) {
		auto sky = std::max(light_map[x + LightMap::light_sampling]
			[y + LightMap::light_sampling][z + LightMap::light_sampling].sky,
			buffer_light_map[x][y][z].sky);
		auto block = std::max(light_map[x + LightMap::light_sampling]
			[y + LightMap::light_sampling][z + LightMap::light_sampling].block,
			buffer_light_map[x][y][z].block);

		return { sky, block };
	}
	if (InLightRange(x + LightMap::light_sampling,
		y + LightMap::light_sampling, z + LightMap::light_sampling))
		return light_map[x + LightMap::light_sampling]
		[y + LightMap::light_sampling][z + LightMap::light_sampling];

	return { 0, 0 };
}

void Chunk::Modify() {
	for (int i = 0; i < 27; i++) {
		if (chunks[i]) {
			chunks[i]->modified = true;
		}
	}
}

void Chunk::Render(unsigned int texture) {
	if (data.face_count == 0) return;

	static float s = Chunk::ChunkSize * 0.5f;
	static float shift = Chunk::ChunkSize * 0.5f - 0.5f;

	glBindTexture(GL_TEXTURE_2D, texture);

	GLfloat* UVs = &uv_data.UVs[0];
	GLfloat* Vertices = &data.Vertices[0];
	GLfloat* Colors = &data.Colors[0];

	glTexCoordPointer(2, GL_FLOAT, 0, UVs);
	glVertexPointer(3, GL_FLOAT, 0, Vertices);
	glColorPointer(3, GL_FLOAT, 0, Colors);
	//17 404
	glDrawArrays(GL_QUADS, 0, data.face_count * 4);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (Debug::bounds) {
		Debug::DrawBox(
			{ shift, shift, shift },
			{ s, s, s },
			{ .9f, .9f, .9f });
	}

	if (ticks <= timeout && ticks > -1) {
		ticks++;
	}

	if (ticks > timeout) {
		UpdateMem();
		Update();
		ticks = -1;
	}
}