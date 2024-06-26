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
#include "ChunkLoader.h"
#include "Chat.h"
#include "GameState.h"

struct Modifier;

class Client {
public:
	struct Page;
	static struct Settings {
		bool smooth_lighting = true;
		bool recursive_lighting = false;
	} settings;

	GLFWwindow* window{};
	World* world{};
	Page* page{};
	Modifier* modifier{};

	UI ui, menu;
	Vector2Int menu_pos;
	Chat chat;
	GameState game_state;

	Player player;
	Inventory inventory;
	Phantom phantom;

	ChunkLoader chunk_loader;
	World::Settings gen_params;
	KeyConfig key_config;

	std::vector<Particles*> particles;

	bool enter = false;
	bool close = false;

	double mouse_pos_x = 0, mouse_pos_y = 0;
	unsigned int main_texture = 0;

	Client();
	~Client() = default;

	[[nodiscard]] static auto instance() -> Client& {
		static Client client;
		return client;
	}

	Client(Client const&) noexcept = delete;
	Client(Client const&&) noexcept = delete;
	Client& operator=(Client const&) noexcept = delete;
	Client& operator=(Client&&) noexcept = delete;

	void Pause(bool active);
	void ToSettings();
	void ToMenu();
	void ToAgree();
	void ToNewWorld();
	void ToRenderDistance();
	void ToTimeSet();
	void ButtonUp();
	void ButtonDown();
	void ButtonRight();
	void ButtonLeft();
	void Escape();
	void OpenChat();

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
	void DrawChat();

	struct Button {
		std::string name;
		std::function<void()> func;
		Button() :name("?"), func([] {}) {}
		Button(std::string name, std::function<void()> func) :name(name), func(func) {}
		virtual void DoFunc() {
			func();
		}
	};

	Button btn_settings{ "���������", [&] {ToSettings(); } };			// ������ ��������
	Button btn_new_world{ "������� ����� ���", [&] {ToNewWorld(); } };	// ������ �������� ����
	Button btn_continue{ "���������� ����", [&] {Pause(false); } };		// ������ ����������� ����
	Button btn_close{ "����� �� ����", [&] {close = true; } };			// ������ ������ �� ����
	Button btn_render_distance{ "��������� ����������", [&] {ToRenderDistance(); } }; // ������ ��������� ����������
	Button btn_time{ "���������� �����", [&] {ToTimeSet(); } };
	Button btn_back{ "�����", [&] {ToMenu(); } };					// ������ �������� � ����
	Button btn_small{ "���������", [&] {SetRenderDistance(3); } };	// ������ ��������� ��������� ��������� ����������
	Button btn_medium{ "�������", [&] {SetRenderDistance(4); } };	// ������ ��������� ������� ��������� ����������
	Button btn_normal{ "����������", [&] {SetRenderDistance(5); } };// ������ ��������� ������� ��������� ����������
	Button btn_large{ "�������", [&] {SetRenderDistance(6); } };	// ������ ��������� ������� ��������� ����������
	Button btn_confirm{ "����������", [&] {NewWorld(); } };			// ������ �����������
	Button btn_cancel{ "��������", [&] {ToMenu(); } };				// ������ ������
	Button btn_create{ "�������", [&] {ToAgree(); } };				// ������ �������� ����

	Button btn_day{ "����", [&]{world->time.set(0);} };
	Button btn_sunrise{ "�����", [&]{world->time.set(1000);} };
	Button btn_night{ "����", [&]{world->time.set(1200);} };
	Button btn_sunset{ "�������", [&]{world->time.set(2200);} };

	struct Switcher : Button {
		std::string name;
		bool* value;

		Switcher(std::string name, bool* value) :Button{}, name(name), value(value) {
			Button::name = name + (*value ? " On" : " Off");
		}

		virtual void DoFunc() {
			*value = !(*value);
			Button::name = name + (*value ? " On" : " Off");
		}
	};

	struct LamdaSwitcher : Switcher {

		LamdaSwitcher(std::string name, bool* value, std::function<void()> func) :
			Switcher(name, value) {
			this->func = func;
		}

		virtual void DoFunc() override {
			*value = !(*value);
			Button::name = name + (*value ? " On" : " Off");
			func();
		}
	};

	Switcher swt_mountains{ "����", &gen_params.params[0] };	// ����������� ���
	Switcher swt_plains{ "�������", &gen_params.params[1] };	// ����������� ������
	Switcher swt_trees{ "�������", &gen_params.params[2] };		// ����������� ��������
	Switcher swt_grass{ "�����", &gen_params.params[3] };		// ����������� �����

	LamdaSwitcher swt_smooth_lighting{ "������� ���������", &settings.smooth_lighting, [&] {world->UpdateWorldLighting(); } };
	LamdaSwitcher swt_recursive_lighting{ "����������� ���������", &settings.recursive_lighting, [&] {world->UpdateWorldLighting(); } };

	struct Page {
		int count;		
		std::string name;
		Button** buttons;
		float* indents;	 
	};
	// �������� ����
	Page page_menu{ 4, "����", new Button * [] {&btn_settings,& btn_new_world,& btn_continue,& btn_close}, new float[] {0, 0, 0, 0} };
	Page page_settings{ 5, "���������", new Button * [] {&btn_render_distance,& btn_time,& swt_smooth_lighting, & swt_recursive_lighting,& btn_back}, new float[] {0, 0, 0, 0, 20} };
	Page page_render_distance{ 5, "��������� ����������", new Button * [] {&btn_small,& btn_medium,& btn_normal,& btn_large,& btn_back}, new float[] {0, 0, 0, 0, 20} };
	Page page_agree{ 2, "������� ��� ����� ������", new Button * [] {&btn_confirm,& btn_cancel}, new float[] {0,0} };
	Page page_new_world{ 6, "��������� ������ ����", new Button * [] {&swt_mountains,& swt_plains,& swt_trees,& swt_grass,& btn_create,& btn_back},new float[] {0, 0, 0, 0, 20, 0} };
	Page page_time{5, "���������� �����", new Button*[]{&btn_day, &btn_sunrise, &btn_night, &btn_sunset, &btn_back},new float[] {0, 0, 0, 0, 20} };
};