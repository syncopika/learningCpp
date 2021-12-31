/*

design idea:

we take a single struct that represents a tetromino.
it will have a position and an SDL_Texture representing the block that
makes up the tetromino.

we also have a grid struct to represent where all the tetrominos that
have reached their final position should be.

for each frame, we move the single tetromino towards the bottom of the screen, as well as
check to see if the current tetromino configuration needs to stop moving by comparing against
the grid struct, which tells us which cells in the grid are blocked.

for drawing the tetromino we take the SDL_Texture and, based on the configuration (e.g. straight, T-shaped, etc.),
we draw 4 blocks with that texture.

when the tetromino reaches a stopping point, we take its position, figure out where in the grid struct its 4 blocks are,
and then mark on the grid that those positions are now blocked. we also assign those cells the SDL_Texture of the blocks.

we also re-render the grid each frame so that we can see the previous tetrominos this way.

then the single tetromino struct gets reset; it should randomly get reassigned a new configuration and block texture. 
its position also gets reset to a position slightly above the screen. in this way we only need to keep track of a single
tetromino at any time and whenever it reaches a stopping point we record it in the grid and the grid renders all the
previous tetrominos.


*/

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

struct Vec2 {
    float x;
    float y;
};

struct GridCell {
    bool hasBlock;
    SDL_Texture* sprite;
};

struct Grid {
    GridCell cells[gridHeight][120/blockSize];
    
    // TODO: since the renderer's width is larger than what I asked for (DPI-related?),
    // query the renderer for the actual dimensions?
    void initGrid(){
        for(int i = 0; i < gridHeight; i++){
            for(int j = 0; j < (120/blockSize); j++){ // 120 is currently what is set for the width per the renderer
                cells[i][j] = GridCell{false, nullptr};
            }
        }
    }
    
    void render(SDL_Renderer *ren){
        for(int i = 0; i < gridHeight; i++){
            for(int j = 0; j < (120/blockSize); j++){
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

// TODO: use just one tetromino struct and come up with a way to
// keep track of the different block configurations
// square, T-, L-, skew
struct StraightTetromino {
    Vec2 pos;
    int speed;
    SDL_Texture* blockSprite;
    
    bool gridCheck(Grid& grid){
        // need to factor in the direction of movement eventually
        // and current rotation
        bool canPlace = true;
        int currPosX = pos.x; // used for placing each block of the tetromino
        int currPosY = pos.y;
        
        if(currPosY >= SCREEN_HEIGHT - blockSize){
            pos.y = SCREEN_HEIGHT - blockSize;
            //std::cout << "reached bottom\n";
            return false;
        }else{
            // can move
            for(int i = 0; i < 4; i++){
                int gridX = currPosX / blockSize;
                int gridY = (currPosY + (blockSize*speed)) / blockSize;
                canPlace = canPlace && !grid.cells[gridY][gridX].hasBlock;
                currPosX += blockSize;
            }
            return canPlace;            
        }
    }
    
    // when the tetromino can't move anymore,
    // mark its position on the grid
    void markGrid(Grid& grid){
        //std::cout << "marking grid\n";
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
    
    void reset(){
        // this block has reached an end so start over and change the tetromino type
        pos.x = 20;
        pos.y = -blockSize*2;
    }
    
    // maybe pass in the grid to check if this
    // tetromino should stop moving
    void render(SDL_Renderer *ren, Grid& grid){
        // this should be if this tetromino can't move further downwards
        if(gridCheck(grid) == false){
            //std::cout << "stop moving\n";
            markGrid(grid);
            reset();
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


void logSDLError(std::ostream &os, const std::string &msg){
    os << msg << " error: " << SDL_GetError() << std::endl;
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
    
    int ww;
    int wh;
    SDL_GetWindowSize(window, &ww, &wh);
    std::cout << "window width in pixels: " << ww << ", window height in pixels: " << wh << '\n'; 
    
    // create the renderer to render the window with 
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == nullptr){
        SDL_DestroyWindow(window);
        logSDLError(std::cout,"SDL_CreateRenderer Error: ");
        SDL_Quit();
        return 1;
    }
    
    std::cout << "renderer width in pixels: " << ww << ", window height in pixels: " << wh << '\n'; 
    SDL_GetRendererOutputSize(renderer, &ww, &wh);
    
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
    
    Vec2 pos{10, -2};
    StraightTetromino tetro{pos, 1, blueBlockSprite};
    
    Grid grid;
    grid.initGrid();
    
    StraightTetromino& currBlock = tetro;
    
    while(!quit){
        SDL_RenderClear(renderer); // clear the frame
        
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
                            // TODO: since the renderer's width is larger than what I asked for,
                            // query the renderer for the actual dimensions?
                            int screenWidth, screenHeight;
                            SDL_GetRendererOutputSize(renderer, &screenWidth, &screenHeight);
                            
                            // currently this is specific to a straight tetromino FYI
                            if(currBlock.pos.x + blockSize < screenWidth - (blockSize*4)){
                                currBlock.pos.x += blockSize;
                            }
                            break;
                    }
                break;
            }
        }
        
        // redraw the background
        renderTexture(bg, renderer, 0, 0, 0);
        
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