#include "DeltaTime.h"
/// <summary>
/// получение значения дельты времени
/// </summary>
/// <returns></returns>
float DeltaTime::Get() 
{
	delta_time = clock() - old_time;	// рассчёт дельты времени
	old_time = clock();					// запись старого времени

	return delta_time;					// возврат значения
}
/// <summary>
/// конструктор
/// </summary>
DeltaTime::DeltaTime() 
{
	Get(); // рассчёт первого значения
}