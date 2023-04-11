#include "Chunk.h"

float Chunk::brightness = 0.2f;

/// <summary>
/// Конструктор чанка
/// </summary>
Chunk::Chunk()
{
	pos = Vector3Int(0, 0, 0); // нулевая позиция

	Init(); // вызов инициализации
}
/// <summary>
/// Деструктор чанка
/// </summary>
Chunk::~Chunk()
{
	SaveChunk();					// сохранение чанка
	world = nullptr;				// обнуление ссылки на мир
	data.Clear();					// очистка данных меша
	uv_data.Clear();				// очситка данных UV-карты
}
/// <summary>
/// Конструктор чанка
/// </summary>
/// <param name="x"> ордината чанка</param>
/// <param name="y"> абсцисса чанка</param>
/// <param name="z"> аппликата чанка</param>
/// <param name="world"> указатель на экземпляр мира</param>
Chunk::Chunk(int x, int y, int z, World* world)
{
	pos = Vector3Int(x, y, z);	// позиция
	this->world = world;		// указатель на мир

	Init();	// вызов инициализации
}
/// <summary>
/// Получение ID блока по координатам
/// </summary>
/// <param name="x"> ордината точки</param>
/// <param name="y"> абсцисса точки</param>
/// <param name="z"> аппликата точки</param>
/// <returns>ID блока</returns>
block_id Chunk::GetBlockID(int x, int y, int z)
{
	if (InRange(x, y, z)) // если внутри чанка
		return blocks[x][y][z]; // вернуть ID блока

	if (z + pos.z < 0 || z + pos.z >= ChunkSize * world->WorldHeight)
		return 0; // если за границами мира по высоте, вернуть ID воздуха
	// если в границах света
	if (InLightRange(ChunkSize + x, ChunkSize + y, ChunkSize + z))
	{
		// рассчёт координат
		int _x = (ChunkSize + x) / ChunkSize;
		int _y = (ChunkSize + y) / ChunkSize;
		int _z = (ChunkSize + z) / ChunkSize;

		Chunk* chunk = chunks[_x + _y * 3 + _z * 9]; // определение чанка

		if (chunk) // если существует
		{
			// расчёт координат
			_x = pos.x - chunk->pos.x + x;
			_y = pos.y - chunk->pos.y + y;
			_z = pos.z - chunk->pos.z + z;

			if (InRange(_x, _y, _z)) // если в границах чанка
				return chunk->blocks[_x][_y][_z]; // верунть ID блока
		}

		return -1; // вернуть ID технической пустоты
	}
	else // иначе получаем значение ID через объект мира из других чанков
		return world->GetBlockID(x + pos.x, y + pos.y, z + pos.z);
}
/// <summary>
/// Очистка данных меша
/// </summary>
void Chunk::ClearCash()
{
	// удаление массивов
	//delete[] Vertices;
	//delete[] UVs;
	//delete[] Colors;
	// очистка данных
	data.Clear();
	uv_data.Clear();
}
/// <summary>
/// Инициализация чанка
/// </summary>
void Chunk::Init()
{
	// инициализация массивов
	//Vertices = new GLfloat;
	//UVs = new GLfloat;
	//Colors = new GLfloat;

	for (int i = 0; i < ChunkSize; i++)
		for (int j = 0; j < ChunkSize; j++)
			for (int k = 0; k < ChunkSize; k++)
				blocks[i][j][k] = 0; // заполнение воздухом по умолчанию

	for (int i = 0; i < 27; i++)
		chunks[i] = nullptr; // по умолчанию указатели на чанки пустые
}
/// <summary>
/// Генерация чанка
/// </summary>
void Chunk::Generate()
{
	if (!LoadChunk()) // если чанк не имеет сохранения, генерируем его
		world->Generate(this);
}
/// <summary>
/// Получение ссылки на экземпляр блока по его ID
/// </summary>
/// <param name="ID"> блока </param>
/// <returns> Указатель на экземпляр блока</returns>
inline Block* Chunk::GetBlock(block_id ID)
{
	return Block::GetBlock(ID);
}
/// <summary>
/// Формирование данных меша
/// </summary>
/// <param name="data"> указатель на объект меша</param>
/// <param name="uv"> указатель на объект данный UV</param>
/// <param name="x"> ордината точки</param>
/// <param name="y"> абсцисса точки</param>
/// <param name="z"> аппликата точки</param>
/// <param name="layer"> слой </param>
void Chunk::GetMeshData(VertexData* data, UVData* uv, int x, int y, int z, int layer)
{
	//if (InRange(x, y, z)) // если в границах чанка
	{
		Block* block = GetBlock(GetBlockID(x, y, z));		// получаем экземпляр блока
		if (block->GetRenderLayer() == layer)				// если у блока текущий слой прорисовки
			block->GetMeshData(data, uv, x, y, z, this);	// добавляем в данные меша данные блока
	}
}
/// <summary>
/// Получение ссылки на экземпляр блока по координатам
/// </summary>
/// <param name="x"> ордината точки</param>
/// <param name="y"> абсцисса точки</param>
/// <param name="z"> аппликата точки</param>
/// <returns>Экземлпяр блока</returns>
inline Block* Chunk::GetBlock(int x, int y, int z)
{
	block_id id = GetBlockID(x, y, z); // получение ID блока
	return Block::GetBlock(id);
}
/// <summary>
/// Проверка на попадание в границы чанка по одному измерению
/// </summary>
/// <param name="x"> ордината точки</param>
/// <returns>Принадлежность точки к отрезку, равному размеру чанка</returns>
inline bool Chunk::InRange(int x)
{
	return x >= 0 && x < ChunkSize;
}
/// <summary>
/// Проверка на попадание в границы чанка по трём измерениям
/// </summary>
/// <param name="x"> ордината точки</param>
/// <param name="y"> абсцисса точки</param>
/// <param name="z"> аппликата точки</param>
/// <returns>Принадлежность точки к явной области чанка</returns>
inline bool Chunk::InRange(int x, int y, int z)
{
	return InRange(x) && InRange(y) && InRange(z); // если в границах по всем измерениям
}
/// <summary>
/// Проверка на попадание в границы чанка по трём измерениям для свтеа
/// </summary>
/// <param name="x"> ордината точки</param>
/// <param name="y"> абсцисса точки</param>
/// <param name="z"> аппликата точки</param>
/// <returns>Принадлежность точки к световой области чанка</returns>
bool Chunk::InLightRange(int x, int y, int z)
{
	return (x >= 0 && x < ChunkSize * 3) && (y >= 0 && y < ChunkSize * 3) && (z >= 0 && z < ChunkSize * 3);
}
/// <summary>
/// Обновление геометрии чанка
/// </summary>
void Chunk::UpdateMesh() // обновление меша
{
	//lock_guard<mutex> guard(mtx); // защита от посторонних потоков

	UpdateLight();

	ticks = 0; // сбрасываем счётчик тиков
	world->chunksUpdateing++; // инкрементация количетсва чанков на обновление

	ClearCash(); // очистка старых данных

	for (int x = 0; x < ChunkSize; x++)
		for (int y = 0; y < ChunkSize; y++)
			for (int z = 0; z < ChunkSize; z++)
				GetMeshData(&data, &uv_data, x, y, z, 0); // получение данных блоков

	// конвертирование векторов в массивы
	//Vertices = data.ToArray();
	//UVs = uv_data.ToArray();
	//Colors = data.ColorsToArray();
}
/// <summary>
/// Обновление состояния чанка
/// </summary>
void Chunk::Update()
{
	int b_count = 0; // счетчик, для определения прозрачности слоя

	for (int x = 0; x < ChunkSize; x++)
		for (int y = 0; y < ChunkSize; y++)
		{
			bool detected = false; // флаг определения блока

			for (int z = 0; z < ChunkSize; z++)
			{
				if (!detected) // если не обнаружен
					if (!GetBlock(x, y, z)->Transparent()) // если не прозрачный
						detected = true; // обнаружен
			}

			if (detected) // если обнаружен
				b_count++; // инкремировать количество
		}

	is_solid = b_count >= ChunkSize * ChunkSize; // твердрый, если количествао больше или равно площади
}

