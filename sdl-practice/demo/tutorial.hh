#include <iostream>
#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void logSDLError(std::ostream &os, const std::string &msg);
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren, int width, int height);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);