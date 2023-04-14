#include "Block.h"
struct Chunk;
vector<Block*> Block::BlockList = vector<Block*>();
Block** Block::blocks = new Block * ();

Block::Block() {}

void Block::GetMeshData(VertexData* data, UVData* uv,
	int x, int y, int z, Chunk* chunk) const
{
	if (chunk->GetBlock(x, y, z + 1)->GetLayer() != GetLayer()
		&& chunk->GetBlock(x, y, z + 1) != null) {
		Vector3 color = Vector3::clrm(chunk->GetLigthColor(x, y, z + 1), Color(Direction::Up));
		ColorSquad squad{ color };
		data->FaceUp(x, y, z, squad);

		float i = TileCoord(Direction::Up).x;
		float j = TileCoord(Direction::Up).y;

		uv->AddUV(i, j);
	}

	if (chunk->GetBlock(x, y, z - 1)->GetLayer() != GetLayer()
		&& chunk->GetBlock(x, y, z - 1) != null) {
		Vector3 color = Vector3::clrm(chunk->GetLigthColor(x, y, z - 1), Color(Direction::Down));
		ColorSquad squad{ color };
		data->FaceDown(x, y, z, squad);

		float i = TileCoord(Direction::Down).x;
		float j = TileCoord(Direction::Down).y;

		uv->AddUV(i, j);
	}
	if (chunk->GetBlock(x, y + 1, z)->GetLayer() != GetLayer()
		&& chunk->GetBlock(x, y + 1, z) != null) {
		Vector3 color = Vector3::clrm(chunk->GetLigthColor(x, y + 1, z), Color(Direction::Front));
		ColorSquad squad{ color };
		data->FaceFront(x, y, z, squad);

		float i = TileCoord(Direction::Front).x;
		float j = TileCoord(Direction::Front).y;

		uv->AddUV(i, j);
	}
	if (chunk->GetBlock(x, y - 1, z)->GetLayer() != GetLayer()
		&& chunk->GetBlock(x, y - 1, z) != null) {
		Vector3 color = Vector3::clrm(chunk->GetLigthColor(x, y - 1, z), Color(Direction::Back));
		ColorSquad squad{ color };
		data->FaceBack(x, y, z, squad);

		float i = TileCoord(Direction::Back).x;
		float j = TileCoord(Direction::Back).y;

		uv->AddUV(i, j);
	}
	if (chunk->GetBlock(x + 1, y, z)->GetLayer() != GetLayer()
		&& chunk->GetBlock(x + 1, y, z) != null) {
		Vector3 color = Vector3::clrm(chunk->GetLigthColor(x + 1, y, z), Color(Direction::Right));
		ColorSquad squad{ color };
		data->FaceRight(x, y, z, squad);

		float i = TileCoord(Direction::Right).x;
		float j = TileCoord(Direction::Right).y;

		uv->AddUV(i, j);
	}
	if (chunk->GetBlock(x - 1, y, z)->GetLayer() != GetLayer()
		&& chunk->GetBlock(x - 1, y, z) != null) {
		Vector3 color = Vector3::clrm(chunk->GetLigthColor(x - 1, y, z), Color(Direction::Left));
		ColorSquad squad{ color };
		data->FaceLeft(x, y, z, squad);

		float i = TileCoord(Direction::Left).x;
		float j = TileCoord(Direction::Left).y;

		uv->AddUV(i, j);
	}
}

Bound Block::GetBounds() const {
	return Bound();
}

bool Block::Solid() const {
	return true;
}

bool Block::Transparent() const {
	return false;
}

char Block::Luminosity() const {
	return 0;
}

Vector3 Block::Color(Direction direction) const {
	return Vector3(1, 1, 1);
}

Vector2 Block::TileCoord(Direction direction) const {
	return Vector2(0, 0);
}

