#include "demo2.hh"
#include <cassert>

int main(int argc, char** argv){
	
	/*** 
		
		create necessary stuff, i.e. window, renderer 
		
	***/
	if(SDL_Init(SDL_INIT_VIDEO) != 0){
		logSDLError(std::cout,"SDL_Init Error: ");
		return 1;
	}
	
	// create a window 
	SDL_Window *window = SDL_CreateWindow("demo2", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
	
	
	/***
	
		set up background
	
	***/
	// try to see if you can stretch "grass.bmp" since it's smaller than the screen 
	SDL_Texture *bg = loadTexture("resources/grassLarge.bmp", renderer);
	if(bg == nullptr){
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		logSDLError(std::cout,"BG creation failed: ");
		SDL_Quit();
		return 1;
	}
	
	// put the background on the screen
	renderTexture(bg, renderer, 0, 0);
	
	/***
	
		set up the character
	
	***/
	Character c1 = Character("myCharacter", "resources/sprite1.bmp", renderer);
	if(c1.getTexture() == nullptr){
		return 1;
	}
	c1.setXPos(SCREEN_WIDTH / 2);
	c1.setYPos(SCREEN_HEIGHT / 2);
	renderTexture(c1.getTexture(), renderer, c1.getXPos(), c1.getYPos());
	
	// show on screen 
	SDL_RenderPresent(renderer);
	
	/***
		
		BEGIN EVENT LOOP 
	
	***/
	bool quit = false; 
	SDL_Event event;
	const Uint8 *keystate;
	
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
		keystate = SDL_GetKeyboardState(NULL);
		determineMovement(c1, keystate);
		
		// redraw the background
		renderTexture(bg, renderer, 0, 0);
		
		// redraw the sprite
		renderTexture(c1.getTexture(), renderer, c1.getXPos(), c1.getYPos());
		
		// update screen 
		SDL_RenderPresent(renderer);
	}
	
	cleanup(window, bg, renderer);
	c1.destroyTexture();
	SDL_Quit();
	return 0;
	
}