#makefile for demo2 

CXX = g++

# directory locations for header and source files 
HD_DIR = headers
SRC_DIR = src

SDL_LIB = -LC:/libraries/SDL2-2.0.10/i686-w64-mingw32/lib -lSDL2main -lSDL2
SDL_INCLUDE = -IC:/libraries/SDL2-2.0.10/i686-w64-mingw32/include

CXXFLAGS = -Wall -c -std=c++14 $(SDL_INCLUDE)
LDFLAGS = -lmingw32 -mwindows -static-libstdc++ -static-libgcc $(SDL_LIB)

OBJS = main.o functions.o Character.o GameObject.o

EXE = demo2.exe

all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $@

functions.o: $(SRC_DIR)/functions.cpp $(HD_DIR)/functions.hh $(HD_DIR)/Character.hh $(HD_DIR)/GameObject.hh
	$(CXX) $(CXXFLAGS) $< -o $@
	
Character.o: $(SRC_DIR)/Character.cpp $(HD_DIR)/Character.hh $(HD_DIR)/GameObject.hh
	$(CXX) $(CXXFLAGS) $< -o $@
	
GameObject.o: $(SRC_DIR)/GameObject.cpp $(HD_DIR)/GameObject.hh 
	$(CXX) $(CXXFLAGS) $< -o $@
	
main.o: $(SRC_DIR)/main.cpp $(HD_DIR)/functions.hh $(HD_DIR)/Character.hh $(HD_DIR)/GameObject.hh
	$(CXX) $(CXXFLAGS) $< -o $@
	
clean:
	rm -f *.o && rm -f $(EXE)