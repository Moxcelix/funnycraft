#pragma once
#include "VertexData.h"
#include "UVData.h"
#include "Block.h"
#include "World.h"
#include "Vector3.h"
#include "Typedefs.h"

#include <GLFW/glfw3.h>
#include<iostream>
#include<fstream>
#include<string>
#include<mutex>

class Block;
class World;
class Terrain;
/// <summary>
/// структура чанка
/// </summary>
struct Chunk
{
public:
	Chunk(); // конструктор чанка
	~Chunk(); // деструктор чанка
	Chunk(int x, int y, int z, World* world); // конструктор чанка

	const static int ChunkSize = 16; // размер чанка
	const static int MaxLight = 7;	// максимальное значение света

	Vector3Int pos;				// позиция чанка
	World* world;				// ссылка на мир
	Chunk* chunks[27];			// массив соседних чанков

	bool modified = false;		// флаг модификации чанка
	bool is_solid = false;		// флаг чанка поверхности (для света)
	static float brightness;	// яркость

	block_id GetBlockID(int x, int y, int z); // получение ID блока по координатам
	inline Block const * GetBlock(int x, int y, int z); // получение ссылки на блок по координатам
	inline Block const * GetBlock(block_id ID); // получение ссылки на блок по ID
	inline bool InRange(int x); // проверка на попадание в рамки чанка по одному измерению
	inline bool InRange(int x, int y, int z); // проверка на попадание в рамки чанка по трём измерениям

	bool InLightRange(int x, int y, int z); // проверка на попадание в рамки чанка по трём измерениям для света
	void Generate(); // генерация чанка
	void GetMeshData(VertexData* data, UVData* uv, int x, int y, int z, int layer = 0); // формирование данных меша
	void Render(unsigned int texture); // рендеринг чанка
	void UpdateMesh(); // обновление меша
	void Update(); // обновление
	void UpdateMem(); // обновление мемофикаций
	void Init(); // инициализация чанка
	void SetBlock(int x, int y, int z, block_id block);	// установка блока по координатам и ID
	void SetBlock(int x, int y, int z, Block* block);	// установка блока по координатам и ссылке
	void Modify(); // изменение чанка
	void SaveChunk(); // сохранение чанка
	bool LoadChunk();  // загрузка чанка
	inline void SetLight(int x, int y, int z, char l); // установка света 
	void UpdateLight(); // обновление карты света
	inline char GetLight(int x, int y, int z); // значение света по координатами
	inline int CheckLight(int x, int y, int z); // проверка значения света по координатами
	Vector3 GetLigthColor(int x, int y, int z); // цвет света по координатам

	
private:
	VertexData data;	// данные вершин
	UVData uv_data;		// данные UV-карты

	block_id blocks[ChunkSize][ChunkSize][ChunkSize];				// трёхмерный массив-карта блоков
	bool sky_light[ChunkSize][ChunkSize];							// двумерный массив-карта небесного света
	char light_map[ChunkSize * 3][ChunkSize * 3][ChunkSize * 3];	// карта значений света
	char buffer_light_map[ChunkSize][ChunkSize][ChunkSize];			// буффер значений света соседних чанков
	
	int ticks = 0;		// тики
	int timeout = 4;	// таймаут повторного обновления

	void ClearCash();	// очистка данных
};
