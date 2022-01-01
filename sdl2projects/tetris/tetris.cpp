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

TODO:
    - implement rotations (absolutely necessary to get a full row)
    - implement different tetromino configurations

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

const int gridWidth = 12;
const int gridHeight = 50;
const int blockSize = 5; // 5px x 5px
const int rowWidth = (120/blockSize); // use this for now until we figure out the width weirdness.

const int SCREEN_WIDTH = gridWidth * blockSize; // TODO: figure out why I can't seem to get the actual screen width smaller than 120 :/
const int SCREEN_HEIGHT = gridHeight * blockSize;

static std::chrono::time_point <std::chrono::steady_clock, std::chrono::milliseconds> start;

void logSDLError(std::ostream &os, const std::string &msg){
    os << msg << " error: " << SDL_GetError() << std::endl;
}

double getTimeElapsed(){
    // https://stackoverflow.com/questions/31487876/getting-a-time-difference-in-milliseconds
    // https://stackoverflow.com/questions/728068/how-to-calculate-a-time-difference-in-c
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::milli>(now - start).count();
}

enum TetrominoType { STRAIGHT, SQUARE, SKEW, T, L };
enum BlockColor { GREEN, BLUE };

struct Vec2 {
    float x;
    float y;
};

struct GridCell {
    bool hasBlock;
    SDL_Texture* sprite;
};

struct Grid {
    // TODO: since the renderer's width is larger than what I asked for (DPI-related?),
    // query the renderer for the actual dimensions?
    // also: maybe just use std::vector so we don't have to know dimensions ahead of time?
    GridCell cells[gridHeight][rowWidth];
    
    void initGrid(){
        for(int i = 0; i < gridHeight; i++){
            for(int j = 0; j < rowWidth; j++){ // 120 is currently what is set for the width per the renderer
                cells[i][j] = GridCell{false, nullptr};
            }
        }
    }
    
    // possibly bad for performance but just for now,
    // iterate through the grid to see if we have any full rows
    // if so, remove them and move previous rows
    void checkFullRows(){
        // step 1: find the full rows (store index of each full row)
        std::vector<int> fullRows;
        
        for(int i = 1; i < gridHeight; i++){
            bool isFullRow = true;
            for(int j = 0; j < rowWidth; j++){
                isFullRow = isFullRow && cells[i][j].hasBlock;
            }
            if(isFullRow) fullRows.push_back(i);
        }
        
        // step 2: remove and move prev rows up
        // get the rows we need to cancel via fullRows.back() - fullRows[0]
        // everything from row 0 to fullRows[0] should be moved up
        if(fullRows.size() > 0){
            for(int i = fullRows[0]; i <= fullRows.back(); i++){
                for(int j = 0; j < rowWidth; j++){
                    // TODO
                    cells[i][j].hasBlock = false;
                    cells[i][j].sprite = nullptr;
                }
            }
            
            // only need to move prev row up if curr row is > 0
            for(int i = fullRows[0]-1; i >= 0; i--){
                // TODO
                // kinda tricky? need to take the row behind fullRows[0]
                // and move it to fullRows.back()
                // so some extra addition needed to find the right row index
            }
        }
    }
    
