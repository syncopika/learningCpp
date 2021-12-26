#makefile for tetris clone

CXX = g++

SDL_LIB = -LC:/SDL2/lib -lSDL2main -lSDL2
#-LC:/libraries/SDL2-2.0.10/i686-w64-mingw32/lib -lSDL2main -lSDL2
SDL_INCLUDE = -IC:/SDL2/include
#-IC:/libraries/SDL2-2.0.10/i686-w64-mingw32/include

CXXFLAGS = -Wall -c -std=c++14 $(SDL_INCLUDE)
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