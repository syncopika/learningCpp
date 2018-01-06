#include "Character.hh"

// set window dimensions 
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SPRITE_WIDTH = 34;
const int SPRITE_HEIGHT = 32;

// directions
const int UP = 1;
const int DOWN = 2;
const int LEFT = 3;
const int RIGHT = 4;
const int NONE = 0;

// log SDL error 
void logSDLError(std::ostream &os, const std::string &msg);

// load texture from file path (bmp image)
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);

// render texture to screen
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int Direction);

/*
    try to change to variadic function:
    https://eli.thegreenplace.net/2014/variadic-templates-in-c/
    http://www.willusher.io/sdl2%20tutorials/2014/08/01/postscript-1-easy-cleanup
*/
void cleanup(SDL_Window* win, SDL_Texture* background, SDL_Renderer* ren);

/*
	determine movement of character
    returns an integer corresponding to direction moved
*/
int determineMovement(Character& character, const Uint8 *state);
