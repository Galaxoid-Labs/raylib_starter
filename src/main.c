#include "jbd_retro.h"
#include "raylib.h"
#include <stdio.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

//----------------------------------------------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------------------------------------------
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const int RENDER_WIDTH = WINDOW_WIDTH;   // Use this to render smaller resolution. Example: WINDOW_WIDTH / 4;
const int RENDER_HEIGHT = WINDOW_HEIGHT; // Use this to render smaller resolution. Example: WINDOW_HEIGHT / 4;
const char *TITLE = "Raylib Template";   // Game Title

//----------------------------------------------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------------------------------------------
void update(float dt);
void draw(float dt);

//----------------------------------------------------------------------------------------------------------------------
// Main Enry Point
//----------------------------------------------------------------------------------------------------------------------
int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, TITLE);
    SetWindowMinSize(320, 180);
    init_jbd_retro();

    // Render texture initialization, used to hold the rendering result so we can easily resize it
    RenderTexture2D target = LoadRenderTexture(RENDER_WIDTH, RENDER_HEIGHT);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT); // Texture scale filter to use

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    while (!WindowShouldClose()) {

        // Compute required framebuffer scaling
        float scale = MIN((float)GetScreenWidth() / RENDER_WIDTH, (float)GetScreenHeight() / RENDER_HEIGHT);

        update(GetFrameTime());

        BeginTextureMode(target);

        draw(GetFrameTime());

        EndTextureMode();

        BeginDrawing();

        ClearBackground(BLACK); // Letterbox color

        // Draw render texture to screen, properly scaled
        DrawTexturePro(target.texture,
                       (Rectangle){0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height},
                       (Rectangle){(GetScreenWidth() - ((float)RENDER_WIDTH * scale)) * 0.5f,
                                   (GetScreenHeight() - ((float)RENDER_HEIGHT * scale)) * 0.5f,
                                   (float)RENDER_WIDTH * scale, (float)RENDER_HEIGHT * scale},
                       (Vector2){0, 0}, 0.0f, WHITE);
        EndDrawing();
    }
#endif

    //------------------------------------------------------------------------------------------------------------------
    // De-Initialization
    //------------------------------------------------------------------------------------------------------------------
    CloseWindow();

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------------------------------------------
void update(float dt) { // GET INPUT AND UPDATE YOUR GAME OBJECTS HERE
    // Get input

    // Update values
}

void draw(float dt) { // DRAW YOUR GAME OBJECTS HERE
    ClearBackground(BLUE);
}
