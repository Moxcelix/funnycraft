#define _USE_MATH_DEFINES
#include "Player.h"
#include <cmath>

Player::Player(){
	body = nullptr;
}

Player::~Player() {
	if (body) {
		delete body;
	}
}

Player::Player(World* world) : Player() {
	this->world = world;
}

void Player::GenereateSphere() {
	sphere.clear(); 
	int render_distance = World::render_distance - 1;

	for (int i = -render_distance; i <= render_distance; i++) {
		for (int j = -render_distance; j <= render_distance; j++) {
			for (int k = -render_distance; k <= render_distance; k++) {
				if (i * i + j * j + k * k < render_distance * render_distance) {
					sphere.push_back({ i, j, k });
				}
			}
		}
	}
	for (int i = 0; i < sphere.size(); i++) {
		for (int j = 0; j < sphere.size() - 1; j++) {
			if (sphere[j] > sphere[j + 1]) {
				std::swap(sphere[j], sphere[j + 1]);
			}
		}
	}

	length = sphere.size();
}

void Player::LoadTerrain(World* world) {
	if (world->chunks_loaded < World::MaxChunksCount - 1) {
		for (const auto i : sphere) {
			int x = this->pos.x + i.x * Chunk::size;
			int y = this->pos.y + i.y * Chunk::size;
			int z = this->pos.z + i.z * Chunk::size;

			world->AddToCreate(x, y, z);
		}
	}
}

float Player::GetDistance(float x, float y, float z) {
	x -= this->pos.x;
	y -= this->pos.y;
	z -= this->pos.z;

	return sqrtf(x * x + y * y + z * z);
}

void Player::MoveUpdate() {
	float time = delta_time.Get();

	speed = (sprint ? run_speed : walk_speed) * (fly ? fly_mult : 1);

	float mult = 100 * time;

	forward.Move(acceleration * time, speed);
	back.Move(acceleration * time, speed);
	right.Move(acceleration * time, speed);
	left.Move(acceleration * time, speed);
	up.Move(acceleration * time, speed);
	down.Move(acceleration * time, speed);

	float _forward = forward.GetVelocity();
	float _back = back.GetVelocity();
	float _right = right.GetVelocity();
	float _left = left.GetVelocity();
	float _up = up.GetVelocity();
	float _down = down.GetVelocity();

	Vector2 xvel = Vector2((_right - _left) * sin(rot + M_PI_2), (_right - _left) * cos(rot + M_PI_2));
	Vector2 yvel = Vector2((_forward - _back) * sin(rot), (_forward - _back) * cos(rot));
	Vector2 direction = xvel + yvel;

	float module = direction.Module();

	if (module > speed)
		direction = direction * (speed / module);

	phasa += direction.Module() * time;

	velocity = Vector3(direction.x, direction.y, _up - _down).Module(); // вектор полной скорости

	static float dx = direction.x * time;
	static float dy = direction.y * time;
	static float dz = 0;

	static float hv = 0;

	dx = direction.x * time;
	dy = direction.y * time;

	if (fly) {
		hv = 0;
		dz = (_up - _down) * time;
	}
	else {
		dz = 0;

		if (body->grounded)
			hv = up.GetMove() ? jump_speed : 0;
		else
			hv -= body->g * time;
	}

	if (body->pos.z < -10)
		body->pos.z = 128;

	body->Move(dx, dy, dz + hv * time);
	pos = body->pos;
}

float Player::GetVelocity() {
	return velocity;
}

void Player::Update() {
	int_position.x = pos.x;
	int_position.y = pos.y;
	int_position.z = pos.z;

	chunk_position.x = static_cast<int>(floor(pos.x / static_cast<double>(Chunk::size))) * Chunk::size;
	chunk_position.y = static_cast<int>(floor(pos.y / static_cast<double>(Chunk::size))) * Chunk::size;
	chunk_position.z = static_cast<int>(floor(pos.z / static_cast<double>(Chunk::size))) * Chunk::size;
}

void Player::RotateCamera(float alpha, float beta) {
	camera.zRot += beta;

	if (camera.zRot < 0)
		camera.zRot += 360;
	if (camera.zRot > 360)
		camera.zRot -= 360;

	camera.xRot += alpha;

	if (camera.xRot < 0)
		camera.xRot = 0;
	if (camera.xRot > 180)
		camera.xRot = 180;
}

void Player::Init(float x, float y, float z, float xRot, float zRot) {
	this->pos = { x, y, z };

	camera.xRot = xRot;
	camera.zRot = zRot;

	int flags = RB_CHECK_LOAD_STATE
		| RB_SPLIT_HEIGHT
		| RB_USE_COLLISION
		| RB_COLLISION_MAP;

	body = new RigidBox{ world, width, height, 9.8f * 0.05f, this->pos, flags };

	GenereateSphere();
}

void Player::Save(std::ofstream& stream) {
	stream << pos.x << std::endl;
	stream << pos.y << std::endl;
	stream << pos.z << std::endl;
	stream << camera.xRot << std::endl;
	stream << camera.zRot << std::endl;
}

void Player::SwitchFly() {
	fly = !fly;
}