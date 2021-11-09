#include <SDL2/SDL.h>
#include <math.h>
#include <chrono>
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

static std::chrono::time_point <std::chrono::steady_clock, std::chrono::milliseconds> start; // for controlling the player's velocity when the player presses the Up arrow key
static bool isMoving = false; // for controlling player movement

enum Direction { LEFT, RIGHT, UP, DOWN, NONE };

struct Vec2 {
    float x;
    float y;
};

struct Asteroid {
    int x;
    int y;
    int mass;
    bool isSplit; // if the asteroid has been split already (so is it a whole asteroid or fragment)
    Vec2 velocity; // velocity of asteroid (the magnitude of the vector (sqrt(x^2 + y^2)) represents the speed). we can also get direction via atan2 and x and y
    SDL_Texture* sprite;
};

struct Player {
    int x;
    int y;
    int score;
    int health;
    float speed;
    Vec2 forward; // the direction the player is facing. should be a unit vector
    SDL_Texture* sprite;
};

void logSDLError(std::ostream &os, const std::string &msg){
	os << msg << " error: " << SDL_GetError() << std::endl;
}

Direction determineMovement(const Uint8* state){
    if(state[SDL_SCANCODE_LEFT]){
        return LEFT;
    }else if(state[SDL_SCANCODE_RIGHT]){
        return RIGHT;
    }else if(state[SDL_SCANCODE_UP]){
        return UP;
    }else if(state[SDL_SCANCODE_DOWN]){
        return DOWN;
    }
    return NONE;
}

double getTimeElapsed(){
    // https://stackoverflow.com/questions/31487876/getting-a-time-difference-in-milliseconds
    // https://stackoverflow.com/questions/728068/how-to-calculate-a-time-difference-in-c
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::milli>(now - start).count();
}

// linear ramp to max speed
// it takes 2 seconds to reach max speed from 0
// and also slows down to 0 in 2 seconds
void linearRampToMaxSpeed(Player& p, double currentTimeElapsed){
    // do some linear interpolation to figure out how fast the player should be right now
    // we need to modify their velocity Vec2
    // we can get the slope from the velocity (i.e. velocity.y / velocity.x)
    // from there we get y = (velocity.y / velocity.x)*x and we can plug in current time for x.
    // if currentTimeElapsed > 3 seconds, stay on max speed
    if(currentTimeElapsed < 3000){
        float sec = currentTimeElapsed / 1000;
        float newSpeed = pow(1.6f, sec)*1.2f;
        p.speed = newSpeed > 5 ? 5 : newSpeed;
    }
}

void linearRampToMinSpeed(Player& p, double currentTimeElapsed){
    if(currentTimeElapsed < 3000){
        float sec = currentTimeElapsed / 1000;
        float newSpeed = pow(0.25f, sec)*2;
        p.speed = newSpeed < 0 ? 0 : newSpeed;
    }
}

