#pragma once
#include "World.h"
#include "PerlinNoise.h"
#include "Typedefs.h"
#include <cmath>

class World;
class Block;
struct Chunk;

class Terrain {
public:
	int seed = 0;
	block_id* ground_mas = new block_id[]{ 4,2 };
	int ground_length = 2;
	int ground_height = 10; 
	int land_height = 120; 

	PerlinNoise perlinNoise; 
	World* world;

	Terrain(int seed, World* world) {
		this->seed = seed;
		this->world = world;
		perlinNoise = PerlinNoise(seed);
	}
	Terrain() {}

	inline float linear(float, float, float, float, float);
	inline float pick(float, float, float);

	inline void SetGround(Chunk*, int, int, int);
	inline void SetBlock(Chunk*, int, int, int, block_id);
	inline void SetBlock(Chunk*, int, int, int, Block*); 

	int GetNoise(int, int, int, float = 0.05f, int = 2); 
	int GetTerrainNoise(int, int, int);

	void Generate(Chunk*); 
	void SetTree(Chunk*, int, int, int, int, block_id, block_id);
	void SetRandomPlant(Chunk*, int, int, int);
	void SetRandomTree(Chunk*, int, int, int);
};

