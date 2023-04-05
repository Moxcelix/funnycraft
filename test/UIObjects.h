#pragma once
#include<iostream>
#include<GLFW/glfw3.h>

#include "Vector3.h"

using namespace std;
/// <summary>
/// структура UI объекта
/// </summary>
struct UIObject
{
	virtual void Draw(int win_width, int win_height) = 0;
};
/// <summary>
/// прицел
/// </summary>
struct Cross : UIObject
{
	const static int size = 16;
	const static int width = 2;

	GLfloat pixels[size][size][4];
	GLfloat pixel_data[size][size][3];
	/// <summary>
	/// очистка
	/// </summary>
	void Clear()
	{
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				for (int k = 0; k < 4; k++)
					pixels[i][j][k] = 0;

	}
	/// <summary>
	/// отрисвока
	/// </summary>
	/// <param name="win_width"></param>
	/// <param name="win_height"></param>
	virtual void Draw(int win_width, int win_height) override
	{
		float x = -(size) / float(win_width);
		float y = -(size) / float(win_height);

		glReadPixels((win_width - size) / 2, (win_height - size) / 2, size, size, GL_RGB, GL_FLOAT, pixel_data);

		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
			{
				if ((i >= (size - width) / 2) && (i < (size + width) / 2)
					|| (j >= (size - width) / 2) && (j < (size + width) / 2))
				{
					pixels[i][j][0] = 1 - pixel_data[i][j][0];
					pixels[i][j][1] = 1 - pixel_data[i][j][1];
					pixels[i][j][2] = 1 - pixel_data[i][j][2];
					pixels[i][j][3] = 1;
				}
			}

		glRasterPos2f(x, y);

		glDrawPixels(size, size, GL_RGBA, GL_FLOAT, pixels);
	}
	/// <summary>
	/// конструктор
	/// </summary>
	Cross()
	{
		Clear();
	}
};

struct BackGround :UIObject 
{
	GLfloat pixels[4];

	BackGround()
	{
		pixels[0] = 1;
		pixels[1] = 1;
		pixels[2] = 1;
		pixels[3] = 1;
	}

	BackGround(float r, float g, float b, float a) 
	{
		pixels[0] = r;
		pixels[1] = g;
		pixels[2] = b;
		pixels[3] = a;
	}

	virtual void Draw(int win_width, int win_height) override
	{
		glScalef(213, 123, 123);
		glDrawPixels(1, 1, GL_RGBA, GL_FLOAT, pixels);
		glScalef(1./213, 1./123, 1./123);
	}
};