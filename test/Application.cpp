#include "Application.h"
#include <stb_image.h>

#include "LightMap.h"
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
	client.Clear();
	system("cls");
	glfwTerminate();
}

void Application::OpenWindow() {
	window = glfwCreateWindow(SIZE_X, SIZE_Y, WIN_TITLE, NULL, NULL);
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
	int width, height, cnt; 
	unsigned char* data = stbi_load(name, &width, &height, &cnt, 0); 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
}

void Application::GameInit() {
	client.Init();

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.5);	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glHint(GL_FOG_HINT, GL_DONT_CARE);
}

void Application::Render() {
	auto distance = World::render_distance * Chunk::ChunkSize;
	auto sky_color = client.world->sky->get_sky_color(
		client.world->time.get_time_normal() +
		client.world->time.get_accumulator_value());
	float fog_color[4] = { sky_color.x, sky_color.y, sky_color.z, 1 };

	Resize();

	glEnable(GL_DEPTH_TEST);
	glFogf(GL_FOG_START, static_cast<GLfloat>(distance * 0.5f));
	glFogf(GL_FOG_END, static_cast<GLfloat>(distance));
	glFogf(GL_FOG_DENSITY, static_cast<GLfloat>(M_E / distance));
	glClearDepth(2);
	glClearColor(sky_color.x, sky_color.y, sky_color.z, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);

	glFogfv(GL_FOG_COLOR, fog_color);
	client.CameraUpdate();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	client.Render();

	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisable(GL_DEPTH_TEST);
}

void Application::Resize(int x, int y) {

	float k = x / static_cast<float>(y);
	float sz = 0.01f;

	glViewport(0, 0, x, y);
	glLoadIdentity();
	glFrustum(-k * sz, k * sz, -sz, sz, 0.01, 100);
}

void Application::Resize() {
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	glViewport(0, 0, width, height);

	float k = width / (float)height;
	float sz = 0.015f;

	glLoadIdentity();
	glFrustum(-k * sz, k * sz, -sz, sz, 0.01, 100);
}

void Application::CalculateFPS() {
	d_time = delta_time.Get();
	fps = (1.0 / d_time) * 1000;

	static int counter = 0;
	counter++;

	if (counter > 30) {
		lfps = static_cast<int>(fps);
		ms = static_cast<int>(d_time);
		counter = 0;
	}
}

void Application::Update() {
	client.Update(d_time);
	client.world->chunks_updating = 0;
}

void Application::RenderUI() {
	client.ui.PrintDebug("FPS: " + std::to_string(lfps) + " (" + std::to_string(ms) + " ms)", 1, 0, 0);
	client.ui.PrintDebug("Позиция: " + std::to_string((int)client.player.pos.x) + " " +
		std::to_string((int)client.player.pos.y) + " " + std::to_string((int)client.player.pos.z), 1, 1, 1);
	client.ui.PrintDebug("Обновляется чанков: " + std::to_string(client.world->chunks_updating), 1, 1, 1);
	client.ui.PrintDebug("Блоков в буфере: " + std::to_string(client.world->global_buffer.size()), 1, 1, 1);
	client.ui.PrintDebug("Зерно генерации: " + std::to_string(World::seed), 1, 1, 1);
	client.ui.PrintDebug("Количество частиц " + std::to_string(client.particles.size()), 1, 1, 1);
	client.ui.PrintDebug("Рабочий каталог: " + World::name, 1, 1, 1);
	client.ui.PrintDebug("ID блока: " + std::to_string(client.world->GetBlockID(client.player.look_pos)), 1, 1, 1);
	client.ui.PrintDebug("Время: " + std::to_string(client.world->time.get_time()), 1, 1, 1);
	client.ui.PrintDebug("[Esc] - Пауза", 0, 1, 1);

	client.RenderUI();
}

void Application::LoadIcon() {
	unsigned char* image;
	int ico_width, ico_height, ico_channels;
	GLFWimage images[1]{};

	image = stbi_load("resources/icon/logo.png", &ico_width, &ico_height, &ico_channels, 4);
	images[0].height = ico_height;
	images[0].width = ico_width;
	images[0].pixels = image;

	if (!images[0].pixels) return;

	glfwSetWindowIcon(window, 1, images);
}

void Application::LoadResources() {
	TextureInit(client.main_texture, "resources/environment/terrain.png");
	TextureInit(client.ui.font, "resources/fonts/font.png");
	TextureInit(client.menu.font, "resources/fonts/font.png");
	TextureInit(client.inventory.ui.font, "resources/fonts/font.png");

}

void Application::SetCallbacks() {
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);
}

void Application::InputWorldName() {
	std::cout << "Введите имя рабочего каталога -> ";
	std::cin >> World::name;

	if (std::filesystem::create_directory("saves"))
		Debug::Log("directory \"saves\" already exists");
	if (std::filesystem::create_directory("saves/worlds"))
		Debug::Log("directory \"worlds\" already exists");
	if (std::filesystem::create_directory((save_folder + World::name + "/").c_str()))
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
	if (yoffset > 0) app.client.ButtonUp();
	if (yoffset < 0) app.client.ButtonDown();
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

		if (key == app.client.key_config.up_arrow)			app.client.ButtonUp(); 
		else if (key == app.client.key_config.down_arrow)	app.client.ButtonDown(); 
		else if (key == app.client.key_config.left_arrow)	app.client.ButtonLeft();
		else if (key == app.client.key_config.right_arrow)	app.client.ButtonRight();
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