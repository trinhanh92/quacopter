#ifndef __SERIAL_H__
#define __SERIAL_H__

#define UART_RECEIVE_BUFF_SIZE 1024

int uart_open();

int uart_write(int fd, unsigned char * tx_buffer, int len);

int uart_read(int fd, unsigned char * rx_buffer, int len);

int uart_close(int fd);

#endif
