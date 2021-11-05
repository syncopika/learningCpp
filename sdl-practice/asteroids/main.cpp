#include <SDL2/SDL.h>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

#define PI 3.14159265

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 480;
const int SPRITE_WIDTH = 34;
const int SPRITE_HEIGHT = 32;

struct Vec2 {
    float x;
    float y;
};

struct Asteroid {
    int x;
    int y;
    bool isSplit; // if the asteroid has been split already (so is it a whole asteroid or fragment)
    float velocity;
    Vec2 direction; // direction of asteroid
    SDL_Texture* sprite;
};

struct Player {
    int x;
    int y;
    int score;
    float velocity;
    Vec2 direction; // direction player is facing
    SDL_Texture* sprite;
};

void logSDLError(std::ostream &os, const std::string &msg){
	os << msg << " error: " << SDL_GetError() << std::endl;
}

SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren){
	// initialize texture to null first 
	SDL_Texture *texture = nullptr;
	
	// load the image 
	SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());
	
	// if the load was successful, loadedImage won't be nullptr 
	if(loadedImage != nullptr){
		// color key the surface to make any 255,255,255 pixels transparent!
		 SDL_SetColorKey(loadedImage, SDL_TRUE, SDL_MapRGB(loadedImage->format, 0xFF, 0xFF, 0xFF ) );
		
		// turn the surface to a texture 
		texture = SDL_CreateTextureFromSurface(ren, loadedImage);
		
		// free loadedImage memory since we're done with it 
		SDL_FreeSurface(loadedImage);
		
		// make sure texture creation was successful 
		if(texture == nullptr){
			logSDLError(std::cout, "CreateTextureFromSurface");
		}
	}else{
		// error with loadedImage 
		logSDLError(std::cout, "LoadBMP");
	}
	
	return texture; 
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, double rotation){
	// set up a destination rectangle to be at the position given by x and y 
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	
	// query the texture to get the width and height 
	// the rectangle will take on the width and height of the texture 
	SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	
	SDL_RenderCopyEx(ren, tex, NULL, &dst, rotation, NULL, SDL_FLIP_NONE);
}

int main(int argc, char** argv){
	if(SDL_Init(SDL_INIT_VIDEO) != 0){
		logSDLError(std::cout,"SDL_Init Error: ");
		return 1;
	}
	
	// create a window 
	SDL_Window *window = SDL_CreateWindow("asteroids", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if(window == nullptr){
		logSDLError(std::cout, "SDL_CreateWindow Error: ");
		SDL_Quit();
		return 1;
	}
	
	// create the renderer to render the window with 
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(renderer == nullptr){
		SDL_DestroyWindow(window);
		logSDLError(std::cout,"SDL_CreateRenderer Error: ");
		SDL_Quit();
		return 1;
	}
	
	// initialize random seed
	srand(time(NULL));
	
	/***
		set up background
	***/
	SDL_Texture *bg = loadTexture("background.bmp", renderer);
	if(bg == nullptr){
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		logSDLError(std::cout,"BG creation failed: ");
		SDL_Quit();
		return 1;
	}
	// put the background on the screen
	renderTexture(bg, renderer, 0, 0, 0);
	
	/***
		set up the player
	***/
    Player p1{SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0, 0.0f, {0.f, 0.f}, nullptr};
	SDL_Texture* pTex = loadTexture("playerSprite.bmp", renderer);
	if(pTex == nullptr){
		return 1;
	}
	p1.sprite = pTex;
	
	// add asteroid sprites
    Asteroid a1{20, 34, false, 0.0f, {0.f, 0.f}, nullptr};
    Asteroid a2{70, 80, false, 0.0f, {1.2f, 1.5f}, nullptr};
	SDL_Texture* ast1 = loadTexture("asteroidSprite1.bmp", renderer);
	SDL_Texture* ast2 = loadTexture("asteroidSprite2.bmp", renderer);
	if(ast1 == nullptr || ast2 == nullptr){
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		logSDLError(std::cout,"asteroid sprite creation failed: ");
		SDL_Quit();
		return 1;
	}
    a1.sprite = ast1;
    a2.sprite = ast2;

	// show on screen 
	SDL_RenderPresent(renderer);
	
	/***		
		BEGIN EVENT LOOP 
	***/
	bool quit = false; 
	SDL_Event event;
	//const Uint8 *keystate;
    
    double angle = 0;
	
	while(!quit){
		// check for actions that will quit the program 
		if(SDL_PollEvent(&event)){
			switch(event.type){
				/* clicking on the X button on top right of screen */
				case SDL_QUIT:
					quit = true;
					break;
				
				/* pressing q or Esc on keyboard */
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym){
						case SDLK_ESCAPE:
						case SDLK_q:
							quit = true;
							break;
					}
				break;
			}
		}
		
		// then handle sprite movement
		//keystate = SDL_GetKeyboardState(NULL);
	
		// redraw the background
		renderTexture(bg, renderer, 0, 0, 0);
		
		// redraw the sprites
        angle = (angle == 360 ? 0 : angle+1);
        renderTexture(p1.sprite, renderer, p1.x, p1.y, angle);
        
        renderTexture(a1.sprite, renderer, a1.x, a1.y, 0);

        // move asteroid
        a2.x += a2.direction.x;
        a2.y += a2.direction.y;
        if(a2.y > SCREEN_HEIGHT || a2.x > SCREEN_WIDTH){
            a2.y = -a2.direction.y;
            a2.x = -a2.direction.x;
        }
        renderTexture(a2.sprite, renderer, a2.x, a2.y, -angle);
		
		// update screen 
		SDL_RenderPresent(renderer);
	}
	
	//cleanup
    SDL_DestroyTexture(bg);
    SDL_DestroyTexture(pTex);
    SDL_DestroyTexture(ast1);
    SDL_DestroyTexture(ast2);
	SDL_Quit();
	return 0;
}