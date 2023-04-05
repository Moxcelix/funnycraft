#pragma once
#include<GLFW/glfw3.h>
#include<iostream>
#include<vector>
#include"UVData.h"
#include"Vector2.h"
#include"Block.h"
#include"UIObjects.h"


#define UI_DRAW_CROSS			0b0001	// отрисовка крестика
#define UI_DRAW_BACK_GROUND		0b0010	// отрисовка фона

using namespace std;

struct Text;
struct Plane;
struct UIElement;
/// <summary>
/// класс пользовательского интерфейса
/// </summary>
class UI
{
private:
	int IDCounter = 0; // счетчик ID
	int flags = 0;	// флаги отрисовки
	int winWidth = 1, winHeight = 1;	// габариты окна
	int debug_line_counter = 0;			// счетчик количества строк отладки
public:
	/// <summary>
	/// перечисление углов
	/// </summary>
	enum class Corner
	{
		left_up,
		left,
		left_down,
		up,
		middle,
		down,
		right_up,
		right,
		right_down
	};

	unsigned int font;					// текстура шрифта

	vector<Text> texts;				// вектор текстов
	vector<Text> static_texts;		// вектор статичных текстов
	vector<Plane> static_planes;	// вектор статичных панелей
	vector<Plane> planes;			// вектор панелей

	Cross cross;
	BackGround bg;

	void PrintText(Corner corner, float size, int x, int y, string str, float r, float g, float b); // Вывод текста на экран
	void PrintText(Text t);			// Вывод текста на экран
	void PrintStatic();				// Печать статичных объектов
	void Render();					// Рендеринг элементов
	void PrintDebug(string str, float r, float g, float b); // Вывод на экран отладочного текста
	void PrintPlane(Plane p);		// Вывод на экран панели
	void RemoveStaticText(Text t);	// Удаление статичного тектса
	void RemoveStaticText();		// Удаление статичных текстов
	void SetSize(int w, int h);		// Установка размеров окна
	void Clear();					// Очистка
	void SetFlags(int flags);		// установка флагов

	Vector2 GetCorner(Corner corner, float xScale, float yScale, int xShift = 0, int yShift = 0); // Угол привязки 
	Text AddStaticText(Corner corner, float size, int x, int y, string str, float r, float g, float b); // Добавление статичного текста 
	Plane AddStaticPlane(Corner corner, float size, int x, int y, int w, int h, ColorSquad colors); // Добавление статичной панели
};
/// <summary>
/// UI элемент
/// </summary>
struct UIElement 
{
	int ID;				// ID

	int x, y;			// координаты
	float r, g, b;		// цвет
	float size;			// размер

	float* vertices;	// массив вершин
	float* colors;		// массив цветов
	float* uvs;			// массив UV координат

	UVData* uvs_data;	// UV данные 
	UI::Corner corner;	// угол привязки

	// массив вершин квадрата
	float rect[12] =
	{
		1, 0, 0,
		0, 0, 0,
		0, 1, 0,
		1, 1, 0
	};

	bool operator==(UIElement val)
	{
		return ID == val.ID;
	}
};
/// <summary>
/// структура панели
/// </summary>
struct Plane : UIElement
{
	int w, h;			// габариты
	ColorSquad squad;	// цвета углов

	/// <summary>
	/// конструктор
	/// </summary>
	Plane(int x, int y, int w, int h, float size, ColorSquad squad, UI::Corner corner)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
		this->size = size;
		this->squad = squad;
		this->corner = corner;
	}
};
/// <summary>
/// структура текста
/// </summary>
struct Text : UIElement
{
	string text; // текст

	float width = 6. / 128., height = .0625f;	// ширина шрифта
	float iwidth = 6, iheight = 8;				// ширина шрифта в пикселях
	int wCount = 21, hCount = 16;				// количество строк и столбцов в шрифте

	// массив вершин буквы
	float quad[12] =
	{
		iwidth, 0, 0,
		0, 0, 0,
		0, iheight, 0,
		iwidth, iheight, 0
	};

	/// <summary>
	/// конструктор
	/// </summary>
	Text(int x, int y, float size, float r, float g, float b, string text, UI::Corner corner)
	{
		this->x = x;
		this->y = y;
		this->size = size;
		this->r = r;
		this->g = g;
		this->b = b;
		this->text = text;
		this->corner = corner;
	}
};

