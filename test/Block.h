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
/// <summary>
/// класс блока
/// </summary>
class Block 
{
public:
	Block(block_id id, std::string name); // конструктор блока
	Block& operator= (const Block&) = delete;
	Block(const Block&) = delete;
	block_id id = 0; // ID блока 
	std::string name = "?"; // им€ блока
	Block(); // конструктор блока
	// ссылки на статичные экземпл€ры блоков
	static Block* air;					// воздух
	static Block* grass;				// дЄрн
	static Block* stone;				// камень
	static Block* oak;					// дуб
	static Block* birch;				// берЄза
	static Block* leaves;				// листва
	static Block* dirt;					// земл€
	static Block* cobblestone;			// булыжник
	static Block* glass;				// стекло
	static Block* tall_grass;			// высока€ трава
	static Block* poppy;				// мак
	static Block* dandelion;			// одуванчик
	static Block* planks;				// доски
	static Block* sand;					// песок
	static Block* wool;					// шерсть
	static Block* tnt;					// тнт
	static Block* obsidian;				// обсидиан
	static Block* glowstone;			// светокамень
	static Block* birch_leaves;			// лисвта берЄзы
	static Block* bedrock;				// коренна€ порода
	static Block* mossy_cobblestone;	// замшелый булыжник
	static Block* bricks;				// кирпичи
	static Block* null;					// пустота
	
	static Block** blocks;				// массив блоков

	operator int() const // оператор приведени€
	{
		return id;
	}

	static std::vector<Block*> BlockList; // вектор блоков

	static constexpr Block* GetBlock(block_id ID)
	{
		if (ID == -1)
			return null;

		return blocks[ID];
	}

	static void Init(); // инициализаци€ 

	enum class Direction : int
	{
		Up = 0,
		Down = 1,
		Right = 2,
		Left = 3,
		Front = 4,
		Back = 5
	};
	/// <summary>
	/// перечисление слоЄв блоков
	/// </summary>
	enum class BlockLayer
	{
		AIR,
		GLASS,
		VOID,
		HERB,
		LEAVES,
		OTHER
	};

	virtual void GetMeshData(VertexData* data, UVData* uv, int x, int y, int z, Chunk* chunk); // получение данных меша блока
	inline virtual BlockLayer GetLayer(); // слой блока
	inline virtual Vector2 TileCoord(Direction direction); // координаты текстур граней блока
	inline virtual Vector3 Color(Direction direction); // цвета граней
	inline virtual char GetRenderLayer(); // получение сло€ рендеринга
	inline virtual bool Transparent(); // прозрачность дл€ света
	inline virtual char Luminosity(); // прозрачность дл€ света
	inline virtual bool Solid(); // твЄрдость
	inline virtual Bound GetBounds(); // границы блока
};

