#include "Client.h"
#include <filesystem>

Client::Settings Client::settings = Client::Settings();

Client::Client() {
	modifier = new Modifier(this);
}

void Client::Init() {
	inventory.Init();
	World::settings.Clear();

	player = Player();

	ui.SetFlags(UI_DRAW_CROSS);
	menu.SetFlags(UI_DRAW_BACK_GROUND);

	Vector3 gray = Vector3(0.2f, 0.2f, 0.2f);
	Vector3 gray2 = Vector3(0.3f, 0.3f, 0.3f);
	Vector3 colors[4]{ gray, gray, gray2, gray2 };
	ColorSquad squad{ colors };

	menu.AddStaticPlane(UI::Corner::middle, 1, 0, 0, 480, 360, squad);

	MenuPos = Vector2Int(0, 0);
	world = new World(&player);
	page = &page_menu;		

	Resize();
}

void Client::Pause(bool active) {
	if (active) {
		ToMenu();
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else {
		int width, height;
		glfwGetWindowSize(window, &width, &height);					
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPos(window, width / 2., height / 2.);			
	}
	pause = active; 
}

void Client::SetBlock(int x, int y, int z, block_id id) {
	if (id && RigidBox::IsLocked(Vector3Int{ x,y,z }))
		return;

	if (player.looking && !pause) {
		world->SetBlock(x, y, z, id, true);
	}
}

void Client::SetBlock(Vector3 pos, block_id id) {
	if (id && RigidBox::IsLocked(Vector3Int(pos.x, pos.y, pos.z)))
		return;

	if (player.looking && !pause) {
		world->SetBlock(pos.x, pos.y, pos.z, id, true);
	}
}

void Client::SetBlock() {
	auto id = inventory.current_block;

	if (id && RigidBox::IsLocked(player.add_pos))
		return;

	if (player.looking && !pause)
		world->SetBlock(player.add_pos, id, true);
}

void Client::DestroyBlock(int x, int y, int z) {
	if (player.looking && !pause)
	{
		const auto block = world->GetBlock(x, y, z);
		particles.push_back(new Particles(block, world, {
			static_cast<float>(x), 
			static_cast<float>(y), 
			static_cast<float>(z) }, 20));
		world->SetBlock(x, y, z, 0, true);
	}
}

void Client::DestroyBlock(Vector3 pos) {
	DestroyBlock(pos.x, pos.y, pos.z);
}

void Client::DestroyBlock() {
	DestroyBlock(player.look_pos);
}

void Client::Update(float deltaTime) {
	player.Update();				
	world->Update();				
	player.LoadTerrain(world);	
	inventory.Update();				
	modifier->Update();		
	world->time.Shift(deltaTime);

	std::vector<int> ToRemove;

	for (int i = 0; i < particles.size(); i++) {
		if (particles[i]->working)
			particles[i]->Update(deltaTime);
		else
			ToRemove.push_back(i);
	}

	for (int i = ToRemove.size() - 1; i >= 0; i--) {
		particles[ToRemove[i]]->Clear();
		particles.erase(particles.begin() + ToRemove[i]);
	}

	ToRemove.clear();
	RigidBox::ClearCollisionMap();

	MoveCamera(deltaTime);
}

void Client::ToSettings() {
	page = &page_settings;
	MenuPos = Vector2Int(0, 0);
}

void Client::ToMenu() {
	page = &page_menu;
	MenuPos = Vector2Int(0, 0);
}

void Client::ToTimeSet() {
	page = &page_time;
	MenuPos = Vector2Int(0, 0);	
}

void Client::ToAgree() {
	page = &page_agree;
	MenuPos = Vector2Int(0, 0);
}

void Client::ToNewWorld() {
	page = &page_new_world;
	MenuPos = Vector2Int(0, 0);
}

void Client::ToRenderDistance() {
	page = &page_render_distance;
	MenuPos = Vector2Int(0, 0);
}

void Client::NewWorld() {
	for (auto p : particles) {
		p->Clear();
	}

	particles.clear();

	World::settings = gen_params;
	delete world;

	for (const auto& entry : std::filesystem::directory_iterator(save_folder + World::name + "/"))
		std::filesystem::remove_all(entry.path());

	world = new World(&player);
	player.world = world;
	player.pos = player.start_pos;

	Pause(false);
}

void Client::ButtonUp() {
	if (pause) {
		MenuPos.y -= 1;
	}
	else {
		if (inventory.current < inventory.count - 1)
			inventory.current++;
		else
			inventory.current = 0;
	}
}

void Client::ButtonDown() {
	if (pause) {
		MenuPos.y += 1;
	}
	else {
		if (inventory.current > 0)
			inventory.current--;
		else
			inventory.current = inventory.count - 1;
	}
}

void Client::ButtonLeft() {
	MenuPos.x -= 1;
}

void Client::ButtonRight() {
	MenuPos.x += 1;
}

void Client::MoveCamera(double delta_time) {
	float sensitivity = 5.1;
	double posx, posy;

	if (delta_time > 500)
		return;

	player.can_move = !pause;

	if (!pause) {
		glfwGetCursorPos(window, &posx, &posy);
		player.RotateCamera(
			(mouse_pos_y - posy) / sensitivity,
			(mouse_pos_x - posx) / sensitivity);
		glfwSetCursorPos(window, mouse_pos_x, mouse_pos_y); 
	}

	float rotation = static_cast<float>(-player.camera.zRot / 180 * M_PI);
	player.rot = rotation;

	player.camera.x = player.pos.x;
	player.camera.y = player.pos.y;
	player.camera.z = player.pos.z + player.head_height - 0.5f;

	player.MoveUpdate();
}

void Client::SetRenderDistance(int d) {
	world->Clear();
	World::render_distance = d;
	player.GenereateSphere();
}

void Client::ReloadChunks() {
	world->Clear();
}

void Client::DrawMenu() {
	if (MenuPos.y < 0) MenuPos.y = 0;
	if (MenuPos.y >= page->count) MenuPos.y = page->count - 1;

	float indent = 0;

	menu.PrintText(UI::Corner::middle, 3, 0, 120, page->name, 1, 1, 0);
	for (int i = 0; i < page->count; i++) {
		float r = 1, g = 1, b = 1;
		std::string btn = page->buttons[i]->name;
		indent += page->indents[i];

		if (MenuPos.y == i) {
			btn = ">" + btn + "<";
			g = 0;
			b = 0;
		}

		menu.PrintText(UI::Corner::middle, 3, 0,
			(page->count - i - 1 - page->count / 2) * 30 - indent,
			btn, r, g, b);
	}

	if (enter) {
		enter = false;
		page->buttons[MenuPos.y]->DoFunc();
	}

	menu.Render();
}

void Client::UpdatePhantomPos() {
	float length;
	Vector3 hit;
	Vector3 ref;

	auto cosA = static_cast<float>(cos((player.camera.xRot - 90) / 180. * M_PI));
	auto cosB = static_cast<float>(cos((player.camera.zRot + 90) / 180. * M_PI));
	auto sinA = static_cast<float>(sin((player.camera.xRot - 90) / 180. * M_PI));
	auto sinB = static_cast<float>(sin((player.camera.zRot + 90) / 180. * M_PI));

	Vector3 dir = Vector3(cosA * cosB, cosA * sinB, sinA); // вектор луча по полярным координатам

	Ray ray(world, Vector3(player.camera.x, player.camera.y, player.camera.z), dir); // создание луча
	if (player.looking = ray.Trace(7, length, hit, ref)) {
		player.look_pos = hit;
		player.add_pos = ref;

		phantom.Render(
			static_cast<int>(hit.x),
			static_cast<int>(hit.y),
			static_cast<int>(hit.z), world);
	}
}

void Client::CameraUpdate() {
	glRotatef(-player.camera.xRot, 1, 0, 0);
	glRotatef(-player.camera.zRot, 0, 0, 1);
	glRotatef((sin(player.phasa) - 0.5f) * 0.1f, 0, 1, 0);
	glTranslatef(-player.camera.x, -player.camera.y, -player.camera.z);
}

void Client::Render() {
	Resize();

	world->Render(main_texture); 

	if (!pause) UpdatePhantomPos();

	for (int i = 0; i < particles.size(); i++) {
		if (particles[i]->working) {
			particles[i]->Render(main_texture);
		}
	}

	player.body->Render();
}

void Client::Resize() {
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	mouse_pos_x = width / 2.;
	mouse_pos_y = height / 2.;

	ui.SetSize(width, height);
	menu.SetSize(width, height);
	inventory.ui.SetSize(width, height);
}

void Client::RenderUI() {
	ui.Render();

	if (pause) DrawMenu();
	else inventory.DrawInventory();
}

void Client::CopyBlock() {
	inventory.AddBlock(world->GetBlockID(player.look_pos));
}

void Client::Clear() {
	pause = false;
	close = false;

	ui.Clear();
	menu.Clear();
	inventory.ui.Clear();

	delete world;
}
