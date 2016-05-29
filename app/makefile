#flag
INCLUDE =   -I./ \


CFLAGS = -Wall -g $(INCLUDE)
LIBS = -lmicrohttpd -lcrypto 
OBJS = main.o misc.o http_resp.o

#tool
CC = gcc

#building
quadcopter: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)
clean:
	-rm -f *.o quadcopter
.c.o:
	${CC} ${CFLAGS} ${INCLUDES} -c $<
