#makefile for demo2 

CXX = g++

SDL_LIB = -LC:/SDL2-2.0.7/i686-w64-mingw32/lib -lSDL2main -lSDL2
SDL_INCLUDE = -IC:/SDL2-2.0.7/i686-w64-mingw32/include

CXXFLAGS = -Wall -c -std=c++14 $(SDL_INCLUDE)
LDFLAGS = -lmingw32 -mwindows -static-libstdc++ -static-libgcc $(SDL_LIB)

OBJS = main.o demo2.o

EXE = demo2.exe

all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $@

demo2.o: demo2.cpp demo2.hh
	$(CXX) $(CXXFLAGS) $< -o $@
	
main.o: main.cpp demo2.hh
	$(CXX) $(CXXFLAGS) $< -o $@
	
clean:
	rm -f *.o && rm -f $(EXE)