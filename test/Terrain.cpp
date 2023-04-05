#include "Terrain.h"
/// <summary>
/// —оздание дерева
/// </summary>
/// <param name="chunk"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
/// <param name="height"></param>
/// <param name="block"></param>
/// <param name="leaves"></param>
void Terrain::SetTree(Chunk* chunk, int x, int y, int z, int height, block_id block, block_id leaves)
{
	static int rad = 5;
	static float wide = 1.5f;
	// ствол
	for (int i = 0; i < height; i++)
	{
		SetBlock(chunk, x, y, z + i, block);
	}
	// листва
	for (int i = -rad; i <= rad; i++)
		for (int j = -rad; j <= rad; j++)
			for (int k = -rad; k <= rad; k++)
			{
				if (sqrt(wide * i * i + wide * j * j + k * k) <= rad - rand() % 3)
				{
					if (i != 0 || j != 0 || k >= 0)
						SetBlock(chunk, x + i, y + j, z + k + height, leaves);
				}
			}
}
/// <summary>
/// расчЄт шума ѕерлина
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
/// <returns>значение плотности пространства</returns>
int Terrain::GetTerrainNoise(int x, int y, int z)
{
	// пол€ дл€ шумов
	static float main2D;
	static float volume2D;
	static float main3D;
	static float hills3D;
	static float hills2D;
	static float shiftX, shiftY;
	static int prevX = -1, prevY = -1;

	// коэффициенты
	const float scale = .05f;
	const float main2D_scale = .8f * scale;
	const float hills2D_scale = .4f * scale;
	const float main3D_scale = .5f * scale;
	const float hills3D_scale = .4f * scale;
	const float volume2D_scale = .3f * scale;
	const float shift_scale = .7f * scale;

	const int shift = 128;
	// шумы смещени€
	shiftX = perlinNoise.noise(x * shift_scale, y * shift_scale, z * shift_scale) * shift;
	shiftY = perlinNoise.noise(y * shift_scale, x * shift_scale, z * shift_scale) * shift;
	// мемефикаци€ двумерных шумов по аппликате
	if ((prevX != x || prevY != y))
	{
		prevY = y;
		prevX = x;

		if (world->settings.params[1])
		{
			main2D = perlinNoise.octave_noise(x * main2D_scale, y * main2D_scale, 2) * .1f;
		}
		else
		{
			main2D = 0.05f;
		}
		hills2D = perlinNoise.octave_noise(x * hills2D_scale, y * hills2D_scale, 2) * .5f + .1f;
		volume2D = perlinNoise.octave_noise(y * hills2D_scale, x * hills2D_scale, 2);
		// пикова€ интерпол€ци€ 
		hills2D = pick(hills2D, 0.3, 0.45) + .1f;
		volume2D = pick(volume2D, 0.35, 0.5);
	}

	if (world->settings.params[0])
	{
		main3D = perlinNoise.octave_noise((x + shiftX) * main3D_scale, (y + shiftY) * main3D_scale, z * main3D_scale, 2) * .9f;
		hills3D = perlinNoise.octave_noise((x - shiftX) * hills3D_scale, (y - shiftY) * hills3D_scale, z * hills3D_scale, 2);
	}

	hills3D *= hills2D;
	main3D *= main3D * volume2D;

	float result = main2D;

	if (main3D > main2D)
		result = main3D;

	result += hills3D;

	int noise = (int)(result * land_height) + ground_height;

	return noise;
}
/// <summary>
/// получение шумов ѕерлина
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
/// <param name="scale"></param>
/// <param name="size"></param>
/// <returns>значение плотности пространства</returns>
int Terrain::GetNoise(int x, int y, int z, float scale, int size)
{
	int noise = int(perlinNoise.noise(x * scale, y * scale, z * scale) * size);

	return noise;
}

