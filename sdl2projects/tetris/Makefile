#makefile for tetris clone

CXX = g++

SDL_LIB = -LC:/libraries/SDL2-2.0.10/i686-w64-mingw32/lib -lSDL2main -lSDL2
#-LC:/SDL2/lib -lSDL2main -lSDL2
SDL_INCLUDE = -IC:/libraries/SDL2-2.0.10/i686-w64-mingw32/include
#-IC:/SDL2/include

CXXFLAGS = -Wall -c -g -std=c++14 $(SDL_INCLUDE)
LDFLAGS = -lmingw32 -static-libstdc++ -static-libgcc $(SDL_LIB)

OBJS = tetris.o

EXE = tetris.exe

all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $@

tetris.o: tetris.cpp
	$(CXX) $(CXXFLAGS) $< -o $@
	
clean:
	rm -f *.o && rm -f $(EXE)