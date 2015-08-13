CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.cpp Chip8Emulator.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=Chip8

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@