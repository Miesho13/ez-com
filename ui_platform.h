#ifndef __UI_PLATFORM__
#define __UI_PLATFORM__

#include <stdint.h>

void ui_platform_render_prolog(void*);
void ui_platform_render_epilog(void*);
void ui_platform_render_rectangle(int x, int y, int h, int w, uint32_t color);
void ui_platform_render_triangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color);
void ui_platform_render_text(const char *text, int x, int y, int font_size, uint32_t color);
void ui_platform_clear(uint32_t coler);

int ui_mesure_text(const char*, int);

void ui_platform_get_mause_pos(int *posx, int *posy);
int ui_platform_get_click();

#endif