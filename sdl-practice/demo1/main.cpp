// http://www.willusher.io/sdl2%20tutorials/2013/08/15/lesson-0-mingw
// sdl test
#include "tutorial.hh"

int main(int argc, char **argv){

	if(SDL_Init(SDL_INIT_VIDEO) != 0){
		logSDLError(std::cout,"SDL_Init Error: ");
		return 1;
	}
	
	// create a window 
	SDL_Window *window = SDL_CreateWindow("Hello World!", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
	
	// then load image into the renderer - turns surface into a texture 
	SDL_Texture *tex = loadTexture("resources/Hydrangeas.bmp", renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	if(tex == nullptr){
		SDL_DestroyTexture(tex);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}
	
	// load a 2nd texture 
	SDL_Texture *tex2 = loadTexture("resources/Tulips.bmp", renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	if(tex2 == nullptr){
		SDL_DestroyTexture(tex2);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}
	
	/***
		START EVENT LOOP 
	***/
	SDL_Event event;
	bool quit = false;
	bool flag = true;
	
	renderTexture(tex, renderer, 0, 0);
	renderTexture(tex, renderer, SCREEN_WIDTH / 2, 0);
	renderTexture(tex, renderer, 0, SCREEN_HEIGHT / 2);
	renderTexture(tex, renderer, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	SDL_RenderPresent(renderer);
	
	while(!quit){
		
		while(SDL_PollEvent(&event)){
			// waits for an event to occur, which breaks the loop
			if(event.type == SDL_QUIT){
				quit = true;
			}
			if(event.type == SDL_KEYDOWN){
				quit = true;
			}
			if(event.type == SDL_MOUSEBUTTONDOWN){
				// if mouse button down, change the picture in the window 
				// note that this action doesn't change the value of "quit", 
				// so the program will keep running, allowing you to toggle between images 
				// on each mouse click 
				if(flag){
					renderTexture(tex2, renderer, 0, 0);
					renderTexture(tex2, renderer, SCREEN_WIDTH / 2, 0);
					renderTexture(tex2, renderer, 0, SCREEN_HEIGHT / 2);
					renderTexture(tex2, renderer, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
					flag = false;
				}else{
					renderTexture(tex, renderer, 0, 0);
					renderTexture(tex, renderer, SCREEN_WIDTH / 2, 0);
					renderTexture(tex, renderer, 0, SCREEN_HEIGHT / 2);
					renderTexture(tex, renderer, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
					flag = true;
				}
				SDL_RenderPresent(renderer);
			}
		}
		
	}
	
	//SDL_Delay(3000);
	
	// clean up 
	SDL_DestroyTexture(tex);
	SDL_DestroyTexture(tex2);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	
	SDL_Quit();
	return 0;
}