CC = gcc
CFLAGS = -c -Wall -g -Isrc/include -DVER_MAJOR=0 -DVER_MINOR=1 -DVER_MICRO=0
LDFLAGS =  
LDFLAGS_TEST = -ljCon
SOURCES = jcon_parser.c jcon.c jcon_serialization.c
SOURCES_TEST = tests.c
OBJECTS = $(SOURCES:.c=.o)
OBJECTS_TEST = $(SOURCES_TEST:.c=.o)

vpath %.c ./src/

EXECUTABLE = jCon_tests
LIB = libjCon.so

.PHONY: tests lib install clean clean-all

lib: $(LIB) $(OBJECTS)

tests: $(SOURCES_TEST) $(EXECUTABLE)

install: lib
	cp $(LIB) /usr/lib/
	cp ./src/include/jCon.h /usr/include/

$(LIB): $(OBJECTS)
	ar cr $@ $(OBJECTS)

$(EXECUTABLE): $(OBJECTS_TEST)
	$(CC) $(OBJECTS_TEST) -o $@ $(LDFLAGS_TEST)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o

clean-all:
	rm -f *.o
	rm -f *.so
	rm -f $(EXECUTABLE)
	rm -f $(LIB)
