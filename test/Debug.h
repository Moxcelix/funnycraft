#pragma once
#include"Vector3.h"
#include<iostream>
#include<fstream>
#include<GLFW/glfw3.h>

class Debug
{
public:
	static bool active; // активность
	static bool bounds; // рамки
	static std::ofstream log; // поток вывода
	std::string file_name = "log.txt"; // имя файла
	static void Log(std::string text); // вывод тектса
	static void DrawBox(Vector3 pos, Vector3 dim, Vector3 color); // отрисовка рамок
	Debug(); // конструктор
	~Debug(); // деструктор
};

