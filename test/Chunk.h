#pragma once
#include "VertexData.h"
#include "UVData.h"
#include "Block.h"
#include "Direction.h"
#include "World.h"
#include "Vector3.h"
#include "Typedefs.h"
#include "LightMap.h"
#include "ChunkUpdateTask.h"

#include <GLFW/glfw3.h>
#include<iostream>
#include<fstream>
#include<string>
#include<mutex>

class Block;
class World;
class Terrain;

struct Chunk {
public:

	constexpr static int size = 16;

	Chunk();
	Chunk(Vector3Int pos, World* world);
	Chunk(int x, int y, int z, World* world);
	~Chunk();

	inline auto get_solidity() const -> bool { return is_solid; }
	inline auto get_pos() const -> Vector3Int { return pos; }

	inline Block const * GetBlock(int x, int y, int z);
	inline Block const * GetBlock(block_id ID);
	inline bool InRange(int x);
	inline bool InRange(int x, int y, int z);
	inline Light GetLight(int x, int y, int z);
	inline Light CheckLight(int x, int y, int z);
	inline block_id GetBlockID(int x, int y, int z);
	Vector3 GetLigthColor(int x, int y, int z);

	void Generate();
	void Render(unsigned int texture); 
	void Update(ChunkUpdateTask = ChunkUpdateTask::FULL_UPDATE); 
	void RecalculateSkyLightSolidity();
	void UpdateMem();
	void UpdateMesh();
	void SetBlock(int x, int y, int z, block_id block);
	void SetBlock(int x, int y, int z, Block* block);
	void Modify();
	void SaveChunk();
	bool LoadChunk();
	void UpdateLight();
	void UpdateBufferLight();

	void Recursive_SetBlockLight(Direction, int, int, int, unsigned char);
	void Recursive_SetBlockLight(int, int, int, unsigned char);
	void Recursive_SetSkyLight(Direction, int, int, int, unsigned char);
	void Recursive_SetSkyLight(int, int, int, unsigned char);
	
private:
	constexpr static int timeout = 4;
	static float brightness;

	bool modified = false;
	bool is_solid = false;
	int ticks = 0;

	VertexData data;
	UVData uv_data;
	Vector3Int pos;

	Chunk* chunks[27];
	World* world;

	bool sky_light[size][size];			
	block_id blocks[size][size][size];
	Light light_map
		[size + LightMap::light_sampling * 2]
		[size + LightMap::light_sampling * 2]
		[size + LightMap::light_sampling * 2];	
	Light buffer_light_map[size][size][size];

	bool InLightRange(int x, int y, int z);

	void ClearCash();
	void Initialize();
	void GetMeshData(VertexData*, UVData*, int, int, int, int = 0);

	constexpr void SetSkyLight(int x, int y, int z, unsigned char l);
	constexpr void SetBlockLight(int x, int y, int z, unsigned char l);
};
