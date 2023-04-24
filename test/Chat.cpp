#include "Chat.h"
void Chat::TraceKey(unsigned int key) {
	if (!active) return;

	const auto symbol = static_cast<char>(key);

	if (symbol == '\n') {
		return;
	}

	if (temp_text.length() >= max_message_length) {
		return;
	}

	temp_text.insert(pos, 1, symbol);
	pos++;
}
void Chat::EnterInput() {
	if (!active) return;
}
void Chat::SendMessage() {

}
void Chat::Clear() {

}
void Chat::Render() {
	static float time = 0;
	static bool visible = true;
	const auto shift = temp_text.length() * font_size * 3;
	time += delta_time.Get();

	if (time > cursor_period) {
		time -= cursor_period;
		visible = !visible;
	}

	ui.PrintText(UI::Corner::middle, font_size, -width/2 + shift +
		tab, -(height - input_height) / 2, temp_text, 1, 1, 0);

	if (visible) {
		ui.PrintText(UI::Corner::middle, font_size, -width / 2 +
			pos * font_size * 6 + tab + 3 * font_size,
			-(height - input_height) / 2 - cursor_shift, "_", 1, 1, 0);
	}

	ui.Render();
}

void Chat::Backspace() {
	if (!active) return;

	if (pos == 0) {
		return;
	}
	auto str = temp_text.substr(0, pos - 1);

	if (pos < temp_text.length()) {
		auto suffix = temp_text.substr(pos, temp_text.size() - pos);
		str += suffix;
	}

	temp_text = str;
	pos--;
}
void Chat::Delete() {
	if (!active) return;

	if (pos == temp_text.length()) {
		return;
	}

	auto str = temp_text.substr(pos + 1, temp_text.size() - pos);

	if (pos > 0) {
		auto prefix = temp_text.substr(0, pos);
		str = prefix + str;
	}

	temp_text = str;
}

// true - right
// false - ledt
void Chat::Move(bool dir) {
	if (!active) return;

	if (dir && pos < temp_text.length()) {
		pos++;
	}

	if (!dir && pos > 0) {
		pos--;
	}
}