#pragma once
class Vector3
{
public:
	float x, y, z; // координаты
	Vector3(float x, float y, float z); // конструктор
	Vector3(); // конструктор
	Vector3 operator+ (Vector3 val); // оператор сложения
	Vector3 operator- (Vector3 val); // оператор вычитания
	Vector3 operator* (float val);  // умножение на число
	bool operator== (Vector3 val); // оператор сравнения
	bool operator< (Vector3 val);  // оператор меншье
	bool operator> (Vector3 val);  // оператор больше
	bool operator<= (Vector3 val); // оператор меньше или равно
	bool operator>= (Vector3 val); // оператор больше или равно
	float Module(); // длина вектора

	static Vector3 clrm(Vector3 a, Vector3 b); // умножение цвета
};

struct Vector3Int 
{
public:
	int x, y, z; // координаты
	Vector3Int(int x, int y, int z); // конструктор
	Vector3Int(); // конструктор
	~Vector3Int(); // деструктор
	int GetHashCode() // хэш код
	{
		int hash = 47;
		hash = hash * 227 + x;
		hash = hash * 227 + y;
		hash = hash * 227 + z;
		return hash;
	}
	Vector3Int operator+ (Vector3Int val); // оператор сложения
	Vector3Int operator- (Vector3Int val); // оператор вычитания
	Vector3Int operator* (float val); // умножение на число
	bool operator< (Vector3Int val);  // оператор меншье
	bool operator> (Vector3Int val);  // оператор больше
	bool operator<= (Vector3Int val); // оператор меньше или равно
	bool operator>= (Vector3Int val); // оператор больше или равно
	bool operator== (Vector3Int val); // оператор сравнения
	static float Distance(Vector3Int a, Vector3Int b); // расстаяние
	float Module(); // длина вектора
};

