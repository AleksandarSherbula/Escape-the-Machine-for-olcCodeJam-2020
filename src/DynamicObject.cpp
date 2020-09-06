#include "DynamicObject.h"
#include "Level.h"

DynamicObject::DynamicObject(){}

DynamicObject::DynamicObject(olc::PixelGameEngine* pge)
	:Object(pge)
{
	isDead = false;
}

DynamicObject::DynamicObject(olc::PixelGameEngine* pge, const olc::vf2d& position, const olc::vi2d& size)
	:Object(pge, position, size)
{
	initPos = position;
	isDead = false;
}

void DynamicObject::Update(float fElapsedTime, Level* level)
{
	WallHit(fElapsedTime, level);
}

void DynamicObject::Restart(Level* level)
{
	position = initPos;
	shrink = false;
	scale = {1.0f, 1.0f};
	isDead = false;
}

void DynamicObject::Shrink(float fElapsedTime, Level* level, bool& gameEnd)
{
	if (scale.x > 0.0f &&
		scale.y > 0.0f)
	{
		scale -= fElapsedTime;
	}
	else if (level->current != 16)
	{
		isDead = true;
	}
	else
	{
		gameEnd = true;
	}
}

void DynamicObject::WallHit(float fElapsedTime, Level* level)
{
	newPos = position + velocity * fElapsedTime;

	if (velocity.x < 0.0f)
	{
		if (level->WallCheck(olc::vi2d(newPos.x, position.y)) || level->WallCheck(olc::vi2d(newPos.x, position.y + 0.9f)))
		{
			newPos.x = (int)newPos.x + 1;
			velocity.x = 0.0f;
		}
	}
	else
	{
		if (level->WallCheck(olc::vi2d(newPos.x + 1, position.y)) || level->WallCheck(olc::vi2d(newPos.x + 1.0f, position.y + 0.9f)))
		{
			newPos.x = (int)newPos.x;
			velocity.x = 0.0f;
		}
	}

	if (velocity.y < 0.0f)
	{
		if (level->WallCheck(olc::vi2d(position.x, newPos.y)) || level->WallCheck(olc::vi2d(position.x + 0.9f, newPos.y)))
		{
			newPos.y = (int)newPos.y + 1;
			velocity.y = 0.0f;
		}
	}
	else
	{
		if (level->WallCheck(olc::vi2d(position.x, newPos.y + 1)) || level->WallCheck(olc::vi2d(position.x + 0.9f, newPos.y + 1.0f)))
		{
			newPos.y = (int)newPos.y;
			velocity.y = 0.0f;
		}
	}

	position = newPos;
}
