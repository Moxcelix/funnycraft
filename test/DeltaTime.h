#pragma once
#include "time.h"

class DeltaTime
{
private:
	float delta_time = 0;	// дельта времени
	float old_time = 0;		// предыдущее значение

public:
	float Get();	// получение дельты времени
	DeltaTime();	// конструктор
};

