#include<iostream>
#include<fstream>

#include "World.h"

Terrain terrain;
int World::render_distance = 4;
int World::seed = 0;
std::string World::name = "my world";
World::Settings World::settings = World::Settings();

World::World(Player* player) {
	this->player = player;
	player->world = this;

	Create();
}

void World::Create() {
	//SetConsoleCP(1251);
	//SetConsoleOutputCP(1251);

	//srand(time(NULL));	// инициализация датчика случайных чисел
	std::ifstream istream;	// поток чтения
	istream.open(save_folder + name + "/world.bin"); // открытие потока

	if (istream.is_open()) {
		float x, y, z, xRot, zRot;
		w_time time_value;
		Debug::Log("world \"" + name + "\" opened");
		istream >> World::seed;
		istream >> x >> y >> z >> xRot >> zRot;
		for (int i = 0; i < 4; i++) istream >> settings.params[i];
		istream >> time_value;
		istream.close();
		player->Init(x, y, z, xRot, zRot);
		time.set(time_value);
	}
	else {
		Debug::Log("world \"" + name + "\" created");
		World::seed = rand();
		player->Init(
			player->start_pos.x,
			player->start_pos.y,
			player->start_pos.z,
			0, 90); \
	}
	chunks_loaded = 0;
	sky = new Sky();
	render_queue = new PosQueue();
	create_queue = new PosQueue();
	update_queue = new PosQueue();
	global_update_queue = new PosQueue();
	terrain = Terrain(World::seed, this);
}

World::~World() {
	std::ofstream stream;
	stream.open(save_folder + name + "/world.bin");
	if (stream.is_open()) {
		stream << World::seed << std::endl;
		player->Save(stream);
		for (int i = 0; i < 4; i++)
			stream << settings.params[i] << std::endl;
		stream << time.get_time() << std::endl;
		stream.close();
	}
	global_buffer.clear();

	delete render_queue;
	delete create_queue;
	delete update_queue;
	delete global_update_queue;
	delete sky;

	// удаление чанков
	for (int i = 0; i < chunks_loaded; i++)
		delete chunks[i];
}

void World::UpdateWorldLighting() {
	Debug::Log("global lighting update");

	for (int i = 0; i < chunks_loaded; i++) {
		auto pos = chunks[i]->get_pos();

		if (!global_update_queue->contains(pos)) {
			global_update_queue->add(pos);
		}
	}
}

void World::Clear() {
	global_buffer.clear();
	render_queue->clear();
	create_queue->clear();
	for (int i = 0; i < chunks_loaded; i++)
		delete chunks[i];
	chunks_loaded = 0;
}

void World::AddToCreate(int x, int y, int z) {
	if (z < 0 || z >= WorldHeight * Chunk::size)
		return;

	if (Chunk* c = GetChunk(x, y, z))
		return;

	x = static_cast<int>(floor(x / static_cast<double>(Chunk::size))) * Chunk::size;
	y = static_cast<int>(floor(y / static_cast<double>(Chunk::size))) * Chunk::size;
	z = static_cast<int>(floor(z / static_cast<double>(Chunk::size))) * Chunk::size;

	create_queue->add({ x, y, z });
}

bool World::CreateChunk(int x, int y, int z) {
	if (chunks_loaded >= MaxChunksCount)
		return true;

	const auto chunk = GetChunk(x, y, z);

	if (chunk)
		return false;

	if (Debug::active)
		Debug::log << "chunk {" << x << ", " << y << ", " << z << "} created" << std::endl;

	float d = Vector3Int::Distance(player->int_position, Vector3Int(x, y, z));

	if (d > render_distance * Chunk::size)
		return false;

	chunks[chunks_loaded] = new Chunk(x, y, z, this);
	chunks[chunks_loaded]->UpdateMem();
	chunks[chunks_loaded]->Generate();
	chunks[chunks_loaded]->RecalculateSkyLightSolidity();
	chunks[chunks_loaded]->Update();

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			for (int k = -1; k <= 1; k++) {
				if (i == 0 && j == 0 && k == 0)
					continue;

				int X = i * Chunk::size + x;
				int Y = j * Chunk::size + y;
				int Z = k * Chunk::size + z;

				RenderIfExcist(X, Y, Z);
			}
		}
	}

	chunks_loaded++;

	return true;
}

