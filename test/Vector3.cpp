#include "Vector3.h"
#include <cmath>
/// <summary>
/// конструктор
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
/// <summary>
/// конструктор
/// </summary>
Vector3::Vector3()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}
/// <summary>
/// оператор меньше
/// </summary>
/// <param name="val"></param>
/// <returns></returns>
bool Vector3::operator< (Vector3 val)
{
	return Module() < val.Module();
}
/// <summary>
/// оператор больше
/// </summary>
/// <param name="val"></param>
/// <returns></returns>
bool Vector3::operator> (Vector3 val)
{
	return Module() > val.Module();
}
/// <summary>
/// оператор меньше или равно
/// </summary>
/// <param name="val"></param>
/// <returns></returns>
bool Vector3::operator<= (Vector3 val)
{
	return Module() <= val.Module();
}
/// <summary>
/// оператор больше или равно
/// </summary>
/// <param name="val"></param>
/// <returns></returns>
bool Vector3::operator>= (Vector3 val)
{
	return Module() >= val.Module();
}
/// <summary>
/// умножение цвета
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <returns>вектор цвета</returns>
Vector3 Vector3::clrm(Vector3 a, Vector3 b) 
{
	return Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
}
/// <summary>
/// декструктор
/// </summary>
Vector3Int::~Vector3Int()
{
	
}
/// <summary>
/// оператор сложения
/// </summary>
/// <param name="val"></param>
/// <returns>вектор суммы</returns>
Vector3 Vector3::operator+ (Vector3 val)
{
	return Vector3(x + val.x, y + val.y, z + val.z);
}
/// <summary>
/// оператор вычитания
/// </summary>
/// <param name="val"></param>
/// <returns>вектор разность</returns>
Vector3 Vector3::operator- (Vector3 val)
{
	return Vector3(x - val.x, y - val.y, z - val.z);
}
/// <summary>
/// оператор умножения на число
/// </summary>
/// <param name="val"></param>
/// <returns>вектор произведения на число</returns>
Vector3 Vector3::operator* (float val)
{
	return Vector3(x * val, y * val, z * val);
}
/// <summary>
/// оператор сравнения
/// </summary>
/// <param name="val"></param>
/// <returns></returns>
bool Vector3::operator== (Vector3 val)
{
	return x == val.x && y == val.y && z == val.z;
}
/// <summary>
/// длина вектора
/// </summary>
/// <returns></returns>
float Vector3::Module()
{
	return sqrtf(x * x + y * y + z * z);
}
/// <summary>
/// конструктор
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
Vector3Int::Vector3Int(int x, int y, int z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
/// <summary>
/// конструктор
/// </summary>
Vector3Int::Vector3Int()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}
/// <summary>
/// оператор сложениея
/// </summary>
/// <param name="val"></param>
/// <returns>вектор суммы</returns>
Vector3Int Vector3Int::operator+ (Vector3Int val)
{
	return Vector3Int(x + val.x, y + val.y, z + val.z);
}
/// <summary>
/// оператор вычитания
/// </summary>
/// <param name="val"></param>
/// <returns>вектор разности </returns>
Vector3Int Vector3Int::operator- (Vector3Int val)
{
	return Vector3Int(x - val.x, y - val.y, z - val.z);
}
/// <summary>
/// оператор умножения на число
/// </summary>
/// <param name="val"></param>
/// <returns>вектор произведения на число</returns>
Vector3Int Vector3Int::operator* (float val)
{
	return Vector3Int(x * val, y * val, z * val);
}
/// <summary>
/// оператор сравнения
/// </summary>
/// <param name="val"></param>
/// <returns></returns>
bool Vector3Int::operator== (Vector3Int val)
{
	return x == val.x && y == val.y && z == val.z;
}
/// <summary>
/// длина вектора
/// </summary>
/// <returns></returns>
float Vector3Int::Module()
{
	return sqrtf(x * x + y * y + z * z);
}
/// <summary>
/// расстояние между точками
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <returns></returns>
float Vector3Int::Distance(Vector3Int a, Vector3Int b) 
{
	return (a - b).Module();
}
/// <summary>
/// оператор меньше
/// </summary>
/// <param name="val"></param>
/// <returns></returns>
bool Vector3Int::operator< (Vector3Int val) 
{
	return Module() < val.Module();
}
/// <summary>
/// оператор больше
/// </summary>
/// <param name="val"></param>
/// <returns></returns>
bool Vector3Int::operator> (Vector3Int val)
{
	return Module() > val.Module();
}
/// <summary>
/// оператор меньше или равно
/// </summary>
/// <param name="val"></param>
/// <returns></returns>
bool Vector3Int::operator<= (Vector3Int val)
{
	return Module() <= val.Module();
}
/// <summary>
/// оператор больше или равно
/// </summary>
/// <param name="val"></param>
/// <returns></returns>
bool Vector3Int::operator>= (Vector3Int val)
{
	return Module() >= val.Module();
}