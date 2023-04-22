#pragma once
#include "VertexData.h"
#include "UVData.h"
#include "Block.h"
#include "World.h"
#include "Vector3.h"
#include "Typedefs.h"
#include "LightMap.h"

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
	Chunk();
	~Chunk();
	Chunk(int x, int y, int z, World* world);

	const static int ChunkSize = 16;

	Vector3Int pos;
	World* world;
	Chunk* chunks[27];

	bool modified = false;
	bool is_solid = false;
	static float brightness;

	block_id GetBlockID(int x, int y, int z);
	inline Block const * GetBlock(int x, int y, int z);
	inline Block const * GetBlock(block_id ID);
	inline bool InRange(int x);
	inline bool InRange(int x, int y, int z);

	bool InLightRange(int x, int y, int z); 
	void Generate();
	void GetMeshData(VertexData* data, UVData* uv, int x, int y, int z, int layer = 0);
	void Render(unsigned int texture); 
	void Update(); 
	void RecalculateSkyLightSolidity();
	void UpdateMem();
	void UpdateMesh();
	void Init(); 
	void SetBlock(int x, int y, int z, block_id block);
	void SetBlock(int x, int y, int z, Block* block);
	void Modify();
	void SaveChunk();
	bool LoadChunk();
	inline void SetSkyLight(int x, int y, int z, unsigned char l);
	inline void SetBlockLight(int x, int y, int z, unsigned char l);
	void UpdateLight();
	inline Light GetLight(int x, int y, int z);
	inline Light CheckLight(int x, int y, int z);
	Vector3 GetLigthColor(int x, int y, int z);

	
private:
	VertexData data;	// данные вершин
	UVData uv_data;		// данные UV-карты

	block_id blocks[ChunkSize][ChunkSize][ChunkSize];
	bool sky_light[ChunkSize][ChunkSize];			
	Light light_map
		[ChunkSize + LightMap::light_sampling * 2]
		[ChunkSize + LightMap::light_sampling * 2]
		[ChunkSize + LightMap::light_sampling * 2];	
	Light buffer_light_map[ChunkSize][ChunkSize][ChunkSize];
	
	int ticks = 0;		// тики
	int timeout = 4;	// таймаут повторного обновления

	void ClearCash();	// очистка данных
};
