# makefile for sdl test 
# http://www.willusher.io/sdl2%20tutorials/2013/08/15/lesson-0-mingw
CXX = g++

# the path to the lib folder of SDL is C:\SDL2-2.0.7\i686-w64-mingw32\lib
SDL_LIB = -LC:/SDL2-2.0.7/i686-w64-mingw32/lib -lSDL2main -lSDL2
SDL_INCLUDE = -IC:/SDL2-2.0.7/i686-w64-mingw32/include

# set up flags 
CXXFLAGS = -Wall -c -std=c++14 $(SDL_INCLUDE)
LDFLAGS = -lmingw32 -mwindows -static-libstdc++ -static-libgcc $(SDL_LIB)
EXE = sdl_demo.exe

# object files 
OBJS = main.o tutorial.o

all: $(EXE)

$(EXE): $(OBJS)
		$(CXX) $(OBJS) $(LDFLAGS) -o $@

# "$<" means the first file that comes after the ":" in "tutorial.o"
# "$@" means "tutorial.o", or the name of the object file to be created.
tutorial.o: tutorial.cpp tutorial.hh
		$(CXX) $(CXXFLAGS) $< -o $@
		
main.o: main.cpp tutorial.hh 
		$(CXX) $(CXXFLAGS) $< -o $@

# note that "rm -f" is used instead of "del" because using Windows
clean:
		rm -f *.o && rm -f $(EXE)