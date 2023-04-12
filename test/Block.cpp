#include "Block.h"
struct Chunk; // заголовок чанка
vector<Block*> Block::BlockList = vector<Block*>(); // инициализация списка блоков
Block** Block::blocks = new Block * (); // массив блоков
/// <summary>
/// конструктор блока
/// </summary>
Block::Block()
{

}
/// <summary>
/// Добавление данных меша
/// </summary>
/// <param name="data"></param>
/// <param name="uv"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
/// <param name="chunk"></param>
void Block::GetMeshData(VertexData* data, UVData* uv, int x, int y, int z, Chunk* chunk)
{
	// добавление сторон куба в соответствии с расположением в чанке
	if (chunk->GetBlock(x, y, z + 1)->GetLayer() != GetLayer()
		&& chunk->GetBlock(x, y, z + 1) != null)
	{
		Vector3 color = Vector3::clrm(chunk->GetLigthColor(x, y, z + 1), Color(Direction::Up));
		ColorSquad squad{ color };
		data->FaceUp(x, y, z, squad); // добавление вершин грани блока

		float i = TileCoord(Direction::Up).x; // получение абсциссы текстуры в атласе
		float j = TileCoord(Direction::Up).y; // получение ординаты текстуры в атласе

		uv->AddUV(i, j); // добавление UV-координат грани блока
	}
	// остальные грани - аналогично
	if (chunk->GetBlock(x, y, z - 1)->GetLayer() != GetLayer()
		&& chunk->GetBlock(x, y, z - 1) != null)
	{
		Vector3 color = Vector3::clrm(chunk->GetLigthColor(x, y, z - 1), Color(Direction::Down));
		ColorSquad squad{ color };
		data->FaceDown(x, y, z, squad);

		float i = TileCoord(Direction::Down).x;
		float j = TileCoord(Direction::Down).y;

		uv->AddUV(i, j);
	}
	if (chunk->GetBlock(x, y + 1, z)->GetLayer() != GetLayer()
		&& chunk->GetBlock(x, y + 1, z) != null)
	{
		Vector3 color = Vector3::clrm(chunk->GetLigthColor(x, y + 1, z), Color(Direction::Front));
		ColorSquad squad{ color };
		data->FaceFront(x, y, z, squad);

		float i = TileCoord(Direction::Front).x;
		float j = TileCoord(Direction::Front).y;

		uv->AddUV(i, j);
	}
	if (chunk->GetBlock(x, y - 1, z)->GetLayer() != GetLayer()
		&& chunk->GetBlock(x, y - 1, z) != null)
	{
		Vector3 color = Vector3::clrm(chunk->GetLigthColor(x, y - 1, z), Color(Direction::Back));
		ColorSquad squad{ color };
		data->FaceBack(x, y, z, squad);

		float i = TileCoord(Direction::Back).x;
		float j = TileCoord(Direction::Back).y;

		uv->AddUV(i, j);
	}
	if (chunk->GetBlock(x + 1, y, z)->GetLayer() != GetLayer()
		&& chunk->GetBlock(x + 1, y, z) != null)
	{
		Vector3 color = Vector3::clrm(chunk->GetLigthColor(x + 1, y, z), Color(Direction::Right));
		ColorSquad squad{ color };
		data->FaceRight(x, y, z, squad);

		float i = TileCoord(Direction::Right).x;
		float j = TileCoord(Direction::Right).y;

		uv->AddUV(i, j);
	}
	if (chunk->GetBlock(x - 1, y, z)->GetLayer() != GetLayer()
		&& chunk->GetBlock(x - 1, y, z) != null)
	{
		Vector3 color = Vector3::clrm(chunk->GetLigthColor(x - 1, y, z), Color(Direction::Left));
		ColorSquad squad{ color };
		data->FaceLeft(x, y, z, squad);

		float i = TileCoord(Direction::Left).x;
		float j = TileCoord(Direction::Left).y;

		uv->AddUV(i, j);
	}
}
/// <summary>
/// границы блока
/// </summary>
/// <returns></returns>
Bound Block::GetBounds() 
{
	return Bound();
}
/// <summary>
/// твёрдость
/// </summary>
/// <returns>твёрдость блока</returns>
bool Block::Solid()
{
	return true;
}
/// <summary>
/// прозрачность блока для света
/// </summary>
/// <returns>прозрачность блока</returns>
bool Block::Transparent()
{
	return false;
}
/// <summary>
/// светимость блока
/// </summary>
/// <returns>уровень светимости</returns>
char Block::Luminosity()
{
	return 0;
}
/// <summary>
/// цвет грани блока
/// </summary>
/// <param name="direction"></param>
/// <returns>вектор с RGB каналами</returns>
Vector3 Block::Color(Direction direction)
{
	return Vector3(1, 1, 1);
}
/// <summary>
/// координаты текстур граней блока 
/// </summary>
/// <param name="direction"></param>
/// <returns>вектор</returns>
Vector2 Block::TileCoord(Direction direction)
{
	return Vector2(0, 0);
}
/// <summary>
/// слой блока
/// </summary>
/// <returns>слой блока</returns>
Block::BlockLayer Block::GetLayer()
{
	return BlockLayer::OTHER;
}

