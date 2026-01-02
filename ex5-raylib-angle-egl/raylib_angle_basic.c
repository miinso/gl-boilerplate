#include "raylib.h"
#include <stdio.h>

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib + ANGLE - OpenGL ES 3.0");

    Vector2 ballPosition = { (float)screenWidth / 2, (float)screenHeight / 2 };
    float ballSpeedX = 5.0f;
    float ballSpeedY = 4.0f;
    int ballRadius = 20;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // Update
        ballPosition.x += ballSpeedX;
        ballPosition.y += ballSpeedY;

        // Check walls collision
        if ((ballPosition.x >= (screenWidth - ballRadius)) || (ballPosition.x <= ballRadius)) ballSpeedX *= -1.0f;
        if ((ballPosition.y >= (screenHeight - ballRadius)) || (ballPosition.y <= ballRadius)) ballSpeedY *= -1.0f;

        // Draw
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawCircleV(ballPosition, ballRadius, MAROON);
            DrawText("Move the ball with bouncing!", 10, 10, 20, DARKGRAY);
            DrawFPS(10, 40);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
