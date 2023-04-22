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

/// <summary>
/// обновление глобального освещения
/// </summary>
void World::UpdateWorldLighting() {
	Debug::Log("global lighting update");

	for (int i = 0; i < chunks_loaded; i++)
	{
		Chunk* chunk = chunks[i];

		if (!global_update_queue->contains(chunk->pos)) {
			global_update_queue->add(chunk->pos);
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
/// <summary>
/// добавление чанка в очередь создания
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
void World::AddToCreate(int x, int y, int z)
{
	if (z < 0 || z >= WorldHeight * Chunk::ChunkSize)
		return;

	//chunk* c = GetChunk(x, y, z)

	if (Chunk* c = GetChunk(x, y, z))
	{
		return;
	}

	if (x < 0)
		x -= Chunk::ChunkSize - 1;
	if (y < 0)
		y -= Chunk::ChunkSize - 1;

	x = (x / Chunk::ChunkSize) * Chunk::ChunkSize;
	y = (y / Chunk::ChunkSize) * Chunk::ChunkSize;
	z = (z / Chunk::ChunkSize) * Chunk::ChunkSize;

	create_queue->add({ x, y, z });
}
/// <summary>
/// создание чанка
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
/// <returns></returns>
bool World::CreateChunk(int x, int y, int z)
{
	if (chunks_loaded >= MaxChunksCount)
		return true;

	Chunk* chunk = GetChunk(x, y, z);

	if (chunk)
		return false;

	if (Debug::active)
		Debug::log << "chunk {" << x << ", " << y << ", " << z << "} created" << std::endl;

	float d = Vector3Int::Distance(player->IntPosition, Vector3Int(x, y, z));

	if (d > render_distance * Chunk::ChunkSize)
		return false;

	chunks[chunks_loaded] = new Chunk(x, y, z, this);
	chunks[chunks_loaded]->UpdateMem();
	chunks[chunks_loaded]->Generate();
	chunks[chunks_loaded]->RecalculateSkyLightSolidity();
	chunks[chunks_loaded]->Update();

	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
			for (int k = -1; k <= 1; k++)
			{
				if (i == 0 && j == 0 && k == 0)
					continue;

				int X = i * Chunk::ChunkSize + x;
				int Y = j * Chunk::ChunkSize + y;
				int Z = k * Chunk::ChunkSize + z;

				RenderIfExcist(X, Y, Z);
			}

	chunks_loaded++;

	return true;
}
/// <summary>
/// рендер мира
/// </summary>
void World::Render(unsigned int texture)
{
	for (int i = 0; i < chunks_loaded; i++)
	{
		if (!chunks[i]) // если чанка нет, пропустить
			continue;

		Vector3Int pos = chunks[i]->pos; // позиция i-того чанка
		glTranslatef(pos.x, pos.y, pos.z); // перемещение отрисовки на координаты
		chunks[i]->Render(texture); // рендер чанка
		glTranslatef(-pos.x, -pos.y, -pos.z); // обраное перемещение 
	}
}
/// <summary>
/// ID блока
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
/// <returns>ID блока </returns>
block_id World::GetBlockID(int x, int y, int z)
{
	Chunk* chunk = GetChunk(x, y, z); // запрос чанка по координатам
	if (chunk) // если чанк есть
	{
		return chunk->GetBlockID(
			x - chunk->pos.x,
			y - chunk->pos.y,
			z - chunk->pos.z);
	}

	if (z < 0 || z >= WorldHeight * Chunk::ChunkSize)
		return 0; // ID воздуха

	return -1; // ID пустоты
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
	if (z < 0 || z >= WorldHeight * Chunk::ChunkSize)
		return nullptr;

	x = (int)(floor(x / (float)Chunk::ChunkSize) * Chunk::ChunkSize);
	y = (int)(floor(y / (float)Chunk::ChunkSize) * Chunk::ChunkSize);
	z = (int)(floor(z / (float)Chunk::ChunkSize) * Chunk::ChunkSize);

	for (int i = 0; i < chunks_loaded; i++)
	{
		Chunk* c = chunks[i];

		if (c->pos.x == x && c->pos.y == y && c->pos.z == z)
		{
			return c;
		}
	}

	return nullptr;
}
/// <summary>
/// расчистка территории 
/// </summary>
/// <param name="pos"></param>
void World::Explode(Vector3 pos)
{
	Explode(pos.x, pos.y, pos.z);
}
/// <summary>
/// расчистка территории
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
void World::Explode(int x, int y, int z)
{
	int r = rand() % 3 + 3;

	for (int i = -r; i <= r; i++)
		for (int j = -r; j <= r; j++)
			for (int k = -r; k <= r; k++)
			{
				int R = r + rand() % 2;
				if (i * i + j * j + k * k <= R * R)
				{
					auto id = GetBlockID(x + i, y + j, z + k);

					if (id == Block::bedrock->id)
						continue;

					SetBlock(x + i, y + j, z + k, 0, false);

					if (id == Block::tnt->id) // та самая взрывная реакция :skull2:
						Explode(x + i, y + j, z + k);

					Chunk* chunk = GetChunk(x + i, y + j, z + k);

					if (chunk)
						chunk->Modify();
				}
			}

	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
			for (int k = -1; k <= 1; k++)
			{
				int X = i * Chunk::ChunkSize + x;
				int Y = j * Chunk::ChunkSize + y;
				int Z = k * Chunk::ChunkSize + z;

				Chunk* chunk = GetChunk(X, Y, Z);

				if (chunk)
					chunk->Update(), chunk->RecalculateSkyLightSolidity();
			}

	AddToUpdateColumn(x, y, z);
}
/// <summary>
/// обновить чанки в точке
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
void World::UpdateAtPoint(int x, int y, int z)
{
	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
			for (int k = -1; k <= 1; k++)
			{
				if (i == 0 && j == 0 && k == 0)
					continue;

				int X = i * Chunk::ChunkSize + x;
				int Y = j * Chunk::ChunkSize + y;
				int Z = k * Chunk::ChunkSize + z;

				UpdateIfExcist(X, Y, Z);
			}
}
/// <summary>
/// добавить в очередь обновления в точке
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
void World::AddToUpdateAtPoint(int x, int y, int z)
{
	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
			for (int k = -1; k <= 1; k++)
			{
				if (i == 0 && j == 0 && k == 0)
					continue;

				int X = i * Chunk::ChunkSize + x;
				int Y = j * Chunk::ChunkSize + y;
				int Z = k * Chunk::ChunkSize + z;

				AddToUpdate(X, Y, Z);
			}
}
/// <summary>
/// отрисовать чанк, если существует
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
void World::RenderIfExcist(int x, int y, int z)
{
	Chunk* chunk = GetChunk(x, y, z);

	if (chunk)
		AddToRender(chunk);
}
/// <summary>
/// обновить чанк, если существует
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
void World::UpdateIfExcist(int x, int y, int z)
{
	Chunk* chunk = GetChunk(x, y, z);

	if (chunk)
	{
		AddToUpdate(chunk);
	}
}
/// <summary>
/// добавить в очередь первичной отрисовки
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
void World::AddToRender(int x, int y, int z)
{
	render_queue->add({ x, y, z });
}
/// <summary>
/// добавить в очередь обновления
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
void World::AddToUpdate(int x, int y, int z)
{
	update_queue->add({ x, y, z });
}

void World::AddToRender(Chunk* chunk) {
	AddToRender(chunk->pos.x, chunk->pos.y, chunk->pos.z);
}

void World::AddToUpdate(Chunk* chunk) {
	AddToUpdate(chunk->pos.x, chunk->pos.y, chunk->pos.z);
}

Light World::GetLight(int x, int y, int z) {
	Chunk* chunk = GetChunk(x, y, z);

	if (chunk)
		return chunk->GetLight(x - chunk->pos.x, y - chunk->pos.y, z - chunk->pos.z);

	return Light{ LightMap::max_light, 0 };
}

void World::AddToUpdateColumn(int x, int y, int z) {
	z = (z / Chunk::ChunkSize) * Chunk::ChunkSize;

	for (int k = z + Chunk::ChunkSize; k >= 0; k -= Chunk::ChunkSize) {
		Chunk* chunk = GetChunk(x, y, k);

		if (!chunk) break;

		for (int i = -Chunk::ChunkSize; i <= Chunk::ChunkSize; i += Chunk::ChunkSize)
			for (int j = -Chunk::ChunkSize; j <= Chunk::ChunkSize; j += Chunk::ChunkSize)
				AddToUpdate(x + i, y + j, k);

		if (chunk->is_solid)
			break;
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
		float d = Vector3Int::Distance(player->IntPosition, chunks[i]->pos);

		if (d > render_distance * Chunk::ChunkSize) {
			DeleteChunk(i);
		}
	}
}
void World::DoRenderChunksCycle() {
	for (int i = 0; i < render_chunk_throughput; i++) {
		if (render_queue->size())
		{
			auto& pos = render_queue->back();

			if (update_queue->contains(pos)) {
				render_queue->pop_back();
				continue;
			}

			if (Chunk* chunk = GetChunk(pos.x, pos.y, pos.z)) {
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
			bool done = CreateChunk(pos.x, pos.y, pos.z);
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

			if (Chunk* chunk = GetChunk(pos.x, pos.y, pos.z)) {
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

			if (Chunk* chunk = GetChunk(pos.x, pos.y, pos.z)) {
				chunk->UpdateMesh();
			}

			global_update_queue->pop_front();
		}
	}
}

void World::DeleteChunk(int index) {
	float d = Vector3Int::Distance(player->IntPosition, chunks[index]->pos);

	if (d <= render_distance * Chunk::ChunkSize) {
		return;
	}

	int x = chunks[index]->pos.x;
	int y = chunks[index]->pos.y;
	int z = chunks[index]->pos.z;

	if (Debug::active) {
		Debug::log << "chunk {" << x << ", " << y << ", " << z << "} deleted" << std::endl;
	}

	delete chunks[index];
	chunks[index] = nullptr;
	chunks_loaded--;

	if (chunks_loaded > 0) {
		for (int i = index; i < chunks_loaded; i++){
			chunks[i] = chunks[i + 1];
		}

		chunks[chunks_loaded] = nullptr;
	}

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			for (int k = -1; k <= 1; k++) {
				if (i == 0 && j == 0 && k == 0)
					continue;

				int X = i * Chunk::ChunkSize + x;
				int Y = j * Chunk::ChunkSize + y;
				int Z = k * Chunk::ChunkSize + z;

				if (Chunk* chunk = GetChunk(X, Y, Z)) {
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

	if (Chunk* chunk = GetChunk(x, y, z)) {
		chunk->Update();
	}
}

void World::RemoveCash() {
	int l = global_buffer.size();
	sort(to_remove.begin(), to_remove.end(), std::greater<int>());
	for (int i = 0; i < to_remove.size(); i++) {
		if (to_remove[i] < l)
			global_buffer.erase(global_buffer.begin() + to_remove[i]);
	}
	to_remove.clear();
}

void World::RationalizeBuffer() {
	for (int i = 0; i < global_buffer.size(); i++) {
		if (player->GetDistance(global_buffer[i].x, global_buffer[i].y, global_buffer[i].z) > (render_distance + 1.1) * Chunk::ChunkSize)
		{
			to_remove.push_back(i);
		}
	}
}

void World::SetBlock(Vector3 pos, block_id block, bool update) {
	SetBlock(pos.x, pos.y, pos.z, block, update);
}

void World::SetBlock(int x, int y, int z, block_id block, bool update)
{
	Chunk* chunk = GetChunk(x, y, z);

	if (chunk) {
		chunk->SetBlock(x - chunk->pos.x, y - chunk->pos.y, z - chunk->pos.z, block);
		if (update) {
			chunk->Update();
			chunk->Modify();
			chunk->RecalculateSkyLightSolidity();

			UpdateIfEqual(x - chunk->pos.x, 0, x - 1, y, z);
			UpdateIfEqual(x - chunk->pos.x, Chunk::ChunkSize - 1, x + 1, y, z);
			UpdateIfEqual(y - chunk->pos.y, 0, x, y - 1, z);
			UpdateIfEqual(y - chunk->pos.y, Chunk::ChunkSize - 1, x, y + 1, z);
			UpdateIfEqual(z - chunk->pos.z, 0, x, y, z - 1);
			UpdateIfEqual(z - chunk->pos.z, Chunk::ChunkSize - 1, x, y, z + 1);

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
	if (Chunk* chunk = GetChunk(x, y, z)) {
		chunk->SetBlock(x - chunk->pos.x, y - chunk->pos.y, z - chunk->pos.z, block);
		return;
	}

	BlockPos block_pos = { x ,y, z, block };
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