#include "raylib.h"
#include <cstdlib>
#include <ctime>
#include <cmath> 

// Pong (PvP || PvAI)


enum GameMode { MODE_MENU, MODE_PVP, MODE_PVAI };

int main()
{
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Pong");

    srand((unsigned)time(NULL));

    GameMode mode = MODE_MENU;
    int menuOption = 0;

    const float paddleWidth = 10.0f;
    const float paddleHeight = 100.0f;
    const float playerSpeed = 450.0f;
    const float aiSpeed = 250.0f;

    const float ballRadius = 8.0f;
    Vector2 ballPos;
    Vector2 ballVel;

    int scoreLeft = 0;
    int scoreRight = 0;

    Rectangle leftPaddle;
    Rectangle rightPaddle;

    bool gameRunning = false;

    while (!WindowShouldClose())
    {
        float delta = GetFrameTime();

        // --- Menu ---
        if (mode == MODE_MENU)
        {
            if (IsKeyPressed(KEY_DOWN)) menuOption = (menuOption + 1) % 2;
            if (IsKeyPressed(KEY_UP)) menuOption = (menuOption + 1) % 2;
            if (IsKeyPressed(KEY_W)) menuOption = (menuOption + 1) % 2;
            if (IsKeyPressed(KEY_S)) menuOption = (menuOption + 1) % 2;

            if (IsKeyPressed(KEY_ENTER))
            {
                mode = (menuOption == 0) ? MODE_PVP : MODE_PVAI;

                scoreLeft = 0;
                scoreRight = 0;
                ballPos = { screenWidth / 2.0f, screenHeight / 2.0f };
                // Initial ball velocity
                ballVel = { 360.0f, 240.0f };

                leftPaddle = { 30.0f, screenHeight / 2.0f - paddleHeight / 2.0f, paddleWidth, paddleHeight };
                rightPaddle = { screenWidth - 40.0f, screenHeight / 2.0f - paddleHeight / 2.0f, paddleWidth, paddleHeight };
                gameRunning = true;
            }
            // --- Drawing Menu ---
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("PONG - Select Mode", screenWidth / 2 - 140, 100, 30, RAYWHITE);
            DrawText(menuOption == 0 ? "-> Player vs Player" : "   Player vs Player", screenWidth / 2 - 140, 200, 20, RAYWHITE);
            DrawText(menuOption == 1 ? "-> Player vs AI" : "   Player vs AI", screenWidth / 2 - 140, 240, 20, RAYWHITE);
            EndDrawing();

            continue;
        }

        if (gameRunning)
        {
            // --- Player 1 movement ---
            if (IsKeyDown(KEY_W) && leftPaddle.y > 0) leftPaddle.y -= playerSpeed * delta;
            if (IsKeyDown(KEY_S) && leftPaddle.y + leftPaddle.height < screenHeight) leftPaddle.y += playerSpeed * delta;

            // --- Player 2 / AI movement ---
            if (mode == MODE_PVP)
            {
                if (IsKeyDown(KEY_UP) && rightPaddle.y > 0) rightPaddle.y -= playerSpeed * delta;
                if (IsKeyDown(KEY_DOWN) && rightPaddle.y + rightPaddle.height < screenHeight) rightPaddle.y += playerSpeed * delta;
            }
            else if (mode == MODE_PVAI)
            {
                float paddleCenter = rightPaddle.y + rightPaddle.height / 2.0f;
                if (ballPos.y < paddleCenter && rightPaddle.y > 0) rightPaddle.y -= aiSpeed * delta;
                if (ballPos.y > paddleCenter && rightPaddle.y + rightPaddle.height < screenHeight) rightPaddle.y += aiSpeed * delta;
            }

            // --- Ball movement ---
            ballPos.x += ballVel.x * delta;
            ballPos.y += ballVel.y * delta;

            // --- Bounce off top/bottom ---
            if (ballPos.y - ballRadius <= 0) { ballPos.y = ballRadius; ballVel.y *= -1; }
            if (ballPos.y + ballRadius >= screenHeight) { ballPos.y = screenHeight - ballRadius; ballVel.y *= -1; }

            // --- Paddle collisions ---
            if (CheckCollisionCircleRec(ballPos, ballRadius, leftPaddle))
            {
                ballPos.x = leftPaddle.x + leftPaddle.width + ballRadius;
                ballVel.x *= -1.05f;
            }
            if (CheckCollisionCircleRec(ballPos, ballRadius, rightPaddle))
            {
                ballPos.x = rightPaddle.x - ballRadius;
                ballVel.x *= -1.05f;
            }

            // --- Scoring ---
            if (ballPos.x - ballRadius <= 0 || ballPos.x + ballRadius >= screenWidth)
            {
                if (ballPos.x - ballRadius <= 0) scoreRight++;
                else scoreLeft++;

                ballPos = { screenWidth / 2.0f, screenHeight / 2.0f };

                // Generate random angle between -45 and +45 degrees
                int angleDeg = GetRandomValue(-45, 45);
                float rad = angleDeg * (3.14159265f / 180.0f);

                // Determine X direction towards player who lost the point
                float dirX = (ballPos.x < screenWidth / 2) ? 1.0f : -1.0f;
                float speed = 360.0f;

                ballVel.x = dirX * speed * cosf(rad);
                ballVel.y = speed * sinf(rad);
            }

            // --- Drawing Game---
            BeginDrawing();
            ClearBackground(BLACK);

            DrawRectangleRec(leftPaddle, RAYWHITE);
            DrawRectangleRec(rightPaddle, RAYWHITE);
            DrawCircleV(ballPos, ballRadius, RAYWHITE);

            DrawText(TextFormat("%i", scoreLeft), screenWidth / 4 - 20, 20, 40, RAYWHITE);
            DrawText(TextFormat("%i", scoreRight), 3 * screenWidth / 4 - 20, 20, 40, RAYWHITE);

            DrawFPS(10, 10);

            EndDrawing();
        }
    }

    CloseWindow();
    return 0;
}
