#pragma once

#include<queue>
#include<stack>
#include <thread>

#include "Terrain.h"
#include "Player.h"
#include "Debug.h"
#include "PosQueue.h"
#include "Sky.h"

class Terrain;
class Player;
struct Chunk;
/// <summary>
/// ����� ����
/// </summary>
class World
{
public:	
	static struct Settings 
	{
		const static int count = 4;
		bool params[count]{true, true, true, true};

		void Clear() 
		{
			for (int i = 0; i < count; i++)
				params[i] = true;
		}
	}settings;

	struct BlockPos
	{
		int x, y, z;
		block_id block;
	};

	static bool is_day; // ���� ���
	const static int MaxChunksCount = 512 * 8; // ������������ ���������� ������
	const static int WorldHeight = 16; // ������������ ������ ����

	PosQueue *render_queue; // ������� ����������� ����������
	PosQueue *update_queue; // ������� ����������
	PosQueue *create_queue; // ������� �� ��������
	Player* player; // ��������� �� ������
	Chunk* Chunks[MaxChunksCount]; // ������ ������
	Sky* sky;

	std::queue<Vector3Int> *global_update_queue; // ������� �� ���������� ����������
	std::vector<BlockPos> GlobalBuffer; // ���������� ����� ������
	std::vector<int> toRemove; // ������� ������� ������ �� ��������

	inline Chunk* GetChunk(int x, int y, int z); // �������� ����

	static int seed; // ����� ���������
	static int render_distance; // ��������� ����������
	static std::string name; // ��� ����

	int chunksUpdateing = 0; // ����������� ������������� ������ 
	int chunks_loaded = 0; // ���������� ����������� ������
	 
	Block const* GetBlock(int x, int y, int z); // ��������� �� ����
	Block const* GetBlock(Vector3 pos); // ��������� �� ����

	block_id GetBlockID(int x, int y, int z); // ID �����
	block_id GetBlockID(Vector3 pos); // ID �����

	bool CreateChunk(int x, int y, int z); // �������� �����
	void UpdateIfExcist(int x, int y, int z); // �������� ����, ���� ����������
	void RenderIfExcist(int x, int y, int z); // ���������� ����, ���� ����������
	void AddToRender(int x, int y, int z); // �������� � ������� ��������� ���������
	void AddToUpdate(int x, int y, int z); // �������� � ������� ����������
	void AddToCreate(int x, int y, int z); // ���������� ����� � ������� ��������
	void AddToRender(Chunk* chunk); // �������� ���� � ������� ��������� ���������
	void AddToUpdate(Chunk* chunk); // �������� ���� � ������� ���������� 
	void Update(); // ����������
	void UpdateWorldLighting(); // ���������� ����������� ���������
	void Generate(Chunk* chunk); // ��������� �����
	void SetBlock(int x, int y, int z, block_id block, bool update = false); // ��������� �����
	void SetBlock(Vector3 pos, block_id block, bool update = false); // ��������� �����
	void Render(unsigned int texture); // ������ ����
	void Clear(); // ������� ����
	void Create(); // �������� ����
	void DeleteChunk(int index); // �������� �����
	void RationalizeBuffer(); // �������������� ������ ������
	void RemoveCash(); // �������� ���� ������
	void UpdateAtPoint(int x, int y, int z); // �������� ����� � �����
	void AddToUpdateAtPoint(int x, int y, int z); // �������� � ������� ���������� � �����
	void UpdateIfEqual(int value1, int value2, int x, int y, int z); // ���������� ��� �������
	void Explode(int x, int y, int z); // ��������� ����������
	void Explode(Vector3 pos); // ��������� ����������
	void AddToUpdateColumn(int x, int y, int z); // �������� � ������� ������� ������
	void SetBufferBlock(int x, int y, int z, block_id block); // �������� ���� � ������
	char GetLight(int x, int y, int z); // �������� ����� �� ������������

	World(Player* player); // �����������
	~World(); // ����������
};