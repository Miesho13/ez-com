#include <stdio.h>
#include <stdlib.h>

#include "app.h"
#include "./raylib/src/raylib.h"
#include "ui.h" 

#define WIN_WIDTH 1200
#define WIN_HEIGHT 1000

int main() {
    // int ret = app_init();
    // if (ret != 0) {
    //     fprintf(stderr, "Err: app cant be init (%d)", ret);
    //     return 1;
    // }

    InitWindow(WIN_WIDTH, WIN_HEIGHT, "ez-com");
    while (!WindowShouldClose()) {
        ui_event_loop(NULL);
        ui_render_frame(NULL);
    }

    return 0;
}
