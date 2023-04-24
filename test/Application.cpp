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

		while (!glfwWindowShouldClose(window) && !Client::instance().close) {
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
	Client::instance().Clear();
	system("cls");
	glfwTerminate();
}

void Application::OpenWindow() {
	window = glfwCreateWindow(SIZE_X, SIZE_Y, WIN_TITLE, NULL, NULL);
	Client::instance().window = window;

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
	Client::instance().Init();

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.5);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glHint(GL_FOG_HINT, GL_DONT_CARE);
}

void Application::Render() {
	auto distance = ChunkLoader::render_distance * Chunk::size;
	auto sky_color = Client::instance().world->sky.get_sky_color(
		Client::instance().world->time.get_time_normal() +
		Client::instance().world->time.get_accumulator_value());
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
	Client::instance().CameraUpdate();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	Client::instance().Render();

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
	Client::instance().Update(d_time);
}

void Application::RenderUI() {
	Client::instance().ui.PrintDebug("FPS: " + std::to_string(lfps) + " (" + std::to_string(ms) + " ms)", 1, 0, 0);
	Client::instance().ui.PrintDebug("Позиция: " + std::to_string((int)Client::instance().player.pos.x) + " " +
		std::to_string((int)Client::instance().player.pos.y) + " " + std::to_string((int)Client::instance().player.pos.z), 1, 1, 1);
	Client::instance().ui.PrintDebug("Обновляется чанков: " + std::to_string(Client::instance().world->chunks_updating), 1, 1, 1);
	Client::instance().ui.PrintDebug("Блоков в буфере: " + std::to_string(Client::instance().world->global_buffer.size()), 1, 1, 1);
	Client::instance().ui.PrintDebug("Зерно генерации: " + std::to_string(World::seed), 1, 1, 1);
	Client::instance().ui.PrintDebug("Количество частиц " + std::to_string(Client::instance().particles.size()), 1, 1, 1);
	Client::instance().ui.PrintDebug("Рабочий каталог: " + World::name, 1, 1, 1);
	Client::instance().ui.PrintDebug("ID блока: " + std::to_string(Client::instance().world->GetBlockID(Client::instance().player.look_pos)), 1, 1, 1);
	Client::instance().ui.PrintDebug("Время: " + std::to_string(Client::instance().world->time.get_time()), 1, 1, 1);
	Client::instance().ui.PrintDebug("[Esc] - Пауза", 0, 1, 1);

	Client::instance().RenderUI();
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
	TextureInit(Client::instance().main_texture, "resources/environment/terrain.png");
	TextureInit(Client::instance().ui.font, "resources/fonts/font.png");
	TextureInit(Client::instance().menu.font, "resources/fonts/font.png");
	TextureInit(Client::instance().inventory.ui.font, "resources/fonts/font.png");
	TextureInit(Client::instance().chat.ui.font, "resources/fonts/font.png");
}

void Application::SetCallbacks() {
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCharCallback(window, character_callback);
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
		if (button == Client::instance().key_config.destroy)	Client::instance().modifier->Destroy();
		if (button == Client::instance().key_config.place)		Client::instance().modifier->Place();
		if (button == Client::instance().key_config.copy)		Client::instance().CopyBlock();
	}
	if (action == GLFW_RELEASE) {
		if (button == Client::instance().key_config.destroy)	Client::instance().modifier->Destroy(false);
		if (button == Client::instance().key_config.place)		Client::instance().modifier->Place(false);
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if (yoffset > 0) Client::instance().ButtonUp();
	if (yoffset < 0) Client::instance().ButtonDown();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (Client::instance().player.can_move) {
			if (key == Client::instance().key_config.explode) {
				if (Client::instance().world->GetBlockID(Client::instance().player.look_pos) == Block::tnt->id) {
					Client::instance().world->Explode(Client::instance().player.look_pos);
				}
			}
			else if (key == Client::instance().key_config.left)		Client::instance().player.left.Move(true);
			else if (key == Client::instance().key_config.right)	Client::instance().player.right.Move(true);
			else if (key == Client::instance().key_config.forward)	Client::instance().player.forward.Move(true);
			else if (key == Client::instance().key_config.back)		Client::instance().player.back.Move(true);
			else if (key == Client::instance().key_config.shift)	Client::instance().player.down.Move(true);
			else if (key == Client::instance().key_config.jump)		Client::instance().player.up.Move(true);
			else if (key == Client::instance().key_config.sprint)	Client::instance().player.sprint = true;
			else if (key == Client::instance().key_config.fly)		Client::instance().player.SwitchFly();
		}

		if (key == Client::instance().key_config.up_arrow)			Client::instance().ButtonUp();
		else if (key == Client::instance().key_config.down_arrow)	Client::instance().ButtonDown();
		else if (key == Client::instance().key_config.left_arrow)	Client::instance().ButtonLeft();
		else if (key == Client::instance().key_config.right_arrow)	Client::instance().ButtonRight();
		else if (key == Client::instance().key_config.enter)		Client::instance().enter = true;
		else if (key == Client::instance().key_config.pause)		Client::instance().Escape();
		else if (key == Client::instance().key_config.chat)			Client::instance().OpenChat();
		else if (key == GLFW_KEY_F3)					app.wireframe = !app.wireframe;
		else if (key == GLFW_KEY_F4)					Debug::bounds = !Debug::bounds;
	}
	else if (action == GLFW_RELEASE) {
		if (key == Client::instance().key_config.left)			Client::instance().player.left.Move(false);
		else if (key == Client::instance().key_config.right)	Client::instance().player.right.Move(false);
		else if (key == Client::instance().key_config.forward)	Client::instance().player.forward.Move(false);
		else if (key == Client::instance().key_config.back)		Client::instance().player.back.Move(false);
		else if (key == Client::instance().key_config.shift)	Client::instance().player.down.Move(false);
		else if (key == Client::instance().key_config.jump)		Client::instance().player.up.Move(false);
		else if (key == Client::instance().key_config.sprint)	Client::instance().player.sprint = false;
	}

	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		if (key == GLFW_KEY_BACKSPACE)		Client::instance().chat.Backspace();
		else if (key == GLFW_KEY_DELETE)	Client::instance().chat.Delete();
		else if (key == GLFW_KEY_LEFT)		Client::instance().chat.Move(false);
		else if (key == GLFW_KEY_RIGHT)		Client::instance().chat.Move(true);
	}
}

void character_callback(GLFWwindow* window, unsigned int codepoint)
{
	Client::instance().chat.TraceKey(codepoint);
}