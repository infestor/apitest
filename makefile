## General Flags
PROJECT = INPA
TARGETS = jobs.exe tmode.exe

CESTA=d:/Dev-Cpp/bin/

CC = $(CESTA)c++.exe
CPP = $(CESTA)c++.exe

## Options common to compile, link and assembly rules
COMMON =

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
#CFLAGS += -std=gnu99
CFLAGS += -Wall -g0 -O2 -s

## Linker flags
LDFLAGS =

## Include Directories
#INCLUDES = -I"/local/benejan/mingw32/include/" -I"/local/benejan/mingw32/i686-w64-mingw32/include/"
#INCLUDES += -I"~/tmp/inpa"

## Library Directories
#LIBDIRS = -L"/local/benejan/mingw32/lib/" -I"/local/benejan/mingw32/i686-w64-mingw32/lib/"
#LIBDIRS += -L"~/tmp/inpa"
LIBDIRS += -L"d:/inpa_prg"

## Objects explicitly added by the user
LIBS = -lapiw32

## Objects that must be built in order to link
OBJECTS = apicalls.o print_result.o make_job.o frame_def.o

## Build
all: $(TARGETS)

## Compile
%.o: %.c
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@
%.o: %.cpp
	$(CPP) $(INCLUDES) $(CFLAGS) -c $< -o $@

##Link
jobs.exe: $(OBJECTS) jobs.o
	$(CC) $(LDFLAGS) $(OBJECTS) jobs.o $(LIBDIRS) $(LIBS) -o $@

tmode.exe: $(OBJECTS) tmode.o
	$(CC) $(LDFLAGS) $(OBJECTS) tmode.o $(LIBDIRS) $(LIBS) -o $@


## Clean target
.PHONY: clean all

clean:
	-rm -f $(OBJECTS) *.o *.exe


## Other dependencies
#-include $(shell mkdir dep 2>NUL) $(wildcard dep/*)

