#pragma once
#include "time.h"

class DeltaTime
{
private:
	float delta_time = 0;	// ������ �������
	float old_time = 0;		// ���������� ��������

public:
	float Get();	// ��������� ������ �������
	DeltaTime();	// �����������
};

