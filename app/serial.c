
#include <stdio.h>
#include <unistd.h>     //Used for UART
#include <fcntl.h>      //Used for UART
#include <termios.h>    //Used for UART
#include <time.h>
#include <string.h>
#include "serial.h"

//OPEN THE UART
  //The flags (defined in fcntl.h):
  //  Access modes (use 1 of these):
  //    O_RDONLY - Open for reading only.
  //    O_RDWR - Open for reading and writing.
  //    O_WRONLY - Open for writing only.
  //
  //  O_NDELAY / O_NONBLOCK (same function) - Enables nonblocking mode. When set read requests on the file can return immediately with a failure status
  //                      if there is no input immediately available (instead of blocking). Likewise, write requests can also return
  //                      immediately with a failure status if the output can't be written immediately.
  //
  //  O_NOCTTY - When set and path identifies a terminal device, open() shall not cause the terminal device to become the controlling terminal for the process.
int uart_open() {
  int uart0_fd = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);    //Open in non blocking read/write mode
  if (-1 == uart0_fd) {
    //ERROR - CAN'T OPEN SERIAL PORT
    printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
    return uart0_fd;
  }
    //CONFIGURE THE UART
  //The flags (defined in /usr/include/termios.h - see http://pubs.opengroup.org/onlinepubs/007908799/xsh/termios.h.html):
  //  Baud rate:- B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000, B921600, B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000
  //  CSIZE:- CS5, CS6, CS7, CS8
  //  CLOCAL - Ignore modem status lines
  //  CREAD - Enable receiver
  //  IGNPAR = Ignore characters with parity errors
  //  ICRNL - Map CR to NL on input (Use for ASCII comms where you want to auto correct end of line characters - don't use for bianry comms!)
  //  PARENB - Parity enable
  //  PARODD - Odd parity (else even)
  struct termios options;
  tcgetattr(uart0_fd, &options);
  options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;   //<Set baud rate
  options.c_iflag = IGNPAR;
  options.c_oflag = 0;
  options.c_lflag = 0;
  tcflush(uart0_fd, TCIFLUSH);
  tcsetattr(uart0_fd, TCSANOW, &options);
  return uart0_fd;
}

/**
* @brief 
*/
int uart_write(int fd, unsigned char * tx_buffer, int len) {
  int count = write(fd, &tx_buffer[0], len) ;   //Filestream, bytes to write, number of bytes to write
  if (count < 0) {
    printf("UART TX error\n");
  }
  return count;
}

/**
*/
int uart_read(int fd, unsigned char * rx_buffer, int len) {
  // Read up to 255 characters from the port if they are there
  // unsigned char rx_buffer[256];
  int rx_length = read(fd, (void*)rx_buffer, len);    //Filestream, buffer to store in, number of bytes to read (max)
  if (rx_length < 0) {
    //An error occured (will occur if there are no bytes)
    printf("Have error when received\n");
  }
  else if (rx_length == 0) {
    //No data waiting
    printf("Have no data received\n");
  }
  else {
    rx_buffer[rx_length] = '\0';
    // printf("%i bytes read : %s\n", rx_length, rx_buffer);
  }
  return rx_length; 
}

/**
* 
*/
int uart_close(int fd) {
  return close (fd);
}

/* TEST PROGRAM */
/*int main() 
{
  unsigned char rx_buffer[1024] = {0};
  unsigned char * p_start_rmc = NULL;
  int cnt = 100;
  int rx_length = -1;
  // open serial port
  uart_open();

  while (cnt--) {
    rx_length = uart_read (rx_buffer, sizeof rx_buffer);
    if (0 < rx_length) {
      p_start_rmc = gps_parser(rx_buffer, rx_length);
      printf("cnt: %d\n%s\n", cnt, p_start_rmc);
    }
    sleep(1);
  }
  return 0; 
}
*/