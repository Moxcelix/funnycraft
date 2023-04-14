#pragma once
#include "VertexData.h"
#include "UVData.h"
#include "Chunk.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Bound.h"
#include "Typedefs.h"
#include<iostream>
#include<vector>

struct Chunk;

class Block {
public:
	Block(block_id id, std::string name); 
	Block(const Block&) = delete;
	Block();

	Block& operator= (const Block&) = delete;
	block_id id = 0; 
	std::string name = "?";

	const static Block* air;					
	const static Block* grass;				
	const static Block* stone;				
	const static Block* oak;					
	const static Block* birch;				
	const static Block* leaves;				
	const static Block* dirt;					
	const static Block* cobblestone;			
	const static Block* glass;				
	const static Block* tall_grass;			
	const static Block* poppy;				
	const static Block* dandelion;			
	const static Block* planks;				
	const static Block* sand;										
	const static Block* tnt;					
	const static Block* obsidian;				
	const static Block* glowstone;			
	const static Block* birch_leaves;			
	const static Block* bedrock;				
	const static Block* mossy_cobblestone;	
	const static Block* bricks;				
	const static Block* sandstone;				
	const static Block* cactus;				
	const static Block* wool_white;				
	const static Block* wool_black;				
	const static Block* null;					
	
	static Block** blocks;

	operator int() const {
		return id;
	}

	static std::vector<Block*> BlockList; 

	static constexpr Block const* GetBlock(block_id ID) {
		if (ID == -1) return null;

		return blocks[ID];
	}

	static void Init();

	enum class Direction : int {
		Up = 0,
		Down = 1,
		Right = 2,
		Left = 3,
		Front = 4,
		Back = 5
	};

	enum class BlockLayer {
		AIR,
		GLASS,
		VOID,
		HERB,
		LEAVES,
		CACTUS,
		OTHER
	};

	virtual void GetMeshData(VertexData*, UVData*, int, int, int, Chunk*) const; 
	inline virtual BlockLayer GetLayer() const;
	inline virtual Vector2 TileCoord(Direction) const; 
	inline virtual Vector3 Color(Direction) const;
	inline virtual char GetRenderLayer() const;
	inline virtual bool Transparent() const;
	inline virtual char Luminosity() const;
	inline virtual bool Solid() const;
	inline virtual Bound GetBounds() const;
};

