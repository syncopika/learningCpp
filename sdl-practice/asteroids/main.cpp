#include <SDL2/SDL.h>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

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
    int mass;
    bool isSplit; // if the asteroid has been split already (so is it a whole asteroid or fragment)
    Vec2 velocity; // velocity of asteroid (the magnitude of the vector (sqrt(x^2 + y^2)) represents the speed)
    SDL_Texture* sprite;
};

struct Player {
    int x;
    int y;
    int score;
    int health;
    Vec2 velocity; // remember that velocity is speed and direction
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
	SDL_QueryTexture(tex, nullptr, nullptr, &dst.w, &dst.h);
	
	SDL_RenderCopyEx(ren, tex, nullptr, &dst, rotation, nullptr, SDL_FLIP_NONE);
}

void moveAsteroid(Asteroid* asteroid){
    double theta = atan2((double)asteroid->velocity.y, (double)asteroid->velocity.x); // in radians
    int speed = sqrt((asteroid->velocity.y*asteroid->velocity.y) + (asteroid->velocity.x*asteroid->velocity.x));
    int ySpeedComponent = speed*sin(theta);
    int xSpeedComponent = speed*cos(theta);
    
    asteroid->x += xSpeedComponent; //asteroid->direction.x;
    asteroid->y += ySpeedComponent; //asteroid->direction.y;
    
    if(asteroid->y > SCREEN_HEIGHT){
        asteroid->y = -30;
    }else if(asteroid->x > SCREEN_WIDTH){
        asteroid->x = -30;
    }else if(asteroid->y < -50){
        asteroid->y = SCREEN_HEIGHT-10; 
    }else if(asteroid->x < -50){
        asteroid->x = SCREEN_WIDTH-10;
    }
}

float dotProduct(Vec2& a, Vec2& b){
    return (a.x * b.x) + (a.y * b.y);
}

// https://www.vobarian.com/collisions/2dcollisions2.pdf
std::pair<Vec2, Vec2> getFinalVelocities(Asteroid* a, Asteroid* b){
    int radius = 25; // estimated radius of each asteroid. TODO: get a more accurate number
    int aCenterX = a->x + radius;
    int aCenterY = a->y + radius;
    int bCenterX = b->x + radius;
    int bCenterY = b->y + radius;
    int xDiff = bCenterX - aCenterX;
    int yDiff = bCenterY - aCenterY;
    float hyp = sqrt(xDiff*xDiff + yDiff*yDiff);
    
    Vec2 unitNormal{xDiff/hyp, yDiff/hyp};
    Vec2 unitTangent{-unitNormal.y, unitNormal.x};
    
    float aInitialVelocityNormal = dotProduct(unitNormal, a->velocity);
    float aInitialVelocityTangent = dotProduct(unitTangent, a->velocity);
    float bInitialVelocityNormal = dotProduct(unitNormal, b->velocity);
    float bInitialVelocityTangent = dotProduct(unitTangent, b->velocity);
    
    float aFinalVelocityNormal = (
        aInitialVelocityNormal*(a->mass - b->mass) + 
        2*b->mass*bInitialVelocityNormal) / 
        (a->mass + b->mass);
    
    float bFinalVelocityNormal = (
        bInitialVelocityNormal*(b->mass - a->mass) + 
        2*a->mass*aInitialVelocityNormal) / 
        (a->mass + b->mass);
        
    Vec2 aFinalVelocityNormalVec{
        aFinalVelocityNormal*unitNormal.x, 
        aFinalVelocityNormal*unitNormal.y
    };
    
    Vec2 aFinalVelocityTangentVec{
        aInitialVelocityTangent*unitTangent.x, 
        aInitialVelocityTangent*unitTangent.y
    };
    
    Vec2 bFinalVelocityNormalVec{
        bFinalVelocityNormal*unitNormal.x, 
        bFinalVelocityNormal*unitNormal.y
    };
    Vec2 bFinalVelocityTangentVec{
        bInitialVelocityTangent*unitTangent.x, 
        bInitialVelocityTangent*unitTangent.y
    };
    
    Vec2 aFinalVelocity{
        aFinalVelocityNormalVec.x + aFinalVelocityTangentVec.x, 
        aFinalVelocityNormalVec.y + aFinalVelocityTangentVec.y
    };
    Vec2 bFinalVelocity{
        bFinalVelocityNormalVec.x + bFinalVelocityTangentVec.x, 
        bFinalVelocityNormalVec.y + bFinalVelocityTangentVec.y
    };
    
    return std::make_pair(aFinalVelocity, bFinalVelocity);
}

bool hasCollision(Asteroid* a, Asteroid* b){
    int width = 50;
    int height = 50;
    //std::cout << "a->x: " << a->x << ", b->x: " << b->x << '\n';
    bool withinX = a->x <= (b->x + width) && (a->x + width) >= b->x;
    bool withinY = a->y <= (b->y + height) && (b->y + height) <= (a->y + height);
    return withinX && withinY;
}

void handleCollisions(Player& p, std::vector<Asteroid*>& asteroids){
    // for now work out collisions between asteroids only
    for(size_t i = 0; i < asteroids.size() - 1; i++){
        for(size_t j = i+1; j < asteroids.size(); j++){
            //std::cout << "testing " << i << " and " << j << '\n';
            Asteroid* a = asteroids[i];
            Asteroid* b = asteroids[j];
            // see if asteroid a and asteroid b should have a collision
            if(hasCollision(a, b) || hasCollision(b,a)){
                //std::cout << "collision!\n";
                SDL_SetTextureColorMod(a->sprite, 255, 0, 0);
                SDL_SetTextureColorMod(b->sprite, 0, 255, 0);
            }
        }
    }
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
	srand(time(nullptr));
	
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
    Player p1{SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0, 0, {0.f, 0.f}, nullptr};
	SDL_Texture* pTex = loadTexture("playerSprite.bmp", renderer);
	if(pTex == nullptr){
		return 1;
	}
	p1.sprite = pTex;
	
	// add asteroid sprites
    Asteroid a1{50, 44, 20, false, {-1.5f, 1.6f}, nullptr};
    Asteroid a2{70, 80, 10, false, {1.8f, 1.8f}, nullptr};
    Asteroid a3{90, 280, 25, false, {-1.4f, -1.5f}, nullptr};
    
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
    a3.sprite = ast1;

	// show on screen 
	SDL_RenderPresent(renderer);
    
    // add asteroids to a queue so we can process them in the event loop
    // using a queue to easily handle many of them
    std::vector<Asteroid*> asteroids;
    asteroids.push_back(&a1);
    asteroids.push_back(&a2);
    asteroids.push_back(&a3);
	
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

        // move asteroids
        handleCollisions(p1, asteroids);
        for(Asteroid* ast : asteroids){
            moveAsteroid(ast);
            renderTexture(ast->sprite, renderer, ast->x, ast->y, 0);
        }
		
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