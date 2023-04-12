#pragma once
#include"World.h"

class Ray
{
private:
	float eps = 0.01f;	// точность

	World* world;		// указатель на мир
	Vector3 start;		// позиция начала
	Vector3 direction;	// направление луча
public:
	Ray(World* world, Vector3 start, Vector3 direction) 
	{
		this->world = world;
		this->start = start;
		this->direction = direction;
	}
	bool Trace(float far, float& length, Vector3 &delpos, Vector3 &addpos); // трассировка луча
};

