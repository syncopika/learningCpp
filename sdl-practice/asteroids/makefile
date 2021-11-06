#makefile for asteroids

CXX = g++

SDL_LIB = -LC:/SDL2/lib -lSDL2main -lSDL2
SDL_INCLUDE = -IC:/SDL2/include

CXXFLAGS = -Wall -c -std=c++14 $(SDL_INCLUDE)
LDFLAGS = -lmingw32 -static-libstdc++ -static-libgcc $(SDL_LIB)

OBJS = main.o

EXE = asteroids.exe

all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $@

main.o: main.cpp
	$(CXX) $(CXXFLAGS) $< -o $@
	
clean:
	rm -f *.o && rm -f $(EXE)