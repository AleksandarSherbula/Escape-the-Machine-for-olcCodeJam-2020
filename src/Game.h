#pragma once

#include "DynamicObject.h"
#include "StarMap.h"
#include "Level.h"

#include <memory>
#include <Windows.h>


class Game : public olc::PixelGameEngine
{
public:
	Game();
	~Game();

	std::shared_ptr<DynamicObject> player;
	std::unique_ptr<StarMap> stars;
	std::unique_ptr<Level> level;

	std::array<std::unique_ptr<Object>, 12> clouds;
	std::unique_ptr<Object> mountains;
	std::unique_ptr<Object> grass;
	olc::Renderable victory;

	olc::Renderable title;
	float titleAlpha;

	bool gameStart;
	bool gameEnd;

	olc::Pixel clearColor;

	float timer;
	float timer2;
	
	bool pause;
public:
	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;
};