/// <summary>
/// пикова€ интерпол€ци€
/// </summary>
/// <param name="x"></param>
/// <param name="x0"></param>
/// <param name="x1"></param>
/// <returns>преобразованное значение</returns>
float Terrain::pick(float x, float x0, float x1)
{
	float m = 1. / (x1 - x0);
	x -= x0;
	x *= m;

	if (x > 1)
		x = 1;
	if (x < 0)
		x = 0;

	return x;
}
/// <summary>
/// линейна€ интерпол€ци€
/// </summary>
/// <param name="x"></param>
/// <param name="x0"></param>
/// <param name="x1"></param>
/// <param name="y0"></param>
/// <param name="y1"></param>
/// <returns>преобразованное значение</returns>
float Terrain::linear(float x, float x0, float x1, float y0, float y1)
{
	float y = ((x - x0) * (y1 - y0) / (x1 - x0)) + y0;

	return y;
}
/// <summary>
/// утсановка сло€ поверхности
/// </summary>
/// <param name="chunk"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
/// <param name="t"></param>
void Terrain::SetGround(Chunk* chunk, int x, int y, int z)
{
	Block* block;
	for (int i = 0; i < ground_length + GetNoise(x, y, z, 0.03f, 4); i++)
	{
		block = chunk->GetBlock(x, y, z - i);
		if (block == Block::stone || block == Block::null)
		{
			int index = i;
			if (index >= ground_length)
				index = ground_length - 1;

			chunk->SetBlock(x, y, z - i, ground_mas[index]);
		}
		else
			break;
	}
}
/// <summary>
/// генераци€ чанка
/// </summary>
/// <param name="chunk"></param>
void Terrain::Generate(Chunk* chunk)
{
	static block_id bedrock = Block::bedrock->id;
	static block_id stone = Block::stone->id;
	static block_id poppy = Block::poppy->id;
	static block_id dandelion = Block::dandelion->id;
	static block_id tall_grass = Block::tall_grass->id;
	static block_id oak_leaves = Block::leaves->id;
	static block_id birch_leaves = Block::birch_leaves->id;
	static block_id oak = Block::oak->id;
	static block_id birch = Block::birch->id;

	srand(seed + chunk->pos.GetHashCode()); // инициализаци€ сида
	// перебор карты блоков
	for (int x = 0; x < Chunk::ChunkSize; x++)
		for (int y = 0; y < Chunk::ChunkSize; y++)
		{
			// мемефикаци€ шума по аппликате
			int noise0 = -1;
			int noise1 = -1;

			for (int z = 0; z < Chunk::ChunkSize; z++)
			{
				// глобальные координаты
				int X = x + chunk->pos.x;
				int Y = y + chunk->pos.y;
				int Z = z + chunk->pos.z;

				if (Z == 0) // слой коренной породы
				{
					SetBlock(chunk, x, y, z, bedrock);
					continue;
				}

				if (noise1 >= 0)
					noise0 = noise1;
				else
					noise0 = GetTerrainNoise(X, Y, Z);

				noise1 = GetTerrainNoise(X, Y, Z + 1); // шум в точке

				if (Z < noise0) // если аппликата меньше плотности
				{
					if (chunk->GetBlockID(x, y, z) == 0)
						SetBlock(chunk, x, y, z, stone);

					if (Z + 1 >= noise1) // создаине поверхности 
					{
						SetGround(chunk, x, y, z);

						int rnd = rand() % 500;

						if (world->settings.params[2]) // деревь€
						{
							if (rnd == 1)
								SetTree(chunk, x, y, z, 5 + rand() % 4, oak, oak_leaves);
							if (rnd == 2)
								SetTree(chunk, x, y, z, 5 + rand() % 4, birch, birch_leaves);
						}

						if (world->settings.params[3]) // цветы и трава
						{
							if (rnd == 3)
								SetBlock(chunk, x, y, z + 1, poppy);
							if (rnd == 4)
								SetBlock(chunk, x, y, z + 1, dandelion);
							if (rnd > 450)
								SetBlock(chunk, x, y, z + 1, tall_grass);
						}
					}
				}
			}
		}
	// обработка буфера блоков
	for (int i = 0; i < world->GlobalBuffer.size(); i++)
	{
		Vector3Int pos = Vector3Int(world->GlobalBuffer[i].x, world->GlobalBuffer[i].y, world->GlobalBuffer[i].z) - chunk->pos;
		if (chunk->InRange(pos.x, pos.y, pos.z))
		{
			if (!chunk->GetBlockID(pos.x, pos.y, pos.z))
				chunk->SetBlock(pos.x, pos.y, pos.z, world->GlobalBuffer[i].block);
			world->toRemove.push_back(i);
		}
	}
}
/// <summary>
/// установка блока 
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
/// <param name="block"></param>
void Terrain::SetBlock(Chunk* chunk, int x, int y, int z, block_id block)
{
	if (z < 0 || z >= world->WorldHeight * Chunk::ChunkSize)
		return;

	if (chunk->InRange(x, y, z))
	{
		auto id = chunk->GetBlockID(x, y, z);

		if (id == 0 || id == -1)
			chunk->SetBlock(x, y, z, block);
	}
	else
	{
		Vector3Int p = chunk->pos;
		world->SetBufferBlock(x + p.x, y + p.y, z + p.z, block);
	}
}
/// <summary>
/// установка блока
/// </summary>
/// <param name="chunk"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
/// <param name="block"></param>
void Terrain::SetBlock(Chunk* chunk, int x, int y, int z, Block* block)
{
	SetBlock(chunk, x, y, z, block->id);
}