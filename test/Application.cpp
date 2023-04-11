#include "Application.h"
#include <stb_image.h>
//💀
void Application::Run() {
	setlocale(LC_ALL, "Russian");

	Debug debug;
	Block::Init();
	
	while (true) {
		InputWorldName();

		if (!glfwInit()) {
			std::cout << "GLFW: произошла фатальная ошибка :skull:" << std::endl;
			continue;
		}

		OpenWindow();
		SetCallbacks();
		LoadResources();
		LoadIcon();
		GameInit();

		// главный цикл программы, использующийся все рабочее время
		while (!glfwWindowShouldClose(window) && !client.close) {
			RenderWorld();
			RenderUI();
			glfwSwapBuffers(window);
			glfwPollEvents();
			CalculateFPS();
		}

		StopWindow();
	}
}

void Application::StopWindow() {
	client.Clear();		// очистка клиента
	system("cls");		// очистка консоли
	glfwTerminate();	//завершение работы
}

void Application::OpenWindow() {
	window = glfwCreateWindow(SIZE_X, SIZE_Y, "Multiverse", NULL, NULL);
	client.window = window;

	if (!window) {
		std::cout << "GLFW: окно не запущено :skull:" << std::endl;
		glfwTerminate();
	}
}

void Application::RenderWorld() {
	glPushMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
	Update();
	Render();
	glPopMatrix();
}

void Application::TextureInit(unsigned int& texture, const char* name) {
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

void Application::GameInit() {
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

void Application::Render() {
	auto distance = World::render_distance * Chunk::ChunkSize;
	auto sky_color = client.world->sky->getSkyColor(World::is_day);
	float fog_color[4] = { sky_color.x, sky_color.y, sky_color.z, 1 }; // цвет неба

	Resize(); //корректировка содержимого под размер окна

	glEnable(GL_DEPTH_TEST);					// Включение глубины отрисовки
	glFogf(GL_FOG_START, distance * 0.5);		// Глубина, с которой начинается туман
	glFogf(GL_FOG_END, distance);				// Глубина, где туман заканчивается
	glFogf(GL_FOG_DENSITY, M_E / distance);		 // Насколько густым будет туман
	glClearDepth(2);
	glClearColor(sky_color.x, sky_color.y, sky_color.z, 1); // Цвет фона (небо)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Флаги очистки 
	glEnable(GL_TEXTURE_2D); // Использование 2D текстур

	glFogfv(GL_FOG_COLOR, fog_color);				// Устанавливаем цвет тумана
	client.CameraUpdate();							// Обновление камеры
	glEnable(GL_CULL_FACE);							// Куллинг поверхностей вкл
	glCullFace(GL_FRONT);							// Отключение отрисовки внутренних граней
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);	// Состояние текстурного массива
	glEnableClientState(GL_VERTEX_ARRAY);			// Состояние массива вершин
	glEnableClientState(GL_COLOR_ARRAY);			// Состояние массива цветов

	client.Render(); // вызов метода рендеринга в клиенте

	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);	// Выход из состояния отрисовки текстур
	glDisableClientState(GL_VERTEX_ARRAY);			// Отключение состояния массива вершин
	glDisableClientState(GL_COLOR_ARRAY);			// Отключение состояния массива вершин
	glDisable(GL_DEPTH_TEST);						// Выключение глубины отрисовки
}

void Application::Resize(int x, int y) {

	float k = x / static_cast<float>(y);	// Отношение сторон
	float sz = 0.01f;		// модификатор угла обзора

	glViewport(0, 0, x, y); // Точка обзора
	glLoadIdentity();		// замена матрицы на матрицу по умолчанию
	glFrustum(-k * sz, k * sz, -sz, sz, 0.01, 100); // перспектива
}

void Application::Resize() {
	int width, height; // высота и ширина окна
	glfwGetWindowSize(window, &width, &height); // получение значений

	glViewport(0, 0, width, height); // корректировка точки зрения по полученным размерам окна

	float k = width / (float)height; // коэффициент отношения ширины к высоте
	float sz = 0.015; // модификатор угла обзора

	glLoadIdentity(); // замена матрицы на матрицу по умолчанию
	glFrustum(-k * sz, k * sz, -sz, sz, 0.01, 100); // установка перспективы
}

void Application::CalculateFPS() {
	d_time = delta_time.Get();		// получение значения дельты времени
	fps = (1.0 / d_time) * 1000;	// кол-во кадров в секунду

	static int counter = 0;		// счётчик тиков
	counter++;					// инкрементация

	if (counter > 30) {
		lfps = static_cast<int>(fps);
		ms = static_cast<int>(d_time);
		counter = 0;
	}
}