Block::BlockLayer Block::GetLayer() const {
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

char Block::GetRenderLayer() const {
	return 0;
}

class BlockAir : public Block{
public:
	using Block::Block;

	virtual bool Solid() const override {
		return false;
	}

	virtual void GetMeshData(VertexData* data, UVData* uv,
		int x, int y, int z, Chunk* chunk) const override {}

	virtual BlockLayer GetLayer() const override {
		return BlockLayer::AIR;
	}

	virtual bool Transparent() const override {
		return true;
	}
};

class BlockGrass : public Block
{
public:
	using Block::Block;

	virtual Vector2 TileCoord(Direction direction) const override {
		switch (direction)
		{
		case Direction::Up:
			return Vector2(0, 15);
		case Direction::Down:
			return Vector2(2, 15);

		}
		return Vector2(3, 15);
	}

	virtual Vector3 Color(Direction direction) const override {
		if (direction == Direction::Up)
			return Vector3(0.56, 0.81, 0.3);

		return Vector3(1, 1, 1);
	}

};

class BlockStone : public Block {
public:
	using Block::Block;

	virtual Vector2 TileCoord(Direction direction) const override {
		return Vector2(1, 15);
	}
};

class BlockDirt : public Block {
public:
	using Block::Block;

	virtual Vector2 TileCoord(Direction direction) const override {
		return Vector2(2, 15);
	}
};

class BlockOak : public Block {
public:
	using Block::Block;

	virtual Vector2 TileCoord(Direction direction) const override {
		switch (direction)
		{
		case Direction::Up:
		case Direction::Down:
			return Vector2(5, 14);

		}
		return Vector2(4, 14);
	}
};

class BlockCobblestone : public Block {
public:
	using Block::Block;

	virtual Vector2 TileCoord(Direction direction) const override {
		return Vector2(0, 14);
	}
};

class BlockMossyCobblestone : public Block {
public:
	using Block::Block;

	virtual Vector2 TileCoord(Direction direction) const override {
		return Vector2(4, 13);
	}
};

class BlockLeaves : public Block {
public:
	using Block::Block;

	virtual void GetMeshData(VertexData* data, UVData* uv,
		int x, int y, int z, Chunk* chunk)  const override {
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

	virtual Vector2 TileCoord(Direction direction) const override {
		return Vector2(4, 12);
	}

	virtual Vector3 Color(Direction direction)  const override {
		return Vector3(0.4, 0.75, 0.25);
	}

	virtual BlockLayer GetLayer() const override {
		return BlockLayer::LEAVES;
	}
};

class BlockBirchLeaves : public BlockLeaves {
public:
	using BlockLeaves::BlockLeaves;

	virtual Vector2 TileCoord(Direction direction) const override {
		return Vector2(4, 12);
	}

	virtual Vector3 Color(Direction direction) const override {
		return Vector3(0.5, 0.7, 0.33);
	}
};

class BlockGlass : public Block {
public:
	using Block::Block;

	virtual Vector2 TileCoord(Direction direction) const override {
		return Vector2(1, 12);
	}

	virtual BlockLayer GetLayer() const override {
		return BlockLayer::GLASS;
	}

	virtual bool Transparent() const override {
		return true;
	}
};

class BlockNull : public Block {
public:
	using Block::Block;

	virtual void GetMeshData(VertexData* data, UVData* uv,
		int x, int y, int z, Chunk* chunk) const override {}

	virtual BlockLayer GetLayer() const override {
		return BlockLayer::VOID;
	}

	virtual bool Solid() const override {
		return false;
	}
};

class BlockBirch : public Block {
public:
	using Block::Block;

	virtual Vector2 TileCoord(Direction direction) const override {
		switch (direction) {
		case Direction::Up:
		case Direction::Down:
			return Vector2(5, 14);

		}
		return Vector2(5, 8);
	}

};

class BlockTallGrass : public Block {
public:
	using Block::Block;

	virtual Vector2 TileCoord(Direction direction) const override {
		return Vector2(7, 13);
	}

	virtual bool Solid() const override {
		return false;
	}

	virtual void GetMeshData(VertexData* data, UVData* uv,
		int x, int y, int z, Chunk* chunk) const override {
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

	virtual bool Transparent() const override {
		return true;
	}

	virtual Vector3 Color(Direction direction) const override {
		return grass->Color(Direction::Up);
	}

	virtual BlockLayer GetLayer() const override {
		return BlockLayer::HERB;
	}

	virtual Bound GetBounds() const override {
		return Bound(.7f, .7f, .7f);
	}
};

class BlockPoppy : public BlockTallGrass {
public:
	using BlockTallGrass::BlockTallGrass;

	virtual Vector2 TileCoord(Direction direction) const override {
		return Vector2(12, 15);
	}

	virtual Vector3 Color(Direction direction) const override {
		return Vector3(1, 1, 1);
	}

	virtual Bound GetBounds() const override {
		return Bound(.4f, .4f, .6f);
	}
};

class BlockDandelion : public BlockPoppy {
public:
	using BlockPoppy::BlockPoppy;

	virtual Vector2 TileCoord(Direction direction) const override {
		return Vector2(13, 15);
	}
};

class BlockPlanks : public Block {
public:
	using Block::Block;

	virtual Vector2 TileCoord(Direction direction) const override {
		return Vector2(4, 15);
	}
};

class BlockSand : public Block {
public:
	using Block::Block;

	virtual Vector2 TileCoord(Direction direction) const override {
		return Vector2(2, 14);
	}
};

class BlockTNT : public Block {
public:
	using Block::Block;

	virtual Vector2 TileCoord(Direction direction) const override {
		switch (direction) {
		case Direction::Up:
			return Vector2(9, 15);
		case Direction::Down:
			return Vector2(10, 15);

		}
		return Vector2(8, 15);
	}
};

class BlockObsidian : public Block {
public:
	using Block::Block;

	virtual Vector2 TileCoord(Direction direction) const override {
		return Vector2(7, 4);
	}
};

class BlockBricks : public Block {
public:
	using Block::Block;

	virtual Vector2 TileCoord(Direction direction) const override {
		return Vector2(7, 15);
	}
};

class BlockBedrock : public Block {
public:
	using Block::Block;

	virtual Vector2 TileCoord(Direction direction) const override {
		return Vector2(1, 14);
	}
};

class BlockGlowstone : public Block {
public:
	using Block::Block;

	virtual Vector2 TileCoord(Direction direction) const override {
		return Vector2(9, 9);
	}

	virtual char Luminosity() const override {
		return Chunk::MaxLight;
	}
};

class BlockSandstone : public Block {
public:
	using Block::Block;

	virtual Vector2 TileCoord(Direction direction) const override {
		switch (direction)
		{
		case Direction::Up:
			return Vector2(0, 4);
		case Direction::Down:
			return Vector2(0, 2);
		}
		return Vector2(0, 3);
	}
};

class BlockCactus : public Block {
public:
	using Block::Block;

	virtual Vector2 TileCoord(Direction direction) const override {
		switch (direction)
		{
		case Direction::Up:
			return Vector2(5, 11);
		case Direction::Down:
			return Vector2(7, 11);
		}
		return Vector2(6, 11);
	}

	virtual void GetMeshData(VertexData* data, UVData* uv,
		int x, int y, int z, Chunk* chunk) const override {
		const auto shift = 1.f / 16.f;
		if (chunk->GetBlock(x, y, z + 1)->GetLayer() != GetLayer()
			&& chunk->GetBlock(x, y, z + 1) != null) {
			Vector3 color = Vector3::clrm(chunk->GetLigthColor(x, y, z + 1), Color(Direction::Up));
			ColorSquad squad{ color };
			data->FaceUp(x, y, z, squad);

			float i = TileCoord(Direction::Up).x;
			float j = TileCoord(Direction::Up).y;

			uv->AddUV(i, j);
		}

		if (chunk->GetBlock(x, y, z - 1)->GetLayer() != GetLayer()
			&& chunk->GetBlock(x, y, z - 1) != null) {
			Vector3 color = Vector3::clrm(chunk->GetLigthColor(x, y, z - 1), Color(Direction::Down));
			ColorSquad squad{ color };
			data->FaceDown(x, y, z, squad);

			float i = TileCoord(Direction::Down).x;
			float j = TileCoord(Direction::Down).y;

			uv->AddUV(i, j);
		}

		Vector3 color = Vector3::clrm(chunk->GetLigthColor(x, y + 1, z), Color(Direction::Front));
		ColorSquad squad{ color };
		data->FaceFront(x, y - shift, z, squad);

		float i = TileCoord(Direction::Front).x;
		float j = TileCoord(Direction::Front).y;

		uv->AddUV(i, j);

		color = Vector3::clrm(chunk->GetLigthColor(x, y - 1, z), Color(Direction::Back));
		squad = color;
		data->FaceBack(x, y + shift, z, squad);

		i = TileCoord(Direction::Back).x;
		j = TileCoord(Direction::Back).y;

		uv->AddUV(i, j);

		color = Vector3::clrm(chunk->GetLigthColor(x + 1, y, z), Color(Direction::Right));
		squad = color;
		data->FaceRight(x - shift, y, z, squad);

		i = TileCoord(Direction::Right).x;
		j = TileCoord(Direction::Right).y;

		uv->AddUV(i, j);

		color = Vector3::clrm(chunk->GetLigthColor(x - 1, y, z), Color(Direction::Left));
		squad = color;
		data->FaceLeft(x + shift, y, z, squad);

		i = TileCoord(Direction::Left).x;
		j = TileCoord(Direction::Left).y;

		uv->AddUV(i, j);
	}

	virtual BlockLayer GetLayer() const override {
		return BlockLayer::CACTUS;
	}

	virtual bool Transparent() const override {
		return true;
	}
};

class BlockWool : public Block {
private:
	Vector3 color;
public:
	using Block::Block;

	BlockWool(block_id id, string name, Vector3 color) : Block(id, name), color(color){}

	virtual Vector2 TileCoord(Direction direction) const override {
		return Vector2(0, 11);
	}

	virtual Vector3 Color(Direction direction) const override {
		return this->color;
	}
};

const Block* Block::null = new BlockNull(-1, "none");
const Block* Block::air = new BlockAir(0, "air");
const Block* Block::stone = new BlockStone(1, "stone");
const Block* Block::dirt = new BlockDirt(2, "dirt");
const Block* Block::cobblestone = new BlockCobblestone(3, "cobblestone");
const Block* Block::grass = new BlockGrass(4, "grass");
const Block* Block::oak = new BlockOak(5, "oak");
const Block* Block::glass = new BlockGlass(6, "glass");
const Block* Block::leaves = new BlockLeaves(7, "leaves");
const Block* Block::birch = new BlockBirch(8, "birch");
const Block* Block::tall_grass = new BlockTallGrass(9, "tall_grass");
const Block* Block::poppy = new BlockPoppy(10, "poppy");
const Block* Block::dandelion = new BlockDandelion(11, "dandelion");
const Block* Block::planks = new BlockPlanks(12, "planks");
const Block* Block::sand = new BlockSand(13, "sand");
const Block* Block::cactus = new BlockCactus(14, "cactus");
const Block* Block::tnt = new BlockTNT(15, "tnt");
const Block* Block::obsidian = new BlockObsidian(16, "obsidian");
const Block* Block::glowstone = new BlockGlowstone(17, "glowstone");
const Block* Block::birch_leaves = new BlockBirchLeaves(18, "birch_leaves");
const Block* Block::bedrock = new BlockBedrock(19, "bedrock");
const Block* Block::mossy_cobblestone = new BlockMossyCobblestone(20, "mossy_cobblestone");
const Block* Block::bricks = new BlockBricks(21, "bricks");
const Block* Block::sandstone = new BlockSandstone(22, "sandstone");
const Block* Block::wool_white = new BlockWool(23, "wool_white", {1.f, 1.f, 1.f});
const Block* Block::wool_black = new BlockWool(24, "wool_black", {.1f, .1f, .1f});