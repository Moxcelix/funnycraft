#include "Debug.h"
ofstream Debug::log = ofstream();
bool Debug::active = true;
bool Debug::bounds = false;
/// <summary>
/// вывод сообщения в файл
/// </summary>
/// <param name="text"></param>
void Debug::Log(string text)
{
	if (active) // если активен
		log << text << endl;
}
/// <summary>
/// отрисовка рамок
/// </summary>
/// <param name="pos"></param>
/// <param name="dim"></param>
void Debug::DrawBox(Vector3 pos, Vector3 dim, Vector3 color)
{
	glPushMatrix();
	glBegin(GL_LINES);
	glColor3f(color.x, color.y, color.z);

	glVertex3f(pos.x - dim.x, pos.y + dim.y, pos.z + dim.z);
	glVertex3f(pos.x + dim.x, pos.y + dim.y, pos.z + dim.z);
	glVertex3f(pos.x + dim.x, pos.y + dim.y, pos.z + dim.z);
	glVertex3f(pos.x + dim.x, pos.y + dim.y, pos.z - dim.z);
	glVertex3f(pos.x + dim.x, pos.y + dim.y, pos.z - dim.z);
	glVertex3f(pos.x - dim.x, pos.y + dim.y, pos.z - dim.z);
	glVertex3f(pos.x - dim.x, pos.y + dim.y, pos.z - dim.z);
	glVertex3f(pos.x - dim.x, pos.y + dim.y, pos.z + dim.z);
	glVertex3f(pos.x - dim.x, pos.y - dim.y, pos.z + dim.z);
	glVertex3f(pos.x + dim.x, pos.y - dim.y, pos.z + dim.z);
	glVertex3f(pos.x + dim.x, pos.y - dim.y, pos.z + dim.z);
	glVertex3f(pos.x + dim.x, pos.y - dim.y, pos.z - dim.z);
	glVertex3f(pos.x + dim.x, pos.y - dim.y, pos.z - dim.z);
	glVertex3f(pos.x - dim.x, pos.y - dim.y, pos.z - dim.z);
	glVertex3f(pos.x - dim.x, pos.y - dim.y, pos.z - dim.z);
	glVertex3f(pos.x - dim.x, pos.y - dim.y, pos.z + dim.z);
	glVertex3f(pos.x - dim.x, pos.y + dim.y, pos.z + dim.z);
	glVertex3f(pos.x - dim.x, pos.y - dim.y, pos.z + dim.z);
	glVertex3f(pos.x + dim.x, pos.y + dim.y, pos.z + dim.z);
	glVertex3f(pos.x + dim.x, pos.y - dim.y, pos.z + dim.z);
	glVertex3f(pos.x - dim.x, pos.y + dim.y, pos.z - dim.z);
	glVertex3f(pos.x - dim.x, pos.y - dim.y, pos.z - dim.z);
	glVertex3f(pos.x + dim.x, pos.y + dim.y, pos.z - dim.z);
	glVertex3f(pos.x + dim.x, pos.y - dim.y, pos.z - dim.z);

	glEnd();
	glPopMatrix();
}
/// <summary>
/// конструктор
/// </summary>
Debug::Debug()
{
	log.open(file_name); // открыть поток записи
}
/// <summary>
/// деструктор
/// </summary>
Debug::~Debug()
{
	if (active) // если активен
		log << "successfully closed" << endl;
	log.close(); // закрыть поток
}