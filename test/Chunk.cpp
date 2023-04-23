#include "Chunk.h"
#include "LightMap.h"
#include "Client.h"

float Chunk::brightness = 0.1f;

Chunk::Chunk() {
	pos = { 0, 0, 0 };

	Initialize();
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

	Initialize();
}

inline block_id Chunk::GetBlockID(int x, int y, int z) {
	if (InRange(x, y, z)) return blocks[x][y][z];

	if (z + pos.z < 0 || z + pos.z >= size * world->WorldHeight)
		return 0;

	if (InLightRange(LightMap::light_sampling + x,
		LightMap::light_sampling + y, LightMap::light_sampling + z)) {
		const auto chunk_x = (size + x) / size;
		const auto chunk_y = (size + y) / size;
		const auto chunk_z = (size + z) / size;

		const auto chunk = chunks[chunk_x + chunk_y * 3 + chunk_z * 9];

		if (chunk) {
			const auto block_x = pos.x - chunk->pos.x + x;
			const auto block_y = pos.y - chunk->pos.y + y;
			const auto block_z = pos.z - chunk->pos.z + z;

			if (InRange(block_x, block_y, block_z)) {
				return chunk->blocks[block_x][block_y][block_z];
			}
			else {
				int i = 2;
			}
		}

		return Block::null->id;
	}
	else {
		return world->GetBlockID(x + pos.x, y + pos.y, z + pos.z);
	}
}

void Chunk::ClearCash() {
	data.Clear();
	uv_data.Clear();
}