inline void Chunk::SetBlock(int x, int y, int z, block_id block)
{
	if (InRange(x, y, z)) blocks[x][y][z] = block;
	else world->SetBlock(x + pos.x, y + pos.y, z + pos.z, block);
}

void Chunk::SetBlock(int x, int y, int z, Block* block) //
{
	SetBlock(x, y, z, block->id); // установить блок
}

void Chunk::SaveChunk()
{
	if (!modified) // если чанк не изменялся, не сохраняем его
		return;

	ofstream stream; // объект потока записи
	stream.open(save_folder + World::name + "/" + to_string(pos.x) + " " + to_string(pos.y) + " " + to_string(pos.z) + ".dat");

	for (int x = 0; x < Chunk::ChunkSize; x++)
		for (int y = 0; y < Chunk::ChunkSize; y++)
			for (int z = 0; z < Chunk::ChunkSize; z++)
			{
				stream << blocks[x][y][z]; // записываем все блоки в чанке
			}

	stream.close(); // закрыть поток
}

bool Chunk::LoadChunk()
{
	ifstream stream;
	stream.open(save_folder + World::name + "/" + to_string(pos.x) + " " + to_string(pos.y) + " " + to_string(pos.z) + ".dat");

	if (stream.is_open())
	{
		for (int x = 0; x < Chunk::ChunkSize; x++) {
			for (int y = 0; y < Chunk::ChunkSize; y++) {
				for (int z = 0; z < Chunk::ChunkSize; z++) {
					blocks[x][y][z] = stream.get();
				}
			}
		}

		stream.close();
		return true;
	}

	return false;
}