void Block::Init()
{
	int size = BlockList.size(); // размер списка блоков

	blocks = new Block * [size]; // инициализация массива указателей блоков 

	// распределение по ID
	for (int j = 0; j < size; j++) {
		if (BlockList[j]->id >= 0)
			blocks[BlockList[j]->id] = BlockList[j]; // :skull:
	}

	BlockList.clear();
}

/// <param name="name"></param>
Block::Block(block_id id, string name)
{
	this->id = id;				// ID блока
	this->name = name;			// имя блока
	BlockList.push_back(this);	// добавление ссылки в список блоков
}
/// <summary>
/// слой рендеринга
/// </summary>
/// <returns>номер слоя</returns>
char Block::GetRenderLayer()
{
	return 0;
}

//Блоки

/// <summary>
///  блок воздуха
/// </summary>
class BlockAir : public Block
{
public:
	using Block::Block;
	/// <summary>
	/// твёрдость 
	/// </summary>
	/// <returns>твёрдость</returns>
	virtual bool Solid()  // нетвёрдый
	{
		return false;
	}
	/// <summary>
	/// данные меша
	/// </summary>
	virtual void GetMeshData(VertexData* data, UVData* uv, int x, int y, int z, Chunk* chunk)
	{
		// блок воздуха не обладает геометрией
	}
	/// <summary>
	/// слой
	/// </summary>
	/// <returns>слой</returns>
	virtual BlockLayer GetLayer()
	{
		return BlockLayer::AIR; // слой воздуха
	}
	/// <summary>
	/// прозрачность
	/// </summary>
	/// <returns>прозрачность</returns>
	virtual bool Transparent()
	{
		return true; // прозрачный
	}
};
/// <summary>
/// блок земли с травой 
/// </summary>
class BlockGrass : public Block
{
public:
	using Block::Block;
	/// <summary>
	/// координаты текстуры
	/// </summary>
	/// <param name="direction"></param>
	/// <returns>вектор</returns>
	virtual Vector2 TileCoord(Direction direction)
	{
		switch (direction)
		{
		case Direction::Up:		// верхняя грань
			return Vector2(0, 15);
		case Direction::Down:	// нижняя грань
			return Vector2(2, 15);

		}
		// остальные грани
		return Vector2(3, 15);
	}
	/// <summary>
	/// цвет 
	/// </summary>
	/// <param name="direction"></param>
	/// <returns>цвет грани</returns>
	virtual Vector3 Color(Direction direction) // верхняя грань блока окрашивается в зелёный
	{
		if (direction == Direction::Up)
			return Vector3(0.56, 0.81, 0.3);
		return Vector3(1, 1, 1);
	}

};
/// <summary>
/// блок камня
/// </summary>
class BlockStone : public Block
{
public:
	using Block::Block;
	/// <summary>
	/// координаты текстуры
	/// </summary>
	/// <param name="direction"></param>
	/// <returns>вектор</returns>
	virtual Vector2 TileCoord(Direction direction)
	{
		return Vector2(1, 15);

	}
};
/// <summary>
/// блок земли
/// </summary>
class BlockDirt : public Block
{
public:
	using Block::Block;
	/// <summary>
	/// координаты текстуры
	/// </summary>
	/// <param name="direction"></param>
	/// <returns>вектор</returns>
	virtual Vector2 TileCoord(Direction direction)
	{
		return Vector2(2, 15);
	}
};
/// <summary>
/// блок дуба
/// </summary>
class BlockOak : public Block
{
public:
	using Block::Block;
	/// <summary>
	/// координаты текстуры
	/// </summary>
	/// <param name="direction"></param>
	/// <returns>вектор</returns>
	virtual Vector2 TileCoord(Direction direction)
	{
		switch (direction)
		{
		case Direction::Up:
		case Direction::Down:
			return Vector2(5, 14);

		}
		return Vector2(4, 14);
	}

};
/// <summary>
/// блок булыжника
/// </summary>
class BlockCobblestone : public Block
{
public:
	using Block::Block;
	/// <summary>
	/// координаты текстуры
	/// </summary>
	/// <param name="direction"></param>
	/// <returns>вектор</returns>
	virtual Vector2 TileCoord(Direction direction)
	{
		return Vector2(0, 14);
	}
};
/// <summary>
/// блок замшелого булыжника
/// </summary>
class BlockMossyCobblestone : public Block
{
public:
	using Block::Block;
	/// <summary>
	/// координаты текстуры
	/// </summary>
	/// <param name="direction"></param>
	/// <returns>вектор</returns>
	virtual Vector2 TileCoord(Direction direction)
	{
		return Vector2(4, 13);
	}
};
/// <summary>
/// блок листвы
/// </summary>
class BlockLeaves : public Block
{
public:
	using Block::Block;
	/// <summary>
	/// данные меша
	/// </summary>
	virtual void GetMeshData(VertexData* data, UVData* uv, int x, int y, int z, Chunk* chunk)
	{
		// верхняя грань
		Vector3 color = Vector3::clrm(chunk->GetLigthColor(x, y, z + 1), Color(Direction::Up));
		ColorSquad squad{ color };
		data->FaceUp(x, y, z, squad); // добавление вершин грани блока

		float i = TileCoord(Direction::Up).x; // получение абсциссы текстуры в атласе
		float j = TileCoord(Direction::Up).y; // получение ординаты текстуры в атласе

		uv->AddUV(i, j); // добавление UV-координат грани блока

		// нижняя грань
		color = Vector3::clrm(chunk->GetLigthColor(x, y, z - 1), Color(Direction::Down));
		squad = ColorSquad{ color };
		data->FaceDown(x, y, z, squad);

		i = TileCoord(Direction::Down).x;
		j = TileCoord(Direction::Down).y;

		uv->AddUV(i, j);

		// передняя грань
		color = Vector3::clrm(chunk->GetLigthColor(x, y + 1, z), Color(Direction::Front));
		squad = ColorSquad{ color };
		data->FaceFront(x, y, z, squad);

		i = TileCoord(Direction::Front).x;
		j = TileCoord(Direction::Front).y;

		uv->AddUV(i, j);

		// задняя грань
		color = Vector3::clrm(chunk->GetLigthColor(x, y - 1, z), Color(Direction::Back));
		squad = ColorSquad{ color };
		data->FaceBack(x, y, z, squad);

		i = TileCoord(Direction::Back).x;
		j = TileCoord(Direction::Back).y;

		uv->AddUV(i, j);

		// правая грань
		color = Vector3::clrm(chunk->GetLigthColor(x + 1, y, z), Color(Direction::Right));
		squad = ColorSquad{ color };
		data->FaceRight(x, y, z, squad);

		i = TileCoord(Direction::Right).x;
		j = TileCoord(Direction::Right).y;

		uv->AddUV(i, j);
		
		// левая грань
		color = Vector3::clrm(chunk->GetLigthColor(x - 1, y, z), Color(Direction::Left));
		squad = ColorSquad{ color };
		data->FaceLeft(x, y, z, squad);

		i = TileCoord(Direction::Left).x;
		j = TileCoord(Direction::Left).y;

		uv->AddUV(i, j);
	}
	/// <summary>
	/// координаты текстуры
	/// </summary>
	/// <param name="direction"></param>
	/// <returns>вектор</returns>
	virtual Vector2 TileCoord(Direction direction)
	{
		return Vector2(4, 12);
	}
	/// <summary>
	/// цвет
	/// </summary>
	/// <param name="direction"></param>
	/// <returns>цвет грани</returns>
	virtual Vector3 Color(Direction direction)
	{
		return Vector3(0.4, 0.75, 0.25); // все грани окрашиваются в зелёный
	}
	/// <summary>
	/// слой
	/// </summary>
	/// <returns>слой</returns>
	virtual BlockLayer GetLayer()
	{
		return BlockLayer::LEAVES; // слой листвы
	}
};
/// <summary>
/// блок листвы
/// </summary>
class BlockBirchLeaves : public BlockLeaves
{
public:
	using BlockLeaves::BlockLeaves;
	/// <summary>
	/// координаты текстуры
	/// </summary>
	/// <param name="direction"></param>
	/// <returns>вектор</returns>
	virtual Vector2 TileCoord(Direction direction)
	{
		return Vector2(4, 12);
	}
	/// <summary>
	/// цвет
	/// </summary>
	/// <param name="direction"></param>
	/// <returns>цвет грани</returns>
	virtual Vector3 Color(Direction direction)
	{
		return Vector3(0.5, 0.7, 0.33); // все грани окрашиваются в зелёный
	}
};
/// <summary>
/// блок стекла
/// </summary>
class BlockGlass : public Block
{
public:
	using Block::Block;
	/// <summary>
	/// координаты текстуры
	/// </summary>
	/// <param name="direction"></param>
	/// <returns>вектор</returns>
	virtual Vector2 TileCoord(Direction direction)
	{
		return Vector2(1, 12);
	}
	/// <summary>
	/// слой
	/// </summary>
	/// <returns>слой</returns>
	virtual BlockLayer GetLayer()
	{
		return BlockLayer::GLASS; // слой стекла
	}
	/// <summary>
	/// прозрачность
	/// </summary>
	/// <returns>прозрачность</returns>
	virtual bool Transparent()
	{
		return true;
	}
};
/// <summary>
/// технический блок пустоты
/// </summary>
class BlockNull : public Block
{
public:
	using Block::Block;
	/// <summary>
	/// данные меша
	/// </summary>
	virtual void GetMeshData(VertexData* data, UVData* uv, int x, int y, int z, Chunk* chunk)
	{

	}
	/// <summary>
	/// слой
	/// </summary>
	/// <returns>слой</returns>
	virtual BlockLayer GetLayer()
	{
		return BlockLayer::VOID; // слой пустоты
	}
	/// <summary>
	/// твёрдость
	/// </summary>
	/// <returns>твёрдость</returns>
	virtual bool Solid()
	{
		return false;
	}
};
/// <summary>
/// блок берёзы 
/// </summary>
class BlockBirch : public Block
{
public:
	using Block::Block;
	/// <summary>
	/// координаты текстуры
	/// </summary>
	/// <param name="direction"></param>
	/// <returns>вектор</returns>
	virtual Vector2 TileCoord(Direction direction)
	{
		switch (direction)
		{
		case Direction::Up:
		case Direction::Down:
			return Vector2(5, 14);

		}
		return Vector2(5, 8);
	}

};
/// <summary>
/// блок высокой травы 
/// </summary>
class BlockTallGrass : public Block
{
public:
	using Block::Block;
	/// <summary>
	/// координаты текстуры
	/// </summary>
	/// <param name="direction"></param>
	/// <returns>вектор</returns>
	virtual Vector2 TileCoord(Direction direction)
	{
		return Vector2(7, 13);
	}
	/// <summary>
	/// твёрдость
	/// </summary>
	/// <returns>твёрдость</returns>
	virtual bool Solid() 
	{
		return false;
	}

