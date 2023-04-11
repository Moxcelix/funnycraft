#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include"Settings.h"
#include"World.h"
#include"Chunk.h"
#include"RigidBox.h"
#include"DeltaTime.h"

#include<vector>
#include<fstream>

class World;
class RigidBox;
/// <summary>
/// класс игрока
/// </summary>
class Player
{
private:
	int length;

	float walk_speed = 0.005f;
	float run_speed = 0.01;
	float lift_speed = 0.01;
	float fly_mult = 1.5f;
public:
	/// <summary>
	/// структура камеры
	/// </summary>
	struct Camera
	{
		float x, y, z;		// координаты
		float xRot, zRot;	// углы поворота
	} camera = { 0, 0, head_height, 0, 0 }; // экземпл€р

	/// <summary>
	/// структуры движени€
	/// </summary>
	struct MovementDirection
	{
	private:
		bool move = false;
		float velocity = 0;
		float mult = 100;
	public:
		void Move(bool state = true)
		{
			move = state;
		}

		void Move(float a, float max_speed)
		{
			if (move)
				velocity += a;
			else
				velocity -= a * velocity * mult;

			if (velocity < 0)
				velocity = 0;

			if (velocity > max_speed)
				velocity = max_speed;
		}
		bool GetMove() 
		{
			return move;
		}

		float GetVelocity()
		{
			return velocity;
		}
	};

	Player(World* world);	// конструктор
	Player();				// конструктор

	RigidBox* body;			// физическое тело персонажа
	DeltaTime delta_time;	// рачсЄт дельты времени

	Vector3 look_pos;	// позици€, куда смотрит игрок
	Vector3 add_pos;	// позици€, куда можно разместить блок
	Vector3 pos;		// позици€ игрока

	MovementDirection forward;		// движение вперЄд	
	MovementDirection back;			// движение назад
	MovementDirection right;		// движение вправо
	MovementDirection left;			// движение влево
	MovementDirection up;			// движение вверх
	MovementDirection down;			// движение вниз

	bool looking = false;			// флаг фокусировки на блок
	bool sprint = false;			// флаг бега
	bool fly = false;				// флаг полЄта
	bool can_move = true;			// флаг движени€

	float rot, velocity, speed;
	
	float phasa = 0; // фаза покачивани€ камеры
	float head_height = 1.7;
	float HeadBobMagnitude = 0.1;	// высота и амлитуда покачивани€ камеры
	float width = 0.8f;				// ширина персонажа
	float height = 1.8f;			// высота персонажа
	float jump_speed = 0.015f;		// ускорение прыжка
	float current_friction = 1;		// коэффициент трени€
	float acceleration = 0.0001;	// ускорение игрока

	double angle = 0; // угол

	void RotateCamera(float alpha, float beta); // поворот камеры
	void MoveUpdate();	// обновление движени€
	void Update(); // обновление
	void LoadTerrain(World* world); // прогрузка территории
	void GenereateSphere(); // генераци€ сферы видимости
	void Save(std::ofstream& stream); // сохранение параметров игрока
	void Init(float x, float y, float z, float xRot, float zRot); // инициализаци€
	void SwitchFly();

	float GetVelocity(); // получение скорости
	float GetDistance(float x, float y, float z); // рассто€ние до точки

	Vector3 start_pos = { 0, 0, 64 };	// стартова€ позици€
	std::vector<Vector3Int*>sphere;			// вектор координат сферы видимости
	Vector3Int IntPosition;				// вектор положени€ в целых координатах
	Vector3Int ChunkPosition;			// вектор чанковой позиции
	World* world;						// указатель на экземпл€р мира
	Vector2 vel = Vector2(0, 0);		// вектор горизонтальной скорости
};

