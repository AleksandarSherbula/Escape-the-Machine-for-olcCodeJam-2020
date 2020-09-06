#include "Level.h"
#include "DynamicObject.h"

#include <sstream>
#include <fstream>

Level::Level(olc::PixelGameEngine* pge, std::shared_ptr<DynamicObject> player)
	:Object(pge)
{
	keyLock = std::make_unique<Object>(pge);
	keyLock->GetImage("KeyLock");
	
	keyLock2 = std::make_unique<Object>(pge);
	keyLock2->GetImage("KeyLock2");
	
	doorSwitch = std::make_unique<Object>(pge);
 	doorSwitch->GetImage("Switch");
	doorSwitch->tileID = { 0, 0 };
	
	doorSwitch2 = std::make_unique<Object>(pge);
	doorSwitch2->GetImage("Switch2");
	doorSwitch2->tileID = { 0, 0 };

	bridgeSwitch = std::make_unique<Object>(pge);
	bridgeSwitch->GetImage("BridgeSwitch");
	bridgeSwitch->tileID = { 0, 0 };
	
	for (int i = 0; i < colorSwitches.size(); i++)
	{
		colorSwitches[i] = std::make_unique<Object>(pge);
		colorSwitches[i]->GetImage("ColorSwitches");
	}
	
	for (int i = 0; i < colorFloors.size(); i++)
	{
		colorFloors[i] = std::make_unique<Object>(pge);		
		colorFloors[i]->GetImage("ColorFloors");
	}	
	
	this->player = player;
	
	current = 1;
	
	Load();
	
	rightLevel = false;
	resetTimer = true;
}

Level::~Level()
{
	floorSwappers.clear();	
}

char Level::GetTile(const olc::vi2d& id)
{
	if (id.x >= 0 && id.x < Width && id.y >= 0 && id.y < Height)
		return map[id.y * Width + id.x];
	else
		return ' ';
}

void Level::SetTile(const olc::vi2d& id, char c)
{
	if (id.x >= 0 && id.x < Width && id.y >= 0 && id.y < Height)
		map[id.y * Width + id.x] = c;
}

void Level::PlayerColorChange()
{
	if (GetTile(player->position) == 'r')
		player->color = olc::RED;
	else if (GetTile(player->position) == 'g')
		player->color = olc::GREEN;
	else if (GetTile(player->position) == 'b')
		player->color = olc::BLUE;
}

void Level::FloorColorClear(char id, const olc::Pixel& color)
{
	if (GetTile(player->position) == id)
	{
		if (player->color == color)
		{
			SetTile(player->position, '0');
		}
		else
			player->shrink = true;
	}
}

void Level::Update(float fElapsedTime, bool& gameEnd)
{	
	if (player->position.x > pge->ScreenWidth() / 16)
	{
		current++;
	
		player->position = { 0.0f, 17.0f };
		player->color = olc::BLUE;
		Load();
	}
	else if (player->position.x < 0.0f)
	{
		player->position = { (float)pge->ScreenWidth() / 16 - 1.0f, 12.0f };
		current--;
		player->color = olc::BLUE;
		Load();
	}

	ActivateSwitch();
	PlayerColorChange();
	FloorColorClear('R', olc::RED);
	FloorColorClear('G', olc::GREEN);
	FloorColorClear('B', olc::BLUE);
	
	if (GetTile(player->position) == '.')
		player->shrink = true;	
	
	FloorSwap(fElapsedTime);
	
	if (player->shrink)
		player->Shrink(fElapsedTime, this, gameEnd);
}

void Level::ActivateSwitch()
{
	char tileID = GetTile(player->position);
	switch (tileID)
	{
	case 'O': doorSwitch->tileID = { 1, 0 };	break;
	case 'o': doorSwitch2->tileID = { 1, 0 };	break;
	case '_': bridgeSwitch->tileID = { 1, 0 };	break;
	}	
}

void Level::Load()
{
	doorSwitch->tileID = { 0, 0 };
	doorSwitch2->tileID = { 0, 0 };
	bridgeSwitch->tileID = { 0, 0 };
	
	switch (current)
	{
	case 1: ReadFile("assets/levels/1.lvl");		break;
	case 2: ReadFile("assets/levels/2.lvl");		break;
	case 3: ReadFile("assets/levels/3.lvl");		break;
	case 4: ReadFile("assets/levels/4.lvl");		break;
	case 5: ReadFile("assets/levels/5.lvl");		break;
	case 6: ReadFile("assets/levels/6.lvl");		break;
	case 7: ReadFile("assets/levels/7.lvl");		break;
	case 8: ReadFile("assets/levels/8.lvl");		break;
	case 9: ReadFile("assets/levels/9.lvl");		break;
	case 10: ReadFile("assets/levels/10.lvl");		break;
	case 11: ReadFile("assets/levels/11.lvl");		break;
	case 12: ReadFile("assets/levels/12.lvl");		break;
	case 13: ReadFile("assets/levels/13.lvl");		break;
	case 14: ReadFile("assets/levels/14.lvl");		break;
	case 15: ReadFile("assets/levels/15.lvl");		break;
	case 16: ReadFile("assets/levels/final.lvl");	break;
	}
	
	if (current == 4 || current == 5 || current == 6 || current == 7 || current == 8
		|| current == 10 || current == 12 || current == 14 || current == 15)
		rightLevel = true;
	else
		rightLevel = false;
	
	if (rightLevel)
	{
		if (floorSwappers.size() != 0)
			floorSwappers.clear();

		for (int x = 0; x < Width; x++)
			for (int y = 0; y < Height; y++)
				if (map[y * Width + x] == 'Q')
					floorSwappers.push_back({ olc::vf2d(x, y), true, 0.0f });
				else if (map[y * Width + x] == 'q')
					floorSwappers.push_back({ olc::vf2d(x, y), false, 0.0f });
	}	
}

