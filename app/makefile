#flag
INCLUDE =   -I./ \
			-I../libraries/json/

LIBS_PATH = -L../libraries/json/

CFLAGS = -Wall -g $(INCLUDE) $(LIBS_PATH)
LIBS = -lmicrohttpd -lcrypto -ljsmn 
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
