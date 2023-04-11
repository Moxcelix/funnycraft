#pragma once
#include<iostream>

#include"DeltaTime.h"
#include"Client.h"

class Client;
/// <summary>
/// ��������� ������������
/// </summary>
struct Modifier
{
private:
	bool d_repeat = false;
	bool p_repeat = false;
	const int max = 200;			// ����� �������
	const int d_repeat_max = 100;	// ����� ������� ��� ������� ����������
	const int p_repeat_max = 130;	// ����� ������� ��� ������� ���������
public:
	Client* client;	// ��������� �� �������
	
	Modifier(Client* client) :client(client) {}

	void Destroy(bool state = true);	// ���������� ������
	void Place(bool state = true);		// ��������� ������

	void Update();	// ����������
};