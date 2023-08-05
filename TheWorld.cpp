#include <iostream>
#include <raylib.h>
#include<vector>
const int screenWidth = 800;
const int screenHeight = 600;

typedef struct {
    Vector2 position;
    Vector2 velocity;
    float roation;
    bool active;
} Knife;


std::vector<Knife> knives;

void initKnife(Knife* knife) {
    knife->position = {0,0};
    knife->velocity = {0,0 };
    knife->roation = 0.0f;
    knife->active = false;
}

void updateKnife(Knife* knife)
{
    if (knife->active) {
        knife->position.x += knife->velocity.x * GetFrameTime();
        knife->position.y += knife->velocity.y * GetFrameTime();
        if (knife->position.x > screenWidth || knife->position.y > screenHeight)
            knife->active = false;
    }
}

void throwKnife(Vector2 position, Vector2 velocity)
{
    for (int i = 0; i < knives.size(); i++) {
        if (!knives[i].active) {
            knives[i].position = position;
            knives[i].velocity = velocity;
            knives[i].active = true;
            return;
        }
    }
    Knife newKnife;
    initKnife(&newKnife);
    newKnife.position = position;
    newKnife.velocity = velocity;
    newKnife.active = true;
    knives.push_back(newKnife);
}


int main()
{   
    InitWindow(screenWidth, screenHeight, "The World");
    SetTargetFPS(120);
    InitAudioDevice();
    Music music = LoadMusicStream("zutomayo.ogg");
    PlayMusicStream(music);
    Vector2 position{ screenWidth/2, screenHeight/2 };
    Vector2 velocity = { 0,0 };
    float vCap = 350.0f;
    float unitSpeed = 25.0f;
    
    while (!WindowShouldClose())
    {   
        UpdateMusicStream(music);
        if (IsKeyPressed(KEY_C)) {
            throwKnife({ position.x + 10, position.y + 10 }, { 300, 0 });
        }

        for (int i = 0; i < knives.size(); i++) {
            updateKnife(&knives[i]);
        }

        knives.erase(std::remove_if(knives.begin(), knives.end(), [](const Knife& knife)
            {
                return !knife.active;
            }), knives.end());

        if (IsKeyDown(KEY_W))
        {
            velocity.y -= unitSpeed;
        }
        else if (IsKeyDown(KEY_S))
        {
            velocity.y += unitSpeed;
        }
        else if (IsKeyDown(KEY_A))
        {
            velocity.x -= unitSpeed;
        }
        else if (IsKeyDown(KEY_D))
        {
            velocity.x += unitSpeed;
        }
        else if (IsKeyDown(KEY_SPACE))
        {
            velocity.x = 0;
            velocity.y = 0;
        }
        if (velocity.y > vCap)
        {
            velocity.y = vCap;
        }
        else if (velocity.y < -vCap)
        {
            velocity.y = -vCap;
        }
        if (velocity.x > vCap)
        {
            velocity.x = vCap;
        }
        else if (velocity.x < -vCap)
        {
            velocity.x = -vCap;
        }

        position.x += velocity.x * GetFrameTime();
        position.y += velocity.y * GetFrameTime();
        if (position.x < 0)
        {
            position.x = 0;
        }
        else if (position.x > screenWidth)
        {
            position.x = screenWidth - 1;
        }
        if (position.y < 0)
        {
            position.y = 0;
        }
        else if (position.y > screenHeight)
        {
            position.y = screenHeight - 1;
        }
        BeginDrawing();
        ClearBackground(BLUE);
        DrawCircleV(position, 25, GREEN);
        for (int i = 0; i < knives.size(); i++) {
            if (knives[i].active) {
                DrawRectangle(knives[i].position.x, knives[i].position.y, 32, 8, RED);
            }
        }
        EndDrawing();
    }
    return 0;
}
