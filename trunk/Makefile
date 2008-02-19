#Will Dietz
#iTouchpad client for iPhone

APPNAME=iTouchpad
SERVER=linux

ROOT=/usr/local/arm-apple-darwin
ARMCC=arm-apple-darwin-gcc
ARMLD=$(ARMCC)
CC=gcc
LD=$(CC)
INCLUDES= -Icommon
CFLAGS=-O2 -Wall -Werror $(INCLUDES)
LDFLAGS= \
	-Wl,-syslibroot,/usr/local/share/iphone-filesystem \
	-lobjc -ObjC -framework CoreFoundation \
	-framework Foundation \
	-framework CoreGraphics \
	-framework GraphicsServices \
	-framework UIKit \
	-framework LayerKit \
	-framework Message
COMMONOBJS= \
			common/mconnection.o
OBJS = \
	iTouchpadApp.o \
	itouchpad.o \
	ViewController.o \
	PrefsView.o \
	MainTable.o \
	itouchpadfunctions.o\
	$(COMMONOBJS)


all:   $(APPNAME) common

$(APPNAME):  $(OBJS)
	$(ARMLD) $(LDFLAGS) -o $@ $^  

itouchpad.o: itouchpad.m
	$(ARMCC) -c $(CFLAGS) $< -o $@

common/%.o: common/%.c common/%.h
	$(ARMCC) -c $(CFLAGS) $< -o $@

%.o: %.m %.h consts.h
	$(ARMCC) -c $(CFLAGS) $< -o $@

clean: local_clean
	cd $(SERVER) && make clean
local_clean:
	rm -f *.o common/*.o $(APPNAME)
install: $(APPNAME) 
	scp $(APPNAME) root@192.168.255.2:
#	scp $(APPNAME) root@10.5.16.180: 
#	scp $(APPNAME) root@172.16.1.34:
# I have 3 wireless networks.. don't ask.

common/mconnection.o: common

common:
	cd common && make

server-linux:
	cd $(SERVER) && make
	