void World::Render(unsigned int texture) {
	for (int i = 0; i < chunks_loaded; i++)
	{
		if (!chunks[i])
			continue;

		const auto& pos = chunks[i]->get_pos();
		glTranslatef(pos.x, pos.y, pos.z);
		chunks[i]->Render(texture);
		glTranslatef(-pos.x, -pos.y, -pos.z);
	}
}

block_id World::GetBlockID(int x, int y, int z) {
	if (const auto chunk = GetChunk(x, y, z)) {
		const auto pos = chunk->get_pos();
		return chunk->GetBlockID(
			x - pos.x,
			y - pos.y,
			z - pos.z);
	}

	if (z < 0 || z >= WorldHeight * Chunk::size)
		return Block::air->id;

	return Block::null->id;
}

block_id World::GetBlockID(Vector3 pos) {
	return GetBlockID(pos.x, pos.y, pos.z);
}

Block const* World::GetBlock(int x, int y, int z) {
	return Block::GetBlock(GetBlockID(x, y, z));
}

Block const* World::GetBlock(Vector3 pos) {
	return Block::GetBlock(GetBlockID(pos.x, pos.y, pos.z));
}

Chunk* World::GetChunk(int x, int y, int z) {
	if (z < 0 || z >= WorldHeight * Chunk::size)
		return nullptr;

	x = static_cast<int>(floor(x / static_cast<double>(Chunk::size))) * Chunk::size;
	y = static_cast<int>(floor(y / static_cast<double>(Chunk::size))) * Chunk::size;
	z = static_cast<int>(floor(z / static_cast<double>(Chunk::size))) * Chunk::size;

	for (int i = 0; i < chunks_loaded; i++) {
		const auto pos = chunks[i]->get_pos();

		if (pos.x == x && pos.y == y && pos.z == z) {
			return chunks[i];
		}
	}

	return nullptr;
}

void World::Explode(Vector3 pos) {
	Explode(pos.x, pos.y, pos.z);
}

void World::Explode(int x, int y, int z) {
	int radius = rand() % 3 + 3;

	for (int i = -radius; i <= radius; i++) {
		for (int j = -radius; j <= radius; j++) {
			for (int k = -radius; k <= radius; k++) {
				int r = radius + rand() % 2;
				if (i * i + j * j + k * k <= r * r) {
					const auto id = GetBlockID(x + i, y + j, z + k);

					if (id == Block::bedrock->id)
						continue;

					SetBlock(x + i, y + j, z + k, 0, false);

					if (id == Block::tnt->id) // та самая взрывная реакция :skull2:
						Explode(x + i, y + j, z + k);

					if (const auto chunk = GetChunk(x + i, y + j, z + k))
						chunk->Modify();
				}
			}
		}
	}

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			for (int k = -1; k <= 1; k++) {
				const auto chunk_x = i * Chunk::size + x;
				const auto chunk_y = j * Chunk::size + y;
				const auto chunk_z = k * Chunk::size + z;

				const auto chunk = GetChunk(chunk_x, chunk_y, chunk_z);

				if (const auto chunk = GetChunk(chunk_x, chunk_y, chunk_z)) {
					chunk->Update();
					chunk->RecalculateSkyLightSolidity();
				}
			}
		}
	}

	AddToUpdateColumn(x, y, z);
}

void World::UpdateAtPoint(int x, int y, int z) {
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			for (int k = -1; k <= 1; k++) {
				if (i == 0 && j == 0 && k == 0)
					continue;

				const auto chunk_x = i * Chunk::size + x;
				const auto chunk_y = j * Chunk::size + y;
				const auto chunk_z = k * Chunk::size + z;

				UpdateIfExcist(chunk_x, chunk_y, chunk_z);
			}
		}
	}
}

void World::AddToUpdateAtPoint(int x, int y, int z) {
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			for (int k = -1; k <= 1; k++) {
				if (i == 0 && j == 0 && k == 0)
					continue;

				const auto chunk_x = i * Chunk::size + x;
				const auto chunk_y = j * Chunk::size + y;
				const auto chunk_z = k * Chunk::size + z;

				AddToUpdate(chunk_x, chunk_y, chunk_z);
			}
		}
	}
}

void World::RenderIfExcist(int x, int y, int z) {
	if (const auto chunk = GetChunk(x, y, z)) {
		AddToRender(chunk);
	}
}

void World::UpdateIfExcist(int x, int y, int z) {
	if (const auto chunk = GetChunk(x, y, z)) {
		AddToUpdate(chunk);
	}
}

void World::AddToRender(int x, int y, int z) {
	render_queue->add({ x, y, z });
}

void World::AddToUpdate(int x, int y, int z) {
	update_queue->add({ x, y, z });
}

void World::AddToRender(Chunk* chunk) {
	const auto pos = chunk->get_pos();
	AddToRender(pos.x, pos.y, pos.z);
}

void World::AddToUpdate(Chunk* chunk) {
	const auto pos = chunk->get_pos();
	AddToUpdate(pos.x, pos.y, pos.z);
}

Light World::GetLight(int x, int y, int z) {
	if (const auto chunk = GetChunk(x, y, z)) {
		const auto pos = chunk->get_pos();
		return chunk->GetLight(x - pos.x, y - pos.y, z - pos.z);
	}

	return Light{ LightMap::max_light, 0 };
}

void World::AddToUpdateColumn(int x, int y, int z) {
	z = (z / Chunk::size) * Chunk::size;

	for (int k = z + Chunk::size; k >= 0; k -= Chunk::size) {
		const auto chunk = GetChunk(x, y, k);

		if (!chunk) break;

		for (int i = -Chunk::size; i <= Chunk::size; i += Chunk::size) {
			for (int j = -Chunk::size; j <= Chunk::size; j += Chunk::size) {
				AddToUpdate(x + i, y + j, k);
			}
		}

		if (chunk->get_solidity()) break;
	}
}

void World::Update() {
	UpdateSkyLightIfNeed();
	DoDeleteChunksCycle();
	DoRenderChunksCycle();
	DoCreateChunksCycle();
	DoUpdateChunksCycle();
	DoGlobalUpdateChunksCycle();
	RationalizeBuffer();
	RemoveCash();
}

void World::DoDeleteChunksCycle() {
	for (int i = 0; i < chunks_loaded; i++) {
		const auto d = Vector3Int::Distance(
			player->int_position, chunks[i]->get_pos());

		if (d > render_distance * Chunk::size) {
			DeleteChunk(i);
		}
	}
}
void World::DoRenderChunksCycle() {
	for (int i = 0; i < render_chunk_throughput; i++) {
		if (render_queue->size()) {
			auto& pos = render_queue->back();

			if (update_queue->contains(pos)) {
				render_queue->pop_back();
				continue;
			}

			if (const auto chunk = GetChunk(pos.x, pos.y, pos.z)) {
				chunk->UpdateMem();
				chunk->Update();
			}
			render_queue->pop_back();
		}
	}
}
void World::DoCreateChunksCycle() {
	for (int i = 0; i < create_chunk_throughput; i++) {
		while (create_queue->size()) {
			auto& pos = create_queue->back();
			const auto done = CreateChunk(pos.x, pos.y, pos.z);
			create_queue->pop_back();
			if (done) break;
		}
	}
}
void World::DoUpdateChunksCycle() {
	for (int i = 0; i < update_chunk_throughput; i++) {
		if (update_queue->size())
		{
			auto& pos = update_queue->front();

			if (const auto chunk = GetChunk(pos.x, pos.y, pos.z)) {
				chunk->Update();
			}

			update_queue->pop_front();
		}
	}
}
void World::DoGlobalUpdateChunksCycle() {
	for (int i = 0; i < render_chunk_throughput * 2; i++) {
		if (global_update_queue->size())
		{
			auto& pos = global_update_queue->front();

			if (const auto chunk = GetChunk(pos.x, pos.y, pos.z)) {
				chunk->UpdateMesh();
			}

			global_update_queue->pop_front();
		}
	}
}

