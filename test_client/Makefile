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
	makedepend $(OBJS:.o=.c) $(INCLUDES)

.c.o :
	$(CC) $(CFLAGS) -c $*.c -o $@
 

clean:
	-$(RM) -f $(APPNAME) *.o

# DO NOT DELETE

itp-client.o: ../common/mouseevent.h ../common/mconnection.c
itp-client.o: ../common/mconnection.h /usr/include/netdb.h
itp-client.o: /usr/include/features.h /usr/include/sys/cdefs.h
itp-client.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
itp-client.o: /usr/include/gnu/stubs-32.h /usr/include/netinet/in.h
itp-client.o: /usr/include/stdint.h /usr/include/bits/wchar.h
itp-client.o: /usr/include/sys/socket.h /usr/include/sys/uio.h
itp-client.o: /usr/include/sys/types.h /usr/include/bits/types.h
itp-client.o: /usr/include/bits/typesizes.h /usr/include/time.h
itp-client.o: /usr/include/endian.h /usr/include/bits/endian.h
itp-client.o: /usr/include/sys/select.h /usr/include/bits/select.h
itp-client.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
itp-client.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
itp-client.o: /usr/include/bits/uio.h /usr/include/bits/socket.h
itp-client.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
itp-client.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
itp-client.o: /usr/include/bits/posix2_lim.h /usr/include/bits/sockaddr.h
itp-client.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
itp-client.o: /usr/include/bits/in.h /usr/include/bits/byteswap.h
itp-client.o: /usr/include/rpc/netdb.h /usr/include/bits/netdb.h
itp-client.o: /usr/include/arpa/inet.h /usr/include/unistd.h
itp-client.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
itp-client.o: /usr/include/getopt.h /usr/include/string.h
itp-client.o: /usr/include/stdio.h /usr/include/libio.h
itp-client.o: /usr/include/_G_config.h /usr/include/wchar.h
itp-client.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
itp-client.o: /usr/include/bits/sys_errlist.h
