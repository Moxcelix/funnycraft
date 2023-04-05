#pragma once
#include"DeltaTime.h"
#include"Client.h"
#include<iostream>

using namespace std;

class Client;
/// <summary>
/// структура модификатора
/// </summary>
struct Modifier
{
private:
	bool d_repeat = false;
	bool p_repeat = false;
	const int max = 200;			// порог времени
	const int d_repeat_max = 100;	// порог времени при повторе разрушения
	const int p_repeat_max = 130;	// порог времени при повторе установки
public:
	Client* client;	// указатель на клиента
	
	Modifier(Client* client) :client(client) {}

	void Destroy(bool state = true);	// разрушение блоков
	void Place(bool state = true);		// установка блоков

	void Update();	// обновление
};