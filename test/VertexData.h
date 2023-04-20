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
	bool inied = false; // ���� �������������

	const static float face_up[12];			// ������� ������� �����
	const static float face_down[12];		// ������� ������ �����
	const static float face_right[12];		// ������� ������ �����
	const static float face_left[12];		// ������� ����� �����
	const static float face_front[12];		// ������� �������� �����
	const static float face_back[12];		// ������� ������ �����
	const static float face_crossRFLB[24];	// ������� ������������ �����
	const static float face_crossRBLF[24];	// ������� ������������ �����

public:
	std::vector<GLfloat> Vertices; // ������ ������
	std::vector<GLfloat> Colors; // ������ ������

	int face_count = 0; // ���������� ������
	VertexData(); // �����������
	~VertexData(); // ����������

	void FaceUp(float x, float y, float z);
	void FaceDown(float x, float y, float z);
	void FaceRight(float x, float y, float z);
	void FaceLeft(float x, float y, float z);
	void FaceFront(float x, float y, float z);
	void FaceBack(float x, float y, float z);

	void FaceUp(float x, float y, float z, ColorSquad colors);		// ������� �����
	void FaceDown(float x, float y, float z, ColorSquad colors);		// ������ �����
	void FaceRight(float x, float y, float z, ColorSquad colors);		// ������ �����
	void FaceLeft(float x, float y, float z, ColorSquad colors);		// ����� �����
	void FaceFront(float x, float y, float z, ColorSquad colors);		// �������� �����
	void FaceBack(float x, float y, float z, ColorSquad colors);		// ������ �����

	void FaceParticle(float x, float y, float z, float scale, ColorSquad colors);		// �����

	void FaceCrossRFLB(float x, float y, float z, ColorSquad colors); // ������������ �����
	void FaceCrossRBLF(float x, float y, float z, ColorSquad colors); // ������������ �����

	void SetLayer(int layer);  // ��������� ����

	void Clear(); // ������� ������
};

