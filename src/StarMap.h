#pragma once
#include "olcPixelGameEngine.h"

#include <array>

class StarMap {
public:
    StarMap(olc::PixelGameEngine* game);

public:
    void Generate();
    void UpdateAndDraw(float fElapsedTime);

protected:
    olc::PixelGameEngine* game;
    std::array<olc::vf2d, 400> stars;
};
