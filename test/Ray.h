#pragma once
#include"World.h"
/// <summary>
/// класс луча
/// </summary>
class Ray
{
private:
	float eps = 0.01f;	// точность

	World* world;		// указатель на мир
	Vector3 start;		// позиция начала
	Vector3 direction;	// направление луча
public:
	/// <summary>
	/// конструктор
	/// </summary>
	/// <param name="world"></param>
	/// <param name="start"></param>
	/// <param name="direction"></param>
	Ray(World* world, Vector3 start, Vector3 direction) 
	{
		this->world = world;
		this->start = start;
		this->direction = direction;
	}
	bool Trace(float far, float& length, Vector3 &delpos, Vector3 &addpos); // трассировка луча
};

