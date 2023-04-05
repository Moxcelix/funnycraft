#pragma once
/// <summary>
/// класс двумерного вектора
/// </summary>
class Vector2
{
public:
	float x, y; // координаты
	Vector2() :x(0), y(0) {} // конструктор
	Vector2(float x, float y); // конструктор
	Vector2 operator+ (Vector2 val); // оператор сложения
	Vector2 operator- (Vector2 val); // оператор вычитания
	Vector2 operator* (float val);  // умножение на число
	bool operator== (Vector2 val); // оператор сравнения
	bool operator< (Vector2 val); // оператор меньше
	bool operator> (Vector2 val); // оператор больше
	bool operator<= (Vector2 val); // оператор меньше или равно
	bool operator>= (Vector2 val); // оператор больше или равно
	float Module(); // длина вектора
};

class Vector2Int
{
public:
	int x, y; // координаты
	Vector2Int() :x(0), y(0) {} // конструктор
	Vector2Int(int x, int y); // конструктор
	Vector2Int operator+ (Vector2Int val); // оператор сложения
	Vector2Int operator- (Vector2Int val); // оператор вычитания
	Vector2Int operator* (float val);  // умножение на число
	bool operator== (Vector2Int val); // оператор сравнения
	bool operator< (Vector2Int val); // оператор меньше
	bool operator> (Vector2Int val); // оператор больше
	bool operator<= (Vector2Int val); // оператор меньше или равно
	bool operator>= (Vector2Int val); // оператор больше или равно
	float Module(); // длина вектора
};


