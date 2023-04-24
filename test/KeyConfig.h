#pragma once
#include<GLFW/glfw3.h>

struct KeyConfig
{
	int forward		= GLFW_KEY_W;
	int back		= GLFW_KEY_S;
	int right		= GLFW_KEY_D;
	int left		= GLFW_KEY_A;
	int jump		= GLFW_KEY_SPACE;
	int shift		= GLFW_KEY_LEFT_SHIFT;
	int sprint		= GLFW_KEY_LEFT_CONTROL;
	int fly			= GLFW_KEY_R;

	int place		= GLFW_MOUSE_BUTTON_RIGHT;
	int destroy		= GLFW_MOUSE_BUTTON_LEFT;
	int copy		= GLFW_MOUSE_BUTTON_MIDDLE;
	int explode		= GLFW_KEY_B;

	int pause		= GLFW_KEY_ESCAPE;
	int enter		= GLFW_KEY_ENTER;
	int up_arrow	= GLFW_KEY_UP;
	int down_arrow	= GLFW_KEY_DOWN;
	int left_arrow	= GLFW_KEY_LEFT;
	int right_arrow	= GLFW_KEY_RIGHT;
};
