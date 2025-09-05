#include "ui_platform.h"
#include "string.h"
#include "raylib.h"

#define COLOR_PLATFORM(color)               \
    (Color) {                               \
        .r = (color & 0xFF000000) >> (8*3), \ 
        .g = (color & 0x00ff0000) >> (8*2), \
        .b = (color & 0x0000ff00) >> (8*1), \
        .a = (color & 0x000000ff)           \
    }                                       \

void ui_platform_render_prolog(void* arg) {
    (void)arg;
    BeginDrawing();
}

void ui_platform_render_epilog(void* arg) {
    (void)arg;
    EndDrawing();
}

void ui_platform_render_rectangle(int x, int y, int h, int w, uint32_t color) {
    DrawRectangle(x, y, w, h, COLOR_PLATFORM(color)); 
}

void ui_platform_render_triangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color) { 
    DrawTriangle((Vector2){x1, y1}, (Vector2){x2, y2}, (Vector2){x3, y3}, COLOR_PLATFORM(color));
}

void ui_platform_render_text(const char *text, int x, int y, int font_size, uint32_t color) { 
    DrawText(text, x, y, font_size, COLOR_PLATFORM(color));
}

void ui_platform_clear(uint32_t color) {
    ClearBackground(COLOR_PLATFORM(color));
}

void ui_platform_get_mause_pos(int *posx, int *posy) {
    *posx = GetMouseX();
    *posy = GetMouseY();
}

int ui_platform_get_click() {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        return 1;
    }
    return 0;
}

int ui_mesure_text(const char* text, int font_size) {
    return MeasureText(text, font_size);
}

void ui_platform_get_input_mouse(int button) {
// RLAPI bool IsMouseButtonPressed(int button);
// RLAPI bool IsMouseButtonDown(int button);
// RLAPI bool IsMouseButtonReleased(int button);
// RLAPI bool IsMouseButtonUp(int button);
}