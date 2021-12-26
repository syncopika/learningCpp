#include <SDL2/SDL.h>
#include <math.h>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#define PI 3.14159265

const int gridWidth = 10;
const int gridHeight = 50;
const int blockSize = 5; // 5px x 5px

const int SCREEN_WIDTH = gridWidth * blockSize;
const int SCREEN_HEIGHT = gridHeight * blockSize;

static std::chrono::time_point <std::chrono::steady_clock, std::chrono::milliseconds> start; // for controlling the player's velocity

enum Direction { LEFT, RIGHT, UP, DOWN, NONE };

struct Vec2 {
    float x;
    float y;
};

struct GridCell {
    bool hasBlock;
    SDL_Texture* sprite;
};

struct Grid {
    GridCell cells[gridHeight][gridWidth];
    
    void initGrid(){
        for(int i = 0; i < gridHeight; i++){
            for(int j = 0; j < gridWidth; j++){
                cells[i][j] = GridCell{false, nullptr};
            }
        }
    }
    
    void render(SDL_Renderer *ren){
        for(int i = 0; i < gridHeight; i++){
            for(int j = 0; j < gridWidth; j++){
                GridCell& c = cells[i][j];
                if(c.hasBlock != false){
                    SDL_Texture* blockSprite = c.sprite;
                    SDL_Rect dst;
                    dst.x = j * blockSize;
                    dst.y = i * blockSize;
                    SDL_QueryTexture(blockSprite, nullptr, nullptr, &dst.w, &dst.h); 
                    SDL_RenderCopyEx(ren, blockSprite, nullptr, &dst, 0, nullptr, SDL_FLIP_NONE);
                }
            }
        }        
    }
};

struct StraightTetromino {
    Vec2 pos;
    int speed;
    SDL_Texture* blockSprite;
    
    bool gridCheck(Grid& grid){
        // need to factor in the direction of movement eventually
        // and current rotation
        bool canPlace = true;
        int currPosX = pos.x; // used for placing each block of the tetromino
        
        if(pos.y == SCREEN_HEIGHT - blockSize){
            // see if all 4 blocks in a row can be placed at the bottom
            for(int i = 0; i < 4; i++){
                int gridX = currPosX / blockSize;
                int gridY = (SCREEN_HEIGHT - blockSize) / blockSize;
                canPlace = canPlace && !grid.cells[gridY][gridX].hasBlock;
                currPosX += blockSize;
            }
            return canPlace;
        }else{
            // can move
            for(int i = 0; i < 4; i++){
                int gridX = currPosX / blockSize;
                int gridY = (pos.y + blockSize + speed) / blockSize;
                canPlace = canPlace && !grid.cells[gridY][gridX].hasBlock;
                currPosX += blockSize;
            }
            return canPlace;            
        }
    }
    
    // when the tetromino can't move anymore,
    // mark its position on the grid
    void markGrid(Grid& grid){
        int currPosX = pos.x;
        int currPosY = pos.y;
        for(int i = 0; i < 4; i++){
            int gridX = currPosX / blockSize;
            int gridY = currPosY / blockSize;
            GridCell& c = grid.cells[gridY][gridX];
            c.hasBlock = true;
            c.sprite = blockSprite;
            currPosX += blockSize;
        }
    }
    
    // maybe pass in the grid to check if this
    // tetromino should stop moving
    void render(SDL_Renderer *ren, Grid& grid){
        // this should be if this tetromino can't move further downwards
        if(!gridCheck(grid)){
            markGrid(grid);
            return;
        }
        
        // show 4 blocks in a row based on pos
        // but what about rotation?
        int currPosX = pos.x;
        int currPosY = pos.y;
        
        for(int i = 0; i < 4; i++){
            SDL_Rect dst;
            dst.x = currPosX;
            dst.y = currPosY;
        
            // query the texture to get the width and height 
            // the rectangle will take on the width and height of the texture 
            SDL_QueryTexture(blockSprite, nullptr, nullptr, &dst.w, &dst.h); 
            SDL_RenderCopyEx(ren, blockSprite, nullptr, &dst, 0, nullptr, SDL_FLIP_NONE);
            
            // assuming the blocks are laid out horizontally for now
            currPosX += blockSize; // blockSize is global. fix that? or rename so it's a bit more clearer it's global
        }
        
        pos.y += speed;
    }
};

struct SquareTetromino {
    Vec2 pos;
    int speed;
    SDL_Texture* blockSprite;
    void render(){
        // display 4 blocks in a square formation based on pos
    }
};

