#ifndef __uart_h__
#define __uart_h__

#include <stdlib.h>
#include <stdint.h>
#include <windows.h>

#define FRAMES_MAX_COUNT (64)

typedef struct {
    uint8_t *data;
    size_t len;
} frame;

typedef struct {
    int br;
    int byte_size;
    int parity;
    int stop_bit;
    const char dev_name[64];

    uint8_t rx_buffer[4*1024];
    size_t  rx_len;

    frame  frames[FRAMES_MAX_COUNT];
    size_t frames_count;
    size_t last_frame_pos;

    OVERLAPPED ov;
    void* _hcom;
} uart_context;


int uart_init(uart_context *uart_ctx);
int uart_read(uart_context *self, uint8_t* rx_buff, size_t rx_len);
void uart_pars_line(uart_context *self);

#endif // __uart_interface__
