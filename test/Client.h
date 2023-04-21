#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <functional>
#include <filesystem>

#include "Modifier.h"
#include "VertexData.h"
#include "Inventory.h"
#include "Vector2.h"
#include "Player.h"
#include "Ray.h"
#include "Phantom.h"
#include "Particles.h"
#include "UI.h"
#include "KeyConfig.h"

struct Modifier;

class Client {
public:
	struct Page;
	static struct Settings {
		bool smooth_lighting = false;
	} settings;

	Client();
	~Client();

	void Pause(bool active);
	void ToSettings();		
	void ToMenu();			
	void ToAgree();			
	void ToNewWorld();		
	void ToRenderDistance();
	void ButtonUp();							
	void ButtonDown();							
	void ButtonRight();							
	void ButtonLeft();							
	
	void SetBlock(int, int, int, block_id);
	void SetBlock(Vector3, block_id);		
	void SetBlock();								
	void DestroyBlock(int, int, int);			
	void DestroyBlock(Vector3);					
	void DestroyBlock();							
	void CopyBlock();								

	void Resize();						
	void MoveCamera(double);	
	void UpdatePhantomPos();			
	void CameraUpdate();				
	void NewWorld();					
	void SetRenderDistance(int);
	void ReloadChunks();

	void Init();					
	void Update(float);	
	void Clear();					
	void Render();					
	void RenderUI();				
	void DrawMenu();				

	GLFWwindow* window;		
	UI ui, menu;	
	Player player;			
	World* world;			
	Page* page;				
	Vector2Int MenuPos;		
	Phantom phantom;		
	Inventory inventory;	
	World::Settings gen_params;
	Modifier* modifier;		
	KeyConfig key_config;	

	vector<Particles*> particles;

	bool pause = false; 
	bool enter = false; 
	bool close = false; 

	double mouse_pos_x, mouse_pos_y;
	unsigned int main_texture; 

	struct Button {
		string name;
		std::function<void()> func;
		Button() :name("?"), func([] {}) {}
		Button(string name, std::function<void()> func) :name(name), func(func) {}
		virtual void DoFunc() {
			func();
		}
	};

	Button btn_settings{ "Настройки", [&] {ToSettings(); } };			// кнопка настроек
	Button btn_new_world{ "Создать новый мир", [&] {ToNewWorld(); } };	// кнопка создания мира
	Button btn_continue{ "Продолжить игру", [&] {Pause(false); } };		// кнопка продолжения игры
	Button btn_close{ "Выйти из игры", [&] {close = true; } };			// кнопка выхода из игры
	Button btn_render_distance{ "Дальность прорисовки", [&] {ToRenderDistance(); } }; // кнопка дальности прорисовки
	Button btn_day{ "Сменить время суток", [&]()						// кнопка смены времени суток
	{
		World::is_day = !World::is_day;
		world->UpdateWorldLighting();
	}, };
	Button btn_back{ "Назад", [&]{ToMenu(); } };					// кнопка перехода в меню
	Button btn_small{ "Маленькая", [&] {SetRenderDistance(3); } };	// кнопка установки маленькой дальности прорисовки
	Button btn_medium{ "Средняя", [&] {SetRenderDistance(4); } };	// кнопка установки средней дальности прорисовки
	Button btn_normal{ "Нормальная", [&] {SetRenderDistance(5); } };// кнопка установки большой дальности прорисовки
	Button btn_large{ "Большая", [&] {SetRenderDistance(6); } };	// кнопка установки большой дальности прорисовки
	Button btn_confirm{ "Продолжить", [&] {NewWorld(); } };			// кнопка продолжения
	Button btn_cancel{ "Отменить", [&] {ToMenu(); } };				// кнопка отмены
	Button btn_create{ "Создать", [&] {ToAgree(); } };				// кнопка создания мира

	struct Switcher : Button {
		string name;	// заголовок переключателя
		bool* value;	// ссылка на переключаемое значение

		Switcher(string name, bool* value) :Button{}, name(name), value(value)
		{
			Button::name = name + (*value ? " On" : " Off");
		}

		virtual void DoFunc()
		{
			*value = !(*value);
			Button::name = name + (*value ? " On" : " Off");
		}
	};

	struct LamdaSwitcher : Switcher {

		LamdaSwitcher(string name, bool* value, std::function<void()> func) :
			Switcher(name, value) {

			this->func = func;
		}

		virtual void DoFunc() override
		{
			*value = !(*value);
			Button::name = name + (*value ? " On" : " Off");
			func();
		}
	};

	Switcher swt_mountains{ "Горы", &gen_params.params[0] };	// выключатель гор
	Switcher swt_plains{ "Равнины", &gen_params.params[1] };	// выключатель равнин
	Switcher swt_trees{ "Деревья", &gen_params.params[2] };		// выключатель деревьев
	Switcher swt_grass{ "Трава", &gen_params.params[3] };		// выключатель травы

	LamdaSwitcher swt_smooth_lighting{ "Плавное освещение", &settings.smooth_lighting, [&] {ReloadChunks(); } };

	struct Page {
		int count;			// количество параметров
		string name;		// заголовок страницы
		Button** buttons;	// массив кнопок
		float* indents;		// массив разметки по вертикали 
	};
	// страницы меню
	Page page_menu{ 4, "Меню", new Button * [] {&btn_settings, &btn_new_world, &btn_continue, &btn_close}, new float[] {0, 0, 0, 0} };
	Page page_settings{ 4, "Настройки", new Button * [] {&btn_render_distance,& btn_day, &swt_smooth_lighting, & btn_back}, new float[] {0, 0, 0, 20} };
	Page page_render_distance{ 5, "Дальность прорисовки", new Button * [] {&btn_small,&btn_medium, &btn_normal, &btn_large,& btn_back}, new float[] {0, 0, 0, 0, 20} };
	Page page_agree{ 2, "Текущий мир будет удален", new Button * [] {&btn_confirm,& btn_cancel}, new float[] {0,0} };
	Page page_new_world{ 6, "Параметры нового мира", new Button * [] {&swt_mountains,& swt_plains,& swt_trees,& swt_grass,& btn_create,& btn_back},new float[] {0, 0, 0, 0, 20, 0} };
};