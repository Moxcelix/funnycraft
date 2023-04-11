#include "UI.h"
void UI::PrintText(Text t)
{
	int l = t.text.length();
	t.ID = IDCounter;
	t.vertices = new float[l * 12];
	t.colors = new float[l * 12];

	t.uvs_data = new UVData();

	t.uvs_data->xUVSize = t.width;
	t.uvs_data->yUVSize = t.height;

	float q = winHeight / float(winWidth);

	Vector2 gab = GetCorner(t.corner, t.iwidth * l * t.size, t.iheight * t.size, 4, 4);

	t.size *= 2. / winHeight;

	float xs = 2 * t.x / float(winWidth);
	float ys = 2 * t.y / float(winHeight);

	const char* s = t.text.c_str();
	for (int i = 0; i < l; i++)
	{
		float shift = i * t.iwidth;

		for (int j = 0; j < 4; j++)
		{
			t.vertices[i * 12 + 3 * j] = (t.quad[j * 3] + shift) * t.size * q + gab.x + xs;
			t.vertices[i * 12 + 3 * j + 1] = t.quad[j * 3 + 1] * t.size + gab.y - ys;
			t.vertices[i * 12 + 3 * j + 2] = 1;
		}
		for (int j = 0; j < 4; j++)
		{
			t.colors[i * 12 + 3 * j] = t.r;
			t.colors[i * 12 + 3 * j + 1] = t.g;
			t.colors[i * 12 + 3 * j + 2] = t.b;
		}
		int k;
		if ('А' <= s[i] && s[i] <= 'Я')
			k = s[i] - 'А' + 5 * t.wCount;
		else if ('а' <= s[i] && s[i] <= 'я')
			k = s[i] - 'а' + 6 * t.wCount + 11;
		else
			k = s[i] - '!';

		if (s[i] == ' ')
			t.uvs_data->AddUV((int)0, 0);
		else
			t.uvs_data->AddUV(int(k % t.wCount), 15 - int(k / t.wCount));
	}
	t.uvs = &t.uvs_data->UVs[0];
	IDCounter++;
	texts.push_back(t);
}
/// <summary>
/// Вывод на экран панели
/// </summary>
/// <param name="p"></param>
void UI::PrintPlane(Plane p)
{
	float q = winHeight / float(winWidth);

	p.vertices = new float[12];
	p.colors = new float[12];

	Vector2 gab = GetCorner(p.corner, p.w * p.size, p.h * p.size, 4, 4);

	p.size *= 2. / winHeight;

	float xs = 2 * p.x / float(winWidth);
	float ys = 2 * p.y / float(winHeight);

	for (int j = 0; j < 4; j++)
	{
		p.vertices[3 * j] = p.rect[j * 3] * p.size * p.w * q + gab.x + xs;
		p.vertices[3 * j + 1] = p.rect[j * 3 + 1] * p.size * p.h + gab.y - ys;
		p.vertices[3 * j + 2] = 1;
	}
	for (int j = 0; j < 4; j++)
	{
		p.colors[3 * j] = p.squad.colors[j].x;
		p.colors[3 * j + 1] = p.squad.colors[j].y;
		p.colors[3 * j + 2] = p.squad.colors[j].z;
	}
	planes.push_back(p);
}
/// <summary>
/// Вывод текста на экран
/// </summary>
/// <param name="corner"></param>
/// <param name="size"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="str"></param>
/// <param name="r"></param>
/// <param name="g"></param>
/// <param name="b"></param>
void UI::PrintText(Corner corner, float size, int x, int y, string str, float r, float g, float b)
{
	Text t{ x, -y, size, r, g, b, str, corner };
	PrintText(t);
	IDCounter++;
}

