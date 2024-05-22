#include "VertexData.h"
#include <iostream>
/// <summary>
/// конструктор
/// </summary>
VertexData::VertexData()
{

}
/// <summary>
/// деструктор
/// </summary>
VertexData::~VertexData()
{
	Clear();
}
/// <summary>
/// очистка данных
/// </summary>
void VertexData::Clear()
{
	Vertices.clear();
	Colors.clear();
	face_count = 0;
}
/// <summary>
/// верхняя грань
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
void VertexData::FaceUp(float x, float y, float z)
{
	face_count++;
	for (int i = 0; i < 12; i++)
	{
		float coord = face_up[i];
		if (i % 3 == 0)
			coord += x;
		if (i % 3 == 1)
			coord += y;
		if (i % 3 == 2)
			coord += z;
		Vertices.push_back(coord);
	}
	for (int i = 0; i < 12; i++)
		Colors.push_back(1);
}
/// <summary>
/// нижняя грань
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
void VertexData::FaceDown(float x, float y, float z)
{
	face_count++;
	for (int i = 0; i < 12; i++)
	{
		float coord = face_down[i];
		if (i % 3 == 0)
			coord += x;
		if (i % 3 == 1)
			coord += y;
		if (i % 3 == 2)
			coord += z;
		Vertices.push_back(coord);
	}
	for (int i = 0; i < 12; i++)
		Colors.push_back(0.7f);
}
/// <summary>
/// правая грань
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
void VertexData::FaceRight(float x, float y, float z)
{
	face_count++;
	for (int i = 0; i < 12; i++)
	{
		float coord = face_right[i];
		if (i % 3 == 0)
			coord += x;
		if (i % 3 == 1)
			coord += y;
		if (i % 3 == 2)
			coord += z;
		Vertices.push_back(coord);
	}
	for (int i = 0; i < 12; i++)
		Colors.push_back(0.9f);
}
/// <summary>
/// левая грань
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
void VertexData::FaceLeft(float x, float y, float z)
{
	face_count++;
	for (int i = 0; i < 12; i++)
	{
		float coord = face_left[i];
		if (i % 3 == 0)
			coord += x;
		if (i % 3 == 1)
			coord += y;
		if (i % 3 == 2)
			coord += z;
		Vertices.push_back(coord);
	}
	for (int i = 0; i < 12; i++)
		Colors.push_back(0.9f);
}
/// <summary>
/// передняя грань
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
void VertexData::FaceFront(float x, float y, float z)
{
	face_count++;
	for (int i = 0; i < 12; i++)
	{
		float coord = face_front[i];
		if (i % 3 == 0)
			coord += x;
		if (i % 3 == 1)
			coord += y;
		if (i % 3 == 2)
			coord += z;
		Vertices.push_back(coord);
	}
	for (int i = 0; i < 12; i++)
		Colors.push_back(0.8f);
}
/// <summary>
/// задняя грань
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
void VertexData::FaceBack(float x, float y, float z)
{
	face_count++;
	for (int i = 0; i < 12; i++)
	{
		float coord = face_back[i];
		if (i % 3 == 0)
			coord += x;
		if (i % 3 == 1)
			coord += y;
		if (i % 3 == 2)
			coord += z;
		Vertices.push_back(coord);
	}
	for (int i = 0; i < 12; i++)
		Colors.push_back(0.8f);
}
/// <summary>
/// верхняя грань
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
/// <param name="colors"></param>
void VertexData::FaceUp(float x, float y, float z, ColorSquad colors)
{
	face_count++;
	for (int i = 0; i < 12; i++)
	{
		float coord = face_up[i];
		if (i % 3 == 0)
			coord += x;
		if (i % 3 == 1)
			coord += y;
		if (i % 3 == 2)
			coord += z;
		Vertices.push_back(coord);
	}

	for (int i = 0; i < 4; i++)
	{
		Colors.push_back(colors.colors[i].x * UP_MULT);
		Colors.push_back(colors.colors[i].y * UP_MULT);
		Colors.push_back(colors.colors[i].z * UP_MULT);
	}
}
/// <summary>
/// нижняя грань
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
/// <param name="colors"></param>
void VertexData::FaceDown(float x, float y, float z, ColorSquad colors)
{
	face_count++;
	for (int i = 0; i < 12; i++)
	{
		float coord = face_down[i];
		if (i % 3 == 0)
			coord += x;
		if (i % 3 == 1)
			coord += y;
		if (i % 3 == 2)
			coord += z;
		Vertices.push_back(coord);
	}
	for (int i = 0; i < 4; i++)
	{
		Colors.push_back(colors.colors[i].x * DOWN_MULT);
		Colors.push_back(colors.colors[i].y * DOWN_MULT);
		Colors.push_back(colors.colors[i].z * DOWN_MULT);
	}
}
/// <summary>
/// правая грань
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
/// <param name="colors"></param>
void VertexData::FaceRight(float x, float y, float z, ColorSquad colors)
{
	face_count++;
	for (int i = 0; i < 12; i++)
	{
		float coord = face_right[i];
		if (i % 3 == 0)
			coord += x;
		if (i % 3 == 1)
			coord += y;
		if (i % 3 == 2)
			coord += z;
		Vertices.push_back(coord);
	}
	for (int i = 0; i < 4; i++)
	{
		Colors.push_back(colors.colors[i].x * X_MULT);
		Colors.push_back(colors.colors[i].y * X_MULT);
		Colors.push_back(colors.colors[i].z * X_MULT);
	}
}
/// <summary>
/// левая грань
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
/// <param name="colors"></param>
void VertexData::FaceLeft(float x, float y, float z, ColorSquad colors)
{
	face_count++;
	for (int i = 0; i < 12; i++)
	{
		float coord = face_left[i];
		if (i % 3 == 0)
			coord += x;
		if (i % 3 == 1)
			coord += y;
		if (i % 3 == 2)
			coord += z;
		Vertices.push_back(coord);
	}
	for (int i = 0; i < 4; i++)
	{
		Colors.push_back(colors.colors[i].x * X_MULT);
		Colors.push_back(colors.colors[i].y * X_MULT);
		Colors.push_back(colors.colors[i].z * X_MULT);
	}
}

