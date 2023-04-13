#pragma once
#include "UI.h"
#include <iostream>

class Inventory
{
public:
	const static int count = 16;	// количество элементов
	int data[count];				// массив элементов
	UI ui;							// пользовательский интерфейс
	int current = 0;				// текщуая позиция
	block_id current_block = 0;		// ID текущего блока
	void DrawInventory();			// отрисовка
	void Init();					// инициализация
	void Update();					// обновление
	void AddBlock(block_id id);		// добавление блока
};

