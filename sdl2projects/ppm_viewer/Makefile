CXX = g++

FLAGS = -Wall -g -std=c++14

all: main.exe

main.o: main.cpp
	$(CXX) $(FLAGS) -IC:\libraries\SDL2-2.0.10\i686-w64-mingw32\include $< -c -o $@
	
main.exe : main.o
	$(CXX) $< -lmingw32 -static-libstdc++ -LC:\libraries\SDL2-2.0.10\i686-w64-mingw32\lib -lSDL2main -lSDL2 -o $@

clean:
	rm *.o && rm *.exe