#include "uart.h"

#ifdef _WIN32
#   include "uart_win_impl.c"
#elif defined(__linux__)
#   include "uart_linux_impl.c"
#else
#error "The platform is not supported."
#endif
