#define _USE_MATH_DEFINES
#define STB_IMAGE_IMPLEMENTATION
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
//подключение библиотек и заголовочных файлов
#include <GLFW/glfw3.h>
#include <format>
#include <iostream>
#include <filesystem>
#include <string>
#include <thread>
#include <string_view>
#include <cmath>
#include <direct.h>
#include <stb_image.h>
#include "Client.h"
#include "Debug.h"
#include "DeltaTime.h"
#include <windows.h>
#include <experimental/filesystem>

using namespace std;

void Resize(int x, int y);	// измениние содержимого под размер окна
void Resize();				// измениние содержимого под размер окна

const int SIZE_X = 1280;	// первоначальная ширина окна в пикселях
const int SIZE_Y = 720;		// первоначальная высота окна в пикселях
GLFWwindow* window;			// указатель на экземляр окна

DeltaTime delta_time;		// экземпляр дельты времени
float d_time;				// дельта времени
bool wireframe = false;		// режим рамочного рендеренга
double fps;					// количество кадров в секунду
int lfps = 0, ms = 1;		// округленное количество кадров в секунду

Vector3 sky_color;	// цвет неба
Client client;		// экземпляр клиента

/// <summary>
/// обработчик событий клавиш мыши (станадртный метод, предложенный разработчиками GLFW)
/// </summary>
/// <param name="window"></param>
/// <param name="button"></param>
/// <param name="action"></param>
/// <param name="mods"></param>
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS) // действие нажатия
	{
		if (button == client.key_config.destroy) // ЛКМ
			client.modifier->Destroy(); // установка состояния для ЛКМ
		if (button == client.key_config.place) // ПКМ
			client.modifier->Place(); // установка состояния для ПКМ
		if (button == client.key_config.copy) // ЦКМ
			client.CopyBlock();	// копирование блока
	}
	if (action == GLFW_RELEASE) // на отжатие 
	{
		if (button == client.key_config.destroy) // ЛКМ
			client.modifier->Destroy(false); // установка состояния для ЛКМ
		if (button == client.key_config.place) // ПКМ
			client.modifier->Place(false); // установка состояния для ПКМ
	}
}
/// <summary>
/// обработчик событий колеса мыши (станадртный метод, предложенный разработчиками GLFW)
/// </summary>
/// <param name="window"></param>
/// <param name="xoffset"></param>
/// <param name="yoffset"></param>
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (yoffset > 0) // колесо мыши вверх
		client.ButtonUp();

	if (yoffset < 0) // колесо мыши вниз
		client.ButtonDown();
}
/// <summary>
/// обработчик событий клавиатуры (станадртный метод, предложенный разработчиками GLFW)
/// </summary>
/// <param name="window"></param>
/// <param name="key"></param>
/// <param name="scancode"></param>
/// <param name="action"></param>
/// <param name="mods"></param>
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) // действия нажатия
	{
		if (key == client.key_config.explode) {
			if (client.world->GetBlockID(client.player.look_pos) == Block::tnt->id)
				client.world->Explode(client.player.look_pos);
		}
		else if (key == client.key_config.left)
			client.player.left.Move(client.player.can_move);
		else if (key == client.key_config.right)
			client.player.right.Move(client.player.can_move);
		else if (key == client.key_config.forward)
			client.player.forward.Move(client.player.can_move);
		else if (key == client.key_config.back)
			client.player.back.Move(client.player.can_move);
		else if (key == client.key_config.shift)
			client.player.down.Move(client.player.can_move);
		else if (key == client.key_config.jump)
			client.player.up.Move(client.player.can_move);
		else if (key == client.key_config.sprint)
			client.player.sprint = true;
		else if (key == client.key_config.fly)
		{
			if (client.player.can_move)
				client.player.fly = !client.player.fly; // вкл/выкл полёт
		}
		else if (key == client.key_config.up)
			client.ButtonUp(); // движение в пользовательском интерфейсе вверх
		else if (key == client.key_config.down)
			client.ButtonDown(); // движение в пользовательском интерфейсе вниз
		else if (key == client.key_config.enter)
			client.enter = true;  // подтверждение в пользовательском интерфейсе
		else if (key == client.key_config.pause)
			client.Pause(!client.pause); // вкл/выкл меню
		else if(key == GLFW_KEY_F3)
			wireframe = !wireframe; // вкл/выкл режим рамо
		else if(key == GLFW_KEY_F4)
			Debug::bounds = !Debug::bounds; // вкл/выкл режим отрисовки границ чанков
	}
	else if (action == GLFW_RELEASE) // отжатие клавиши
	{
		if (key == client.key_config.left)
			client.player.left.Move(false);
		else if (key == client.key_config.right)
			client.player.right.Move(false);
		else if (key == client.key_config.forward)
			client.player.forward.Move(false);
		else if (key == client.key_config.back)
			client.player.back.Move(false);
		else if (key == client.key_config.shift)
			client.player.down.Move(false);
		else if (key == client.key_config.jump)
			client.player.up.Move(false);
		else if (key == client.key_config.sprint)
			client.player.sprint = false;
	}
}
/// <summary>
/// инициализация тектсур (стандартный метод, предложенный разработчиками STB Image)
/// </summary>
/// <param name="texture"></param>
/// <param name="name"></param>
void TextureInit(unsigned int& texture, const char* name)
{
	int width, height, cnt; // параметры изображения
	unsigned char* data = stbi_load(name, &width, &height, &cnt, 0); //загрузка текстуры
	glGenTextures(1, &texture); // ссылка на текстуру
	glBindTexture(GL_TEXTURE_2D, texture); // цель текстурирования 
	// параметры текстуры 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glBindTexture(GL_TEXTURE_2D, 0);	// отвязка цели текстурирования 
	stbi_image_free(data);				// освобождение памяти
}
/// <summary>
/// инициализация игры
/// </summary>
void GameInit()
{
	client.Init(); // инициализация клиента

	TextureInit(client.main_texture, "terrain.png");		// Инициализация текстур
	TextureInit(client.ui.font, "myfont.png");				// Инициализация шрифта
	TextureInit(client.Menu.font, "myfont.png");			// Инициализация шрифта
	TextureInit(client.inventory.ui.font, "myfont.png");	// Инициализация шрифта

	glEnable(GL_ALPHA_TEST);		// Включение отрисовки альфа-канала
	glAlphaFunc(GL_GREATER, 0.5);	// Утсановка границы Cut-off
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Скрыть курсор

	glEnable(GL_FOG);                       // Включение тумана 
	glFogi(GL_FOG_MODE, GL_LINEAR);			// Выбор типа тумана
	glHint(GL_FOG_HINT, GL_DONT_CARE);      // Вспомогательная установка тумана
}
/// <summary>
/// отрисовка кадра
/// </summary>
void Render()
{
	int distance = World::render_distance * Chunk::ChunkSize; // максимальное расстояние

	Resize(); //корректировка содержимого под размер окна

	glEnable(GL_DEPTH_TEST);				// Включение глубины отрисовки
	glFogf(GL_FOG_START, distance * 0.5);	// Глубина, с которой начинается туман
	glFogf(GL_FOG_END, distance);			// Глубина, где туман заканчивается
	glFogf(GL_FOG_DENSITY, M_E/distance);   // Насколько густым будет туман

	if (World::is_day) // переключение цвета неба в зависимости от времени суток
		sky_color = Vector3(0.6, 0.8, 1);	// день
	else
		sky_color = Vector3(0, 0, 0.1);		// ночь

	glClearDepth(2);
	glClearColor(sky_color.x, sky_color.y, sky_color.z, 1); // Цвет фона (небо)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Флаги очистки 
	glEnable(GL_TEXTURE_2D); // Использование 2D текстур

	float fog_color[4] = { sky_color.x, sky_color.y, sky_color.z, 1 }; // цвет неба
	glFogfv(GL_FOG_COLOR, fog_color);				// Устанавливаем цвет тумана
	client.CameraUpdate();							// Обновление камеры
	glEnable(GL_CULL_FACE);							// Куллинг поверхностей вкл
	glCullFace(GL_FRONT);							// Отключение отрисовки внутренних граней
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);	// Состояние текстурного массива
	glEnableClientState(GL_VERTEX_ARRAY);			// Состояние массива вершин
	glEnableClientState(GL_COLOR_ARRAY);			// Состояние массива цветов

	client.Render(); // вызов метода рендеринга в клиенте

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);	// Выход из состояния отрисовки текстур
	glDisableClientState(GL_VERTEX_ARRAY);			// Отключение состояния массива вершин
	glDisableClientState(GL_COLOR_ARRAY);			// Отключение состояния массива вершин
	glDisable(GL_DEPTH_TEST);						// Выключение глубины отрисовки
}
/// <summary>
/// Корректировка содержимого под заданный размер окна
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
void Resize(int x, int y)
{
	glViewport(0, 0, x, y); // Точка обзора

	float k = x / (float)y;	// Отношение сторон
	float sz = 0.01f;		// модификатор угла обзора

	glLoadIdentity();		// замена матрицы на матрицу по умолчанию
	glFrustum(-k * sz, k * sz, -sz, sz, 0.01, 100); // перспектива
}
/// <summary>
/// Корректировка содержимого под размер окна
/// </summary>
void Resize()
{
	int width, height; // высота и ширина окна
	glfwGetWindowSize(window, &width, &height); // получение значений

	glViewport(0, 0, width, height); // корректировка точки зрения по полученным размерам окна

	float k = width / (float)height; // коэффициент отношения ширины к высоте
	float sz = 0.015; // модификатор угла обзора

	glLoadIdentity(); // замена матрицы на матрицу по умолчанию
	glFrustum(-k * sz, k * sz, -sz, sz, 0.01, 100); // установка перспективы
}
///<summary>
///расчет времени прогрузки кадра для корректной физики	и количесвто кадров в секунду
///</summary>
void CalculateFPS() 
{
	d_time = delta_time.Get();		// получение значения дельты времени
	fps = (1.0 / d_time) * 1000;	// кол-во кадров в секунду

	static int counter = 0;		// счётчик тиков
	counter++;					// инкрементация

	if (counter > 30)		// если тиков больше порога
	{
		lfps = (int)fps;	// установка вычисленного значения FPS
		ms = (int)d_time;	// установка вычисленного значения миллисекунд
		counter = 0;		// обнуление счётчика
	}
}
/// <summary>
/// обновление состояний
/// </summary>
void Update()
{
	client.Update(d_time);				// обновление клиента
	client.world->chunksUpdateing = 0;	// сброс кол-ва обновляющихся чанков
}
/// <summary>
/// рендеринг пользовательского интерфейса
/// </summary>
void RenderUI()
{
	// вывод текста отладки
	client.ui.PrintDebug("FPS: " + to_string(lfps) + " (" + to_string(ms) + " ms)", 1, 0, 0);
	client.ui.PrintDebug("Позиция: " + to_string((int)client.player.pos.x) + " " +
		std::to_string((int)client.player.pos.y) + " " + to_string((int)client.player.pos.z), 1, 1, 1);
	client.ui.PrintDebug("Обновляется чанков: " + to_string(client.world->chunksUpdateing), 1, 1, 1);
	client.ui.PrintDebug("Блоков в буфере: " + to_string(client.world->GlobalBuffer.size()), 1, 1, 1);
	client.ui.PrintDebug("Зерно генерации: " + to_string(World::seed), 1, 1, 1);
	client.ui.PrintDebug("Количество частиц " + to_string(client.particles.size()), 1, 1, 1);
	client.ui.PrintDebug("Рабочий каталог: " + World::name, 1, 1, 1);
	client.ui.PrintDebug("ID блока: " + to_string(client.world->GetBlockID(client.player.look_pos)), 1, 1, 1);
	client.ui.PrintDebug("[Esc] - Пауза", 0, 1, 1);
	
	client.RenderUI();
}
/// <summary>
/// точка входа
/// </summary>
/// <returns></returns>
int main()
{
	Debug debug;					// экземляр вывода сообщений для отладки 
	setlocale(LC_ALL, "Russian");	// русский язык в консоли

	debug.active = false;

	while (true) 
	{
		cout << "Введите имя рабочего каталога -> ";
		cin >> World::name;

		if (_mkdir("saves"))
			Debug::Log("directory \"saves\" already exists");
		if (_mkdir("saves/worlds"))
			Debug::Log("directory \"worlds\" already exists");
		if (_mkdir((save_folder + World::name + "/").c_str()))
			Debug::Log("directory \"" + World::name + "\" already exists");

		if (!glfwInit()) // если инициализация не успешна
			return -1;

		// создание окна GLFW
		window = glfwCreateWindow(SIZE_X, SIZE_Y, "Multiverse", NULL, NULL);
		client.window = window;

		if (!window) // если окно не создалось
		{
			glfwTerminate(); // остановить GLFW
		}

		glfwMakeContextCurrent(window);  //	установка контекста окна текущим для вызывающего потока
		glfwSetKeyCallback(window, key_callback); // включение событий клавиш
		glfwSetMouseButtonCallback(window, mouse_button_callback); // события кнопок мыши
		glfwSetScrollCallback(window, scroll_callback); // события колеса мыши

		GameInit(); // Инициализация игры

		// главный цикл программы, использующийся все рабочее время
		while (!glfwWindowShouldClose(window) && !client.close)
		{
			glPushMatrix(); // начало записи новой матрицы
				// режим рамок для отладки
			glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);

			Update(); // обновление состояний
			Render(); // отрисовка

			glPopMatrix();	// конец записи матрицы

			RenderUI();		// отрисовка пользовательского интерфейса

			glfwSwapBuffers(window);	// Обмен буферов рендеринга 
			glfwPollEvents();			// Проверка новых событий  

			CalculateFPS();				// рассчёт кол-ва кадров в секунду
		}

		client.Clear();	// очистка клиента

		system("cls"); // очистка консоли

		glfwTerminate(); //завершение работы
	}

	return 0;
}