void Chunk::Initialize() {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			for (int k = 0; k < size; k++) {
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
	const auto block = GetBlock(GetBlockID(x, y, z));

	if (block->GetRenderLayer() == layer)
		block->GetMeshData(data, uv, x, y, z, this);
}

inline Block const* Chunk::GetBlock(int x, int y, int z) {
	const auto id = GetBlockID(x, y, z);
	return Block::GetBlock(id);
}

inline bool Chunk::InRange(int x) {
	return x >= 0 && x < size;
}

inline bool Chunk::InRange(int x, int y, int z) {
	return InRange(x) && InRange(y) && InRange(z);
}

bool Chunk::InLightRange(int x, int y, int z) {
	return (x >= 0 && x < size + LightMap::light_sampling * 2) &&
		(y >= 0 && y < size + LightMap::light_sampling * 2) &&
		(z >= 0 && z < size + LightMap::light_sampling * 2);
}

void Chunk::UpdateMesh() {
	ClearCash();

	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			for (int z = 0; z < size; z++) {
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

	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			bool detected = false;
			for (int z = 0; z < size; z++) {
				if (!detected) {
					if (!GetBlock(x, y, z)->Transparent()) {
						detected = true;
					}
				}
			}

			if (detected) b_count++;
		}
	}

	is_solid = b_count >= size * size;
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

	for (int x = 0; x < Chunk::size; x++) {
		for (int y = 0; y < Chunk::size; y++) {
			for (int z = 0; z < Chunk::size; z++) {
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
		for (int x = 0; x < Chunk::size; x++) {
			for (int y = 0; y < Chunk::size; y++) {
				for (int z = 0; z < Chunk::size; z++) {
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
				const auto shift_x = (x - 1) * size;
				const auto shift_y = (y - 1) * size;
				const auto shift_z = (z - 1) * size;

				chunks[x + y * 3 + z * 9] =
					world->GetChunk(
						pos.x + shift_x,
						pos.y + shift_y,
						pos.z + shift_z);
			}
		}
	}
}

void Chunk::UpdateLight() {
	for (int x = 0; x < size + LightMap::light_sampling * 2; x++) {
		for (int y = 0; y < size + LightMap::light_sampling * 2; y++) {
			for (int z = 0; z < size + LightMap::light_sampling * 2; z++) {
				light_map[x][y][z] = { 0, 0 };
			}
		}
	}

	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			for (int z = 0; z < size; z++)
			{
				buffer_light_map[x][y][z] = { 0, 0 };

				if (const auto light = GetBlock(x, y, z)->Luminosity()) {

					if (Client::settings.recursive_lighting) {
						Recursive_SetBlockLight(x, y, z, light);
					}
					else {
						light_map[x + LightMap::light_sampling]
							[y + LightMap::light_sampling]
						[z + LightMap::light_sampling].block = light;
					}
				}
			}
		}
	}

	const auto chunk = chunks[22]; // верхний чанк

	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			sky_light[x][y] = true;
			for (int z = size - 1; z >= 0; z--) {
				if ((!chunk || chunk->sky_light[x][y]) && GetBlock(x, y, z)->Transparent()) {
					if (Client::settings.recursive_lighting) {
						Recursive_SetSkyLight(x, y, z, LightMap::max_light);
					}
					else {
						light_map[x + LightMap::light_sampling]
							[y + LightMap::light_sampling]
						[z + LightMap::light_sampling].sky = LightMap::max_light;
					}
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
			const auto sx = pos.x - chunks[i]->pos.x;
			const auto sy = pos.y - chunks[i]->pos.y;
			const auto sz = pos.z - chunks[i]->pos.z;

			for (int x = 0; x < size; x++) {
				for (int y = 0; y < size; y++) {
					for (int z = 0; z < size; z++) {
						if (!InLightRange(x + sx + LightMap::light_sampling,
							y + sy + LightMap::light_sampling,
							z + sz + LightMap::light_sampling)) continue;

						// значение света в соседнем чанке в области этого чанка
						const auto l = chunks[i]->CheckLight(x + sx, y + sy, z + sz);

						if (l.sky > buffer_light_map[x][y][z].sky)
							buffer_light_map[x][y][z].sky = l.sky;

						if (l.block > buffer_light_map[x][y][z].block)
							buffer_light_map[x][y][z].block = l.block;
					}
				}
			}
		}
	}

	if (Client::settings.recursive_lighting) return;

	for (unsigned char l = LightMap::max_light; l > 0; l--) {
		for (int x = 0; x < size + LightMap::light_sampling * 2; x++) {
			for (int y = 0; y < size + LightMap::light_sampling * 2; y++) {
				for (int z = 0; z < size + LightMap::light_sampling * 2; z++) {
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

void Chunk::Recursive_SetBlockLight(Direction direction,
	int x, int y, int z, unsigned char l) {
	if (light_map
		[x + LightMap::light_sampling]
	[y + LightMap::light_sampling]
	[z + LightMap::light_sampling].block >= l) {
		return;
	}

	light_map
		[x + LightMap::light_sampling]
	[y + LightMap::light_sampling]
	[z + LightMap::light_sampling].block = l;

	if (l == 1) return;

	if (direction != Direction::Left && GetBlock(x + 1, y, z)->Transparent())
		Recursive_SetBlockLight(Direction::Right, x + 1, y, z, l - 1);
	if (direction != Direction::Right && GetBlock(x - 1, y, z)->Transparent())
		Recursive_SetBlockLight(Direction::Left, x - 1, y, z, l - 1);
	if (direction != Direction::Back && GetBlock(x, y + 1, z)->Transparent())
		Recursive_SetBlockLight(Direction::Front, x, y + 1, z, l - 1);
	if (direction != Direction::Front && GetBlock(x, y - 1, z)->Transparent())
		Recursive_SetBlockLight(Direction::Back, x, y - 1, z, l - 1);
	if (direction != Direction::Down && GetBlock(x, y, z + 1)->Transparent())
		Recursive_SetBlockLight(Direction::Up, x, y, z + 1, l - 1);
	if (direction != Direction::Up && GetBlock(x, y, z - 1)->Transparent())
		Recursive_SetBlockLight(Direction::Down, x, y, z - 1, l - 1);
}

void Chunk::Recursive_SetSkyLight(Direction direction,
	int x, int y, int z, unsigned char l) {
	if (light_map
		[x + LightMap::light_sampling]
	[y + LightMap::light_sampling]
	[z + LightMap::light_sampling].sky >= l) {
		return;
	}

	light_map
		[x + LightMap::light_sampling]
	[y + LightMap::light_sampling]
	[z + LightMap::light_sampling].sky = l;

	if (l == 1) return;

	if (direction != Direction::Left && GetBlock(x + 1, y, z)->Transparent())
		Recursive_SetSkyLight(Direction::Right, x + 1, y, z, l - 1);
	if (direction != Direction::Right && GetBlock(x - 1, y, z)->Transparent())
		Recursive_SetSkyLight(Direction::Left, x - 1, y, z, l - 1);
	if (direction != Direction::Back && GetBlock(x, y + 1, z)->Transparent())
		Recursive_SetSkyLight(Direction::Front, x, y + 1, z, l - 1);
	if (direction != Direction::Front && GetBlock(x, y - 1, z)->Transparent())
		Recursive_SetSkyLight(Direction::Back, x, y - 1, z, l - 1);
	if (direction != Direction::Down && GetBlock(x, y, z + 1)->Transparent())
		Recursive_SetSkyLight(Direction::Up, x, y, z + 1, l - 1);
	if (direction != Direction::Up && GetBlock(x, y, z - 1)->Transparent())
		Recursive_SetSkyLight(Direction::Down, x, y, z - 1, l - 1);
}

void Chunk::Recursive_SetBlockLight(int x, int y, int z, unsigned char l) {
	if (l <= 0) return;

	if (GetBlock(x + 1, y, z)->Transparent()) {
		Recursive_SetBlockLight(Direction::Right, x + 1, y, z, l - 1);
	}

	if (GetBlock(x - 1, y, z)->Transparent()) {
		Recursive_SetBlockLight(Direction::Left, x - 1, y, z, l - 1);
	}

	if (GetBlock(x, y + 1, z)->Transparent()) {
		Recursive_SetBlockLight(Direction::Front, x, y + 1, z, l - 1);
	}

	if (GetBlock(x, y - 1, z)->Transparent()) {
		Recursive_SetBlockLight(Direction::Back, x, y - 1, z, l - 1);
	}

	if (GetBlock(x, y, z + 1)->Transparent()) {
		Recursive_SetBlockLight(Direction::Up, x, y, z + 1, l - 1);
	}

	if (GetBlock(x, y, z - 1)->Transparent()) {
		Recursive_SetBlockLight(Direction::Down, x, y - 1, z, l - 1);
	}
}

void Chunk::Recursive_SetSkyLight(int x, int y, int z, unsigned char l) {
	if (l <= 0) return;

	if (GetBlock(x + 1, y, z)->Transparent()) {
		Recursive_SetSkyLight(Direction::Right, x + 1, y, z, l - 1);
	}

	if (GetBlock(x - 1, y, z)->Transparent()) {
		Recursive_SetSkyLight(Direction::Left, x - 1, y, z, l - 1);
	}

	if (GetBlock(x, y + 1, z)->Transparent()) {
		Recursive_SetSkyLight(Direction::Front, x, y + 1, z, l - 1);
	}

	if (GetBlock(x, y - 1, z)->Transparent()) {
		Recursive_SetSkyLight(Direction::Back, x, y - 1, z, l - 1);
	}

	if (GetBlock(x, y, z + 1)->Transparent()) {
		Recursive_SetSkyLight(Direction::Up, x, y, z + 1, l - 1);
	}

	if (GetBlock(x, y, z - 1)->Transparent()) {
		Recursive_SetSkyLight(Direction::Down, x, y - 1, z, l - 1);
	}
}

constexpr void Chunk::SetSkyLight(int x, int y, int z, unsigned char l) {
	if (!GetBlock(x, y, z)->Transparent()) return;

	if (light_map[x + LightMap::light_sampling][y + LightMap::light_sampling][z + LightMap::light_sampling].sky < l) {
		light_map[x + LightMap::light_sampling][y + LightMap::light_sampling][z + LightMap::light_sampling].sky = l;
	}
}

constexpr void Chunk::SetBlockLight(int x, int y, int z, unsigned char l) {
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
		if (pos.z + z < 0 || pos.z + z >= world->WorldHeight * size) {
			auto light = light_map[x + LightMap::light_sampling]
				[y + LightMap::light_sampling][z + LightMap::light_sampling];

			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					if (i == 1 && j == 1) continue;

					if (const auto chunk = chunks[i + j * 3 + 9]) {
						const auto _x = x + (2 - i) * size;
						const auto _y = y + (2 - j) * size;
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

	const auto s = Chunk::size * 0.5f;
	const auto shift = Chunk::size * 0.5f - 0.5f;

	glBindTexture(GL_TEXTURE_2D, texture);

	const auto uvs = &uv_data.UVs[0];
	const auto vertices = &data.Vertices[0];
	const auto colors = &data.Colors[0];

	glTexCoordPointer(2, GL_FLOAT, 0, uvs);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glColorPointer(3, GL_FLOAT, 0, colors);
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
		UpdateMesh();
		ticks = -1;
	}
}