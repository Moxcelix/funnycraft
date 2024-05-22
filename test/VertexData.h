#pragma once
#include <iostream>
#include <vector>
#include "Vector3.h"
#include <GLFW/glfw3.h>

constexpr auto UP_MULT = 1;
constexpr auto DOWN_MULT = 0.5;
constexpr auto Y_MULT = 0.67;
constexpr auto X_MULT = 0.83;

struct ColorSquad
{
	Vector3 colors[4];
	float alpha[4]{ 1,1,1,1 };

	ColorSquad(Vector3 color)
	{
		for (int i = 0; i < 4; i++)
			colors[i] = color;
	}

	ColorSquad()
	{
		for (int i = 0; i < 4; i++)
			colors[i] = {0,0,0};
	}

	ColorSquad(Vector3 colors[4])
	{
		for (int i = 0; i < 4; i++)
			this->colors[i] = colors[i];
	}

	ColorSquad(Vector3 a, Vector3 b, Vector3 c, Vector3 d)
	{
		colors[0] = a;
		colors[1] = b;
		colors[2] = c;
		colors[3] = d;
	}
};

class VertexData
{
private:
	bool inied = false; // флаг инициализации

	const static float face_up[12];			// вершины верхней грани
	const static float face_down[12];		// вершины нижней грани
	const static float face_right[12];		// вершины правой грани
	const static float face_left[12];		// вершины левой грани
	const static float face_front[12];		// вершины передней грани
	const static float face_back[12];		// вершины задней грани
	const static float face_crossRFLB[24];	// вершины диагональной грани
	const static float face_crossRBLF[24];	// вершины диагональной грани

public:
	std::vector<GLfloat> Vertices; // вектор вершин
	std::vector<GLfloat> Colors; // вектор цветов

	int face_count = 0; // количество граней
	VertexData(); // конструктор
	~VertexData(); // деструктор

	void FaceUp(float x, float y, float z);
	void FaceDown(float x, float y, float z);
	void FaceRight(float x, float y, float z);
	void FaceLeft(float x, float y, float z);
	void FaceFront(float x, float y, float z);
	void FaceBack(float x, float y, float z);

	void FaceUp(float x, float y, float z, ColorSquad colors);		// верхняя грань
	void FaceDown(float x, float y, float z, ColorSquad colors);		// нижняя грань
	void FaceRight(float x, float y, float z, ColorSquad colors);		// правая грань
	void FaceLeft(float x, float y, float z, ColorSquad colors);		// левая грань
	void FaceFront(float x, float y, float z, ColorSquad colors);		// передняя грань
	void FaceBack(float x, float y, float z, ColorSquad colors);		// задняя грань

	void FaceParticle(float x, float y, float z, float scale, ColorSquad colors);		// грань

	void FaceCrossRFLB(float x, float y, float z, ColorSquad colors); // диагональная грань
	void FaceCrossRBLF(float x, float y, float z, ColorSquad colors); // диагональная грань

	void SetLayer(int layer);  // установка слоя

	void Clear(); // очистка данных
};

