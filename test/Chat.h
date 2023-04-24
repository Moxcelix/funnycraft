#pragma once
#include "UI.h"
#include "Vector3.h"
#include "DeltaTime.h"
#include <string>
#include <vector>
#include <algorithm>

struct Message {
	std::string text;
	Vector3 color;
};

struct Record {
	std::vector<Message> messages;
};

class Chat
{
private:
	constexpr static int max_messages_count = 5;
	constexpr static int max_message_length = 52;
	constexpr static int font_size = 2;
	constexpr static int width = 640;
	constexpr static int height = 480;
	constexpr static int input_height = 30;
	constexpr static int tab = font_size * 3;
	constexpr static int cursor_shift = 6;
	constexpr static int cursor_period = 500;

	Vector3 box_color = { 0.f, 0.f, 0.7f };
	Vector3 field_color = { 0.f, 0.f, 0.5f };

	size_t pos;
	std::vector<Record> records;
	std::string temp_text;

	DeltaTime delta_time;
public:
	UI ui;
	bool active;

	Chat() {
		ui = {};
		records = {};
		active = false;
		pos = 0;
		temp_text = "";

		ui.AddStaticPlane(UI::Corner::middle, 1, 0, 0,
			width, height - input_height, box_color);
		ui.AddStaticPlane(UI::Corner::middle, 1, 0,
			(height - input_height)/2, width, 
			input_height, field_color);
	}

	void TraceKey(unsigned int);
	void EnterInput();
	void SendMessage();
	void Clear();
	void Render();
	void Backspace();
	void Delete();
	void Move(bool);
};

