#ifndef jbd_retro_h
#define jbd_retro_h

#include "raylib.h"
#include "stdlib.h"
#include "time.h"

//----------------------------------------------------------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------------------------------------------------------

#define ARRAY_LENGTH(x) (sizeof(x) / sizeof((x)[0]))

// clang-format off
// Colors
#define PICO_LIGHT_GRAY (Color) { 194, 195, 199, 255 }
#define PICO_WHITE      (Color) { 255, 241, 232, 255 }
#define PICO_RED        (Color) { 255, 0, 77, 255 }
#define PICO_ORANGE     (Color) { 255, 163, 0, 255 }
#define PICO_YELLOW     (Color) { 255, 236, 39, 255 }
#define PICO_GREEN      (Color) { 0, 228, 54, 255 }
#define PICO_BLUE       (Color) { 41, 173, 255, 255 }
#define PICO_INDIGO     (Color) { 131, 118, 156, 255 }
#define PICO_PINK       (Color) { 255, 119, 168, 255 }
#define PICO_PEACH      (Color) { 255, 204, 170, 255 }
// clang-format on

static inline void init_jbd_retro() {
    srand(time(NULL));
    rand(); // for some reason I always get 25 the first time I call rand. So
            // calling it here...
}

//----------------------------------------------------------------------------------------------------------------------
// Random Helpers
//----------------------------------------------------------------------------------------------------------------------

static inline Color get_random_rgb_color(void) { return (Color){rand() % 256, rand() % 256, rand() % 256, 255}; }

static inline Color get_random_pico_color(void) {

    Color colors[] = {

        PICO_LIGHT_GRAY, PICO_WHITE,  PICO_RED,  PICO_ORANGE, PICO_YELLOW, PICO_GREEN,
        PICO_BLUE,       PICO_INDIGO, PICO_PINK, PICO_PEACH

    };

    return colors[rand() % 10];
}

static inline float get_random_range(float min, float max) {

    float random = ((float)rand()) / (float)RAND_MAX;
    float range = max - min;

    return (random * range) + min;
}

static inline Texture2D get_array_texture(Color pixels[], int width, int height) {

    Image image;
    image.data = NULL;
    image.width = width;
    image.height = height;
    image.mipmaps = 1;
    image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;

    int k = 0;

    image.data = (unsigned char *)RL_MALLOC(image.width * image.height * 4 * sizeof(unsigned char));

    for (int i = 0; i < image.width * image.height * 4; i += 4) {
        ((unsigned char *)image.data)[i] = pixels[k].r;
        ((unsigned char *)image.data)[i + 1] = pixels[k].g;
        ((unsigned char *)image.data)[i + 2] = pixels[k].b;
        ((unsigned char *)image.data)[i + 3] = pixels[k].a;
        k++;
    }

    Texture2D texture = LoadTextureFromImage(image);
    UnloadImage(image);

    return texture;
}

// must be even number
static inline Texture2D get_random_texture(int width, int height) {

    int half_width = width / 2;
    Color *l_array[height * half_width];

    Color colors[] = {BLANK, WHITE};

    // create left side
    for (int i = 0; i < height * half_width; i++) {
        l_array[i] = &colors[rand() % 2];
    }

    Color r_array[height * width];

    int r_index = 0;
    int counter = 0;

    // fill left side of return array
    for (int i = 0; i < height * half_width; i++) {

        r_array[r_index] = *l_array[i];

        r_index++;
        counter++;

        if (counter == half_width) {
            counter = 0;
            r_index += half_width;
        }
    }

    r_index = half_width;
    counter = 0;
    int i_index = half_width - 1;

    // fill right side of return array
    for (int i = 0; i < height * half_width; i++) {

        r_array[r_index] = *l_array[i_index];

        i_index--;
        r_index++;
        counter++;

        if (counter == half_width) {
            counter = 0;
            r_index += half_width;
            i_index = i + half_width;
        }
    }

    return get_array_texture(r_array, width, height);
}

#endif /* jbd_retro_h */
