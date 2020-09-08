#include "Game.h"

#include <time.h>

Game::Game()
{
    sAppName = "Escape the Machine";
}

Game::~Game()
{

}

bool Game::OnUserCreate()
{
    srand(time(NULL));
    ImageAssets::get().LoadSprites();

    player = std::make_shared<DynamicObject>(this,
        olc::vf2d(2.0f, 17.0f), olc::vi2d(16, 16));
    player->GetImage("Tile");
    player->color = olc::BLUE;

    level = std::make_unique<Level>(this, player);
    level->GetImage("Tile");    

    stars = std::make_unique<StarMap>(this);
    stars->Generate();

    gameStart = false;
    gameEnd = false;
    titleAlpha = 0.0f;

    for (int i = 0; i < clouds.size(); i++)
    {
        clouds[i] = std::make_unique<Object>(this);
        clouds[i]->GetImage("Cloud");
        clouds[i]->position = { (float)(i * 2), (float)(rand() % 4) };
    }

    mountains = std::make_unique<Object>(this);
    mountains->GetImage("Mountains");
    mountains->position = {-10, -40};

    grass = std::make_unique<Object>(this);
    grass->GetImage("Grass");
    grass->position = {0.0f, 222.0f};

    title.Load("assets/images/Title.png");
    victory.Load("assets/images/Victory.png");

    timer = 0.0f;
    timer2 = 0.0f;

    return true;
}

bool Game::OnUserUpdate(float fElapsedTime)
{   
    if (gameStart && !gameEnd)
    {
        player->velocity = { 0.0f, 0.0f };

        if (GetKey(olc::A).bHeld && !player->shrink) player->velocity.x = -10.0f;
        if (GetKey(olc::D).bHeld && !player->shrink) player->velocity.x = 10.0f;
        if (GetKey(olc::W).bHeld && !player->shrink) player->velocity.y = -10.0f;
        if (GetKey(olc::S).bHeld && !player->shrink) player->velocity.y = 10.0f;

        player->Update(fElapsedTime, level.get());

        level->Update(fElapsedTime, gameEnd);
        level->GoingHome(timer, fElapsedTime);

        if (player->isDead && player->shrink)
        {
            timer += fElapsedTime;
            if (timer > 2.0f)
            {
                player->Restart(level.get());
                player->color = olc::BLUE;
                level->Load();
                timer = 0.0f;
            }
        }
    }
    else if (!gameStart)
    {
        if (timer < 1.0f)
            timer += fElapsedTime;
        else
        {
            if (titleAlpha <= 255)
                titleAlpha += 200 * fElapsedTime;
            else
            {
                if (GetKey(olc::ENTER).bPressed)
                {
                    gameStart = true;
                    timer = 0.0f;
                    timer2 = 0.0f;
                }

                timer2 += fElapsedTime;
                if (timer2 > 2.0f)
                    timer2 = 0.0f;
            }
        }
    }

    ShowSystemMouseCursor(false);	//Add to PGE by Moros1138

    if (gameEnd)
        Clear(olc::CYAN);
    else
        Clear(olc::Pixel(0, 0, 30));

    if (gameStart && !gameEnd)
    {
        level->Draw();

        player->DrawDecal();
    }
    else if (!gameStart)
    {
        DrawDecal(olc::vi2d(120, 80), title.Decal(), { 1.0f, 1.0f }, olc::Pixel(255, 255, 255, (uint8_t)titleAlpha));
        if (titleAlpha > 255 && timer2 > 1.0f && timer2 < 2.0f)
        {
            DrawString(olc::vi2d(150, 240), "Press Enter to start");
        }
    }
    else if (gameEnd)
    {
        timer += fElapsedTime;
        player->position = { 10.0f, 13.0f };
        if (timer > 1.0f)
        {
            if (player->scale.x < 1.0f &&
                player->scale.y < 1.0f)
            {
                player->scale += fElapsedTime;
            }
            else
            {
                player->scale = { 1.0f, 1.0f };
            }
        }

        for (int i = 0; i < clouds.size(); i++)
        {
            //DrawPartialDecal(cloud, decCloud, olc::vi2d(0, 0), olc::vi2d(2 * tileSize, tileSize));
            clouds[i]->DrawPartialDecal(clouds[i]->position * 16, olc::vi2d(32, 16),
                clouds[i]->decal.get(), olc::vi2d(0, 0),
                olc::vi2d(4 * clouds[i]->size.x, 2 * clouds[i]->size.y));
        }

        FillRect(olc::vi2d(0, 210), olc::vi2d(480, 110), olc::Pixel(0, 128, 255));

        mountains->DrawDecal(mountains->position, mountains->decal.get());

        for (int i = 0; i < 15; i++)
            grass->DrawDecal(olc::vi2d(grass->position.x + i * 32, grass->position.y), grass->decal.get());

        player->DrawDecal();

        if (player->scale.x == 1.0f)
        {
            DrawDecal(olc::vi2d(120, 120), victory.Decal());
            DrawString(olc::vi2d(120, 280), "Press Escape to exit the game");
        }
    }
    else
    {
        Clear(olc::Pixel(0, 0, 30));
    }

    if (!gameEnd)
        stars->UpdateAndDraw(fElapsedTime);    

    return !GetKey(olc::ESCAPE).bPressed;
}
