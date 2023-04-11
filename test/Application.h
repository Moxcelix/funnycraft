#pragma once
#define _USE_MATH_DEFINES
#define STB_IMAGE_IMPLEMENTATION

#include <GLFW/glfw3.h>
#include <filesystem>

#include "Client.h"
#include "Debug.h"
#include "DeltaTime.h"

class Application
{
public:
	const int SIZE_X = 1280;	// �������������� ������ ���� � ��������
	const int SIZE_Y = 720;		// �������������� ������ ���� � ��������

	GLFWwindow* window;			// ��������� �� �������� ����
	DeltaTime delta_time;		// ��������� ������ �������

	float d_time;				// ������ �������
	double fps;					// ���������� ������ � �������
	int lfps = 0, ms = 1;		// ����������� ���������� ������ � �������
	bool wireframe = false;		// ����� ��������� ����������

	Client client;				// ��������� �������

	void Resize(int x, int y);	// ��������� ����������� ��� ������ ����
	void Resize();				// ��������� ����������� ��� ������ ����
	void CalculateFPS();
	void Update();
	void RenderUI();
	void RenderWorld();
	void OpenWindow();
	void StopWindow();
	void Run();
	void Render();
	void TextureInit(unsigned int& texture, const char* name);
	void GameInit();
	void LoadIcon();
	void LoadResources();
	void SetCallbacks();
	void InputWorldName();

}inline app;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

