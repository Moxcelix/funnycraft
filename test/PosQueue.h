#pragma once
#include"Vector3.h"
#include<mutex>
#include<iostream>

using namespace std;
/// <summary>
/// класс очереди позиций
/// </summary>
class PosQueue
{
private:
	mutex mtx;
	const static int MaxCount = 512; // максиммальное количество элементов в очереди
	Vector3Int* elements[MaxCount]; // массив указателей на элементы
	int elementsAdded = 0; // количество добавленных элементов
public:	
	void add(Vector3Int*); // добавление элемента
	void pop_back(); // удаления последнего элемента
	void pop_front(); // удаление первого элемента
	void clear(); // очистка очереди 
	Vector3Int* front(); // первый элемент
	Vector3Int* back(); // последний элемент
	int contains(Vector3Int*); // проверка на наличие элемента в очереди
	int size(); // размер очереди
};

