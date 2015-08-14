CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.cpp Chip8Emulator.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=Chip8

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

main.o: main.cpp
	$(CC) $(CFLAGS) $< -o $@

Chip8Emulator.o: Chip8Emulator.cpp Chip8Emulator.h
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o Chip8