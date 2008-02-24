#Will Dietz
#sample client for testing the servers


DRAG = itp-client-drag
MOVE = itp-client-move

APPS = $(DRAG) $(DRAG)-arm $(MOVE) $(MOVE)-arm

ARMCC=arm-apple-darwin-gcc
ARMLD=$(ARMCC)
CC=gcc
LD=$(CC)
INCLUDES= -I../common
CFLAGS= -g -O2 -Wall -Werror $(INCLUDES)
LDFLAGS=
ARMLDFLAGS= -Wl,-syslibroot,/usr/local/share/iphone-filesystem 

DEPS= ../common/mouseevent.h ../common/mconnection.c ../common/mconnection.h

all:  $(APPS) 

$(DRAG):  $(DRAG).o
	$(LD) $(LDFLAGS) -o $@ $^  
$(DRAG)-arm: $(DRAG).armo
	$(ARMLD) $(ARMLDFLAGS) -o $@ $^

$(MOVE): $(MOVE).o 
	$(LD) $(LDFLAGS) -o $@ $^  
$(MOVE)-arm: $(MOVE).armo 
	$(ARMLD) $(ARMLDFLAGS) -o $@ $^


%.armo: %.c
	$(ARMCC) $(CFLAGS) -c $*.c -o $@
	
%.o : %.c
	$(CC) $(CFLAGS) -c $*.c -o $@
 

clean:
	-$(RM) -f $(APPS) *.o *.armo
install:
	scp $(DRAG)-arm $(MOVE)-arm root@192.168.255.2:
