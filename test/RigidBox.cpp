#include "RigidBox.h"

vector<Vector3Int> RigidBox::positions{}; // инициализация вектора позиций

/// <summary>
/// конструктор
/// </summary>
RigidBox::RigidBox()
{
	RigidBox(nullptr, 0, 0);
}

/// <summary>
/// конструктор
/// </summary>
/// <param name="world"></param>
/// <param name="width"></param>
/// <param name="height"></param>
/// <param name="g"></param>
/// <param name="pos"></param>
/// <param name="flags"></param>
RigidBox::RigidBox(World* world, float width, float height, float g, Vector3 pos, int flags)
{
	// установка полям значений 
	this->world = world;		// указатель на мир
	this->width = width;		// ширина бокса
	this->height = height;		// высота бокса
	this->g = g * phys_mod;		// ускорение свободного падения
	this->pos = pos;			// позиция
	this->flags = flags;		// флаги 

	w = width / 2.;		// половина ширины
	h0 = height / 2.;	// нижняя часть хит-бокса
	h1 = height / 2.;	// верхняя часть хит-бокса

	if (flags & RB_SPLIT_HEIGHT)	// разделение высоты
	{
		h0 = 0.5f;			// нижняя половина
		h1 = height - h0;	// верхняя половина
	}
}
/// <summary>
/// движение бокса
/// </summary>
/// <param name="dx"></param>
/// <param name="dy"></param>
/// <param name="dz"></param>
void RigidBox::Move(float dx, float dy, float dz)
{
	grounded = false;

	if (flags & RB_CHECK_LOAD_STATE) // проверка прогруженности территории
		if (!Readiness(pos.x + dx, pos.y + dy, pos.z + dz)) // если территория не прогружена
			return;

	pos.x += dx;			// приращение по х
	Collision(dx, 0, 0);	// колизия по х
	pos.y += dy;			// приращение по y
	Collision(0, dy, 0);	// коллизия по y
	pos.z += dz;			// прирпщение по z
	Collision(0, 0, dz);	// колллизия по z

	if (flags & RB_COLLISION_MAP)	// если разрешено создание карты коллизии
		SetCollisionMap();			// создание карты коллизии
}
/// <summary>
/// создание карты коллизии
/// </summary>
void RigidBox::SetCollisionMap()
{
	float xn = floor(pos.x - w + half);		// начальная абсцисса
	float xp = (pos.x + w + salf);			// конечная абсцисса
	float yn = floor(pos.y - w + half);		// начальная ордината
	float yp = (pos.y + w + salf);			// конечная ордината
	float zn = floor(pos.z - h0 + half);	// начальная аппликата
	float zp = (pos.z + h1 + salf);			// конечная аппликата

	for (int X = xn; X < xp; X++)
		for (int Y = yn; Y < yp; Y++)
			for (int Z = zn; Z < zp; Z++)
			{
				AddPos({ X, Y, Z });	// добавить точку
			}
}
/// <summary>
/// коллизия
/// </summary>
/// <param name="dx"></param>
/// <param name="dy"></param>
/// <param name="dz"></param>
void RigidBox::Collision(float dx, float dy, float dz)
{
	float xn = floor(pos.x - w + half);		// начальная абсцисса
	float xp = (pos.x + w + salf);			// конечная абсцисса
	float yn = floor(pos.y - w + half);		// начальная ордината
	float yp = (pos.y + w + salf);			// конечная ордината
	float zn = floor(pos.z - h0 + half);	// начальная аппликата
	float zp = (pos.z + h1 + salf);			// конечная аппликата

	for (int X = xn; X < xp; X++)
		for (int Y = yn; Y < yp; Y++)
			for (int Z = zn; Z < zp; Z++)
			{
				if (world->GetBlock(X, Y, Z)->Solid()) // если блок твёрдый
				{
					if (dx > 0) pos.x = X - w - half;	// столкновение x-
					if (dx < 0) pos.x = X + w + half;	// столкновение x+
					if (dy > 0) pos.y = Y - w - half;	// столкновение y-
					if (dy < 0) pos.y = Y + w + half;	// столкновение y+
					if (dz > 0) pos.z = Z - h1 - half;	// столкновение z-
					if (dz < 0)
					{
						pos.z = Z + h0 + half;
						grounded = true; // приземлённость
					}// столкновение z+
				}
			}
}
/// <summary>
/// добавление точки в карту коллизии
/// </summary>
/// <param name="pos"></param>
void RigidBox::AddPos(Vector3Int pos)
{
	for (auto p : positions) // для всех позиций
	{
		if (p == pos)	// если уже есть в карте
			return;
	}
	// иначе
	positions.push_back(pos);	// добавить точку в вектор
}
/// <summary>
/// готовность территории
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
/// <returns></returns>
bool RigidBox::Readiness(int x, int y, int z)
{
	if (world) // если мир существует
	{
		Chunk* c = world->GetChunk(x, y, z); // получить чанк в точке
		// если чанка нет и область находится в зоне мира по высоте
		return c || z < 0 || z >= Chunk::ChunkSize * World::WorldHeight;
	}
	return false;
}
/// <summary>
/// очистка карты коллизии
/// </summary>
void RigidBox::ClearCollisionMap()
{
	positions.clear(); // очистка вектора
}
/// <summary>
/// проверка на заблокированность точки
/// </summary>
/// <param name="pos"></param>
/// <returns></returns>
bool RigidBox::IsLocked(Vector3 pos)
{
	// переадрессация вызова на функцию с типом параметра Vector3Int
	return IsLocked(Vector3Int{ (int)pos.x, (int)pos.y, (int)pos.z });
}
/// <summary>
/// проверка на заблокированность точки
/// </summary>
/// <param name="pos"></param>
/// <returns></returns>
bool RigidBox::IsLocked(Vector3Int pos)
{
	for (auto p : positions) // для всех позиций из карты коллизии
	{
		if (p == pos) // если позиция нашлась
			return true; // точка заблокирована
	}
	// иначе точка свободна
	return false;
}

void RigidBox::Render()
{
	if (Debug::bounds)
		Debug::DrawBox({ pos.x, pos.y, pos.z - h0 + (h1 + h0) * .5f }, { w,w,(h1 + h0) * .5f }, { 0,1,1 });
}