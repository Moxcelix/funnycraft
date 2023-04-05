#pragma once
#include "World.h"
#include "PerlinNoise.h"
#include "Typedefs.h"
#include <cmath>

class World;
class Block;
struct Chunk;
/// <summary>
/// класс генератора поверхности
/// </summary>
class Terrain
{
public:
	int seed = 0; // зерно генерации 
	block_id* ground_mas = new block_id[] {4,2}; // массив блоков поверхности
	int ground_length = 2; // длина массива блоков поверхности
	int ground_height = 10; // минимальна€ высота
	int land_height = 120; // максимальна€ высота гор

	PerlinNoise perlinNoise; // объект шумов перлина
	World* world;

	inline float linear(float x, float x0, float x1, float y0, float y1); // линейна€ интерпол€ци€
	inline float pick(float x, float x0, float x1); // пикова€ интерпол€ци€
	inline void SetGround(Chunk* chunk, int x, int y, int z); // установка сло€ поверхности
	int GetTerrainNoise(int x, int y, int z); // расчЄт шума ѕерлина
	int GetNoise(int x, int y, int z, float scale = 0.05f, int size = 2); // получение шумов ѕерлина
	void Generate(Chunk* chunk); // генераци€ чанка
	inline void SetBlock(Chunk* chunk, int x, int y, int z, block_id block); // установка блока
	inline void SetBlock(Chunk* chunk, int x, int y, int z, Block* block); // установка блока
	/// <summary>
	/// конструктор
	/// </summary>
	/// <param name="seed"></param>
	Terrain(int seed, World* world) 
	{
		this->seed = seed;
		this->world = world;
		perlinNoise = PerlinNoise(seed);
	}
	Terrain()
	{
	}
	void SetTree(Chunk* chunk, int x, int y, int z, int height, block_id block, block_id leaves); // создание дерева
};

