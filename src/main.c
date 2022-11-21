#include "jbd_retro.h"
#include "raylib.h"

#define RAYGUI_IMPLEMENTATION


#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------------------------------------------------------

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define SHOW_FPS // Comment out to hide this

//----------------------------------------------------------------------------------------------------------------------
// Global Variables
//----------------------------------------------------------------------------------------------------------------------
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const int RENDER_WIDTH = WINDOW_WIDTH;   // Use this to render smaller resolution. Example: WINDOW_WIDTH / 4;
const int RENDER_HEIGHT = WINDOW_HEIGHT; // Use this to render smaller resolution. Example: WINDOW_HEIGHT / 4;
const char *TITLE = "Raylib Template";   // Game Title
const Color LETTERBOX_COLOR = BLACK;

//----------------------------------------------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------------------------------------------
void start(void);
void update(float dt);
void draw(float dt);

//----------------------------------------------------------------------------------------------------------------------
// Main Entry Point
// Notes: You shouldn't need to modify anything in main. Use start(), update(float dt) and draw(float dt)
//----------------------------------------------------------------------------------------------------------------------
int main() {

    //------------------------------------------------------------------------------------------------------------------
    // Initialization
    //------------------------------------------------------------------------------------------------------------------
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    SetTargetFPS(60);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, TITLE);
    SetWindowMinSize(320, 180);
    init_jbd_retro();

    // Render texture initialization, used as framebuffer
    RenderTexture2D target = LoadRenderTexture(RENDER_WIDTH, RENDER_HEIGHT);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT); // Texture scale filter to use

    // Used implementation of this function to init your game data, etc
    start();

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

        ClearBackground(LETTERBOX_COLOR); // Letterbox color

        // Draw render texture to screen, properly scaled
        DrawTexturePro(target.texture,
                       (Rectangle){0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height},
                       (Rectangle){((float)GetScreenWidth() - ((float)RENDER_WIDTH * scale)) * 0.5f,
                                   ((float)GetScreenHeight() - ((float)RENDER_HEIGHT * scale)) * 0.5f,
                                   (float)RENDER_WIDTH * scale, (float)RENDER_HEIGHT * scale},
                       (Vector2){0, 0}, 0.0f, WHITE);

        #if defined(SHOW_FPS)
            DrawFPS(20, 20);
        #endif

        EndDrawing();
    }
#endif

    //------------------------------------------------------------------------------------------------------------------
    // De-Initialization
    //------------------------------------------------------------------------------------------------------------------
    UnloadRenderTexture(target);
    CloseWindow();

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------------------------------------------

// Called at the start of the program. Use this to initialize your game vars and other data.
void start(void) {
}

// Called every frame before drawing. Use this to update your game vars and other data per frame.
void update(float dt) {
}

// Called every frame after updating. Use this to draw your game.
void draw(float dt) {
    ClearBackground(PICO_DARK_BLUE);
}
