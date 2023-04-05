#pragma once
#include<GLFW/glfw3.h>
#include"World.h"
class World;
/// <summary>
/// класс фантома
/// </summary>
class Phantom
{
private:
	float f = 0.01f, d = -0.01f;
public:
	bool up = false;	// флаг блока сверху
	bool down = false;	// флаг блока снизу
	bool right = false;	// флаг блока справа
	bool left = false;	// флаг блока слева
	bool front = false; // флаг блока спереди
	bool back = false;	// флаг блока сзади

	void Render(int x, int y, int z, World* world); // отрисовка
};

