#pragma once
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#define _USE_MATH_DEFINES
#include <iostream>
#include <functional>
#include <experimental/filesystem>
#include <cmath>
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

using namespace std;
/// <summary>
/// класс клиента
/// </summary>
class Client
{
public:
	struct Page; // заголовок структуры страницы меню 

	void Pause(bool active);					// активность меню
	void ToSettings();							// переход к странице настроек
	void ToMenu();								// переход к главной странице меню
	void ToAgree();								// переход к странице согласия
	void ToNewWorld();							// переход к странице создания нового мира
	void ToRenderDistance();					// переход к настройкам дальности прорисовки
	void ButtonUp();							// кнопка вверх
	void ButtonDown();							// кнопка вверх
	
	void SetBlock(int x, int y, int z, int id); // установка блока
	void SetBlock(Vector3 pos, int id);			// установка блока
	void SetBlock();							// установка блока
	void DestroyBlock(int x, int y, int z);		// разрушение блока
	void DestroyBlock(Vector3 pos);				// разрушение блока
	void DestroyBlock();						// разрушение блока
	void CopyBlock();							// копирование блока

	void Resize();								// изменение размеров окна
	void MoveCamera(double delta_time);			// движение камеры
	void UpdatePhantomPos();					// обновление позиции фантома
	void CameraUpdate();						// обновление камеры
	void NewWorld();							// создание нового мира
	void SetRenderDistance(int d);				// установка дальности прорисовки

	void Init();								// инициализация
	void Update(float deltaTime);				// обновление 
	void Clear();								// очистка
	void Render();								// рендеринг
	void RenderUI();							// рендеринг UI
	void DrawMenu();							// отрисовка меню

	GLFWwindow* window;			// указатель на экземляр окна
	UI ui, Menu, Settings;		// экземпляры пользовательского интерфейса
	Player player;				// экземпляр игрока
	World* world;				// указатель на экземпляр мира
	Page* page;					// указатель на текущую страницу меню
	Vector2Int MenuPos;			// позиция в меню
	Phantom phantom;			// фантом (рамки)
	Inventory inventory;		// инвентарь
	World::Settings gen_params;	// параметры генерации
	Modifier* modifier;			// модификатор
	KeyConfig key_config;		// конфигурация клавишей

	vector<Particles*> particles;

	bool pause = false; // флаг паузы
	bool enter = false; // флаг подтверждения
	bool close = false; // флаг закрытия

	double mouse_pos_x, mouse_pos_y; // центральное положение курсора
	unsigned int main_texture; // тексутрный атлас

	struct Button // кнопка в меню
	{
		string name;				// заголовок кнопки
		std::function<void()> func; // ссылка на процедуру
		Button() :name("?"), func([] {}) {}
		Button(string name, std::function<void()> func) :name(name), func(func)
		{
		}
		virtual void DoFunc() // вызов переданной функции
		{
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
	Button btn_back{ "Назад", [&] {ToMenu(); } };					// кнопка перехода в меню
	Button btn_small{ "Маленькая", [&] {SetRenderDistance(4); } };	// кнопка установки маленькой дальности прорисовки
	Button btn_medium{ "Средняя", [&] {SetRenderDistance(6); } };	// кнопка установки средней дальности прорисовки
	Button btn_large{ "Большая", [&] {SetRenderDistance(8); } };	// кнопка установки большой дальности прорисовки
	Button btn_confirm{ "Продолжить", [&] {NewWorld(); } };			// кнопка продолжения
	Button btn_cancel{ "Отменить", [&] {ToMenu(); } };				// кнопка отмены
	Button btn_create{ "Создать", [&] {ToAgree(); } };				// кнопка создания мира

	struct Switcher : Button // кнопка-переключатель 
	{
		string name;	// заголовок переключателя
		bool* value;	// ссылка на переключаемое значение
		/// <summary>
		/// конструктор
		/// </summary>
		Switcher(string name, bool* value) :name(name), value(value)
		{
			Button::name = name + (*value ? " On" : " Off");
		}
		/// <summary>
		/// переключение значения 
		/// </summary>
		virtual void DoFunc()
		{
			*value = !(*value);
			Button::name = name + (*value ? " On" : " Off");
		}
	};

	Switcher swt_mountains{ "Горы", &gen_params.params[0] };	// выключатель гор
	Switcher swt_plains{ "Равнины", &gen_params.params[1] };	// выключатель равнин
	Switcher swt_trees{ "Деревья", &gen_params.params[2] };		// выключатель деревьев
	Switcher swt_grass{ "Трава", &gen_params.params[3] };		// выключатель травы
	/// <summary>
	/// структура страницы меню
	/// </summary>
	struct Page
	{
		int count;			// количество параметров
		string name;		// заголовок страницы
		Button** buttons;	// массив кнопок
		float* indents;		// массив разметки по вертикали 
	};
	// страницы меню
	Page page_menu{ 4, "Меню", new Button * [] {&btn_settings,& btn_new_world,& btn_continue,& btn_close}, new float[] {0, 0, 0, 0} };
	Page page_settings{ 3, "Настройки", new Button * [] {&btn_render_distance,& btn_day,& btn_back}, new float[] {0, 0, 20} };
	Page page_render_distance{ 4, "Дальность прорисовки", new Button * [] {&btn_small,& btn_medium,& btn_large,& btn_back}, new float[] {0, 0, 0, 20} };
	Page page_agree{ 2, "Текущий мир будет удален", new Button * [] {&btn_confirm,& btn_cancel}, new float[] {0,0} };
	Page page_new_world{ 6, "Параметры нового мира", new Button * [] {&swt_mountains,& swt_plains,& swt_trees,& swt_grass,& btn_create,& btn_back},new float[] {0, 0, 0, 0, 20, 0} };
};