void Level::FloorSwap(float fElapsedTime)
{
	if (rightLevel)
	{
		for (int i = 0; i < floorSwappers.size(); i++)
		{			
			if ((GetTile(player->position) == 'Q' || GetTile(player->position) == 'q') &&
				(olc::vi2d)player->position  == (olc::vi2d)floorSwappers[i].position &&
				 !floorSwappers[i].floorOn)
			{
				player->shrink = true;			
			}
			floorSwappers[i].timer += fElapsedTime;
	
			if (floorSwappers[i].timer > 1.0f && !player->shrink)
			{
				floorSwappers[i].floorOn = !floorSwappers[i].floorOn;
				floorSwappers[i].timer = 0.0f;
			}
		}		
	}
}

bool Level::WallCheck(const olc::vi2d& id)
{
	return GetTile(id) == '#' ||
		   GetTile(id) == '|' ||
		   GetTile(id) == 'l';
}

void Level::GoingHome(float& timer, float fElapsedTime)
{
	if (GetTile(player->position) == 'C')
	{
		if (resetTimer)
		{
			timer = 0.0f;
			resetTimer = false;
		}
		player->shrink = true;
	}
}

void Level::ReadFile(const std::string& filepath)
{
	std::ifstream stream(filepath);
	std::string line;
	std::stringstream ss;

	int lineCount = 0;
	while (getline(stream, line))
	{
		Width = line.size();
		ss << line;
		++lineCount;
	}

	Height = lineCount;

	map = ss.str();
}

void Level::Draw()
{
	for (int x = 0; x < Width; x++)
		for (int y = 0; y < Height; y++)
		{
			char TileID = GetTile(olc::vi2d(x, y));
			switch (TileID)
			{
			case '#': DrawDecal(olc::vi2d(x * size.x, y * size.y), decal.get(), olc::DARK_GREY);                                                            break;
			case '0': DrawDecal(olc::vi2d(x * size.x, y * size.y), decal.get(), olc::GREY);                                                                 break;
			case 'C': DrawDecal(olc::vi2d(x * size.x, y * size.y), decal.get(), olc::Pixel(0, 200, 255));																	break;
			case 'R': colorFloors[0]->DrawPartialDecal(olc::vi2d(x * size.x, y * size.y), size, colorFloors[0]->decal.get(), {0, 0}, size);                 break;
			case 'G': colorFloors[1]->DrawPartialDecal(olc::vi2d(x * size.x, y * size.y), size, colorFloors[1]->decal.get(), { 16, 0 }, size);              break;
			case 'B': colorFloors[2]->DrawPartialDecal(olc::vi2d(x * size.x, y * size.y), size, colorFloors[2]->decal.get(), { 32, 0 }, size);              break;
			case 'r': colorSwitches[0]->DrawPartialDecal(olc::vi2d(x * size.x, y * size.y), size, colorSwitches[0]->decal.get(), { 0, 0 }, size);           break;
			case 'g': colorSwitches[1]->DrawPartialDecal(olc::vi2d(x * size.x, y * size.y), size, colorSwitches[1]->decal.get(), { 16, 0 }, size);          break;
			case 'b': colorSwitches[2]->DrawPartialDecal(olc::vi2d(x * size.x, y * size.y), size, colorSwitches[2]->decal.get(), { 32, 0 }, size);          break;
			case 'O': doorSwitch->DrawPartialDecal(olc::vi2d(x * size.x, y * size.y), size, doorSwitch->decal.get(), doorSwitch->tileID * size, size);      break;
			case 'o': doorSwitch2->DrawPartialDecal(olc::vi2d(x * size.x, y * size.y), size, doorSwitch2->decal.get(), doorSwitch2->tileID * size, size);   break;
			case '_': bridgeSwitch->DrawPartialDecal(olc::vi2d(x * size.x, y * size.y), size, bridgeSwitch->decal.get(), bridgeSwitch->tileID * size, size);break;
			case '-': if (bridgeSwitch->tileID.x == 1) SetTile(olc::vi2d(x, y), '0');                                                                       break;
			case 'Q': case 'q':
				for (int i = 0; i < floorSwappers.size(); i++)
				{
					if (floorSwappers[i].floorOn &&
						(olc::vi2d)floorSwappers[i].position == olc::vi2d(x, y))
						DrawDecal(olc::vi2d(x * size.x, y * size.y), decal.get(), olc::GREY);				
				}			
				break;
			case 'l':
				if (doorSwitch2->tileID.x == 0)
					keyLock2->DrawDecal(olc::vi2d(x * size.x, y * size.y), keyLock2->decal.get());
				else
					SetTile(olc::vi2d(x, y), '0');
				break;
			case '|': 
				if (doorSwitch->tileID.x == 0)
					keyLock->DrawDecal(olc::vi2d(x * size.x, y * size.y), keyLock->decal.get());
				else
					SetTile(olc::vi2d(x, y), '0');
				break;
			}
		}	
}
