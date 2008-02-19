#Will Dietz
#sample client for testing the servers


APPNAME = itp-client

CC=gcc
LD=$(CC)
INCLUDES= -I../common
CFLAGS= -g -O2 -Wall -Werror $(INCLUDES)
LDFLAGS= -lX11 -lXtst 


OBJS = \
	   itp-client.o


all:   $(APPNAME) 

$(APPNAME):  $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^  

rebuild: clean depend all

depend:
	makedepend $(OBJS:.o=.c)

.c.o :
	$(CC) $(CFLAGS) -c $*.c -o $@
 

clean:
	-$(RM) -f $(APPNAME) *.o

# DO NOT DELETE

itp-client.o: /usr/include/stdio.h /usr/include/features.h
itp-client.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
itp-client.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
itp-client.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
itp-client.o: /usr/include/libio.h /usr/include/_G_config.h
itp-client.o: /usr/include/wchar.h /usr/include/bits/wchar.h
itp-client.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
itp-client.o: /usr/include/bits/sys_errlist.h
