#pragma once
#include"Vector3.h"
#include<iostream>
#include<fstream>
#include<GLFW/glfw3.h>

class Debug
{
public:
	static bool active; // ����������
	static bool bounds; // �����
	static std::ofstream log; // ����� ������
	std::string file_name = "log.txt"; // ��� �����
	static void Log(std::string text); // ����� ������
	static void DrawBox(Vector3 pos, Vector3 dim, Vector3 color); // ��������� �����
	Debug(); // �����������
	~Debug(); // ����������
};