Vector3 Chunk::GetLigthColor(int x, int y, int z)
{
	float l = (GetLight(x, y, z) / float(MaxLight)) * (1 - brightness) + brightness; // значение яркости
	return Vector3(l, l, l); // значение цвета
}

void Chunk::UpdateMem(){
	for (int x = 0; x < 3; x++)
		for (int y = 0; y < 3; y++)
			for (int z = 0; z < 3; z++)
			{
				// расчёт координат
				int X = (x - 1) * ChunkSize;
				int Y = (y - 1) * ChunkSize;
				int Z = (z - 1) * ChunkSize;
				// установка укзатаеля на чанк
				chunks[x + y * 3 + z * 9] =
					world->GetChunk(pos.x + X, pos.y + Y, pos.z + Z);
			}
}

void Chunk::UpdateLight(){
	for (int x = 0; x < ChunkSize * 3; x++)
		for (int y = 0; y < ChunkSize * 3; y++)
			for (int z = 0; z < ChunkSize * 3; z++)
				light_map[x][y][z] = 0; // очистка карты света

	for (int x = 0; x < ChunkSize; x++)
		for (int y = 0; y < ChunkSize; y++)
			for (int z = 0; z < ChunkSize; z++)
			{
				buffer_light_map[x][y][z] = 0; // очистка карты буфера света

				char light = GetBlock(x, y, z)->Luminosity(); // светимость блока

				if (light) // если у блока светимость не 0
					light_map[x + ChunkSize][y + ChunkSize][z + ChunkSize] = light + 1; // значение светимости + 1
			}


	Chunk* chunk = chunks[22]; // верхний чанк

	if (World::is_day) // если день
		for (int x = 0; x < ChunkSize; x++)
			for (int y = 0; y < ChunkSize; y++)
			{
				sky_light[x][y] = true; // маска глобального освещения
				for (int z = ChunkSize - 1; z >= 0; z--)
				{
					// если блок прозрачный и сверху нет тени, заполнить максимальным значением
					if ((!chunk || chunk->sky_light[x][y]) && GetBlock(x, y, z)->Transparent())
						light_map[x + ChunkSize][y + ChunkSize][z + ChunkSize] = MaxLight;
					else // иначе отмечаем на маске тень и останавливаем луч
					{
						sky_light[x][y] = false;
						break;
					}
				}
			}

	for (int i = 0; i < 27; i++) // свет соседних чанков
	{
		if (i == 13) // 13 - этот чанк
			continue;

		if (chunks[i]) // если i-тый чанк есть
		{
			// рассчёт координат
			int sx = pos.x - chunks[i]->pos.x;
			int sy = pos.y - chunks[i]->pos.y;
			int sz = pos.z - chunks[i]->pos.z;

			for (int x = 0; x < ChunkSize; x++)
				for (int y = 0; y < ChunkSize; y++)
					for (int z = 0; z < ChunkSize; z++)
					{
						// значение света в соседнем чанке в области этого чанка
						char l = chunks[i]->CheckLight(x + sx, y + sy, z + sz);

						if (l > buffer_light_map[x][y][z]) // если значение больше ранее установленного
							buffer_light_map[x][y][z] = l; // установить новое значение света буфера
					}
		}
	}

	for (int l = MaxLight + 1; l > 0; l--) // цикл ярксти света
		for (int x = 0; x < ChunkSize * 3; x++)
			for (int y = 0; y < ChunkSize * 3; y++)
				for (int z = 0; z < ChunkSize * 3; z++)
				{
					if (light_map[x][y][z] == l) // если яркость в точке равна текущей яркости, распространить свет
					{
						// вызовы распространения света
						SetLight(x + 1 - ChunkSize, y - ChunkSize, z - ChunkSize, light_map[x][y][z] - 1);
						SetLight(x - 1 - ChunkSize, y - ChunkSize, z - ChunkSize, light_map[x][y][z] - 1);
						SetLight(x - ChunkSize, y + 1 - ChunkSize, z - ChunkSize, light_map[x][y][z] - 1);
						SetLight(x - ChunkSize, y - 1 - ChunkSize, z - ChunkSize, light_map[x][y][z] - 1);
						SetLight(x - ChunkSize, y - ChunkSize, z + 1 - ChunkSize, light_map[x][y][z] - 1);
						SetLight(x - ChunkSize, y - ChunkSize, z - 1 - ChunkSize, light_map[x][y][z] - 1);
					}
				}


}

