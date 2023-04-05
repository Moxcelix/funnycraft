#include "Client.h"

void Client::Init()
{
	modifier = new Modifier(this);

	World::settings.Clear();		// очистка по умолчанию параметров генерации
	Block::Init();					// инициализация массива блоков
	player = Player();				// инициализация персонажа
	MenuPos = Vector2Int(0, 0);		// позиция в меню
	world = new World(&player);		// инициализация мира
	page = &page_menu;				// ссылка на страницу меню

	ui.SetFlags(UI_DRAW_CROSS);
	Menu.SetFlags(UI_DRAW_BACK_GROUND);

	Resize(); // изменения размера содержимого

	// цвета меню
	Vector3 gray = Vector3(0.2, 0.2, 0.2);
	Vector3 gray2 = Vector3(0.3, 0.3, 0.3);
	Vector3 colors[4]{ gray, gray, gray2, gray2 };
	ColorSquad squad{ colors };

	Menu.AddStaticPlane(UI::Corner::middle, 1, 0, 0, 480, 360, squad); // задний фон меню
	inventory.Init(); // инициализация инвентаря 
}
/// <summary>
/// активность меню 
/// </summary>
/// <param name="active"></param>
void Client::Pause(bool active)
{
	if (active) // если активно
	{
		ToMenu();	// в меню
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // включение отображения курсора
	}
	else
	{
		int width, height;
		glfwGetWindowSize(window, &width, &height);						// получение размеров окна
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);	// выключение отображения курсора
		glfwSetCursorPos(window, width / 2, height / 2);				// установка курсора по середине экрана
	}
	pause = active; // установка значения актвиности
}
/// <summary>
/// установка блока 
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
/// <param name="id"></param>
void Client::SetBlock(int x, int y, int z, block_id id)
{
	if (id && RigidBox::IsLocked(Vector3Int{x,y,z}))
		return;

	if (player.looking && !pause) // если не пауза и игрок смотрит на блок
	{
		world->SetBlock(x, y, z, id, true); // установить блок с обновлением мира
	}
}
/// <summary>
/// установка блока 
/// </summary>
/// <param name="pos"></param>
/// <param name="id"></param>
void Client::SetBlock(Vector3 pos, block_id id)
{
	if (id && RigidBox::IsLocked(Vector3Int(pos.x, pos.y, pos.z)))
		return;

	if (player.looking && !pause) // если не пауза и игрок смотрит на блок
	{
		world->SetBlock(pos.x, pos.y, pos.z, id, true); // установить блок с обновлением мира
	}
}
/// <summary>
/// установка блока 
/// </summary>
/// <param name="pos"></param>
/// <param name="id"></param>
void Client::SetBlock()
{
	auto id = inventory.current_block;

	if (id && RigidBox::IsLocked(player.add_pos))
		return;

	if (player.looking && !pause) // если не пауза и игрок смотрит на блок
		world->SetBlock(player.add_pos, id, true); // установить блок с обновлением мира
}
/// <summary>
/// разрушение блока
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
void Client::DestroyBlock(int x, int y, int z)
{
	if (player.looking && !pause) // если не пауза и игрок смотрит на блок
	{
		Block* block = world->GetBlock(x, y, z); // получение блока
		particles.push_back(new Particles(block, world, { (float)x, (float)y, (float)z }, 20));
		world->SetBlock(x, y, z, 0, true); // установить блок с обновлением мира
	}
}
/// <summary>
/// разрушение блока
/// </summary>
/// <param name="pos"></param>
void Client::DestroyBlock(Vector3 pos)
{
	DestroyBlock(pos.x, pos.y, pos.z); // вызов полиморфа функции 
}
/// <summary>
/// разрушение блока
/// </summary>
/// <param name="pos"></param>
void Client::DestroyBlock()
{
	DestroyBlock(player.look_pos); // вызов полиморфа функции 
}
/// <summary>
/// Обновление
/// </summary>
/// <param name="deltaTime"></param>
void Client::Update(float deltaTime) 
{
	player.Update();				// обновление игрока
	world->Update();				// обновление мира
	player.LoadTerrain(world);		// прогрузка территории
	inventory.Update();				// обновление инвенторя
	modifier->Update();				// обновления модификации мира
	
	vector<int> ToRemove;			// вектор на удаление частиц

	for (int i = 0; i < particles.size(); i++) // перебор всех систем частиц
	{
		if (particles[i]->working)				// если работает
			particles[i]->Update(deltaTime);	// обновление системы
		else
			ToRemove.push_back(i);				// иначе добавить на очистку
	}

	for (int i = ToRemove.size() - 1; i >= 0; i--) 
	{
		particles[ToRemove[i]]->Clear();					// очистка данных системы частиц
		particles.erase(particles.begin() + ToRemove[i]);	// удаление системы частиц из вектора
	}

	ToRemove.clear();				// очиска вектора на удаление
	RigidBox::ClearCollisionMap();	// очистка карты коллизий

	MoveCamera(deltaTime);			// движение камеры
}
/// <summary>
/// переход к странице настроек 
/// </summary>
void Client::ToSettings()
{
	page = &page_settings;		// страница настроек
	MenuPos = Vector2Int(0, 0);	// обнуление позиции в меню
}
/// <summary>
/// переход к главной странице меню
/// </summary>
void Client::ToMenu()
{
	page = &page_menu;			// страница меню
	MenuPos = Vector2Int(0, 0);	// обнуление позиции в меню
}
/// <summary>
/// переход к странице согласия
/// </summary>
void Client::ToAgree()
{
	page = &page_agree;			// страница подтверждения
	MenuPos = Vector2Int(0, 0); // обнуление позиции в меню
}
/// <summary>
/// переход к странице создания нового мира
/// </summary>
void Client::ToNewWorld()
{
	page = &page_new_world;		// страница создания нового мира
	MenuPos = Vector2Int(0, 0);	// обнуление позиции в меню
}
/// <summary>
/// переход к настройкам дальности прорисовки
/// </summary>
void Client::ToRenderDistance()
{
	page = &page_render_distance;	// страница настройки дальности прорисовки
	MenuPos = Vector2Int(0, 0);		// обнуление позиции в меню
}
/// <summary>
/// создание нового мира
/// </summary>
void Client::NewWorld()
{
	for (auto p : particles) // очистка данных всех систем частиц
		p->Clear();

	particles.clear();		// очистка вектора систем частиц

	World::settings = gen_params;	// присвоение настроек создания
	delete world;					// удаление старого экземпляра

	for (const auto& entry : experimental::filesystem::directory_iterator(save_folder + World::name + "/"))
		std::experimental::filesystem::remove_all(entry.path()); // удаление файлов старого мира

	world = new World(&player);		// создание нового экземпляра мира
	player.world = world;			// обновление указателя на миры
	player.pos = player.start_pos;	// обнуление позицию игрока

	Pause(false);					// скрываем меню
}
/// <summary>
/// кнопка вверх
/// </summary>
void Client::ButtonUp()
{
	if (pause) // если пауза
		MenuPos.y -= 1; // позиция меню вниз
	else // иначе вниз в инвентаре
		if (inventory.current < inventory.count - 1) 
			inventory.current++;
		else
			inventory.current = 0;
}
/// <summary>
/// кнопка вверх
/// </summary>
void Client::ButtonDown()
{
	if (pause) // если пауза
		MenuPos.y += 1; // позиция меню вверх
	else // иначе вверх в инвентаре
		if (inventory.current > 0)
			inventory.current--;
		else
			inventory.current = inventory.count - 1;
}
/// <summary>
///  движение камеры
/// </summary>
/// <param name="delta_time"></param>
void Client::MoveCamera(double delta_time)
{
	float sensitivity = 5.1; // чувствительность 
	double posx, posy; // координаты курсора

	if (delta_time > 500) // в случае недопустимой для расчёта физики дельты времени
		return;
	
	player.can_move = !pause;

	if (!pause) // если не пауза
	{
		glfwGetCursorPos(window, &posx, &posy); // передаем позицю курсора по ссылкам
		player.RotateCamera((mouse_pos_y - posy) / sensitivity, (mouse_pos_x - posx) / sensitivity); // поворачиваем камеру, согласно смещению курсора
		glfwSetCursorPos(window, mouse_pos_x, mouse_pos_y); // возвращаем курсор на место
	}

	float rotation = -player.camera.zRot / 180 * M_PI; // поворот игрока
	player.rot = rotation;	// поворот игрока

	// обновление позиции камеры
	player.camera.x = player.pos.x;
	player.camera.y = player.pos.y;
	player.camera.z = player.pos.z + player.head_height - 0.5f;

	// обновление двидения игрового персонажа
	player.MoveUpdate();
}
/// <summary>
/// установка дальности прорисовки 
/// </summary>
/// <param name="d"></param>
void Client::SetRenderDistance(int d)
{
	world->Clear(); // очистка старых чанков
	World::render_distance = d; // обновление дальности прорисовки
	player.GenereateSphere(); // генерация сферы видимости
}
/// <summary>
/// отрисовка меню
/// </summary>
void Client::DrawMenu()
{
	// если позиция меню ваходит за границы, зацикливаем значение
	if (MenuPos.y < 0)
		MenuPos.y = 0;
	if (MenuPos.y >= page->count)
		MenuPos.y = page->count - 1;

	float indent = 0; // отступ

	Menu.PrintText(UI::Corner::middle, 3, 0, 120, page->name, 1, 1, 0); // вывод на экран заголовка
	for (int i = 0; i < page->count; i++) // вывод всех кнопок
	{
		float r = 1, g = 1, b = 1;				// каналы цветов
		string btn = page->buttons[i]->name;	// имя кнопки
		indent += page->indents[i];				// добавление отступа

		if (MenuPos.y == i) // если выбрана i-тая кнопка
		{
			// выделение кнопки символами и цветом
			btn = ">" + btn + "<"; 
			g = 0;
			b = 0;
		}
		// вывод строки
		Menu.PrintText(UI::Corner::middle, 3, 0,
			(page->count - i - 1 - page->count / 2) * 30 - indent,
			btn, r, g, b);
	}

	if (enter) // вызов метода по ссылке, согласно выбранной кнопке
	{
		enter = false;	// падение флага
		page->buttons[MenuPos.y]->DoFunc();	// вызов метода
	}

	Menu.Render(); // рендеринг меню
}
/// <summary>
/// обновление позиции фантома 
/// </summary>
void Client::UpdatePhantomPos()
{
	float length; // длина луча
	Vector3 hit; // позиция поподания
	Vector3 ref; // позиция отражения луча по нормали

	// расчёт тригонометрических функций
	double cosA = cos((player.camera.xRot - 90) / 180. * M_PI);
	double cosB = cos((player.camera.zRot + 90) / 180. * M_PI);
	double sinA = sin((player.camera.xRot - 90) / 180. * M_PI);
	double sinB = sin((player.camera.zRot + 90) / 180. * M_PI);

	Vector3 dir = Vector3(cosA * cosB, cosA * sinB, sinA); // вектор луча по полярным координатам

	Ray ray(world, Vector3(player.camera.x, player.camera.y, player.camera.z), dir); // создание луча
	if (player.looking = ray.Trace(7, length, hit, ref)) // если луч попал на блок
	{
		// позиция, куда смотрит игровой персонаж
		player.look_pos = hit;
		// позиция, куда нужно будет помещать поставленный блок
		player.add_pos = ref;

		phantom.Render(hit.x, hit.y, hit.z, world); // отображаем фантом
	}
}
/// <summary>
/// обновление камеры
/// </summary>
void Client::CameraUpdate()
{
	glRotatef(-player.camera.xRot, 1, 0, 0); // поворот по оси X
	glRotatef(-player.camera.zRot, 0, 0, 1); // поворот по оси Z
	glRotatef((sin(player.phasa) - 0.5) * 0.1, 0, 1, 0); // покачивание камеры
	glTranslatef(-player.camera.x, -player.camera.y, -player.camera.z); // перемещение точки обзора на позицию камеры
}
/// <summary>
/// рендеринг 
/// </summary>
void Client::Render()
{
	Resize(); // изменение размера содержимого

	world->Render(main_texture); // Рендеринг чанков

	if (!pause) // Если не пауза
		UpdatePhantomPos(); // Обновить Позицию Фантома

	for (int i = 0; i < particles.size(); i++)
	{
		if (particles[i]->working)	// если система частиц работает
			particles[i]->Render(main_texture);	// рендеринг системы частиц
	}

	player.body->Render();
}
/// <summary>
/// измениение размера и масштаб содержимого окна
/// </summary>
void Client::Resize()
{
	int width, height;	// ширина и высота
	glfwGetWindowSize(window, &width, &height); // Получение данных о размерах окна

	mouse_pos_x = width / 2;	// позиция курсора абсцисса
	mouse_pos_y = height / 2;	// позиция курсора ордината

	//Измениение размера и масштаб содержимого окна
	ui.SetSize(width, height);
	Menu.SetSize(width, height);
	inventory.ui.SetSize(width, height);
}
/// <summary>
/// рендеринг пользовательского интерфейса
/// </summary>
void Client::RenderUI()
{
	ui.Render();

	if (pause) // если пауза, рисуем меню
		DrawMenu();
	else // если нет, рисуем инвентарь
		inventory.DrawInventory();
}
/// <summary>
/// копирование блока
/// </summary>
void Client::CopyBlock() 
{
	inventory.AddBlock(world->GetBlockID(player.look_pos));
}
/// <summary>
/// очистка
/// </summary>
void Client::Clear()
{
	pause = false;
	close = false;
	ui.Clear();
	Menu.Clear();
	inventory.ui.Clear();
	delete world; // удаление мира и вызов деструктора
}
