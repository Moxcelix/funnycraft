#pragma once
/// <summary>
/// структура рамок
/// </summary>
struct Bound
{
	float width; // ширина
	float length; // длина
	float height; // выоста

	/// <summary>
	/// конструктор
	/// </summary>
	Bound() 
	{
		Set(1, 1, 1);
	}
	/// <summary>
	/// конструктор
	/// </summary>
	/// <param name="width"></param>
	/// <param name="length"></param>
	/// <param name="height"></param>
	Bound(float width, float length, float height)
	{
		Set(width, length, height);
	}
	/// <summary>
	/// установка границ
	/// </summary>
	/// <param name="width"></param>
	/// <param name="length"></param>
	/// <param name="height"></param>
	void Set(float width, float length, float height)
	{
		this->width = width;
		this->length = length;
		this->height = height;
	}
};

