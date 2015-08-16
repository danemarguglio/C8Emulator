CC=g++
CFLAGS=-c -Wall `sdl2-config --cflags`
LDFLAGS=`sdl2-config --libs`
SOURCES=main.cpp Chip8Emulator.cpp SDLGraphics.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=Chip8

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

main.o: main.cpp Chip8Emulator.h SDLGraphics.h
	$(CC) $(CFLAGS) $< -o $@

Chip8Emulator.o: Chip8Emulator.cpp chip8emulator.h
	$(CC) $(CFLAGS) $< -o $@

SDLGraphics.o: SDLGraphics.cpp SDLGraphics.h
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o Chip8