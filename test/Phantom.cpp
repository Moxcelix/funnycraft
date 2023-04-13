#include "Phantom.h"
/// <summary>
/// ��������� �������
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
/// <param name="world"></param>
void Phantom::Render(int x, int y, int z, World* world)
{
	// ������ ������
	up = world->GetBlockID(x, y, z + 1);
	down = world->GetBlockID(x, y, z - 1);
	right = world->GetBlockID(x + 1, y, z);
	left = world->GetBlockID(x - 1, y, z);
	front = world->GetBlockID(x, y + 1, z);
	back = world->GetBlockID(x, y - 1, z);

	const auto block = world->GetBlock(x, y, z);

	const auto w = block->GetBounds().width / 2.f;
	const auto l = block->GetBounds().length / 2.f;
	const auto h0 = .5f;
	const auto h1 = block->GetBounds().height - .5f;

	glBegin(GL_LINES); // ����� �����
	glColor3f(0, 0, 0);
	// �������
	glVertex3f(x - w - (left ? d : f), y + l + (front ? d : f), z + h1 + (up ? d : f));
	glVertex3f(x + w + (right ? d : f), y + l + (front ? d : f), z + h1 + (up ? d : f));
	glVertex3f(x + w + (right ? d : f), y + l + (front ? d : f), z + h1 + (up ? d : f));
	glVertex3f(x + w + (right ? d : f), y + l + (front ? d : f), z - h0 - (down ? d : f));
	glVertex3f(x + w + (right ? d : f), y + l + (front ? d : f), z - h0 - (down ? d : f));
	glVertex3f(x - w - (left ? d : f), y + l + (front ? d : f), z - h0 - (down ? d : f));
	glVertex3f(x - w - (left ? d : f), y + l + (front ? d : f), z - h0 - (down ? d : f));
	glVertex3f(x - w - (left ? d : f), y + l + (front ? d : f), z + h1 + (up ? d : f));

	glVertex3f(x - w - (left ? d : f), y - l -(back ? d : f), z + h1 + (up ? d : f));
	glVertex3f(x + w + (right ? d : f), y - l -(back ? d : f), z + h1 + (up ? d : f));
	glVertex3f(x + w + (right ? d : f), y - l -(back ? d : f), z + h1 + (up ? d : f));
	glVertex3f(x + w + (right ? d : f), y - l -(back ? d : f), z - h0 - (down ? d : f));
	glVertex3f(x + w + (right ? d : f), y - l -(back ? d : f), z - h0 - (down ? d : f));
	glVertex3f(x - w - (left ? d : f), y - l -(back ? d : f), z - h0 - (down ? d : f));
	glVertex3f(x - w - (left ? d : f), y - l -(back ? d : f), z - h0 - (down ? d : f));
	glVertex3f(x - w - (left ? d : f), y - l -(back ? d : f), z + h1 + (up ? d : f));

	glVertex3f(x - w - (left ? d : f), y + l + (front ? d : f), z + h1 + (up ? d : f));
	glVertex3f(x - w - (left ? d : f), y - l -(back ? d : f), z + h1 + (up ? d : f));

	glVertex3f(x + w + (right ? d : f), y + l + (front ? d : f), z + h1 + (up ? d : f));
	glVertex3f(x + w + (right ? d : f), y - l -(back ? d : f), z + h1 + (up ? d : f));

	glVertex3f(x - w - (left ? d : f), y + l + (front ? d : f), z - h0 - (down ? d : f));
	glVertex3f(x - w - (left ? d : f), y - l -(back ? d : f), z - h0 - (down ? d : f));

	glVertex3f(x + w + (right ? d : f), y + l + (front ? d : f), z - h0 - (down ? d : f));
	glVertex3f(x + w + (right ? d : f), y - l -(back ? d : f), z - h0 - (down ? d : f));

	glEnd();
}