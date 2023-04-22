#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include"Settings.h"
#include"World.h"
#include"Chunk.h"
#include"RigidBox.h"
#include"DeltaTime.h"

#include<vector>
#include<fstream>

class World;
class RigidBox;

class Player
{
private:
	int length;

	float walk_speed = 0.005f;
	float run_speed = 0.01f;
	float lift_speed = 0.01f;
	float fly_mult = 1.5f;
public:

	struct Camera {
		float x, y, z;
		float xRot, zRot;
	} camera = { 0, 0, head_height, 0, 0 };


	struct MovementDirection {
	private:
		bool move = false;
		float velocity = 0;
		float mult = 100;
	public:
		void Move(bool state = true) {
			move = state;
		}

		void Move(float a, float max_speed) {
			if (move)
				velocity += a;
			else
				velocity -= a * velocity * mult;

			if (velocity < 0)
				velocity = 0;

			if (velocity > max_speed)
				velocity = max_speed;
		}
		bool GetMove() {
			return move;
		}

		float GetVelocity() {
			return velocity;
		}
	};

	Player(World* world);
	Player();

	RigidBox* body;	
	DeltaTime delta_time;

	Vector3 look_pos;
	Vector3 add_pos;
	Vector3 pos;

	MovementDirection forward;	
	MovementDirection back;		
	MovementDirection right;	
	MovementDirection left;		
	MovementDirection up;		
	MovementDirection down;	

	bool looking = false;
	bool sprint = false;
	bool fly = false;	
	bool can_move = true;

	float rot, velocity, speed;
	
	float phasa = 0.f; 
	float head_height = 1.7f;
	float HeadBobMagnitude = 0.1f;
	float width = 0.8f;			
	float height = 1.8f;		
	float jump_speed = 0.015f;	
	float current_friction = 1.f;
	float acceleration = 0.0001f;

	double angle = 0; // угол

	void RotateCamera(float alpha, float beta);
	void MoveUpdate();
	void Update();
	void LoadTerrain(World* world);
	void GenereateSphere();
	void Save(std::ofstream& stream);
	void Init(float x, float y, float z, float xRot, float zRot); 
	void SwitchFly();

	float GetVelocity();
	float GetDistance(float x, float y, float z);

	Vector3 start_pos = { 0, 0, 64 };
	std::vector<Vector3Int*>sphere;	
	Vector3Int IntPosition;			
	Vector3Int ChunkPosition;		
	World* world;					
	Vector2 vel = Vector2(0, 0);	
};