	/// <summary>
	/// данные меша
	/// </summary>
	virtual void GetMeshData(VertexData* data, UVData* uv, int x, int y, int z, Chunk* chunk)
	{
		// отличная от кубической геометрия
		float i = TileCoord(Direction::Up).x; // абсцисса текстуры
		float j = TileCoord(Direction::Up).y; // ордината текстуры

		// цвет
		Vector3 color = Vector3::clrm(chunk->GetLigthColor(x, y, z), Color(Direction::Up));
		ColorSquad squad{ color };
		
		data->FaceCrossRFLB(x, y, z, squad); // первая диагональная грань
		data->FaceCrossRBLF(x, y, z, squad); // вторая диагональная грань
		// добавление UV текстуры
		uv->AddUV(i, j);
		uv->AddUV(i, j);
		uv->AddUV(i, j);
		uv->AddUV(i, j);
	}
	/// <summary>
	/// прозрачность
	/// </summary>
	/// <returns>прозрачность</returns>
	virtual bool Transparent()
	{
		return true;
	}
	/// <summary>
	/// цвет
	/// </summary>
	/// <param name="direction"></param>
	/// <returns>цвет грани</returns>
	virtual Vector3 Color(Direction direction)
	{
		return grass->Color(Direction::Up);
	}
	/// <summary>
	/// слой
	/// </summary>
	/// <returns>слой</returns>
	virtual BlockLayer GetLayer()
	{
		return BlockLayer::HERB; // слой для кресто-образных объектов
	}
	/// <summary>
	/// границы
	/// </summary>
	/// <returns></returns>
	virtual Bound GetBounds() 
	{
		return Bound(.7f, .7f, .7f);
	}
};
/// <summary>
/// блок мака
/// </summary>
class BlockPoppy : public BlockTallGrass
{
public:
	using BlockTallGrass::BlockTallGrass;
	/// <summary>
	/// координаты текстуры
	/// </summary>
	/// <param name="direction"></param>
	/// <returns>вектор</returns>
	virtual Vector2 TileCoord(Direction direction)
	{
		return Vector2(12, 15);
	}
	/// <summary>
	/// цвет
	/// </summary>
	/// <param name="direction"></param>
	/// <returns>цвет грани</returns>
	virtual Vector3 Color(Direction direction)
	{
		return Vector3(1, 1, 1);
	}
	/// <summary>
	/// границы
	/// </summary>
	/// <returns></returns>
	virtual Bound GetBounds()
	{
		return Bound(.4f, .4f, .6f);
	}
};
/// <summary>
/// блок одуванчика
/// </summary>
class BlockDandelion : public BlockPoppy
{
public:
	using BlockPoppy::BlockPoppy;
	/// <summary>
	/// координаты текстуры
	/// </summary>
	/// <param name="direction"></param>
	/// <returns>вектор</returns>
	virtual Vector2 TileCoord(Direction direction)
	{
		return Vector2(13, 15);
	}
};
/// <summary>
/// блок досок
/// </summary>
class BlockPlanks : public Block
{
public:
	using Block::Block;
	/// <summary>
	/// координаты текстуры
	/// </summary>
	/// <param name="direction"></param>
	/// <returns>вектор</returns>
	virtual Vector2 TileCoord(Direction direction)
	{
		return Vector2(4, 15);
	}
};
/// <summary>
/// блок песка 
/// </summary>
class BlockSand : public Block
{
public:
	using Block::Block;
	/// <summary>
	/// координаты текстуры
	/// </summary>
	/// <param name="direction"></param>
	/// <returns>вектор</returns>
	virtual Vector2 TileCoord(Direction direction)
	{
		return Vector2(2, 14);
	}
};
/// <summary>
/// блок шерсти
/// </summary>
class BlockWool : public Block
{
public:
	using Block::Block;
	/// <summary>
	/// координаты текстуры
	/// </summary>
	/// <param name="direction"></param>
	/// <returns>вектор</returns>
	virtual Vector2 TileCoord(Direction direction)
	{
		return Vector2(0, 11);
	}
};
/// <summary>
/// блок тнт 
/// </summary>
class BlockTNT : public Block
{
public:
	using Block::Block;
	/// <summary>
	/// координаты текстуры
	/// </summary>
	/// <param name="id"></param>
	/// <param name="name"></param>
	virtual Vector2 TileCoord(Direction direction)
	{
		switch (direction)
		{
		case Direction::Up:
			return Vector2(9, 15);
		case Direction::Down:
			return Vector2(10, 15);

		}
		return Vector2(8, 15);
	}
};
/// <summary>
/// блок обсидиана
/// </summary>
class BlockObsidian : public Block
{
public:
	using Block::Block;
	/// <summary>
	/// координаты текстуры
	/// </summary>
	/// <param name="direction"></param>
	/// <returns>вектор</returns>
	virtual Vector2 TileCoord(Direction direction)
	{
		return Vector2(7, 4);
	}
};
/// <summary>
/// блок обсидиана
/// </summary>
class BlockBricks : public Block
{
public:
	using Block::Block;
	/// <summary>
	/// координаты текстуры
	/// </summary>
	/// <param name="direction"></param>
	/// <returns>вектор</returns>
	virtual Vector2 TileCoord(Direction direction)
	{
		return Vector2(7, 15);
	}
};
/// <summary>
/// блок коренной породы
/// </summary>
class BlockBedrock : public Block
{
public:
	using Block::Block;
	/// <summary>
	/// координаты текстуры
	/// </summary>
	/// <param name="direction"></param>
	/// <returns>вектор</returns>
	virtual Vector2 TileCoord(Direction direction)
	{
		return Vector2(1, 14);
	}
};
/// <summary>
/// блок обсидиана 
/// </summary>
class BlockGlowstone : public Block
{
public:
	using Block::Block;
	/// <summary>
	/// координаты текстуры
	/// </summary>
	/// <param name="direction"></param>
	/// <returns></returns>
	virtual Vector2 TileCoord(Direction direction)
	{
		return Vector2(9, 9);
	}
	/// <summary>
	/// светимость
	/// </summary>
	/// <returns>яркость</returns>
	virtual char Luminosity()
	{
		return Chunk::MaxLight;
	}
};
// ссылки на статичные экземпляры блоков
Block* Block::null = new BlockNull(-1, "none");
Block* Block::air = new BlockAir(0, "air");
Block* Block::stone = new BlockStone(1, "stone");
Block* Block::dirt = new BlockDirt(2, "dirt");
Block* Block::cobblestone = new BlockCobblestone(3, "cobblestone");
Block* Block::grass = new BlockGrass(4, "grass");
Block* Block::oak = new BlockOak(5, "oak");
Block* Block::glass = new BlockGlass(6, "glass");
Block* Block::leaves = new BlockLeaves(7, "leaves");
Block* Block::birch = new BlockBirch(8, "birch");
Block* Block::tall_grass = new BlockTallGrass(9, "tall_grass");
Block* Block::poppy = new BlockPoppy(10, "poppy");
Block* Block::dandelion = new BlockDandelion(11, "dandelion");
Block* Block::planks = new BlockPlanks(12, "planks");
Block* Block::sand = new BlockSand(13, "sand");
Block* Block::wool = new BlockWool(14, "wool");
Block* Block::tnt = new BlockTNT(15, "tnt");
Block* Block::obsidian = new BlockObsidian(16, "obsidian");
Block* Block::glowstone = new BlockGlowstone(17, "glowstone");
Block* Block::birch_leaves = new BlockBirchLeaves(18, "birch_leaves");
Block* Block::bedrock = new BlockBedrock(19, "bedrock");
Block* Block::mossy_cobblestone = new BlockMossyCobblestone(20, "mossy_cobblestone");
Block* Block::bricks = new BlockBricks(21, "bricks");