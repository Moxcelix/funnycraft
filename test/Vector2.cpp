#include "Vector2.h"
#include <cmath>
/// <summary>
/// Конструктор
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
Vector2::Vector2(float x, float y) 
{
	this->x = x;
	this->y = y;
}
/// <summary>
/// оператор сложения
/// </summary>
/// <param name="val"></param>
/// <returns>вектор суммы</returns>
Vector2 Vector2::operator+ (Vector2 val)
{
	return Vector2(x + val.x, y + val.y); 
}
/// <summary>
/// оператор вычитания
/// </summary>
/// <param name="val"></param>
/// <returns>вектор разности</returns>
Vector2 Vector2::operator- (Vector2 val)
{ 
	return Vector2(x - val.x, y - val.y);
}
/// <summary>
/// оператор умножения
/// </summary>
/// <param name="val"></param>
/// <returns>скалярное произведение</returns>
Vector2 Vector2::operator* (float val)
{
	return Vector2(x * val, y * val);
}
/// <summary>
/// длина вектора
/// </summary>
/// <returns>длина вектора</returns>
float Vector2::Module() 
{
	return sqrtf(x * x + y * y);
}

bool Vector2::operator== (Vector2 val) { return x == val.x && y == val.y; }		// оператор сравнения
bool Vector2::operator< (Vector2 val) { return Module() < val.Module(); }		// оператор меншье
bool Vector2::operator> (Vector2 val) { return Module() > val.Module(); }		// оператор больше
bool Vector2::operator<= (Vector2 val) { return Module() <= val.Module(); }		// оператор меншье или равно
bool Vector2::operator>= (Vector2 val) { return Module() >= val.Module(); }		// оператор больше или равно
/// <summary>
/// Конструктор
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
Vector2Int::Vector2Int(int x, int y)
{
	this->x = x;
	this->y = y;
}
/// <summary>
/// оператор сложения
/// </summary>
/// <param name="val"></param>
/// <returns>вектор суммы</returns>
Vector2Int Vector2Int::operator+ (Vector2Int val)
{
	return Vector2Int(x + val.x, y + val.y);
}
/// <summary>
/// оператор вычитания
/// </summary>
/// <param name="val"></param>
/// <returns>вектор разности</returns>
Vector2Int Vector2Int::operator- (Vector2Int val)
{
	return Vector2Int(x - val.x, y - val.y);
}
/// <summary>
/// оператор умножения
/// </summary>
/// <param name="val"></param>
/// <returns>скалярное произведение</returns>
Vector2Int Vector2Int::operator* (float val)
{
	return Vector2Int(x * val, y * val);
}
/// <summary>
/// длина вектора
/// </summary>
/// <returns>длина вектора</returns>
float Vector2Int::Module()
{
	return sqrtf(x * x + y * y);
}

bool Vector2Int::operator== (Vector2Int val) { return x == val.x && y == val.y; }	// оператор сравнения
bool Vector2Int::operator< (Vector2Int val) { return Module() < val.Module(); }		// оператор меншье
bool Vector2Int::operator> (Vector2Int val) { return Module() > val.Module(); }		// оператор больше
bool Vector2Int::operator<= (Vector2Int val) { return Module() <= val.Module(); }	// оператор меншье или равно
bool Vector2Int::operator>= (Vector2Int val) { return Module() >= val.Module(); }	// оператор больше или равно