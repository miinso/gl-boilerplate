#include "raylib.h"
#include "rlgl.h"
#include <stdio.h>
#include <math.h>

// Custom vertex shader (GLSL 300 es)
const char *vertexShaderCode = "#version 300 es\n"
    "precision mediump float;\n"
    "in vec3 vertexPosition;\n"
    "in vec2 vertexTexCoord;\n"
    "in vec4 vertexColor;\n"
    "uniform mat4 mvp;\n"
    "uniform float time;\n"
    "out vec2 fragTexCoord;\n"
    "out vec4 fragColor;\n"
    "void main()\n"
    "{\n"
    "    fragTexCoord = vertexTexCoord;\n"
    "    fragColor = vertexColor;\n"
    "    vec3 pos = vertexPosition;\n"
    "    pos.y += sin(pos.x * 4.0 + time) * 0.1;\n"
    "    gl_Position = mvp * vec4(pos, 1.0);\n"
    "}\n";

// Custom fragment shader (GLSL 300 es)
const char *fragmentShaderCode = "#version 300 es\n"
    "precision mediump float;\n"
    "in vec2 fragTexCoord;\n"
    "in vec4 fragColor;\n"
    "uniform float time;\n"
    "out vec4 finalColor;\n"
    "void main()\n"
    "{\n"
    "    vec2 uv = fragTexCoord;\n"
    "    vec3 col = 0.5 + 0.5*cos(time + uv.xyx + vec3(0,2,4));\n"
    "    finalColor = vec4(col, 1.0) * fragColor;\n"
    "}\n";

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib + ANGLE - GLSL 300 es Shaders");

    // Load custom shader
    Shader shader = LoadShaderFromMemory(vertexShaderCode, fragmentShaderCode);
    
    // Get shader uniform locations
    int timeLoc = GetShaderLocation(shader, "time");
    
    float time = 0.0f;

    Camera camera = { 0 };
    camera.position = (Vector3){ 0.0f, 2.0f, 4.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };

    SetTargetFPS(60);

    printf("Shader loaded successfully!\n");
    printf("Using GLSL 300 es shaders with ANGLE backend\n");

    while (!WindowShouldClose())
    {
        // Update
        time += GetFrameTime();
        
        // Set shader uniform
        SetShaderValue(shader, timeLoc, &time, SHADER_UNIFORM_FLOAT);

        // Draw
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                BeginShaderMode(shader);

                    DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, WHITE);
                    DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);

                EndShaderMode();

                DrawGrid(10, 1.0f);

            EndMode3D();

            DrawText("Custom GLSL 300 es Shader", 10, 10, 20, DARKGRAY);
            DrawText("Wavy cube with animated colors", 10, 35, 20, DARKGRAY);
            DrawFPS(10, 60);

        EndDrawing();
    }

    UnloadShader(shader);
    CloseWindow();

    return 0;
}