void VertexData::FaceFront(float x, float y, float z, ColorSquad colors)
{
	face_count++;
	for (int i = 0; i < 12; i++)
	{
		float coord = face_front[i];
		if (i % 3 == 0)
			coord += x;
		if (i % 3 == 1)
			coord += y;
		if (i % 3 == 2)
			coord += z;
		Vertices.push_back(coord);
	}
	for (int i = 0; i < 4; i++)
	{
		Colors.push_back(colors.colors[i].x * Y_MULT);
		Colors.push_back(colors.colors[i].y * Y_MULT);
		Colors.push_back(colors.colors[i].z * Y_MULT);
	}
}

void VertexData::FaceBack(float x, float y, float z, ColorSquad colors) {
	face_count++;
	for (int i = 0; i < 12; i++)
	{
		float coord = face_back[i];
		if (i % 3 == 0)
			coord += x;
		if (i % 3 == 1)
			coord += y;
		if (i % 3 == 2)
			coord += z;
		Vertices.push_back(coord);
	}
	for (int i = 0; i < 4; i++)
	{
		Colors.push_back(colors.colors[i].x * Y_MULT);
		Colors.push_back(colors.colors[i].y * Y_MULT);
		Colors.push_back(colors.colors[i].z * Y_MULT);
	}
}
/// <summary>
/// диагональная грань
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
/// <param name="colors"></param>
void VertexData::FaceCrossRFLB(float x, float y, float z, ColorSquad colors)
{
	face_count += 2;
	for (int i = 0; i < 24; i++)
	{
		float coord = face_crossRFLB[i];
		if (i % 3 == 0)
			coord += x;
		if (i % 3 == 1)
			coord += y;
		if (i % 3 == 2)
			coord += z;
		Vertices.push_back(coord);
	}
	for (int i = 0; i < 8; i++)
	{
		Colors.push_back(colors.colors[i % 4].x * UP_MULT);
		Colors.push_back(colors.colors[i % 4].y * UP_MULT);
		Colors.push_back(colors.colors[i % 4].z * UP_MULT);
	}
}
/// <summary>
/// диагональная грань
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
/// <param name="colors"></param>
void VertexData::FaceCrossRBLF(float x, float y, float z, ColorSquad colors)
{
	face_count += 2;
	for (int i = 0; i < 24; i++)
	{
		float coord = face_crossRBLF[i];
		if (i % 3 == 0)
			coord += x;
		if (i % 3 == 1)
			coord += y;
		if (i % 3 == 2)
			coord += z;
		Vertices.push_back(coord);
	}
	for (int i = 0; i < 8; i++)
	{
		Colors.push_back(colors.colors[i % 4].x * UP_MULT);
		Colors.push_back(colors.colors[i % 4].y * UP_MULT);
		Colors.push_back(colors.colors[i % 4].z * UP_MULT);
	}
}
void VertexData::FaceParticle(float x, float y, float z, float scale, ColorSquad colors)
{
	face_count++;
	for (int i = 0; i < 12; i++)
	{
		float coord = face_up[i] * scale;

		if (i % 3 == 0) 
			coord += x;
		if (i % 3 == 1) 
			coord += y;
		if (i % 3 == 2) 
			coord += z;

		Vertices.push_back(coord);
	}
	for (int i = 0; i < 4; i++)
	{
		Colors.push_back(colors.colors[i % 4].x * UP_MULT);
		Colors.push_back(colors.colors[i % 4].y * UP_MULT);
		Colors.push_back(colors.colors[i % 4].z * UP_MULT);
	}
}
/// <summary>
/// установка слоя
/// </summary>
/// <param name="layer"></param>
void VertexData::SetLayer(int layer)
{
	//Layer.push_back(layer);
}

