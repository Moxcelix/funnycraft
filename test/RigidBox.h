#pragma once
#include "World.h"
#include "Debug.h"
#include <iostream>
#include <vector>

#define RB_CHECK_LOAD_STATE		0b0001	// провекрка прогруженности территории
#define RB_USE_COLLISION		0b0010	// использование коллизии
#define RB_SPLIT_HEIGHT			0b0100	// разделение по высоте бокса
#define RB_COLLISION_MAP		0b1000	// рассчёт карты коллизий

class World;
class Player;
/// <summary>
/// класс бокса твёрдого тела
/// </summary>
class RigidBox
{
private:
	static std::vector<Vector3Int> positions; // вектор позиций карты коллизий

	float eps = 0.01f;
	float w = 0;				// половина ширины
	float h0 = 0;				// нижняя часть хит-бокса
	float h1 = 0;				// верхняя часть хит-бокса
	float half = 0.5f;			// половина
	float salf = 0.499f;		// половина

	void SetCollisionMap(); // создание карты коллизий
	static void AddPos(Vector3Int pos); // добавление точки в карту коллизии
public:
	int flags = 0;		// флаги параметров

	float width;		// ширина бокса
	float height;		// высота бокса
	float g;			// ускорение свободного падения
	float phys_mod = 0.0001f; // множитель для физики

	bool grounded = false; // приземлённость

	Vector3 pos;	// позиция
	World* world;	// указатель на мир

	RigidBox();		// конструкторы с параметрами и без
	RigidBox(World* world, float width, float height, float g = 9.8f, Vector3 pos = {0,0,0}, int flags = 0);

	void Render();									// ренлеринг
	void Move(float dx, float dy, float dz);		// движение
	void Collision(float dx, float dy, float dz);	// коллизия
	bool Readiness(int x, int y, int z);			// готовность территории 

	static void ClearCollisionMap();				// очистка карты коллизий
	static bool IsLocked(Vector3Int);				// заблокированность точки
	static bool IsLocked(Vector3);					// заблокированность точки
};