void World::DeleteChunk(int index) {
	const auto pos = chunks[index]->get_pos();
	const auto d = Vector3Int::Distance(player->int_position, pos);

	if (d <= render_distance * Chunk::size) {
		return;
	}

	const auto x = pos.x;
	const auto y = pos.y;
	const auto z = pos.z;

	if (Debug::active) {
		Debug::log << "chunk {" << x << ", " << y << ", " << z << "} deleted" << std::endl;
	}

	delete chunks[index];
	chunks[index] = nullptr;
	chunks_loaded--;

	if (chunks_loaded > 0) {
		for (int i = index; i < chunks_loaded; i++) {
			chunks[i] = chunks[i + 1];
		}

		chunks[chunks_loaded] = nullptr;
	}

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			for (int k = -1; k <= 1; k++) {
				if (i == 0 && j == 0 && k == 0)
					continue;

				int chunk_x = i * Chunk::size + x;
				int chunk_y = j * Chunk::size + y;
				int chunk_z = k * Chunk::size + z;

				if (const auto chunk = GetChunk(chunk_x, chunk_y, chunk_z)) {
					chunk->UpdateMem();
				}
			}
		}
	}
}

void World::UpdateIfEqual(int value1, int value2, int x, int y, int z) {
	if (value1 == value2) {
		return;
	}

	if (const auto chunk = GetChunk(x, y, z)) {
		chunk->Update();
	}
}

void World::RemoveCash() {
	const auto length = global_buffer.size();
	sort(to_remove.begin(), to_remove.end(), std::greater<int>());
	for (int i = 0; i < to_remove.size(); i++) {
		if (to_remove[i] < length) {
			global_buffer.erase(global_buffer.begin() + to_remove[i]);
		}
	}
	to_remove.clear();
}

void World::RationalizeBuffer() {
	for (int i = 0; i < global_buffer.size(); i++) {
		if (player->GetDistance(global_buffer[i].x, 
			global_buffer[i].y, global_buffer[i].z) > 
			(render_distance + 1.1) * Chunk::size){
			to_remove.push_back(i);
		}
	}
}

void World::SetBlock(Vector3 pos, block_id block, bool update) {
	SetBlock(pos.x, pos.y, pos.z, block, update);
}

void World::SetBlock(int x, int y, int z, block_id block, bool update)
{
	if (const auto chunk = GetChunk(x, y, z)) {
		const auto pos = chunk->get_pos();
		chunk->SetBlock(x - pos.x, y - pos.y, z - pos.z, block);
		if (update) {
			chunk->Update();
			chunk->Modify();
			chunk->RecalculateSkyLightSolidity();

			UpdateIfEqual(x - pos.x, 0, x - 1, y, z);
			UpdateIfEqual(x - pos.x, Chunk::size - 1, x + 1, y, z);
			UpdateIfEqual(y - pos.y, 0, x, y - 1, z);
			UpdateIfEqual(y - pos.y, Chunk::size - 1, x, y + 1, z);
			UpdateIfEqual(z - pos.z, 0, x, y, z - 1);
			UpdateIfEqual(z - pos.z, Chunk::size - 1, x, y, z + 1);

			UpdateAtPoint(x, y, z);
			AddToUpdateColumn(x, y, z);
			AddToUpdate(chunk);
		}
	}
	else {
		SetBufferBlock(x, y, z, block);
	}
}

void World::SetBufferBlock(int x, int y, int z, block_id block) {
	if (const auto chunk = GetChunk(x, y, z)) {
		const auto pos = chunk->get_pos();
		chunk->SetBlock(x - pos.x, y - pos.y, z - pos.z, block);
		return;
	}

	const auto block_pos = BlockPos{ x ,y, z, block };
	BlockPos* finded = nullptr;

	if (find_if(global_buffer.begin(), global_buffer.end(),
		[&](BlockPos p) -> bool {
			if (p.x == block_pos.x && p.y == block_pos.y && p.z == block_pos.z) {
				finded = &p;
				return true;
			}
			return false;
		}) == global_buffer.end()) {
		global_buffer.push_back(block_pos);
	}
	else {
		finded->block = block;
	}
}

void World::UpdateSkyLightIfNeed() {
	const auto curr_time_value = time.get_time_normal();

	if (prev_time_value != curr_time_value) {
		prev_time_value = curr_time_value;

		UpdateWorldLighting();
	}
}

void World::Generate(Chunk* chunk) {
	terrain.Generate(chunk);
}