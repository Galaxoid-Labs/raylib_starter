#include "jbd_retro.h"
#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

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
const int RENDER_WIDTH = WINDOW_WIDTH / 4;   // Use this to render smaller resolution. Example: WINDOW_WIDTH / 4;
const int RENDER_HEIGHT = WINDOW_HEIGHT / 4; // Use this to render smaller resolution. Example: WINDOW_HEIGHT / 4;
const char *TITLE = "Raylib Template";       // Game Title
const Color LETTERBOX_COLOR = BLACK;

Texture2D tex;

int display_width(void);
int display_height(void);
void try_toggle_fullscreen(void);
//----------------------------------------------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------------------------------------------
void start(void);
void update(float dt);
void draw(float dt);
void draw_unscaled(float dt);

//----------------------------------------------------------------------------------------------------------------------
// Main Entry Point
// Notes: You shouldn't need to modify anything in main. Use start(), update(float dt) and draw(float dt)
//----------------------------------------------------------------------------------------------------------------------
int main() {

    //------------------------------------------------------------------------------------------------------------------
    // Initialization
    //------------------------------------------------------------------------------------------------------------------
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    // SetTargetFPS(60);
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
        float scale = MIN((float)display_width() / RENDER_WIDTH, (float)display_height() / RENDER_HEIGHT);

        int mouse_offset_x = -((display_width() - (RENDER_WIDTH * scale)) / 2);
        int mouse_offset_y = -((display_height() - (RENDER_HEIGHT * scale)) / 2);

        SetMouseOffset(mouse_offset_x, mouse_offset_y);
        SetMouseScale(1 / scale, 1 / scale);

        update(GetFrameTime());

        BeginTextureMode(target);

        draw(GetFrameTime());

        //        DrawRectangleLinesEx((Rectangle){0, 0, RENDER_WIDTH, RENDER_HEIGHT}, 2, PICO_BLUE);

        EndTextureMode();

        BeginDrawing();

        ClearBackground(LETTERBOX_COLOR); // Letterbox color

        Rectangle source_rect = (Rectangle){0.0f, 0.0f, target.texture.width, -target.texture.height};
        float dest_y = ((display_width() - (RENDER_WIDTH * scale)) * 0.5f);
        float dest_x = ((display_height() - (RENDER_HEIGHT * scale)) * 0.5f);
        Rectangle dest_rect = {dest_y, dest_x, RENDER_WIDTH * scale, RENDER_HEIGHT * scale};

        // Draw render texture to screen, properly scaled
        DrawTexturePro(target.texture, source_rect, dest_rect, (Vector2){0, 0}, 0.0f, WHITE);

#if defined(SHOW_FPS)
        DrawFPS(20, 20);
#endif

        SetMouseOffset(0, 0);
        SetMouseScale(1, 1);

        draw_unscaled(GetFrameTime());

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

int display_width(void) {
    if (IsWindowFullscreen())
        return GetMonitorWidth(GetCurrentMonitor());
    else
        return GetScreenWidth(); // * (int)GetWindowScaleDPI().x;
}

int display_height(void) {
    if (IsWindowFullscreen())
        return GetMonitorHeight(GetCurrentMonitor());
    else
        return GetScreenHeight(); // * (int)GetWindowScaleDPI().y;
}

void try_toggle_fullscreen(void) {
    if (!IsWindowFullscreen()) {
        SetWindowSize(display_width() * GetWindowScaleDPI().x, display_height() * GetWindowScaleDPI().y);
        SetWindowPosition(0, 0);
        ToggleFullscreen();
    } else {
        ToggleFullscreen();
        SetWindowPosition((GetScreenWidth() - WINDOW_WIDTH) / 2, (GetScreenHeight() - WINDOW_HEIGHT) / 2);
        SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    }
}

//----------------------------------------------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------------------------------------------

// Called at the start of the program. Use this to initialize your game vars and other data.
void start(void) { tex = get_random_texture(8, 8); }

// Called every frame before drawing. Use this to update your game vars and other data per frame.
void update(float dt) {
    if (IsKeyDown(KEY_SPACE)) {
        UnloadTexture(tex);
        tex = get_random_texture(8, 8);
    }
    if (IsKeyPressed(KEY_A)) {
        try_toggle_fullscreen();
    }
}

// Called every frame after updating. Use this to draw your game.
void draw(float dt) {
    ClearBackground(PICO_DARK_PURPLE);
    // DrawRectangleLines(0, 0, RENDER_WIDTH, RENDER_HEIGHT, RED);
    //  DrawCircleV(GetMousePosition(), 25.0f, GREEN);
    DrawTexturePro(tex, (Rectangle){0, 0, tex.width, tex.height},
                   (Rectangle){RENDER_WIDTH / 2, RENDER_HEIGHT / 2, tex.width * 4, tex.height * 4},
                   (Vector2){tex.width * 4 / 2, tex.height * 4 / 2}, 0.0f, PINK);
    // DrawTextureEx(tex, (Vector2){RENDER_WIDTH / 2, RENDER_HEIGHT / 2}, 0.0f, 4.0f, PINK);
    DrawRectangleLinesEx((Rectangle){0, 0, RENDER_WIDTH, RENDER_HEIGHT}, 2, BLACK);
    DrawRectangleLinesEx((Rectangle){2, 2, RENDER_WIDTH - 4, RENDER_HEIGHT - 4}, 2, PICO_BLUE);

    DrawText(TextFormat("Mouse: %i, %i", GetScreenWidth(), display_height()), 20, 20, 20, WHITE);
}

// Called every frame after updating and draw. This is also is not scaled.
void draw_unscaled(float dt) {}
