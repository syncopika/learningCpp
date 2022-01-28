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
    - end game when there's no more room to move (I think it might just segfault at the moment lol)
    - BUG: tetrominos can collide with already placed ones -> I think the check for hasBlock is mostly working
           when moving right (still might not be correct), and currently not implemented for moving left
    - figure out what happens when there's no more room to move
      e.g. first row is blocking

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

// TODO: make these #define ?
const int gridWidth = 15;
const int gridHeight = 40;
const int blockSize = 5; // 5px x 5px blocks

const int SCREEN_WIDTH = gridWidth * blockSize;
const int SCREEN_HEIGHT = gridHeight * blockSize;

static std::chrono::time_point <std::chrono::steady_clock, std::chrono::milliseconds> start;

double getTimeElapsed(){
    // https://stackoverflow.com/questions/31487876/getting-a-time-difference-in-milliseconds
    // https://stackoverflow.com/questions/728068/how-to-calculate-a-time-difference-in-c
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::milli>(now - start).count();
}

enum TetrominoType { STRAIGHT, SQUARE, SKEW, T, L };
enum Orientation { LEFT, UP, RIGHT, DOWN };
enum BlockColor { GREEN, BLUE, RED };

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
    
    // possibly bad for performance but just for now,
    // iterate through the grid to see if we have any full rows
    // if so, remove them and move previous rows
    void checkFullRows(){
        // step 1: find the full rows (store index of each full row)
        std::vector<int> fullRows;
        
        for(int i = 1; i < gridHeight; i++){
            bool isFullRow = true;
            for(int j = 0; j < gridWidth; j++){
                isFullRow = isFullRow && cells[i][j].hasBlock;
            }
            if(isFullRow) fullRows.push_back(i); // record the index of the full row
        }
        
        // step 2: remove and move prev rows up
        int numFullRows = (int)fullRows.size();
        //std::cout << "num full rows: " << numFullRows << '\n';
        if(numFullRows > 0){
            for(int i = fullRows[0]; i <= fullRows.back(); i++){
                for(int j = 0; j < gridWidth; j++){
                    cells[i][j].hasBlock = false;
                    cells[i][j].sprite = nullptr;
                }
            }
            
            // only need to move prev row up if curr row is > 0
            for(int i = fullRows[0]-1; i >= 0; i--){
                int newRowIndex = i + numFullRows;
                for(int j = 0; j < gridWidth; j++){
                    // move everything down
                    cells[newRowIndex][j].hasBlock = cells[i][j].hasBlock;
                    cells[newRowIndex][j].sprite = cells[i][j].sprite;
                    
                    // then reset
                    cells[i][j].hasBlock = false;
                    cells[i][j].sprite = nullptr;
                }
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

// TODO: use just one tetromino struct and come up with a way to
// keep track of the different block configurations
// straight -> 4 blocks in a row; pos.x, pos.x+blockSize, pos.x+blockSize*2, ..., same pos.y for all
// square -> (pos.x, pos.y), (pos.x+blockSize, pos.y), (pos.x, pos.y+blockSize), (pos.x+blockSize, pos.y+blockSize)
// T -> (pos.x, pos.y), (pos.x+blockSize, pos.y), (pos.x+blockSize*2, pos.y), (pos.x+blockSize, pos.y+blockSize)
// L -> (pos.x, pos.y), (pos.x, pos.y+blockSize), (pos.x, pos.y+blockSize*2), (pos.x+blockSize, pos.y+blockSize*2)
// skew -> (pos.x, pos.y), (pos.x, pos.y+blockSize), (pos.x+blockSize, pos.y+blockSize), (pos.x+blockSize, pos.y+blockSize*2)
// map of tetromino configurations to functions? :D function pointers
//std::unordered_map<TetrominoType, std::vector<std::vector<int>>> rotations;

struct Tetromino {
    Vec2 pos;
    int speed;
    TetrominoType configuration;
    Orientation orientation;
    SDL_Texture* blockSprite;
    
    bool gridCheck(Grid& grid){
        bool canPlace = true;
        int currPosX = pos.x;
        int currPosY = pos.y;
        
        if(configuration == TetrominoType::STRAIGHT && 
            orientation == Orientation::LEFT && 
            currPosY >= SCREEN_HEIGHT - blockSize){
                pos.y = SCREEN_HEIGHT - blockSize;
                //std::cout << "reached bottom\n";
                return false;
        }else if(
            configuration == TetrominoType::STRAIGHT &&
            orientation == Orientation::UP && 
            currPosY >= SCREEN_HEIGHT - blockSize*4){
                pos.y = SCREEN_HEIGHT - blockSize*4;
                return false;
        }else{
            // can move
            for(int i = 0; i < 4; i++){
                int gridX = currPosX / blockSize;
                int gridY = (currPosY + blockSize) / blockSize;
                canPlace = canPlace && !grid.cells[gridY][gridX].hasBlock;
                
                if(configuration == TetrominoType::STRAIGHT && orientation == Orientation::LEFT)
                    currPosX += blockSize;
                
                if(configuration == TetrominoType::STRAIGHT && orientation == Orientation::UP)
                    currPosY += blockSize;
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
            
            if(configuration == TetrominoType::STRAIGHT && orientation == Orientation::LEFT)
                currPosX += blockSize;
            
            if(configuration == TetrominoType::STRAIGHT && orientation == Orientation::UP)
                currPosY += blockSize;
        }
    }
    
    void reset(std::unordered_map<BlockColor, SDL_Texture*>& blockMap){
        // this block has reached an end so start over and change the tetromino type
        // TODO: assign random configuration, orientation
        pos.x = 20;
        pos.y = -blockSize * 2;
        blockSprite = blockMap[BlockColor(rand()%3)]; // only 3 colors right now
    }
    
    // maybe pass in the grid to check if this
    // tetromino should stop moving
    void render(SDL_Renderer *ren, Grid& grid, std::unordered_map<BlockColor, SDL_Texture*>& blockMap){
        // this should be if this tetromino can't move further downwards
        // TODO: should this really be here? kinda funny it belongs to Tetromino?
        if(gridCheck(grid) == false){
            //std::cout << "stop moving\n";
            markGrid(grid);
            grid.checkFullRows(); // remove any full rows and move prev rows down
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
            
            if(configuration == TetrominoType::STRAIGHT && orientation == Orientation::LEFT)
                currPosX += blockSize;
            
            if(configuration == TetrominoType::STRAIGHT && orientation == Orientation::UP)
                currPosY += blockSize;
        }
        
        pos.y += speed;
    }
};

void logSDLError(std::ostream &os, const std::string &msg){
    os << msg << " error: " << SDL_GetError() << std::endl;
}

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

void updateOrientation(Tetromino& currTetro){
    if(currTetro.configuration == TetrominoType::STRAIGHT){
        if(currTetro.orientation == Orientation::LEFT){
            currTetro.orientation = Orientation::UP;
        }else{
            currTetro.orientation = Orientation::LEFT;
        }
    }
}

void drawGridLines(SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, SDL_ALPHA_OPAQUE);
    
    for(int i = 0; i <= SCREEN_WIDTH - blockSize; i += blockSize){
        SDL_RenderDrawLine(renderer, i, 0, i, SCREEN_HEIGHT);
    }
    
    for(int j = 0; j <= SCREEN_HEIGHT - blockSize; j += blockSize){
        SDL_RenderDrawLine(renderer, 0, j, SCREEN_WIDTH, j);
    }
}

int main(int argc, char** argv){
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        logSDLError(std::cout,"SDL_Init Error: ");
        return 1;
    }
    
    std::cout << "press 'esc' to quit\n"; 
    //std::cout << "SCREEN_WIDTH: " << SCREEN_WIDTH << '\n';
    
    // seems to need to be borderless in order to actually get the screen width whatever I specified
    SDL_Window *window = SDL_CreateWindow("tetris", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_BORDERLESS);
    
    if(window == nullptr){
        logSDLError(std::cout, "SDL_CreateWindow Error: ");
        SDL_Quit();
        return 1;
    }
    
    int ww;
    int wh;
    SDL_GetWindowSize(window, &ww, &wh);
    //std::cout << "window width in pixels: " << ww << ", window height in pixels: " << wh << '\n'; 
    
    // create the renderer to render the window with 
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == nullptr){
        SDL_DestroyWindow(window);
        logSDLError(std::cout,"SDL_CreateRenderer Error: ");
        SDL_Quit();
        return 1;
    }
    
    //std::cout << "renderer width in pixels: " << ww << ", window height in pixels: " << wh << '\n'; 
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
    SDL_Texture* redBlockSprite = loadTexture("redBlock.bmp", renderer);
    
    if(blueBlockSprite == nullptr || greenBlockSprite == nullptr || redBlockSprite == nullptr){
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        logSDLError(std::cout,"block sprite creation failed: ");
        SDL_Quit();
        return 1;
    }
    
    blockMap[BlockColor::BLUE] = blueBlockSprite;
    blockMap[BlockColor::GREEN] = greenBlockSprite;
    blockMap[BlockColor::RED] = redBlockSprite;
    
    /***        
        BEGIN EVENT LOOP
    ***/
    bool quit = false; 
    SDL_Event event;
    
    Vec2 pos{10, -2};
    Tetromino currTetro{pos, 1, TetrominoType::STRAIGHT, Orientation::LEFT, blueBlockSprite};
    
    Grid grid;
    grid.initGrid();
    
    while(!quit){
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer); // clear the frame (make #000)
        drawGridLines(renderer);
        
        // check for actions that will quit the program 
        if(SDL_PollEvent(&event)){
            int screenWidth, screenHeight;
            SDL_GetRendererOutputSize(renderer, &screenWidth, &screenHeight);
            
            switch(event.type){
                // clicking on the X button on top right of screen
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
                        case SDLK_r:
                            // rotate the current tetromino
                            // if orientation is up and at the edge, rotation should not be possible
                            if(currTetro.orientation == Orientation::UP && 
                               currTetro.pos.x == screenWidth - blockSize){
                                break;
                            }
                            updateOrientation(currTetro);
                            break;
                        case SDLK_LEFT:
                            if(currTetro.pos.x - blockSize >= 0){
                                currTetro.pos.x -= blockSize;
                            }
                            break;
                        case SDLK_RIGHT:
                            int gridX = (currTetro.pos.x + blockSize) / blockSize;
                            int gridY = (currTetro.pos.y + blockSize) / blockSize;
                            
                            if(
                                currTetro.configuration == TetrominoType::STRAIGHT &&
                                currTetro.orientation == Orientation::LEFT &&
                                currTetro.pos.x + blockSize <= screenWidth - (blockSize*4) &&
                                !grid.cells[gridY][gridX].hasBlock){ // TODO: this hasBlock check doesn't seem to be working
                                    currTetro.pos.x += blockSize;
                            }else if(
                                currTetro.configuration == TetrominoType::STRAIGHT &&
                                currTetro.orientation == Orientation::UP &&
                                currTetro.pos.x + blockSize <= screenWidth - blockSize &&
                                !grid.cells[gridY][gridX].hasBlock){
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
    
    for(std::pair<BlockColor, SDL_Texture*> kv : blockMap){
        std::cout << "deleting block texture...\n";
        SDL_DestroyTexture(kv.second);
    }
    
    SDL_Quit();
    return 0;
}