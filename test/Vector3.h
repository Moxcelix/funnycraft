#pragma once
class Vector3 {
public:
	float x, y, z;
	Vector3(float, float, float);
	Vector3();
	Vector3 operator+ (Vector3);
	Vector3 operator- (Vector3);
	Vector3 operator* (float);
	bool operator== (Vector3);
	bool operator< (Vector3);
	bool operator> (Vector3);
	bool operator<= (Vector3);
	bool operator>= (Vector3);
	float Module();

	static Vector3 clrm(Vector3, Vector3);
	static Vector3 lerp(Vector3, Vector3, double);
};

struct Vector3Int {
public:
	int x, y, z;
	Vector3Int(int x, int y, int z);
	Vector3Int();
	int GetHashCode()
	{
		int hash = 47;
		hash = hash * 227 + x;
		hash = hash * 227 + y;
		hash = hash * 227 + z;
		return hash;
	}
	Vector3Int operator+ (Vector3Int);
	Vector3Int operator- (Vector3Int);
	Vector3Int operator* (float);
	bool operator< (Vector3Int);
	bool operator> (Vector3Int);
	bool operator<= (Vector3Int);
	bool operator>= (Vector3Int);
	bool operator== (Vector3Int);
	static float Distance(Vector3Int a, Vector3Int b);
	float Module();
};

