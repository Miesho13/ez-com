#include <stdio.h>
#include "ui.h"
#include "ui_platform.h"
#include "raylib.h"
#include <stdbool.h>
#include <string.h>


uint32_t darken_color(uint32_t color, float factor) {
    uint8_t r = (color >> 24) & 0xFF;
    uint8_t g = (color >> 16) & 0xFF;
    uint8_t b = (color >> 8)  & 0xFF;
    uint8_t a =  color        & 0xFF;

    r = (uint8_t)(r * factor);
    g = (uint8_t)(g * factor);
    b = (uint8_t)(b * factor);

    return ((uint32_t)r << 24) |
           ((uint32_t)g << 16) |
           ((uint32_t)b << 8)  |
           (uint32_t)a;
}

static int button_recpy(button_t* button) {
    uint32_t base_color = button->color;

    int fix_with = ui_mesure_text(button->text, button->font_size);

    int scale_pading_left_up = 5;
    int scale_pading_right_down = scale_pading_left_up*2;

    ui_platform_render_rectangle(
        button->x, 
        button->y, 
        button->height, 
        fix_with + scale_pading_left_up*2,
        darken_color(base_color, 0.5));

    ui_platform_render_rectangle(
        button->x + scale_pading_left_up, 
        button->y + scale_pading_left_up, 
        button->height - scale_pading_right_down, 
        fix_with, 
        button->color);

    ui_platform_render_text(
        button->text, 
        button->x + scale_pading_left_up, button->y + button->height / 4, 
        button->font_size, button->text_color);

    return 0;
}

int render_component(component_t *com) {

    switch (com->type) {
        case UI_BUTTON: {
            return button_recpy((button_t*)com);
        }

        default: { }
    }

    return 0;
}


void currsor_on_button(component_t *com, bool is_on) {
    button_t *butt = (button_t*)com;

    if (is_on) {
        butt->color = 0x0909a8ff;
    }
    else {
        butt->color = 0x3399ffff; 
    }
}

bool draw_chuj = false;
void button_on_click(component_t *com) {
    button_t *butt = (button_t*)com;
    printf("CHUJ\n");

    if (!draw_chuj) {
        draw_chuj = true;
    }
    else { 
        draw_chuj = false;
    }
}

button_t button = {
    .id = 0,
    .type = UI_BUTTON,
    .color = 0x3399ffff,
    .x = 10,
    .y = 10,
    .height = 60,
    .width = 105,
    .text = "PIZDOLEC",
    .text_color = 0xffffffff,
    .font_size = 32,
    .event = {
        .currsor_on_commponent = currsor_on_button,
        .currsor_on_click = button_on_click,
    }
};

void ui_render_frame(ui_state *ui) {
    (void)ui;
    render_component((component_t*)&button);

    if (draw_chuj) {
        ui_platform_render_text("CHUJ", 350, 200, 52, 0xd000ffff);
    }

    ui_platform_render_epilog(NULL);
}

static void cursor_on_component_event(int posx, int posy, component_t *com) {
    if (posx >= com->x && 
            posx <= com->x + com->width &&
            posy >= com->y && 
            posy <= com->y + com->height) {
        com->event.currsor_on_commponent(com, true);

        int ret = ui_platform_get_click();
        if (ret) {
            com->event.currsor_on_click(com);
        }
    }
    else {
        com->event.currsor_on_commponent(com, false);
    }
}

void ui_event_loop(ui_state*) {
    ui_platform_render_prolog(NULL);
    ui_platform_clear(0x181818ff);

    int posx = 0;
    int posy = 0;
    ui_platform_get_mause_pos(&posx, &posy);
    cursor_on_component_event(posx, posy, (component_t*)&button);
}