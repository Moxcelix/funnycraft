#include "Vector3.h"
#include <cmath>

Vector3::Vector3(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3::Vector3() {
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

bool Vector3::operator< (Vector3 val) {
	return Module() < val.Module();
}

bool Vector3::operator> (Vector3 val) {
	return Module() > val.Module();
}

bool Vector3::operator<= (Vector3 val) {
	return Module() <= val.Module();
}

bool Vector3::operator>= (Vector3 val) {
	return Module() >= val.Module();
}

Vector3 Vector3::clrm(Vector3 a, Vector3 b) {
	return Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
}

Vector3 Vector3::operator+ (Vector3 val) {
	return Vector3(x + val.x, y + val.y, z + val.z);
}

Vector3 Vector3::operator- (Vector3 val) {
	return Vector3(x - val.x, y - val.y, z - val.z);
}

Vector3 Vector3::operator* (float val) {
	return Vector3(x * val, y * val, z * val);
}

bool Vector3::operator== (const Vector3& val) const {
	return x == val.x && y == val.y && z == val.z;
}

float Vector3::Module() {
	return sqrtf(x * x + y * y + z * z);
}

Vector3 Vector3::lerp(Vector3 a, Vector3 b, double t) {
	const auto linear = [](float x, float x0, float y0, float x1, float y1) {
		return y0 + (y1 - y0) / (x1 - x0) * (x - x0);
	};

	return {
		linear(t, 0.f, a.x, 1.f, b.x),
		linear(t, 0.f, a.y, 1.f, b.y),
		linear(t, 0.f, a.z, 1.f, b.z)
	};
}

Vector3Int::Vector3Int(int x, int y, int z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3Int::Vector3Int() {
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Vector3Int Vector3Int::operator+ (Vector3Int val) {
	return Vector3Int(x + val.x, y + val.y, z + val.z);
}

Vector3Int Vector3Int::operator- (Vector3Int val) {
	return Vector3Int(x - val.x, y - val.y, z - val.z);
}

Vector3Int Vector3Int::operator* (float val) {
	return Vector3Int(x * val, y * val, z * val);
}

bool Vector3Int::operator== (const Vector3Int& val) const {
	return x == val.x && y == val.y && z == val.z;
}

float Vector3Int::Module() {
	return sqrtf(x * x + y * y + z * z);
}

float Vector3Int::Distance(Vector3Int a, Vector3Int b) {
	return (a - b).Module();
}

bool Vector3Int::operator< (Vector3Int val) {
	return Module() < val.Module();
}

bool Vector3Int::operator> (Vector3Int val) {
	return Module() > val.Module();
}

bool Vector3Int::operator<= (Vector3Int val) {
	return Module() <= val.Module();
}

bool Vector3Int::operator>= (Vector3Int val) {
	return Module() >= val.Module();
}