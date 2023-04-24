#pragma once
#include <vector>

#include "Vector3.h"
#include "Player.h"
#include "World.h"

class ChunkLoader
{
private:
	std::vector<Vector3Int>sphere;
	Vector3Int int_position;
	World* world;
	Player* player;

	int positions_count;
public:
	static unsigned int render_distance;
	
	ChunkLoader() {
		world = nullptr;
		player = nullptr;
		positions_count = 0;
		GenereateSphere();
	}

	ChunkLoader(World* world, Player* player) :
		world(world), player(player) {
		positions_count = 0;
		GenereateSphere();
	}

	void Update();
	void UpdatePoint();
	void LoadTerrain();
	void UnloadTerrain();
	void GenereateSphere();
};

