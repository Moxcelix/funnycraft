#include "Phantom.h"
/// <summary>
/// отрисовка фантома
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
/// <param name="world"></param>
void Phantom::Render(int x, int y, int z, World* world)
{
	// расчёт флагов
	up = world->GetBlockID(x, y, z + 1);
	down = world->GetBlockID(x, y, z - 1);
	right = world->GetBlockID(x + 1, y, z);
	left = world->GetBlockID(x - 1, y, z);
	front = world->GetBlockID(x, y + 1, z);
	back = world->GetBlockID(x, y - 1, z);

	Block* block = world->GetBlock(x, y, z);

	float w = block->GetBounds().width / 2.f;
	float l = block->GetBounds().length / 2.f;
	float h0 = .5f;
	float h1 = block->GetBounds().height - .5f;

	glBegin(GL_LINES); // режим линий
	glColor3f(0, 0, 0);
	// вершины
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