inline void Chunk::SetLight(int x, int y, int z, char l)
{
	if (light_map[x + ChunkSize][y + ChunkSize][z + ChunkSize] < l && GetBlock(x, y, z)->Transparent())
	{
		if (InRange(x + ChunkSize, y + ChunkSize, z + ChunkSize)) // если в границах
		{
			// если предыдущее значение меньше и блок прозрачный
			if (buffer_light_map[x + ChunkSize][y + ChunkSize][z + ChunkSize] < l)
				light_map[x + ChunkSize][y + ChunkSize][z + ChunkSize] = l; // установить новое значение
		}
		else if (InLightRange(x + ChunkSize, y + ChunkSize, z + ChunkSize)) // если в границах 
		{
			light_map[x + ChunkSize][y + ChunkSize][z + ChunkSize] = l; // установить новое значение
		}
	}
}

inline char Chunk::GetLight(int x, int y, int z)
{
	if (InRange(x, y, z)) // если в границах
	{
		// максимум между картой света и картой буфера света
		return max(light_map[x + ChunkSize][y + ChunkSize][z + ChunkSize], buffer_light_map[x][y][z]);
	}
	if (InRange(x) && InRange(y)) // для света за границами мира по оси аппликат
	{
		if (pos.z + z < 0 || pos.z + z >= world->WorldHeight * ChunkSize)
		{
			// значение света в точке
			char light = light_map[x + ChunkSize][y + ChunkSize][z + ChunkSize];

			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
				{
					if (i == 1 && j == 1) // этот чанк 
						continue;

					Chunk* chunk = chunks[i + j * 3 + 9]; // чанк из среднего слоя

					if (chunk) // если существует
					{
						// перевод координат в координаты соседних чанков
						int _x = x + (2 - i) * ChunkSize;
						int _y = y + (2 - j) * ChunkSize;
						int _z = z + ChunkSize;
						// получение значение света из соседнего чанка
						char l = chunk->light_map[_x][_y][_z];

						if (l > light) // если новое значение света больше
							light = l; // установить новое значение
					}
				}
			return light; // рассчитаное значение света
		}
	}
	// если за рамками чанка, возврат света через объект мира
	return world->GetLight(x + pos.x, y + pos.y, z + pos.z);
}

inline int Chunk::CheckLight(int x, int y, int z)
{
	if (InRange(x, y, z)) // если в рамках чанка
		return max(light_map[x + ChunkSize][y + ChunkSize][z + ChunkSize], buffer_light_map[x][y][z]);
	if (InLightRange(x + ChunkSize, y + ChunkSize, z + ChunkSize)) // если в буферной области чанка
		return light_map[x + ChunkSize][y + ChunkSize][z + ChunkSize];
	return 0;
}

void Chunk::Modify()
{
	for (int i = 0; i < 27; i++) {
		if (chunks[i]) {
			chunks[i]->modified = true;
		}
	}
}

void Chunk::Render(unsigned int texture)
{
	if (data.face_count == 0) // если нет вершин в чанке, то не выолняется код ниже
		return;

	static float s = Chunk::ChunkSize * 0.5f; // половина размера чанка
	static float shift = Chunk::ChunkSize * 0.5f - 0.5f; // смещение

	glBindTexture(GL_TEXTURE_2D, texture); // Привязка текстуры к цели текстурирования
	// установка указателей в массивах для отрисовки меша
	GLfloat* UVs = &uv_data.UVs[0];
	GLfloat* Vertices = &data.Vertices[0];
	GLfloat* Colors = &data.Colors[0];

	glTexCoordPointer(2, GL_FLOAT, 0, UVs);
	glVertexPointer(3, GL_FLOAT, 0, Vertices);
	glColorPointer(3, GL_FLOAT, 0, Colors);
	//17 404
	glDrawArrays(GL_QUADS, 0, data.face_count * 4); // отрисвока граней
	glBindTexture(GL_TEXTURE_2D, 0);				// Отвзяка текстуры от цели текстурирования

	if (Debug::bounds) // если включен режим границ чанков
		Debug::DrawBox({ shift, shift, shift}, { s, s, s }, { .9f, .9f, .9f });

	// дополнительное обновление
	if (ticks <= timeout && ticks > -1)
		ticks++;

	if (ticks > timeout)
	{
		UpdateMem();
		UpdateMesh();
		ticks = -1;
	}
}