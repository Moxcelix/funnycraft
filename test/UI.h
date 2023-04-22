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

struct Text;
struct Plane;
struct UIElement;

class UI {
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

	unsigned int font;					

	std::vector<Text> texts;			
	std::vector<Text> static_texts;		
	std::vector<Plane> static_planes;	
	std::vector<Plane> planes;			

	Cross cross;
	BackGround bg;

	void PrintText(Corner corner, float size, int x, int y, std::string str, float r, float g, float b); 
	void PrintText(Text t);	
	void PrintStatic();		
	void Render();			
	void PrintDebug(std::string str, float r, float g, float b); 
	void PrintPlane(Plane p);	
	void RemoveStaticText(Text t);
	void RemoveStaticText();	
	void SetSize(int w, int h);	
	void Clear();				
	void SetFlags(int flags);	

	Vector2 GetCorner(Corner corner, float xScale, float yScale, int xShift = 0, int yShift = 0);
	Text AddStaticText(Corner corner, float size, int x, int y, std::string str, float r, float g, float b);
	Plane AddStaticPlane(Corner corner, float size, int x, int y, int w, int h, ColorSquad colors);
};

struct UIElement {
	int ID;

	int x, y;
	float r, g, b;
	float size;

	float* vertices;
	float* colors;
	float* uvs;

	UVData* uvs_data;
	UI::Corner corner;


	float rect[12] = {
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

struct Plane : UIElement {
	int w, h;
	ColorSquad squad;

	Plane(int x, int y, int w, int h, float size,
		ColorSquad squad, UI::Corner corner){
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
		this->size = size;
		this->squad = squad;
		this->corner = corner;
	}
};

struct Text : UIElement {
	std::string text; 

	float width = 6. / 128., height = .0625f;
	float iwidth = 6, iheight = 8;			
	int wCount = 21, hCount = 16;			

	float quad[12] =
	{
		iwidth, 0, 0,
		0, 0, 0,
		0, iheight, 0,
		iwidth, iheight, 0
	};


	Text(int x, int y, float size, float r, float g, float b,
		std::string text, UI::Corner corner) {
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