    void render(SDL_Renderer *ren){
        for(int i = 0; i < gridHeight; i++){
            for(int j = 0; j < rowWidth; j++){
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
// straight, square, T-, L-, skew
// straight -> 4 blocks in a row; pos.x, pos.x+blockSize, pos.x+blockSize*2, ..., same pos.y for all
// square -> (pos.x, pos.y), (pos.x+blockSize, pos.y), (pos.x, pos.y+blockSize), (pos.x+blockSize, pos.y+blockSize)
// T -> (pos.x, pos.y), (pos.x+blockSize, pos.y), (pos.x+blockSize*2, pos.y), (pos.x+blockSize, pos.y+blockSize)
// L -> (pos.x, pos.y), (pos.x, pos.y+blockSize), (pos.x, pos.y+blockSize*2), (pos.x+blockSize, pos.y+blockSize*2)
// skew -> (pos.x, pos.y), (pos.x, pos.y+blockSize), (pos.x+blockSize, pos.y+blockSize), (pos.x+blockSize, pos.y+blockSize*2)
// map of tetromino configurations to functions? :D
struct Tetromino {
    Vec2 pos;
    int speed;
    TetrominoType configuration;
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
                int gridY = (currPosY + blockSize) / blockSize;
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
    
    void reset(std::unordered_map<BlockColor, SDL_Texture*>& blockMap){
        // this block has reached an end so start over and change the tetromino type
        pos.x = 20;
        pos.y = -blockSize * 2;
        blockSprite = blockMap[BlockColor(rand()%2)]; // only 2 colors right now
    }
    
    // maybe pass in the grid to check if this
    // maybe pass in the grid to check if this
    // tetromino should stop moving
    void render(SDL_Renderer *ren, Grid& grid, std::unordered_map<BlockColor, SDL_Texture*>& blockMap){
        // this should be if this tetromino can't move further downwards
        if(gridCheck(grid) == false){
            //std::cout << "stop moving\n";
            markGrid(grid);
            reset(blockMap);
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
    std::cout << "SCREEN_WIDTH: " << SCREEN_WIDTH << '\n';
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
    std::unordered_map<BlockColor, SDL_Texture*> blockMap;
    SDL_Texture* blueBlockSprite = loadTexture("blueBlock.bmp", renderer);
    SDL_Texture* greenBlockSprite = loadTexture("greenBlock.bmp", renderer);
    
    if(blueBlockSprite == nullptr || greenBlockSprite == nullptr){
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        logSDLError(std::cout,"block sprite creation failed: ");
        SDL_Quit();
        return 1;
    }
    
    blockMap[BlockColor::BLUE] = blueBlockSprite;
    blockMap[BlockColor::GREEN] = greenBlockSprite;
    
    /***        
        BEGIN EVENT LOOP
    ***/
    bool quit = false; 
    SDL_Event event;
    
    Vec2 pos{10, -2};
    Tetromino currTetro{pos, 1, TetrominoType::STRAIGHT, blueBlockSprite};
    
    Grid grid;
    grid.initGrid();
    
    while(!quit){
        SDL_RenderClear(renderer); // clear the frame
        
        // check for actions that will quit the program 
        if(SDL_PollEvent(&event)){
            switch(event.type){
                /* clicking on the X button on top right of screen */
                case SDL_QUIT:
                    quit = true;
                    break;
                
                // pressing q or Esc on keyboard and handles movement for player
                // we're avoiding SDL_GetKeyboardState because that registers multiples events for a single button press
                // which we don't want in this case
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym){
                        case SDLK_ESCAPE:
                        case SDLK_q:
                            quit = true;
                            break;
                        case SDLK_LEFT:
                            if(currTetro.pos.x - blockSize >= 0){
                                currTetro.pos.x -= blockSize;
                            }
                            break;
                        case SDLK_RIGHT:
                            // TODO: since the renderer's width is larger than what I asked for,
                            // query the renderer for the actual dimensions?
                            int screenWidth, screenHeight;
                            SDL_GetRendererOutputSize(renderer, &screenWidth, &screenHeight);
                            
                            // currently this is specific to a straight tetromino FYI
                            if(currTetro.pos.x + blockSize < screenWidth - (blockSize*4)){
                                currTetro.pos.x += blockSize;
                            }
                            break;
                    }
                break;
            }
        }
        
        // redraw the background
        renderTexture(bg, renderer, 0, 0, 0);
        
        // update player's current tetromino position
        currTetro.render(renderer, grid, blockMap);
        
        // render all the blocks currently in place on the grid
        grid.render(renderer);
        
        // update screen 
        SDL_RenderPresent(renderer);
    }
    
    // cleanup
    SDL_DestroyTexture(bg);
    SDL_DestroyTexture(blueBlockSprite);
    SDL_DestroyTexture(greenBlockSprite);
    SDL_Quit();
    return 0;
}