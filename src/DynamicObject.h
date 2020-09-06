#pragma once

#include "Object.h"

class Level;

class DynamicObject : public Object
{
public:
	olc::vf2d initPos;
	olc::vf2d newPos;
	olc::vf2d velocity;
	bool isDead;
public:
	DynamicObject();
	DynamicObject(olc::PixelGameEngine* pge);
	DynamicObject(olc::PixelGameEngine* pge, const olc::vf2d& position,
		const olc::vi2d& size);	

	void Update(float fElapsedTime, Level* level);

	void Restart(Level* level);

	void Shrink(float fElapseTime, Level* level, bool& gameEnd);
private:
	void WallHit(float fElapsedTime, Level* level);
};