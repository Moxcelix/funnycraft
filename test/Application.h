#pragma once
#define _USE_MATH_DEFINES
#define STB_IMAGE_IMPLEMENTATION

#include <GLFW/glfw3.h>
#include <filesystem>

#include "Client.h"
#include "Debug.h"
#include "DeltaTime.h"

class Application {
public:
	const int SIZE_X = 1280;
	const int SIZE_Y = 720;
	const char* const WIN_TITLE = "Funnycraft";

	GLFWwindow* window;
	DeltaTime delta_time{};
	Client client;

	float d_time{};
	double fps{};
	int lfps = 0;
	int ms = 1;
	bool wireframe = false;

	void Resize(int, int);
	void Resize();
	void CalculateFPS();
	void Update();
	void RenderUI();
	void RenderWorld();
	void OpenWindow();
	void StopWindow();
	void Run();
	void Render();
	void TextureInit(unsigned int&, const char*);
	void GameInit();
	void LoadIcon();
	void LoadResources();
	void SetCallbacks();
	void InputWorldName();

}inline app;

void mouse_button_callback(GLFWwindow*, int, int, int);
void key_callback(GLFWwindow*, int, int, int, int);
void scroll_callback(GLFWwindow*, double, double);