struct TTetromino {
    Vec2 pos;
    int speed;
    SDL_Texture* blockSprite;
    void render(){
        // display 4 blocks in a T formation based on pos
    }
};

struct LTetromino {
    Vec2 pos;
    int speed;
    SDL_Texture* blockSprite;
    void render(){
        // display 4 blocks in a L formation based on pos
    }
};

struct SkewTetromino {
    Vec2 pos;
    int speed;
    SDL_Texture* blockSprite;
    void render(){
        // display 4 blocks in a skewed formation based on pos
    }
};

struct Player {
    int x;
    int y;
    int score;
    Vec2 forward; // down or left or right
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

/* linear ramp to max speed
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
}*/

/*
void movePlayer(Player& p, Direction dir){
    if(dir != NONE){
        if(dir == DOWN){
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
            int newX = p.x + forwardMagnitude;
            int newY = p.y + forwardMagnitude;
            
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
*/

SDL_Texture* loadTexture(const std::string& file, SDL_Renderer *ren){
    // initialize texture to null first 
    SDL_Texture *texture = nullptr;
    
    // load the image 
    SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());
    
    // if the load was successful, loadedImage won't be nullptr 
    if(loadedImage != nullptr){
        // color key the surface to make any 255,255,255 pixels transparent!
        SDL_SetColorKey(loadedImage, SDL_TRUE, SDL_MapRGB(loadedImage->format, 0xFF, 0xFF, 0xFF));
        
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

void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y, double rotation){
    // set up a destination rectangle to be at the position given by x and y 
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    
    // query the texture to get the width and height 
    // the rectangle will take on the width and height of the texture 
    SDL_QueryTexture(tex, nullptr, nullptr, &dst.w, &dst.h);
    
    SDL_RenderCopyEx(ren, tex, nullptr, &dst, rotation, nullptr, SDL_FLIP_NONE);
}

/*
template <class T, class S>
bool hasCollision(T* a, S* b){
    int width = 50; // TODO: don't hard code these values? should be taken from bmp image data
    int height = 50;
    bool withinX = a->x <= (b->x + width) && (a->x + width) >= b->x;
    bool withinY = a->y <= (b->y + height) && (b->y + height) <= (a->y + height);
    return withinX && withinY;
}


void handleCollisions(Tetromino& block, std::set<Tetromino*>& otherBlocks){
    //
}
*/

int main(int argc, char** argv){
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        logSDLError(std::cout,"SDL_Init Error: ");
        return 1;
    }
    
    // create a window 
    SDL_Window *window = SDL_CreateWindow("tetris", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
    SDL_Texture* bg = loadTexture("background.bmp", renderer);
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
        
        load in block sprites
        
    ***/
    std::unordered_map<std::string, SDL_Texture*> blockMap;
    SDL_Texture* blueBlockSprite = loadTexture("blueBlock.bmp", renderer);
    SDL_Texture* greenBlockSprite = loadTexture("greenBlock.bmp", renderer);
    if(blueBlockSprite == nullptr || greenBlockSprite == nullptr){
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        logSDLError(std::cout,"block sprite creation failed: ");
        SDL_Quit();
        return 1;
    }
    blockMap["blue"] = blueBlockSprite;
    blockMap["green"] = greenBlockSprite;
    
    /***        
        
        BEGIN EVENT LOOP
        
    ***/
    bool quit = false; 
    SDL_Event event;
    const Uint8 *keystate;
    
    Vec2 pos{10, -2};
    StraightTetromino tetro{pos, 1, blueBlockSprite};
    
    Grid grid;
    grid.initGrid();
    
    while(!quit){
        SDL_RenderClear(renderer); // clear the frame
        
        StraightTetromino& currBlock = tetro;
        
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
                            if(currBlock.pos.x - blockSize >= 0){
                                currBlock.pos.x -= blockSize;
                            }
                            break;
                        case SDLK_RIGHT:
                            if(currBlock.pos.x + blockSize <= SCREEN_WIDTH - blockSize){
                                currBlock.pos.x += blockSize;
                            }
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
        Direction dir = determineMovement(keystate);
        if(dir == DOWN){
            // TODO
        }
        
        // update player's current tetromino position
        currBlock.render(renderer, grid);
        
        // render all the blocks currently in place on the grid
        grid.render(renderer);
        
        // update screen 
        SDL_RenderPresent(renderer);
    }
    
    //cleanup
    SDL_DestroyTexture(bg);
    SDL_DestroyTexture(blueBlockSprite);
    SDL_DestroyTexture(greenBlockSprite);
    SDL_Quit();
    return 0;
}