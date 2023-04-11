#pragma once
#include <iostream>
#include <vector>
using std::vector;
/// <summary>
/// класс UV данных
/// </summary>
class UVData
{
private:
	bool inied = false; // флаг инициализации
public:
	UVData(); // конструктор
	~UVData(); // деструктор
	static float stface[8]; // координаты грани
	vector<float> UVs; // вектор UV
	void AddFace(); // Добавление стороны блока
	void AddUV(float coords[], int length); // Добавление UV
	void AddUV(float x, float y); // Добавление UV
	void AddUV(float x, float y, float dx, float dy); // Добавление UV
	void Clear(); // Очистка карты UV
	float xUVSize = 0.0625f; // UV масштаб по Х
	float yUVSize = 0.0625f; // UV масштаб по Y
};

