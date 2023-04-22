#include "Debug.h"
std::ofstream Debug::log = std::ofstream();
bool Debug::active = false;
bool Debug::bounds = false;
/// <summary>
/// ����� ��������� � ����
/// </summary>
/// <param name="text"></param>
void Debug::Log(std::string text)
{
	if (active) // ���� �������
		log << text << std::endl;
}
/// <summary>
/// ��������� �����
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
/// �����������
/// </summary>
Debug::Debug()
{
	log.open(file_name); // ������� ����� ������
}
/// <summary>
/// ����������
/// </summary>
Debug::~Debug()
{
	if (active) // ���� �������
		log << "successfully closed" << std::endl;
	log.close(); // ������� �����
}