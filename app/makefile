#flag
INCLUDE =   -I./ \


CFLAGS = -Wall -g $(INCLUDE)
LIBS = -lmicrohttpd -lcrypto -lwiringPi -pthread
OBJS = main.o misc.o http_resp.o gps.o serial.o

#tool
CC = gcc

#building
quadcopter: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)
clean:
	-rm -f *.o quadcopter
.c.o:
	${CC} ${CFLAGS} ${INCLUDES} -c $<
