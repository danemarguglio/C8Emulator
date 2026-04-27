CC=g++
CFLAGS=-c -Wall -Wextra `sdl2-config --cflags`
LDFLAGS=`sdl2-config --libs`
SOURCES=main.cpp Emulator/Chip8Emulator.cpp Graphics/SDLGraphics.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=Chip8

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

main.o: main.cpp Emulator/Chip8Emulator.h Graphics/SDLGraphics.h
	$(CC) $(CFLAGS) $< -o $@

Emulator/Chip8Emulator.o: Emulator/Chip8Emulator.cpp Emulator/Chip8Emulator.h
	$(CC) $(CFLAGS) $< -o $@

Graphics/SDLGraphics.o: Graphics/SDLGraphics.cpp Graphics/SDLGraphics.h Emulator/Chip8Emulator.h
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f main.o Emulator/*.o Graphics/*.o $(EXECUTABLE)