CC=g++
CFLAGS=-c -Wall `sdl2-config --cflags`
LDFLAGS=`sdl2-config --libs`
SOURCES=main.cpp Emulator/Chip8Emulator.cpp Graphics/SDLGraphics.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=Chip8

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

main.o: main.cpp Emulator/Chip8Emulator.h Graphics/SDLGraphics.h
	$(CC) $(CFLAGS) $< -o $@

Chip8Emulator.o: Emulator/Chip8Emulator.cpp Emulator/Chip8Emulator.h
	$(CC) $(CFLAGS) $< -o $@

SDLGraphics.o: Graphics/SDLGraphics.cpp Graphics/SDLGraphics.h
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o Chip8