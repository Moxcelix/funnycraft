#pragma once
#include"Vector3.h"
#include<iostream>
#include<fstream>
#include<GLFW/glfw3.h>
using namespace std;
using std::string;
/// <summary>
/// класс отладки
/// </summary>
class Debug
{
public:
	static bool active; // активность
	static bool bounds; // рамки
	static ofstream log; // поток вывода
	string file_name = "log.txt"; // имя файла
	static void Log(string text); // вывод тектса
	static void DrawBox(Vector3 pos, Vector3 dim, Vector3 color); // отрисовка рамок
	Debug(); // конструктор
	~Debug(); // деструктор
};

