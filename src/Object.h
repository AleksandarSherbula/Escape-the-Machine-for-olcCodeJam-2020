#pragma once

#include "olcPixelGameEngine.h"

class ImageAssets
{
public:
	static ImageAssets& get()
	{
		static ImageAssets me;
		return me;
	}

	ImageAssets(ImageAssets const&) = delete;
	void operator=(ImageAssets const&) = delete;

	std::map<std::string, std::shared_ptr<olc::Sprite>> Sprite;

	void Clear();

	void LoadSprites();
private:
	ImageAssets();
};

class Object
{
protected:
	olc::PixelGameEngine* pge;
public:
	std::shared_ptr<olc::Sprite> sprite;
	std::unique_ptr<olc::Decal> decal;
	olc::vf2d position;
	olc::vi2d size;
	olc::Pixel color;
	olc::vi2d tileID;
	olc::vf2d scale = {1.0f, 1.0f};
	float shrink = false;
public:
	Object();
	Object(olc::PixelGameEngine* pge);
	Object(olc::PixelGameEngine* pge, const olc::vf2d& position, const olc::vi2d& size);
	~Object();

	void GetImage(const std::string& name);

	virtual void Dead(float fElapsedTime);

	bool Draw();
	bool Draw(const olc::vi2d& pos, olc::Pixel p = olc::WHITE);

	void DrawDecal();
	void DrawDecal(const olc::vi2d& pos, olc::Decal* decal, olc::Pixel color = olc::WHITE, const olc::vf2d& scale = {1.0f, 1.0f});

	void DrawPartialDecal();
	void DrawPartialDecal(const olc::vf2d& pos, const olc::vf2d& size, olc::Decal* decal, const olc::vf2d& sourcePos, const olc::vf2d& sourceSize, olc::Pixel color = olc::WHITE);

	void DrawRect();
	void DrawRect(const olc::vi2d& pos, const olc::vi2d& size, olc::Pixel p = olc::WHITE);

	void FillRect();
	void FillRect(const olc::vi2d& pos, const olc::vi2d& size, olc::Pixel p = olc::WHITE);
};