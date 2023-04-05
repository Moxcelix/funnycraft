#include "PosQueue.h"
#include<iostream>
/// <summary>
/// добавление элемента в очередь
/// </summary>
/// <param name="element"></param>
void PosQueue::add(Vector3Int* element)
{
	lock_guard<mutex> guard(mtx);

	//if (int l = contains(element))
	//{
	//	/*delete elements[l - 1];

	//	for (int i = l; i < elementsAdded; i++)
	//	{
	//		elements[i - 1] = elements[i];
	//	}

	//	elements[elementsAdded - 1] = element;*/

	//	return;
	//}

	if (elementsAdded < MaxCount) // если элементов меньше максимального значения
	{
		elements[elementsAdded] = element; // добавление элемента
		elementsAdded++;	// инкрементация кол-ва добавленных элементов
	}
	else
		delete element; // удалить указатель
}
/// <summary>
/// проверка на наличие элемента в очереди
/// </summary>
/// <param name="element"></param>
/// <returns>индекс элемента + 1</returns>
int PosQueue::contains(Vector3Int* element)
{
	for (int i = 0; i < elementsAdded; i++)
	{
		if (*elements[i] == *element) // если i-тый элемент равен элементу
			return i + 1; // вернуть индекс + 1
	}
	return 0;
}
/// <summary>
/// очистка очереди
/// </summary>
void PosQueue::clear()
{
	lock_guard<mutex> guard(mtx);

	for (int i = 0; i < elementsAdded; i++)
		delete elements[i]; // удалить i-тый элемент
	elementsAdded = 0;		// кол-во добавленных элементов равно нулю
}
/// <summary>
/// удаления последнего элемента
/// </summary>
void PosQueue::pop_back()
{
	lock_guard<mutex> guard(mtx);

	if (elementsAdded == 0) // если кол-во равно 0
		return;				// вернуть

	elementsAdded--;				// декриментация кол-ва 
	delete elements[elementsAdded];	// удаление элемента
}
/// <summary>
/// удаление первого элемента
/// </summary>
void PosQueue::pop_front()
{
	lock_guard<mutex> guard(mtx);

	if (elementsAdded == 0) // если кол-во равно 0
		return;				// вернуть

	delete elements[0];		// удалить первый
	// смещение элементов
	for (int i = 1; i < elementsAdded; i++)
	{
		elements[i - 1] = elements[i];
	}

	elementsAdded--; // декриментация кол-ва
}
/// <summary>
/// последний элемент
/// </summary>
/// <returns>последний элемент</returns>
Vector3Int* PosQueue::back()
{
	return elements[elementsAdded - 1];
}
/// <summary>
/// первый элемент
/// </summary>
/// <returns>первый элемент</returns>
Vector3Int* PosQueue::front()
{
	return elements[0];
}
/// <summary>
/// размер очереди
/// </summary>
/// <returns>количество элементов</returns>
int PosQueue::size()
{
	return elementsAdded; // вернуть кол-во элементов
}