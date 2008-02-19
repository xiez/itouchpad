#Will Dietz
#linux server for iTouchpad


APPNAME = itp-server

CC=gcc
LD=$(CC)
CFLAGS= -g -O2 -Wall -Werror 
LDFLAGS= -lX11 -lXtst -lcurses


OBJS = \
	   itp-server.o


all:   $(APPNAME) 

$(APPNAME):  $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^  

rebuild: clean depend all

depend:
	makedepend $(OBJS:.o=.c)

.cpp.o :
	$(CXX) $(CFLAGS) -c $*.cpp -o $@
 

clean:
	-$(RM) -f $(APPNAME) *.o

# DO NOT DELETE

itp-server.o: /usr/include/curses.h /usr/include/ncurses_dll.h
itp-server.o: /usr/include/stdio.h /usr/include/features.h
itp-server.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
itp-server.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
itp-server.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
itp-server.o: /usr/include/libio.h /usr/include/_G_config.h
itp-server.o: /usr/include/wchar.h /usr/include/bits/wchar.h
itp-server.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
itp-server.o: /usr/include/bits/sys_errlist.h /usr/include/unctrl.h
itp-server.o: /usr/include/stdlib.h /usr/include/sys/types.h
itp-server.o: /usr/include/time.h /usr/include/endian.h
itp-server.o: /usr/include/bits/endian.h /usr/include/sys/select.h
itp-server.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
itp-server.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
itp-server.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
itp-server.o: /usr/include/string.h /usr/include/X11/Xlib.h
itp-server.o: /usr/include/X11/X.h /usr/include/X11/Xfuncproto.h
itp-server.o: /usr/include/X11/Xosdefs.h /usr/include/X11/Xutil.h
itp-server.o: /usr/include/X11/keysym.h /usr/include/X11/keysymdef.h
itp-server.o: /usr/include/X11/extensions/XTest.h
itp-server.o: /usr/include/X11/extensions/XInput.h
itp-server.o: /usr/include/X11/extensions/XI.h