/// <summary>
/// Угол привязки 
/// </summary>
/// <param name="corner"></param>
/// <param name="width"></param>
/// <param name="height"></param>
/// <param name="xShift"></param>
/// <param name="yShift"></param>
/// <returns>положение относительно угла привязки</returns>
Vector2 UI::GetCorner(Corner corner, float width, float height, int xShift, int yShift)
{
	float xs = 2 * xShift / float(winWidth);
	float ys = 2 * yShift / float(winHeight);
	height *= 2 / float(winHeight);
	width *= 2 / float(winWidth);

	switch (corner)
	{
	case Corner::left_up:
		return Vector2(-1 + xs, 1 - height - ys);
	case Corner::left:
		return Vector2(-1 + xs, 0);
	case Corner::left_down:
		return Vector2(-1 + xs, -1 + ys);
	case Corner::up:
		return Vector2(0, 1 - height - ys);
	case Corner::middle:
		return Vector2(-width * 0.5, -height * 0.5);
	case Corner::down:
		return Vector2(0, -1 + ys);
	case Corner::right_up:
		return Vector2(1 - xs - width, 1 - height - ys);
	case Corner::right:
		return Vector2(1 - xs - width, 0);
	case Corner::right_down:
		return Vector2(1 - xs - width, -1 + ys);
	}
}
/// <summary>
/// Вывод на экран отладочного текста
/// </summary>
/// <param name="str"></param>
/// <param name="r"></param>
/// <param name="g"></param>
/// <param name="b"></param>
void UI::PrintDebug(string str, float r, float g, float b)
{
	const int step = 20;
	PrintText(Corner::left_up, 2, 0, -step * debug_line_counter, str, r, g, b);
	debug_line_counter++;
}
/// <summary>
/// Рендеринг элементов
/// </summary>
void UI::Render()
{
	PrintStatic();
	glPushMatrix();
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	// рендеринг панелей
	for (int i = 0; i < planes.size(); i++)
	{
		glVertexPointer(3, GL_FLOAT, 0, planes[i].vertices);
		glColorPointer(3, GL_FLOAT, 0, planes[i].colors);

		glDrawArrays(GL_QUADS, 0, planes.size() * 4);
	}

	// рендеринг букв
	glBindTexture(GL_TEXTURE_2D, font);
	for (int i = 0; i < texts.size(); i++)
	{
		glTexCoordPointer(2, GL_FLOAT, 0, texts[i].uvs);
		glVertexPointer(3, GL_FLOAT, 0, texts[i].vertices);
		glColorPointer(3, GL_FLOAT, 0, texts[i].colors);

		glDrawArrays(GL_QUADS, 0, texts[i].text.length() * 4);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glPopMatrix();

	if(flags & UI_DRAW_CROSS)
		cross.Draw(winWidth, winHeight);
	//bg = BackGround(1, 1, 1, 0.5);
	//bg.Draw(winWidth, winHeight);

	for (const Text& t : texts)
	{
		delete t.uvs_data;
		delete[] t.colors;
		delete[] t.vertices;
	}

	for (const Plane& t : planes)
	{
		delete[] t.colors;
		delete[] t.vertices;
	}

	texts.clear();
	planes.clear();

	debug_line_counter = 0;
}
/// <summary>
/// Печать статичного текста
/// </summary>
void UI::PrintStatic()
{
	float q = winHeight / float(winWidth);
	for (const Plane& p : static_planes)
	{
		PrintPlane(p);
	}
	for (const Text& t : static_texts)
	{
		PrintText(t);
	}
}
/// <summary>
/// Добавление статичного текста
/// </summary>
/// <param name="corner"></param>
/// <param name="size"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="str"></param>
/// <param name="r"></param>
/// <param name="g"></param>
/// <param name="b"></param>
/// <returns>объект текста</returns>
Text UI::AddStaticText(Corner corner, float size, int x, int y, string str, float r, float g, float b)
{
	Text t{ x, -y, size, r, g, b, str , corner };
	t.ID = IDCounter;
	static_texts.push_back(t);
	IDCounter++;
	return t;
}
/// <summary>
/// Удаление статичных текстов
/// </summary>
void UI::RemoveStaticText()
{
	static_texts.clear();
}
/// <summary>
/// Удаление статичного тектса
/// </summary>
/// <param name="t"></param>
void UI::RemoveStaticText(Text t)
{
	int j = -1;
	for (int i = 0; i < static_texts.size(); i++)
		if (t == static_texts[i])
		{
			j = i;
			break;
		}
	if (j >= 0)
		static_texts.erase(static_texts.begin() + j);
}
/// <summary>
/// Добавление статичной панели
/// </summary>
/// <param name="corner"></param>
/// <param name="size"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="w"></param>
/// <param name="h"></param>
/// <param name="colors"></param>
/// <returns>объект панели</returns>
Plane UI::AddStaticPlane(Corner corner, float size, int x, int y, int w, int h, ColorSquad colors)
{
	Plane p{ x, y, w, h, size, colors , corner };
	p.ID = IDCounter;
	static_planes.push_back(p);
	IDCounter++;
	return p;
}

/// <summary>
/// Установка размеров окна
/// </summary>
/// <param name="w"></param>
/// <param name="h"></param>
void UI::SetSize(int w, int h)
{
	winHeight = h;
	winWidth = w;
}
/// <summary>
/// утсановка флагов
/// </summary>
/// <param name="flags"></param>
void UI::SetFlags(int flags)
{
	this->flags = flags;
}
/// <summary>
/// Очистка
/// </summary>
void UI::Clear()
{
	texts.clear();				// вектор текстов
	static_texts.clear();		// вектор статичных текстов
	static_planes.clear();		// вектор статичных панелей
	planes.clear();				// вектор панелей
}