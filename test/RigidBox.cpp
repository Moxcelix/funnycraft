#include "RigidBox.h"

std::vector<Vector3Int> RigidBox::positions{};


RigidBox::RigidBox() {
	RigidBox(nullptr, 0, 0);
}


RigidBox::RigidBox(World* world, float width,
	float height, float g, Vector3 pos, int flags){
	this->world = world;	
	this->width = width;	
	this->height = height;	
	this->g = g * phys_mod;	
	this->pos = pos;		
	this->flags = flags;	

	w = width / 2.;	
	h0 = height / 2.;
	h1 = height / 2.;

	if (flags & RB_SPLIT_HEIGHT) {
		h0 = 0.5f;
		h1 = height - h0;
	}
}

void RigidBox::Move(float dx, float dy, float dz) {
	grounded = false;

	if (flags & RB_CHECK_LOAD_STATE) 
		if (!Readiness(pos.x + dx, pos.y + dy, pos.z + dz))
			return;

	pos.x += dx;		
	Collision(dx, 0, 0);
	pos.y += dy;		
	Collision(0, dy, 0);
	pos.z += dz;		
	Collision(0, 0, dz);

	if (flags & RB_COLLISION_MAP)
		SetCollisionMap();
}

void RigidBox::SetCollisionMap() {
	float xn = floor(pos.x - w + half);	
	float xp = (pos.x + w + salf);		
	float yn = floor(pos.y - w + half);	
	float yp = (pos.y + w + salf);		
	float zn = floor(pos.z - h0 + half);
	float zp = (pos.z + h1 + salf);		

	for (int X = xn; X < xp; X++) {
		for (int Y = yn; Y < yp; Y++) {
			for (int Z = zn; Z < zp; Z++) {
				AddPos({ X, Y, Z });
			}
		}
	}
}

void RigidBox::Collision(float dx, float dy, float dz)
{
	float xn = floor(pos.x - w + half);	
	float xp = (pos.x + w + salf);		
	float yn = floor(pos.y - w + half);	
	float yp = (pos.y + w + salf);		
	float zn = floor(pos.z - h0 + half);
	float zp = (pos.z + h1 + salf);		

	for (int X = xn; X < xp; X++) {
		for (int Y = yn; Y < yp; Y++) {
			for (int Z = zn; Z < zp; Z++)
			{
				if (world->GetBlock(X, Y, Z)->Solid())
				{
					if (dx > 0) pos.x = X - w - half;
					if (dx < 0) pos.x = X + w + half;
					if (dy > 0) pos.y = Y - w - half;
					if (dy < 0) pos.y = Y + w + half;
					if (dz > 0) pos.z = Z - h1 - half;
					if (dz < 0) {
						pos.z = Z + h0 + half;
						grounded = true;
					}
				}
			}
		}
	}
}

void RigidBox::AddPos(Vector3Int pos) {
	for (auto p : positions) {
		if (p == pos)
			return;
	}

	positions.push_back(pos);
}

bool RigidBox::Readiness(int x, int y, int z) {
	if (world) {
		Chunk* c = world->GetChunk(x, y, z); 
		return c || z < 0 || z >= Chunk::ChunkSize * World::WorldHeight;
	}
	return false;
}

void RigidBox::ClearCollisionMap() {
	positions.clear();
}

bool RigidBox::IsLocked(Vector3 pos) {
	return IsLocked(Vector3Int{ (int)pos.x, (int)pos.y, (int)pos.z });
}

bool RigidBox::IsLocked(Vector3Int pos) {
	for (auto p : positions) {
		if (p == pos) 
			return true; 
	}

	return false;
}

void RigidBox::Render() {
	if (Debug::bounds)
		Debug::DrawBox({ pos.x, pos.y, pos.z - h0 + (h1 + h0) * .5f }, { w,w,(h1 + h0) * .5f }, { 0,1,1 });
}