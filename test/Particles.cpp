#include "Particles.h"
/// <summary>
/// конструктор
/// </summary>
/// <param name="block"></param>
/// <param name="world"></param>
/// <param name="pos"></param>
/// <param name="count"></param>
Particles::Particles(Block* block, World* world, Vector3 pos, int count)
{
	this->world = world;	// указатель на мир
	this->pos = pos;		// позиция центра

	working = true;			// работает

	for (int i = 0; i < count; i++)
	{
		// случаные координаты в диапозоне от -3/8 до +3/8
		float x = (rand() / (float)RAND_MAX - .5f) * _magnitude;
		float y = (rand() / (float)RAND_MAX - .5f) * _magnitude;
		float z = (rand() / (float)RAND_MAX - .5f) * _magnitude;

		float t = rand() / (float)RAND_MAX * 1.5f; // время жизни частицы
		// добавление частицы в вектор
		particles.push_back(new Particle(block, x, y, z, 0.1f + t, world, pos));
	}
}
/// <summary>
/// очисктка данных
/// </summary>
void Particles::Clear()
{
	for (auto p : particles) // для каждой частицы
	{
		delete p; // удалить частицу
	}
	particles.clear(); // очистить вектор
}
/// <summary>
/// рендеринг
/// </summary>
void Particles::Render(unsigned int texture)
{
	/// конвертирование векторов в массивы
	GLfloat* vertices = vertexData.ToArray();
	float* uvs = uvData.ToArray();
	float* colors = vertexData.ColorsToArray();

	int size = particles.size(); // кол-во систем частиц
	glBindTexture(GL_TEXTURE_2D, texture);
	// установка указателей рендеринга
	glTexCoordPointer(2, GL_FLOAT, 0, uvs);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glColorPointer(3, GL_FLOAT, 0, colors);

	for (int i = 0; i < size; i++)
	{
		if (particles[i]->is_dead) // если частица умерла, пропускаем итерацию
			continue;
		// углы поворота частицы
		float alpha = particles[i]->alpha * M_1_PI * 180 - 90;
		float beta = -abs(particles[i]->beta * M_1_PI * 180) + 90;
		float gamma = abs(particles[i]->gamma * M_1_PI * 180) - 90;
		// перевод координат в точку
		glTranslatef(particles[i]->my_pos.x, particles[i]->my_pos.y, particles[i]->my_pos.z);
		// запись новой матрицы
		glPushMatrix();
		// поворот на углы
		glRotatef(alpha, 1, 0, 0);
		glRotatef(beta, 0, 1, 0);
		glRotatef(gamma, 0, 0, 1);
		
		glDrawArrays(GL_QUADS, i*4, 4); // отрисвока частиц

		glPopMatrix(); // конец записи новой матрицы
		// возвращение точки отсчёта
		glTranslatef(-particles[i]->my_pos.x, -particles[i]->my_pos.y, -particles[i]->my_pos.z);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	// удаление массивов
	delete[] vertices;
	delete[] uvs;
	delete[] colors;
}
/// <summary>
/// обновление
/// </summary>
/// <param name="deltaTime"></param>
void Particles::Update(double deltaTime)
{
	// очистка данных
	vertexData.Clear();
	uvData.Clear();

	int size = particles.size(); // кол-во систем частиц

	working = false; // флаг работы

	for (Particle* p : particles) // для каждой частицы
	{
		if (!p->is_dead) // если не мертва
		{
			p->Move(deltaTime); // движение частицы
			working = true;		// поднять флаг работы
		}

		p->SetMesh(&vertexData, &uvData); // расчёт меша
	}
}