#pragma once
#include "UI.h"
#include "Vector3.h"
#include <string>
#include <vector>

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

	UI ui;
	bool active;
	std::vector<Record> records;
	std::string temp_text;
public:
	Chat() {
		ui = {};
		records = {};
		active = false;
	}

	void TraceKey(unsigned int key);
	void EnterInput();
	void SendMessage();
	void Clear();
	void Render();
};

