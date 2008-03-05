#Will Dietz
#linux server for iTouchpad


APPNAME = itp-server

CC=gcc
LD=$(CC)
INCLUDES= -I../common
CFLAGS= -g -O2 -Wall -Werror $(INCLUDES)
LDFLAGS= -lX11 -lXtst


OBJS = \
	   itp-server.o


all:   $(APPNAME) 

$(APPNAME):  $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^  

rebuild: clean depend all

depend:
	makedepend $(OBJS:.o=.c) $(INCLUDES)

.c.o :
	$(CC) $(CFLAGS) -c $*.c -o $@
 

clean:
	-$(RM) -f $(APPNAME) *.o

