#include "Inventory.h"
/// <summary>
/// отрисовка инвентаря
/// </summary>
void Inventory::DrawInventory()
{
	int size = 30; // расстояние между строк

	for (int i = 0; i < count; i++)
	{
		// цвет надписей по умолчанию - светло-серый
		float r = 0.9;
		float g = 0.9;
		float b = 0.9;

		if (i == current) // цвет выбранного блока - красный
			b = 0, g = 0, r = 1;

		//вывод на экран текста справа
		ui.PrintText(UI::Corner::right, 3, 0, (i - count / 2)*size, Block::GetBlock(data[i])->name + " <", r, g, b);
	}
	ui.Render(); // ренендеринг пользовательсого интерфейса инвентаря 
}
/// <summary>
/// обновленире инвентаря
/// </summary>
void Inventory::Update()
{
	current_block = data[current]; // текущий блок 
}
/// <summary>
/// добавление блока в инвентарь
/// </summary>
/// <param name="id"></param>
void Inventory::AddBlock(block_id id)
{
	for (int i = 0; i < count; i++) // проверка наличия блока в инвентаре
	{
		if (data[i] == id) // если блок есть, делаем его слот активным
		{
			current = i; 
			break;
		}
	}
	data[current] = id; // если нет, добавляем его в активный слот
}
/// <summary>
/// инициализация инвентаря 
/// </summary>
void Inventory::Init()
{
	data[0] = Block::stone->id;
	data[1] = Block::cobblestone->id;
	data[2] = Block::oak->id;
	data[3] = Block::glass->id;
	data[4] = Block::dirt->id;
	data[5] = Block::planks->id;
	data[6] = Block::grass->id;
	data[7] = Block::sand->id;
	data[8] = Block::wool->id;
	data[9] = Block::tnt->id;
	data[10] = Block::obsidian->id;
	data[11] = Block::glowstone->id;
	data[12] = Block::mossy_cobblestone->id;
	data[13] = Block::bricks->id;
}