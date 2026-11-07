#ifndef UART_H
#define UART_H


void uart_init();
void uart_tx(char c);
char uart_rx(void);
void uart_print_str(const char* str);
#endif