#pragma once

#include<queue>
#include<stack>
#include <thread>

#include "Terrain.h"
#include "Player.h"
#include "Debug.h"
#include "PosQueue.h"

using namespace std;
class Terrain;
class Player;
struct Chunk;
/// <summary>
/// класс мира
/// </summary>
class World
{
public:	
	/// <summary>
	/// структура параметров
	/// </summary>
	static struct Settings 
	{
		const static int count = 4;
		bool params[count]{true, true, true, true};

		void Clear() 
		{
			for (int i = 0; i < count; i++)
				params[i] = true;
		}
	}settings;
	/// <summary>
	/// структура положеиня блока
	/// </summary>
	struct BlockPos
	{
		int x, y, z;
		block_id block;
	};

	static bool is_day; // флаг дня
	const static int MaxChunksCount = 512 * 8; // максимальное количество чанков
	const static int WorldHeight = 16; // максимальная высота мира

	PosQueue *RenderQueue; // очередь рпервичного обновления
	PosQueue *UpdateQueue; // очередь обновления
	queue<Vector3Int> *GlobalUpdateQueue; // очередь на глобальное обновление
	PosQueue *CreateQueue; // очерель на создание
	Player* player; // указатель на игрока
	vector<BlockPos> GlobalBuffer; // глобальный буфер блоков
	vector<int> toRemove;; // вектора позиций буфера на удаление
	Chunk* Chunks[MaxChunksCount]; // массив чанков

	inline Chunk* GetChunk(int x, int y, int z); // получить чанк

	static int seed; // зерно генерации
	static int render_distance; // дальность прорисовки
	static string name; // имя мира
	int chunksUpdateing = 0; // количесвтво обновляющихся чанков 
	int chunksLoaded = 0; // количество загруженных чанков
	 
	Block* GetBlock(int x, int y, int z); // указатель на блок
	Block* GetBlock(Vector3 pos); // указатель на блок

	bool CreateChunk(int x, int y, int z); // создание чанка
	block_id GetBlockID(int x, int y, int z); // ID блока
	block_id GetBlockID(Vector3 pos); // ID блока
	void UpdateIfExcist(int x, int y, int z); // обновить чанк, если существует
	void RenderIfExcist(int x, int y, int z); // отрисовать чанк, если существует
	void AddToRender(int x, int y, int z); // добавить в очередь первичной отрисовки
	void AddToUpdate(int x, int y, int z); // добавить в очередь обновления
	void AddToCreate(int x, int y, int z); // добавление чанка в очередь создания
	void AddToRender(Chunk* chunk); // добавить чанк в очередь первичной отрисовки
	void AddToUpdate(Chunk* chunk); // добавить чанк в очередь обновления 
	void Update(); // обновление
	void UpdateWorldLighting(); // обновление глобального освещения
	void Generate(Chunk* chunk); // генерация чанка
	void SetBlock(int x, int y, int z, block_id block, bool update = false); // установка блока
	void SetBlock(Vector3 pos, block_id block, bool update = false); // установка блока
	void Render(unsigned int texture); // рендер мира
	void Clear(); // очистка мира
	void Create(); // создание мира
	void DeleteChunk(int index); // удаление чанка
	void RationalizeBuffer(); // рационализация буфера блоков
	void RemoveCash(); // удаление кэша блоков
	void UpdateAtPoint(int x, int y, int z); // обновить чанки в точке
	void AddToUpdateAtPoint(int x, int y, int z); // добавить в очередь обновления в точке
	void UpdateIfEqual(int value1, int value2, int x, int y, int z); // обновление при условии
	void Explode(int x, int y, int z); // расчистка территории
	void Explode(Vector3 pos); // расчистка территории
	void AddToUpdateColumn(int x, int y, int z); // добавить в очередь колонну чанков
	void SetBufferBlock(int x, int y, int z, block_id block); // добавить блок в буффер
	char GetLight(int x, int y, int z); // значение света по координатами
	World(Player* player); // конструктор
	~World(); // деструктор
};