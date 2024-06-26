#include"Modifier.h"

/// <summary>
/// ���������� ������
/// </summary>
/// <param name="state"></param>
void Modifier::Destroy(bool state)
{
	d_repeat = state;
}
/// <summary>
/// ��������� ������
/// </summary>
/// <param name="state"></param>
void Modifier::Place(bool state)
{
	p_repeat = state;
}
/// <summary>
/// ����������
/// </summary>
void Modifier::Update() 
{
	static float dc = 0, dt = static_cast<float>(max);
	static float pc = 0, pt = static_cast<float>(max);
	static DeltaTime time;

	float delta_time = time.Get();

	if (d_repeat) // ���������� ������ 
	{
		if (dc == 0) // ������� �������� ��� ����������� �������������� (����������� �������)
		{
			client->DestroyBlock(); // ���� ������� � ��������� �����
			dc += delta_time;		// ���������� �������� �������
		}
		else if (dc < dt)
		{
			// ���������� �������� ������� � ����������� �� ��������
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
		// �������������� ��������� ��������
		dc = 0;
		dt = static_cast<float>(max);
	}

	if (p_repeat) // ��������� ������
	{
		if (pc == 0)
		{
			client->SetBlock(); // ���������� ��������� ����
			pc += delta_time;	// ��������� �������� �������
		}
		else if (pc < pt)
		{
			// ���������� �������� ������� � ����������� �� ��������
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
		// �������������� ��������� ��������
		pc = 0;
		pt = static_cast<float>(max);
	}
}