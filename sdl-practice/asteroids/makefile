#makefile for asteroids

CXX = g++

# directory locations for header and source files 
HD_DIR = headers

SDL_LIB = -LC:/libraries/SDL2-2.0.10/i686-w64-mingw32/lib -lSDL2main -lSDL2
SDL_INCLUDE = -IC:/libraries/SDL2-2.0.10/i686-w64-mingw32/include

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