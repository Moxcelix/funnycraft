#pragma once
#include "UI.h"
#include <iostream>

class Inventory
{
public:
	const static int count = 18;	// ���������� ���������
	int data[count];				// ������ ���������
	UI ui;							// ���������������� ���������
	int current = 0;				// ������� �������
	block_id current_block = 0;		// ID �������� �����
	void DrawInventory();			// ���������
	void Init();					// �������������
	void Update();					// ����������
	void AddBlock(block_id id);		// ���������� �����
};

