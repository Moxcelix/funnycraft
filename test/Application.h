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
	const int SIZE_X = 1280;	// первоначальна€ ширина окна в пиксел€х
	const int SIZE_Y = 720;		// первоначальна€ высота окна в пиксел€х

	GLFWwindow* window;			// указатель на экземл€р окна
	DeltaTime delta_time{};		// экземпл€р дельты времени

	float d_time{};				// дельта времени
	double fps{};					// количество кадров в секунду
	int lfps = 0, ms = 1;		// округленное количество кадров в секунду
	bool wireframe = false;		// режим рамочного рендеренга

	Client client;				// экземпл€р клиента

	void Resize(int x, int y);	// измениние содержимого под размер окна
	void Resize();				// измениние содержимого под размер окна
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

