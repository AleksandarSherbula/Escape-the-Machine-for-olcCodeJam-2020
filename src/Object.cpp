#include "Object.h"

ImageAssets::ImageAssets()
{
}

void ImageAssets::Clear()
{
	
}

void ImageAssets::LoadSprites()
{
	auto load = [&](std::string name, std::string filename)
	{
		std::shared_ptr<olc::Sprite> s = std::make_shared<olc::Sprite>(filename);
		Sprite[name] = s;
	};

	load("Tile",			"assets/images/Floor.png");
	load("KeyLock",			"assets/images/KeyLock.png");
	load("KeyLock2",		"assets/images/KeyLock2.png");
	load("Switch",			"assets/images/Switch.png");
	load("Switch2",			"assets/images/Switch2.png");
	load("ColorSwitches",	"assets/images/ColorSwitches.png");
	load("ColorFloors",		"assets/images/ColoredFloors.png");
	load("BridgeSwitch",	"assets/images/BridgeSwitch.png");
	load("Cloud",			"assets/images/Cloud.png");
	load("Mountains",		"assets/images/Mountains.png");
	load("Grass",			"assets/images/Grass.png");
}


Object::Object()
{
}

Object::Object(olc::PixelGameEngine* pge)
{
	this->pge = pge;
	this->size = { 16, 16 };
}

Object::Object(olc::PixelGameEngine* pge, const olc::vf2d& position, const olc::vi2d& size)
{
	this->pge = pge;
	this->position = position;
	this->size = size;

	color = olc::WHITE;
}

Object::~Object()
{
}

void Object::GetImage(const std::string& name)
{
	sprite = ImageAssets::get().Sprite[name];
	decal = std::make_unique<olc::Decal>(sprite.get());
}

void Object::Dead(float fElapsedTime)
{
}

bool Object::Draw()
{
	return pge->Draw(position, color);	
}

bool Object::Draw(const olc::vi2d& pos, olc::Pixel p)
{
	return pge->Draw(pos, p);
}

void Object::DrawDecal()
{
	pge->DrawDecal(position * size, decal.get(), scale, color);
}

void Object::DrawDecal(const olc::vi2d& pos, olc::Decal* decal, olc::Pixel color, const olc::vf2d& scale)
{
	pge->DrawDecal(pos, decal, scale, color);
}

void Object::DrawPartialDecal()
{
	pge->DrawPartialDecal(position, olc::vi2d(16, 16), decal.get(), tileID * size, size, color);
}

void Object::DrawPartialDecal(const olc::vf2d& pos, const olc::vf2d& size, olc::Decal* decal, const olc::vf2d& sourcePos, const olc::vf2d& sourceSize, olc::Pixel color)
{
	pge->DrawPartialDecal(pos, size, decal, sourcePos, sourceSize, color);
}

void Object::DrawRect()
{
	pge->DrawRect(position, size, color);
}

void Object::DrawRect(const olc::vi2d& pos, const olc::vi2d& size, olc::Pixel p)
{
	pge->DrawRect(pos, size, p);
}

void Object::FillRect()
{
	pge->FillRect(position, size, color);
}

void Object::FillRect(const olc::vi2d& pos, const olc::vi2d& size, olc::Pixel p)
{
	pge->FillRect(pos, size, p);
}