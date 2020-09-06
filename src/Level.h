#pragma once

#include "Object.h"

class DynamicObject;

#include <string>
#include <vector>
#include <map>

struct FloorSwapper
{
	olc::vf2d position;
	bool floorOn;
	float timer;
};

class Level : public Object
{
private:
	std::string map;
	int Width;
	int Height;
	std::shared_ptr<DynamicObject> player;
	std::unique_ptr<Object> keyLock;
	std::unique_ptr<Object> keyLock2;
	std::array<std::unique_ptr<Object>, 3> colorSwitches;
	std::array<std::unique_ptr<Object>, 3> colorFloors;	
	std::vector<FloorSwapper> floorSwappers;
	std::unique_ptr<Object> doorSwitch;
	std::unique_ptr<Object> doorSwitch2;
	std::unique_ptr<Object> bridgeSwitch;
	bool rightLevel;
	float timer;
	bool resetTimer;
public:
	int current;
public:
	Level(olc::PixelGameEngine* pge, std::shared_ptr<DynamicObject> player);
	~Level();
	void Load();
	char GetTile(const olc::vi2d& id);
	void SetTile(const olc::vi2d& id, char c);

	void Update(float fElapsedTime, bool& gameEnd);

	void PlayerColorChange();
	void FloorColorClear(char id, const olc::Pixel& color);
	bool WallCheck(const olc::vi2d& id);
	void GoingHome(float& timer, float fElapsedTime);

	void Draw();

private:
	void ReadFile(const std::string& filepath);
	void ActivateSwitch();

	void FloorSwap(float fElapsedTime);

};