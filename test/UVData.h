#pragma once
#include <iostream>
#include <vector>
/// <summary>
/// класс UV данных
/// </summary>
class UVData
{
private:
	bool inied = false; 
public:
	UVData();
	~UVData();
	static float stface[8];
	std::vector<float> UVs;
	void AddFace();
	void AddUV(float coords[], int length);
	void AddUV(float x, float y);
	void AddUV(float x, float y, float dx, float dy);
	void Clear();
	float xUVSize = 0.0625f;
	float yUVSize = 0.0625f;
	double accuracy = 0;
};