const float VertexData::face_up[12]
{
	-0.5, -0.5, 0.5,
	-0.5, 0.5, 0.5,
	0.5, 0.5, 0.5,
	0.5, -0.5, 0.5
};
// вершины нижней грани
const float VertexData::face_down[12]
{
	0.5, 0.5, -0.5,
	-0.5, 0.5, -0.5,
	-0.5, -0.5, -0.5,
	0.5, -0.5, -0.5
};
// вершины правой грани
const float VertexData::face_right[12]
{
	0.5, 0.5, -0.5,
	0.5, -0.5, -0.5,
	0.5, -0.5, 0.5,
	0.5, 0.5, 0.5
};
// вершины левой грани
const float VertexData::face_left[12]
{
	-0.5, -0.5, -0.5,
	-0.5, 0.5, -0.5,
	-0.5, 0.5, 0.5,
	-0.5, -0.5, 0.5
};
// вершины передней грани
const float VertexData::face_front[12]
{
	-0.5, 0.5, -0.5,
	0.5, 0.5, -0.5,
	0.5, 0.5, 0.5,
	-0.5, 0.5, 0.5
};
// вершины задней грани
const float VertexData::face_back[12]
{
	0.5, -0.5, -0.5,
	-0.5, -0.5, -0.5,
	-0.5, -0.5, 0.5,
	0.5, -0.5, 0.5
};
// вершины диагональной грани
const float VertexData::face_crossRFLB[24]
{
	0.5, 0.5, -0.5,
	-0.5, -0.5, -0.5,
	-0.5, -0.5, 0.5,
	0.5, 0.5, 0.5,
	-0.5, -0.5, -0.5,
	.5, 0.5, -0.5,
	0.5, 0.5, 0.5,
	-0.5, -0.5, 0.5
};
// вершины диагональной грани
const float VertexData::face_crossRBLF[24]
{
	0.5, -0.5, -0.5,
	-0.5, 0.5, -0.5,
	-0.5, 0.5, 0.5,
	0.5, -0.5, 0.5,
	-0.5, 0.5, -0.5,
	0.5, -0.5, -0.5,
	0.5, -0.5, 0.5,
	-0.5, 0.5, 0.5
};