void Application::Update() {
	client.Update(d_time);				// обновление клиента
	client.world->chunksUpdateing = 0;	// сброс кол-ва обновляющихся чанков
}

void Application::RenderUI() {
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

void Application::LoadIcon() {
	unsigned char* image;
	int ico_width, ico_height, ico_channels;
	GLFWimage images[1]{};

	image = stbi_load("logo.png", &ico_width, &ico_height, &ico_channels, 4);
	images[0].height = ico_height;
	images[0].width = ico_width;
	images[0].pixels = image;

	if (!images[0].pixels) return;

	glfwSetWindowIcon(window, 1, images);
}

void Application::LoadResources() {
	TextureInit(client.main_texture, "terrain.png");		// Инициализация текстур
	TextureInit(client.ui.font, "myfont.png");				// Инициализация шрифта
	TextureInit(client.Menu.font, "myfont.png");			// Инициализация шрифта
	TextureInit(client.inventory.ui.font, "myfont.png");	// Инициализация шрифта
}

void Application::SetCallbacks() {
	glfwMakeContextCurrent(window);								//	установка контекста окна текущим для вызывающего потока
	glfwSetKeyCallback(window, key_callback);					// включение событий клавиш
	glfwSetMouseButtonCallback(window, mouse_button_callback);	// события кнопок мыши
	glfwSetScrollCallback(window, scroll_callback);				// события колеса мыши
}

void Application::InputWorldName() {
	std::cout << "Введите имя рабочего каталога -> ";
	std::cin >> World::name;

	if (std::experimental::filesystem::create_directory("saves"))
		Debug::Log("directory \"saves\" already exists");
	if (std::experimental::filesystem::create_directory("saves/worlds"))
		Debug::Log("directory \"worlds\" already exists");
	if (std::experimental::filesystem::create_directory((save_folder + World::name + "/").c_str()))
		Debug::Log("directory \"" + World::name + "\" already exists");
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS) {
		if (button == app.client.key_config.destroy)	app.client.modifier->Destroy();
		if (button == app.client.key_config.place)		app.client.modifier->Place();
		if (button == app.client.key_config.copy)		app.client.CopyBlock();
	}
	if (action == GLFW_RELEASE) {
		if (button == app.client.key_config.destroy)	app.client.modifier->Destroy(false);
		if (button == app.client.key_config.place)		app.client.modifier->Place(false);
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if (yoffset > 0) // колесо мыши вверх
		app.client.ButtonUp();

	if (yoffset < 0) // колесо мыши вниз
		app.client.ButtonDown();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (app.client.player.can_move) {
			if (key == app.client.key_config.explode) {
				if (app.client.world->GetBlockID(app.client.player.look_pos) == Block::tnt->id) {
					app.client.world->Explode(app.client.player.look_pos);
				}
			}
			else if (key == app.client.key_config.left)		app.client.player.left.Move(true);
			else if (key == app.client.key_config.right)	app.client.player.right.Move(true);
			else if (key == app.client.key_config.forward)	app.client.player.forward.Move(true);
			else if (key == app.client.key_config.back)		app.client.player.back.Move(true);
			else if (key == app.client.key_config.shift)	app.client.player.down.Move(true);
			else if (key == app.client.key_config.jump)		app.client.player.up.Move(true);
			else if (key == app.client.key_config.sprint)	app.client.player.sprint = true;
			else if (key == app.client.key_config.fly)		app.client.player.SwitchFly();
		}

		if (key == app.client.key_config.up)			app.client.ButtonUp(); 
		else if (key == app.client.key_config.down)		app.client.ButtonDown(); 
		else if (key == app.client.key_config.enter)	app.client.enter = true; 
		else if (key == app.client.key_config.pause)	app.client.Pause(!app.client.pause); 
		else if (key == GLFW_KEY_F3)					app.wireframe = !app.wireframe;
		else if (key == GLFW_KEY_F4)					Debug::bounds = !Debug::bounds; 
	}
	else if (action == GLFW_RELEASE) {
		if (key == app.client.key_config.left)			app.client.player.left.Move(false);
		else if (key == app.client.key_config.right)	app.client.player.right.Move(false);
		else if (key == app.client.key_config.forward)	app.client.player.forward.Move(false);
		else if (key == app.client.key_config.back)		app.client.player.back.Move(false);
		else if (key == app.client.key_config.shift)	app.client.player.down.Move(false);
		else if (key == app.client.key_config.jump)		app.client.player.up.Move(false);
		else if (key == app.client.key_config.sprint)	app.client.player.sprint = false;
	}
}