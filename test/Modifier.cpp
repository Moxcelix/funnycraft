#include"Modifier.h"

/// <summary>
/// разрушение блоков
/// </summary>
/// <param name="state"></param>
void Modifier::Destroy(bool state)
{
	d_repeat = state;
}
/// <summary>
/// установка блоков
/// </summary>
/// <param name="state"></param>
void Modifier::Place(bool state)
{
	p_repeat = state;
}
/// <summary>
/// обновление
/// </summary>
void Modifier::Update() 
{
	static float dc = 0, dt = static_cast<float>(max);
	static float pc = 0, pt = static_cast<float>(max);
	static DeltaTime time;

	float delta_time = time.Get();

	if (d_repeat) // разрушение блоков 
	{
		if (dc == 0) // система таймеров для длительного взаимодействия (удерживание клавиши)
		{
			client->DestroyBlock(); // блок воздуха в выбранное место
			dc += delta_time;		// увеличение значения таймера
		}
		else if (dc < dt)
		{
			// увеличение значения таймера в зависимости от скорости
			dc += delta_time * (.3f + client->player.GetVelocity() * 100);
		}
		else
		{
			dc = 0;
			dt = static_cast<float>(d_repeat_max);
		}
	}
	else
	{
		// восстановление начальных значений
		dc = 0;
		dt = static_cast<float>(max);
	}

	if (p_repeat) // установка блоков
	{
		if (pc == 0)
		{
			client->SetBlock(); // утсановить выбранный блок
			pc += delta_time;	// увеличить значение таймера
		}
		else if (pc < pt)
		{
			// увеличение значения таймера в зависимости от скорости
			pc += delta_time * (.3f + client->player.GetVelocity() * 100);
		}
		else
		{
			pc = 0;
			pt = static_cast<float>(p_repeat_max);
		}
	}
	else
	{
		// восстановление начальных значений
		pc = 0;
		pt = static_cast<float>(max);
	}
}