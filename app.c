#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "app.h"
#include "uart.h"

typedef struct {
    uart_context uart;    
} app_context;

static app_context g_app_ctx = {0};

int app_init() {
    g_app_ctx.uart.br = 9600;
    g_app_ctx.uart.byte_size = 8;
    g_app_ctx.uart.parity = NOPARITY;
    g_app_ctx.uart.stop_bit = ONESTOPBIT;
    sprintf(g_app_ctx.uart.dev_name, "\\\\.\\COM10");
    int ret = uart_init(&g_app_ctx.uart);

    if (ret != 0) {
        fprintf(stderr, "Err: you cant init uart context (%d)\n", ret);
        return 1;
    }
    return 0;
}

int last = 0;
int app_update() {
    uint8_t rx_buff[255] = {0};
    size_t len = uart_read(&g_app_ctx.uart, rx_buff, sizeof(rx_buff));
    if (len < 0) {
        exit(69);
    }
    uart_pars_line(&g_app_ctx.uart);

    if (last < g_app_ctx.uart.frames_count) {
        for (int i = 0; i < g_app_ctx.uart.frames[last].len; i++) {
            printf("%c", g_app_ctx.uart.frames[last].data[i]);
        }
        printf("\n");

        last = g_app_ctx.uart.frames_count;
    }

}

