#include "PosQueue.h"
#include<iostream>

void PosQueue::add(Vector3Int element) {
	std::lock_guard guard(mtx);

	if (elementsAdded < MaxCount) {
		elements[elementsAdded] = element; 
		elementsAdded++;
	}
}

int PosQueue::contains(Vector3Int& element) {
	for (int i = 0; i < elementsAdded; i++)
	{
		if (elements[i] == element)
			return i + 1;
	}
	return 0;
}

void PosQueue::clear() {
	std::lock_guard guard(mtx);

	elementsAdded = 0;
}

void PosQueue::pop_back() {
	std::lock_guard guard(mtx);

	if (elementsAdded == 0)
		return;

	elementsAdded--;
}

void PosQueue::pop_front() {
	std::lock_guard guard(mtx);

	if (elementsAdded == 0)
		return;	

	for (int i = 1; i < elementsAdded; i++)
	{
		elements[i - 1] = elements[i];
	}

	elementsAdded--;
}

Vector3Int& PosQueue::back() {
	return elements[elementsAdded - 1];
}

Vector3Int& PosQueue::front() {
	return elements[0];
}

int PosQueue::size() {
	return elementsAdded;
}