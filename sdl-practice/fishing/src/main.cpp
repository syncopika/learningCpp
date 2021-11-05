#include "../headers/functions.hh"
#include <cstdlib>
#include <ctime>

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
	
	// initialize random seed for the fishing part
	srand(time(NULL));
	
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
	renderTexture(bg, renderer, 0, 0, NONE);
	
	
	/***
	
		set up the character
	
	***/
	Character c1 = Character("myCharacter", "resources/sprite1.bmp", renderer);
	if(c1.getTexture() == nullptr){
		return 1;
	}
	c1.setXPos(SCREEN_WIDTH / 2);
	c1.setYPos(SCREEN_HEIGHT / 2);
	renderTexture(c1.getTexture(), renderer, c1.getXPos(), c1.getYPos(), DOWN);
	
	// add walking sprites for character 
	SDL_Texture *walk1 = loadTexture("resources/sprite1-walk1.bmp", renderer);
	SDL_Texture *walk2 = loadTexture("resources/sprite1-walk2.bmp", renderer);
	if(walk1 == nullptr || walk2 == nullptr){
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		logSDLError(std::cout,"walk sprite creation failed: ");
		SDL_Quit();
		return 1;
	}
	c1.addWalkTexture(walk1);
	c1.addWalkTexture(walk2);
	
	// I'm being a bit lazy, so I'm going to add a character_fishing texture to 
	// the character's vector of walking sprite textures.
	// since my character should be able to go fishing, I probably should have 
	// a field for that (I only need one character_fishing sprite since I can just rotate it
	// depending on direction)
	SDL_Texture *fishing = loadTexture("resources/sprite1-fishing.bmp", renderer);
	c1.addWalkTexture(fishing);
	
	/***
	
		set up water 
	
	***/
	GameObject water = GameObject("water", "resources/water.bmp", renderer);
	
	// put the water at 100,100 on the screen
	water.setXPos(100);
	water.setYPos(100);
	// put the water on the screen 
	renderTexture(water.getTexture(), renderer, water.getXPos(), water.getYPos(), NONE);

	// show on screen 
	SDL_RenderPresent(renderer);
	
	/***
		
		BEGIN EVENT LOOP 
	
	***/
	bool quit = false; 
	SDL_Event event;
	const Uint8 *keystate;
	
	// keep track of what step to alternate to for the walking sprite
	int currentStep = 0;
	Direction direction = NONE;
	Direction lastDirection = direction;
	
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
		
		// for this demo I wanted to try checking for any texture overlapping between
		// the character and an obstacle (i.e. water)
		
		// I was thinking about another way I might check if a coordinate is safe to move to or not:
		// how about keep a 2D matrix, where each coordinate maps to a coordinate on the screen (i.e. the whole map) 
		// you can fill it with 1's and 0's, where 0's correspond to the coordinates 
		// the player can't go to. if you have a rectangle representing a body of water,
		// you can fill all the coordinates that are within the area of the water with 0 
		// in the matrix 
		// then just before the player moves, check if moving would move the player into 
		// a coordinate with 0?
	
		// get the direction first that the player wants to move to. can change character position
		// if coordinates not legal
		direction = determineMovement(c1, keystate);
		
		// x and y ranges for obstacle
		int xMinWater = water.getXPos();
		int xMaxWater = xMinWater + water.getWidth();
		int yMinWater = water.getYPos();
		int yMaxWater = yMinWater + water.getHeight();
		
		// not really sure of a good method to check overlap (need to research this!)
		// for now, it might be sufficient to create an expanded box around the character using just 4 points,  
		// given that they have an even spread over the texture. 
		// I chose 10 as a distance that corresponds to how far my expanded box extends 
		int charMidpointY = c1.getYPos() + c1.getHeight()/2;
		int charMidpointX = c1.getXPos() + c1.getWidth()/2;
		int northPointY = charMidpointY - 10;
		int northPointX = charMidpointX;
		int westPointY = charMidpointY;
		int westPointX = charMidpointX - 10;
		int eastPointY = charMidpointY;
		int eastPointX = 10 + charMidpointX;
		int southPointY = charMidpointY + 10;
		int southPointX = charMidpointX;
		
		if((northPointX > xMinWater && northPointX < xMaxWater && northPointY > yMinWater && northPointY < yMaxWater) ||
			(southPointX < xMaxWater && southPointX > xMinWater && southPointY < yMaxWater && southPointY > yMinWater) ||
			(westPointX < xMaxWater && westPointX > xMinWater && westPointY < yMaxWater && westPointY > yMinWater) ||
			(eastPointX < xMaxWater && eastPointX > xMinWater && eastPointY < yMaxWater && eastPointY > yMinWater)){
				// revert direction 
				if(direction == DOWN){
					// correct the character's position
					c1.setYPos(c1.getYPos() - 5);
				}else if(direction == UP){
					c1.setYPos(c1.getYPos() + 5);
				}else if(direction == LEFT){
					c1.setXPos(c1.getXPos() + 5);
				}else{
					// going right
					c1.setXPos(c1.getXPos() - 5);
				}
		}
		
		// implement fishing here! 
		// if player hits one of the borders of the water, and they press the spacebar,
		// the character should fish for 2 seconds 
		if(keystate[SDL_SCANCODE_SPACE]){
			// check if character is adjacent to the water 
			// these coordinates represent the expanded box around the character
			if(lastDirection == UP){
				northPointY -= 5;
				eastPointY -= 5;
				westPointY -= 5;
				southPointY -= 5;
			}else if(lastDirection == DOWN){
				northPointY += 5;
				eastPointY += 5;
				westPointY += 5;
				southPointY += 5;
			}else if(lastDirection == LEFT){
				northPointX -= 5;
				southPointX -= 5;
				eastPointX -= 5;
				westPointX -= 5;
			}else{
				northPointX += 5;
				southPointX += 5;
				eastPointX += 5;
				westPointX += 5;
			}
			
			if((northPointX > xMinWater && northPointX < xMaxWater && northPointY > yMinWater && northPointY < yMaxWater) ||
			(southPointX < xMaxWater && southPointX > xMinWater && southPointY < yMaxWater && southPointY > yMinWater) ||
			(westPointX < xMaxWater && westPointX > xMinWater && westPointY < yMaxWater && westPointY > yMinWater) ||
			(eastPointX < xMaxWater && eastPointX > xMinWater && eastPointY < yMaxWater && eastPointY > yMinWater)){
				// redraw everything but this time with the fishing sprite 
				renderTexture(bg, renderer, 0, 0, NONE);
				renderTexture(water.getTexture(), renderer,  water.getXPos(), water.getYPos(), NONE);
				renderTexture(c1.getWalkTextures()[2], renderer,  c1.getXPos(), c1.getYPos(), lastDirection);
				SDL_RenderPresent(renderer);
				
				// hold fishing pose for 2 seconds
				SDL_Delay(2000);
				
				// get a random number between 1 and 10. see if player caught anything
				int randomNum = rand() % 10 + 1;
				
				if(randomNum == 4){
					// player caught a salmon
					SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Congratulations!", "You caught a salmon!", NULL);
				}else if(randomNum == 2){
					// player caught a tilapia
					SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Congratulations!", "You caught a tilapia!", NULL);
				}else if(randomNum == 10){
					// player caught a shark
					SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Congratulations!", "You caught a shark!", NULL);
				}
			}
		}

		// redraw the background
		SDL_Delay(50);
		renderTexture(bg, renderer, 0, 0, NONE);
		renderTexture(water.getTexture(), renderer,  water.getXPos(), water.getYPos(), NONE);
		
		if(direction){
			renderTexture(c1.getWalkTextures()[currentStep], renderer, c1.getXPos(), c1.getYPos(), direction);
			SDL_RenderPresent(renderer);
			currentStep = (currentStep == 1 ? 0 : 1) ;
			lastDirection = direction;
		}
		
		SDL_Delay(50);
		
		// redraw the sprite in standing position
		if(lastDirection == UP){
			renderTexture(c1.getTexture(), renderer, c1.getXPos(), c1.getYPos(), UP);
		}else if(lastDirection ==  LEFT){
			renderTexture(c1.getTexture(), renderer, c1.getXPos(), c1.getYPos(), LEFT);
		}else if(lastDirection == RIGHT){
			renderTexture(c1.getTexture(), renderer, c1.getXPos(), c1.getYPos(), RIGHT);
		}else{
			renderTexture(c1.getTexture(), renderer, c1.getXPos(), c1.getYPos(), DOWN);
		}
		
		// update screen 
		SDL_RenderPresent(renderer);
	}
	
	cleanup(window, bg, renderer);
	water.destroyTexture();
	c1.destroyTexture();
	SDL_Quit();
	return 0;
	
}