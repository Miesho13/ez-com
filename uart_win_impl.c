#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "uart.h"

static void prv_com_info(DCB *dcb) {
    printf("COM INFO:\n");
    printf("dcb->BaudRate = %d\n", (int)dcb->BaudRate);
    printf("dcb->ByteSize = %d\n", (int)dcb->ByteSize);
    printf("dcb->Parity   = %d\n", (int)dcb->Parity);
    printf("dcb->StopBits = %d\n", (int)dcb->StopBits);
}

int uart_init(uart_context *self) {

    self->_hcom = CreateFile(
        self->dev_name,
        GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
    
    if (self->_hcom == INVALID_HANDLE_VALUE) {
        fprintf (stderr, "CreateFile failed with err %lu.\n", GetLastError());
        return 1;
    }

    memset(&self->ov, 0, sizeof(self->ov));
    self->ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    DCB dcb = {0};
    dcb.DCBlength = sizeof(DCB);

    dcb.BaudRate = self->br; 
    dcb.ByteSize = self->byte_size; 
    dcb.Parity   = self->parity; 
    dcb.StopBits = self->stop_bit;

    if (!SetCommState(self->_hcom, &dcb)) {
        fprintf(stderr, "SetCommState with err (%lu).\n", GetLastError());
        return 2;
    }

    if (!GetCommState(self->_hcom, &dcb)) {
        fprintf(stderr, "GetCommState with err (%lu).\n", GetLastError());
        return 3;
    }

    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout         = MAXDWORD;
    timeouts.ReadTotalTimeoutConstant    = 0;
    timeouts.ReadTotalTimeoutMultiplier  = 0;
    timeouts.WriteTotalTimeoutConstant   = 0;
    timeouts.WriteTotalTimeoutMultiplier = 0;
    SetCommTimeouts(self->_hcom, &timeouts);

    SetCommMask(self->_hcom, EV_RXCHAR);
    memset(self->rx_buffer, 0, sizeof(self->rx_buffer));
    self->rx_len = 0;

    prv_com_info(&dcb);
    return 0;
}

static void uart_clear_buffer(uart_context *self) {
    self->rx_len = 0;
}


int uart_read(uart_context *self, uint8_t* rx_buff, size_t rx_len) {
    DWORD event_mask;

    COMSTAT st = {0};
    DWORD errs = 0;
    ClearCommError(self->_hcom, &errs, &st);

    if (st.cbInQue == 0) {
        return 0;
    }

    int free_space = sizeof(self->rx_buffer) - self->rx_len;
    if (free_space == 0) {
        uart_clear_buffer(self);
        free_space = sizeof(self->rx_buffer); 
    }

    int to_read = st.cbInQue;
    if (to_read > free_space) {
        to_read = free_space;
    }


    int read_byte = 0;
    ResetEvent(self->ov.hEvent);
    BOOL ok = ReadFile(
        self->_hcom, self->rx_buffer + self->rx_len, 
        to_read, &read_byte, &self->ov);
    if (!ok) {
        if (GetLastError() != ERROR_IO_PENDING) {
            fprintf(stderr, "ReadFile failed: %lu\n", GetLastError());
            return -1;
        }
        return 0;
    }

    self->rx_len += read_byte;
    return (int)read_byte;
}

void uart_pars_line(uart_context *self) {
    if (FRAMES_MAX_COUNT <= self->frames_count) {
        self->frames_count = 0;
    }

    uint8_t *beg_for_finde = self->rx_buffer + self->last_frame_pos;
    uint8_t *iter = beg_for_finde;

    int guard = self->rx_len - self->last_frame_pos;
    while ((guard - 1) >= 0 && *iter != '\r') {
        iter += 1;
        --guard;
    }

    if (*iter == '\r' && *(iter + 1) == '\n' ) {
        self->frames[self->frames_count].data = beg_for_finde;
        self->frames[self->frames_count].len = (iter - beg_for_finde); // remove \n and \t
        self->last_frame_pos += self->frames[self->frames_count].len + 2;
        self->frames_count++;
    }
}


// a a a a a a \r \t a a a a \r \t a a a a a a \r \t a a a \r \t a a 