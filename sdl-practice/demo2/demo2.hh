#include <iostream>
#include <SDL2/SDL.h>

// set window dimensions 
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SPRITE_WIDTH = 34;
const int SPRITE_HEIGHT = 32;

// create a character class 
class Character{

	private:
		std::string charName;
		SDL_Texture *charTexture;
		int xPos;
		int yPos;

	public:
	
		// constructor 
		// @param name = the character's name 
		// @param file = the file path to the character's .bmp image
		// 
		Character(const std::string name, const std::string file, SDL_Renderer *ren);
		
		
		// get the character's name 
		std::string getName();
		
		// get the character's x position
		int getXPos();
		
		// get the character's y position 
		int getYPos();
		
		// get texture 
		SDL_Texture* getTexture();
		
		
		// set x pos 
		void setXPos(int x);
		
		// set y pos 
		void setYPos(int y);
		
		
		// destroy texture member 
		void destroyTexture();
		
};

void logSDLError(std::ostream &os, const std::string &msg);
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);

// try to change to variadic function:
// https://eli.thegreenplace.net/2014/variadic-templates-in-c/
// http://www.willusher.io/sdl2%20tutorials/2014/08/01/postscript-1-easy-cleanup
void cleanup(SDL_Window* win, SDL_Texture* background, SDL_Renderer* ren);

void determineMovement(Character& character, const Uint8 *state);
