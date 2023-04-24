#pragma once

#include<queue>
#include<stack>
#include <thread>

#include "Terrain.h"
#include "Player.h"
#include "Debug.h"
#include "LightMap.h"
#include "WorldTime.h"
#include "ChunkUpdateTask.h"
#include "Sky.h"

#include "static_queue.h"

class Terrain;
class Player;
struct Chunk;

class World {
public:
	static struct Settings {
		const static int count = 4;
		bool params[count]{ true, true, true, true };

		void Clear() {
			for (int i = 0; i < count; i++)
				params[i] = true;
		}
	}settings;

	struct BlockPos {
		int x, y, z;
		block_id block;
	};

	constexpr static int max_chunks_count = 512 * 8;
	constexpr static int world_height = 16;

	std::vector<BlockPos> global_buffer;
	std::vector<int> to_remove;

	Player* player;
	Chunk* chunks[max_chunks_count];

	Sky sky;
	WorldTime time;

	unsigned int render_distance;

	inline Chunk* GetChunk(int x, int y, int z);

	static int seed;
	static std::string name;

	int chunks_updating = 0;
	int chunks_loaded = 0;

	Block const* GetBlock(int x, int y, int z);
	Block const* GetBlock(Vector3 pos);

	block_id GetBlockID(int x, int y, int z);
	block_id GetBlockID(Vector3 pos);

	void AddToCreate(Vector3Int);
	void AddToDelete(Vector3Int);
	bool CreateChunk(Vector3Int pos);

	void AddToRender(Chunk* chunk);
	void AddToRender(Vector3Int pos);
	void RenderIfExcist(Vector3Int pos);

	void AddToUpdate(Chunk* chunk, ChunkUpdateTask task);
	void AddToUpdate(Vector3Int pos, ChunkUpdateTask task);
	void UpdateIfExcist(Vector3Int pos, ChunkUpdateTask task);

	void Update();
	void UpdateWorldLighting();
	void Generate(Chunk* chunk);
	void SetBlock(int x, int y, int z, block_id block, bool update = false);
	void SetBlock(Vector3 pos, block_id block, bool update = false);
	void Render(unsigned int texture);
	void Clear();
	void Create();
	void DeleteChunk(Vector3Int);
	void RationalizeBuffer();
	void RemoveCash();
	void UpdateAtPoint(Vector3Int pos, ChunkUpdateTask task);
	void UpdateIfEqual(int value1, int value2, int x, int y, int z);
	void Explode(int x, int y, int z);
	void Explode(Vector3 pos);
	void AddToUpdateColumn(int x, int y, int z);
	void SetBufferBlock(int x, int y, int z, block_id block);
	void UpdateSkyLightIfNeed();
	void DoDeleteChunksCycle();
	void DoRenderChunksCycle();
	void DoCreateChunksCycle();
	void DoUpdateChunksCycle();
	void DoGlobalUpdateChunksCycle();
	Light GetLight(int x, int y, int z);

	World(Player* player);
	~World();

private:
	utl::static_queue<Vector3Int> render_queue;
	utl::static_queue<Vector3Int> create_queue;
	utl::static_queue<Vector3Int> delete_queue;
	utl::static_queue<Vector3Int> global_update_queue;
	utl::static_queue<std::pair<Vector3Int, ChunkUpdateTask>> update_queue;

	double prev_time_value{};
};