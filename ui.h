#ifndef __ui_h__
#define __ui_h__

#include <stdint.h>
#include <stdbool.h>

typedef enum _type {
    UI_BUTTON,
    UI_TEXT,
} COMP_TYPE_T;

struct _component_t;

typedef struct _events {
    void (*currsor_on_commponent)(struct _component_t*, bool);
    void (*currsor_on_click)(struct _component_t*);
} events;

#define COMP_RES_TYPE (1024) 
typedef struct _component_t {
    uint32_t id;
    COMP_TYPE_T type;
    uint32_t x;
    uint32_t y;
    uint32_t height;
    uint32_t width;
    events event;
    uint8_t res[COMP_RES_TYPE];
} component_t;

/* BUTTON COMPONNENT */
#define BUTTON_RES_TYPE (1024 - sizeof(uint32_t) - sizeof(uint32_t) - sizeof(char*) - sizeof(uint32_t))
typedef struct {
    uint32_t id;
    COMP_TYPE_T type;
    uint32_t x;
    uint32_t y;
    uint32_t height;
    uint32_t width;
    events event;
    uint32_t color;
    const char* text;
    uint32_t text_color;
    uint32_t font_size;
    uint8_t res[BUTTON_RES_TYPE];
} button_t;

typedef struct {
    component_t component[1024];
} ui_state;

/* PAIN TEXT COMPONENT */

/* LIST COMPONENT */

void ui_define(ui_state*);

void ui_event_loop(ui_state*);
// reander poligon in pos (x, y) in color (0xRRGGBBAA) on with and height (w, h)
// reander text on (x, y) in color (0xRRGGBBAA) on font sizie (px)
// reander img in pos (x, y) in color (0xRRGGBBAA) on with and height (w, h)
void ui_render_frame(ui_state*);

#endif // __ui_h__