void movePlayer(Player& p, Direction dir){
    if(dir != NONE){
        if(dir == UP){
            if(!isMoving){
                isMoving = true;
                start = std::chrono::time_point_cast<std::chrono::milliseconds>(
                    std::chrono::steady_clock::now()
                );
            }
        }else if(dir == LEFT){
            // rotate left
            float newAngle = -(PI/4); // 45 deg
            float cosAngle = cos(newAngle);
            float sinAngle = sin(newAngle);
            
            // rotating the forward vector
            float newForwardX = cosAngle*p.forward.x - sinAngle*p.forward.y;
            float newForwardY = sinAngle*p.forward.x + cosAngle*p.forward.y;
            
            p.forward.x = newForwardX;
            p.forward.y = newForwardY;
        }else if(dir == RIGHT){
            // rotate right
            float newAngle = (PI/4); // 45 deg
            float cosAngle = cos(newAngle);
            float sinAngle = sin(newAngle);
            
            float newForwardX = cosAngle*p.forward.x - sinAngle*p.forward.y;
            float newForwardY = sinAngle*p.forward.x + cosAngle*p.forward.y;
            
            p.forward.x = newForwardX;
            p.forward.y = newForwardY;
        }
        
        if(isMoving){
            // increase speed gradually to some maximum
            double elapsedTime = getTimeElapsed();
            linearRampToMaxSpeed(p, elapsedTime);
            
            // find the new x and y components of the current velocity
            float forwardMagnitude = sqrt(p.forward.x*p.forward.x + p.forward.y*p.forward.y) * p.speed;
            double angle = atan2(p.forward.y, p.forward.x);
            double xComponent = cos(angle)*forwardMagnitude;
            double yComponent = sin(angle)*forwardMagnitude;
            
            int newX = p.x + int(xComponent);
            int newY = p.y + int(yComponent);
            
            p.x = newX;
            p.y = newY;
        }
    }else{
        // player is at rest
        if(isMoving){
            isMoving = false;
            start = std::chrono::time_point_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now()
            );
        }
        if(!isMoving && p.speed > 0.0){
            double elapsedTime = getTimeElapsed();
            linearRampToMinSpeed(p, elapsedTime);
            
            // find the new x and y components of the current velocity
            float forwardMagnitude = sqrt(p.forward.x*p.forward.x + p.forward.y*p.forward.y) * p.speed;
            double angle = atan2(p.forward.y, p.forward.x);
            double xComponent = cos(angle)*forwardMagnitude;
            double yComponent = sin(angle)*forwardMagnitude;
            
            int newX = p.x + int(xComponent);
            int newY = p.y + int(yComponent);
            
            p.x = newX;
            p.y = newY;
        }
    }
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
    
    // TODO: figure out a better way but this seems to help
    // prevent asteroids from reaching a velocity that seems like 0 (so they appear to freeze) after a collision
    if(aFinalVelocity.x < 1 || aFinalVelocity.x < 0){
        aFinalVelocity.x++;
    }
    
    if(aFinalVelocity.y < 1 || aFinalVelocity.y < 0){
        aFinalVelocity.y++;
    }
    
    if(bFinalVelocity.x < 1 || bFinalVelocity.x < 0){
        bFinalVelocity.x++;
    }
    
    if(bFinalVelocity.y < 1 || bFinalVelocity.y < 0){
        bFinalVelocity.y++;
    }
    
    return std::make_pair(aFinalVelocity, bFinalVelocity);
}

bool hasCollision(Asteroid* a, Asteroid* b){
    int width = 50; // TODO: don't hard code these values? should be taken from bmp image data
    int height = 50;
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
                std::pair<Vec2, Vec2> finalVelocities = getFinalVelocities(a, b);
                a->velocity = finalVelocities.first;
                b->velocity = finalVelocities.second;
                
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
    Player p1{SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0, 0, 0.f, {0.f, -1.f}, nullptr};
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
	const Uint8 *keystate;
	
	while(!quit){
		// check for actions that will quit the program 
		if(SDL_PollEvent(&event)){
			switch(event.type){
				/* clicking on the X button on top right of screen */
				case SDL_QUIT:
					quit = true;
					break;
				
				/* pressing q or Esc on keyboard and handles movement for player */
                // we're avoiding SDL_GetKeyboardState because that registers multiples events for a single button press
                // which we don't want in this case
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym){
						case SDLK_ESCAPE:
						case SDLK_q:
							quit = true;
							break;
                        case SDLK_LEFT:
                            movePlayer(p1, LEFT);
                            break;
                        case SDLK_RIGHT:
                            movePlayer(p1, RIGHT);
                            break;
					}
				break;
			}
		}
        // redraw the background
		renderTexture(bg, renderer, 0, 0, 0);
        
		// then handle sprite movement
		keystate = SDL_GetKeyboardState(NULL);
		
		// redraw the sprites
        //angle = (angle == 360 ? 0 : angle+1);
        Direction dir = determineMovement(keystate);
        if(dir == UP || dir == NONE){
            movePlayer(p1, dir);
        }
        
        double currAngle = (atan2(p1.forward.y, p1.forward.x) * 180) / PI;
        renderTexture(p1.sprite, renderer, p1.x, p1.y, currAngle);

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