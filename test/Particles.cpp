#include "Particles.h"

Particles::Particles(Block const * block, World* world, Vector3 pos, int count) {
	this->world = world;	// указатель на мир
	this->pos = pos;		// позиция центра

	working = true;			// работает

	for (int i = 0; i < count; i++)
	{
		// -3/8  +3/8
		float x = (rand() / (float)RAND_MAX - .5f) * _magnitude;
		float y = (rand() / (float)RAND_MAX - .5f) * _magnitude;
		float z = (rand() / (float)RAND_MAX - .5f) * _magnitude;

		float t = rand() / (float)RAND_MAX * 1.5f;

		particles.push_back(new Particle(block, x, y, z, 0.1f + t, world, pos));
	}
}

void Particles::Clear() {
	for (auto p : particles) {
		delete p; 
	}
	particles.clear(); 
}

void Particles::Render(unsigned int texture) {
	GLfloat* vertices = &vertexData.Vertices[0];
	GLfloat* uvs = &uvData.UVs[0];
	GLfloat* colors = &vertexData.Colors[0];

	int size = particles.size();
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexCoordPointer(2, GL_FLOAT, 0, uvs);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glColorPointer(3, GL_FLOAT, 0, colors);

	for (int i = 0; i < size; i++)
	{
		if (particles[i]->is_dead) 
			continue;
		float alpha = particles[i]->alpha * M_1_PI * 180 - 90;
		float beta = -abs(particles[i]->beta * M_1_PI * 180) + 90;
		float gamma = abs(particles[i]->gamma * M_1_PI * 180) - 90;
		glTranslatef(particles[i]->my_pos.x, particles[i]->my_pos.y, particles[i]->my_pos.z);
		glPushMatrix();
		glRotatef(alpha, 1, 0, 0);
		glRotatef(beta, 0, 1, 0);
		glRotatef(gamma, 0, 0, 1);
		glDrawArrays(GL_QUADS, i*4, 4); // отрисвока частиц

		glPopMatrix(); 
		glTranslatef(-particles[i]->my_pos.x, -particles[i]->my_pos.y, -particles[i]->my_pos.z);
	}
}

void Particles::Update(double deltaTime) {
	vertexData.Clear();
	uvData.Clear();

	int size = particles.size();

	working = false; 

	for (Particle* p : particles) {
		if (!p->is_dead) {
			p->Move(deltaTime);
			working = true;
		}

		p->SetMesh(&vertexData, &uvData);
	}
}