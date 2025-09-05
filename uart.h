#ifndef __uart_h__
#define __uart_h__

#include <stdlib.h>
#include <stdint.h>

#define UART_PARITY_NOPARITY            0
#define UART_PARITY_ODDPARITY           1
#define UART_PARITY_EVENPARITY          2
#define UART_PARITY_MARKPARITY          3
#define UART_PARITY_SPACEPARITY         4

#define UART_STOPBIT_ONESTOPBIT          0
#define UART_STOPBIT_ONE5STOPBITS        1
#define UART_STOPBIT_TWOSTOPBITS         2

#define FRAMES_MAX_COUNT (64)

typedef struct {
    uint8_t *data;
    size_t len;
} frame;

// TODO: neet to refactor to static windows think

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
} uart_context;


int uart_init(uart_context *self);
int uart_read(uart_context *self);
int uart_service(uart_context *self);
void uart_pars_line(uart_context *self);
frame* uart_get_line(uart_context *self);

#endif // __uart_interface__
