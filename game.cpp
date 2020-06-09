#include "game.h"
#include "maze.h"
#include <fstream>
#include <string>

static const int TILESIZE = 16;
static const int SPEED = 6;

Game::Game(int maze_size) {
    //Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " <<
        SDL_GetError() << std::endl;
        return;
    }
    //Initialize window size
    SCREEN_WIDTH = (2*maze_size+1)*TILESIZE;
    SCREEN_HEIGHT = (2*maze_size+1)*TILESIZE;
    //Create window
    win = NULL;
    win = SDL_CreateWindow("Get_to_the_Pink_Square", SDL_WINDOWPOS_UNDEFINED, 
    SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        std::cout << "Could not create window! SDL_Error: " <<
        SDL_GetError() << std::endl;
        return;
    }
    //Create renderer
    ren = NULL;
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (ren == NULL) {
        std::cout << "Could not create renderer! SDL_ERROR: " <<
        SDL_GetError() << std::endl;
        return;
    }
    //Initialize maze map size
    m_size = maze_size;
    //Initialize rects
    walls = new SDL_Rect[3*m_size*m_size + 4*m_size];
    //Clear the screen to white
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    SDL_RenderClear(ren);
    //Call main loop
    mainloop();
}

Game::~Game() {
    //Destroy renderer
    SDL_DestroyRenderer(ren);
    ren = NULL;

    //Destroy window
    SDL_DestroyWindow(win);
    win = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

void Game::mainloop() {
    quit = false;
    while (!quit) {
        //Load player and finish point
        loadRect();

        //Generate maze in maze.txt
        make_maze();

        //Load maze map
        if (!loadMaze()) {
            std::cout << "Could not load maze map!" << std::endl;
            return;
        }
        //Single level loop
        gameloop();
    }
}

void Game::gameloop() {
    while (!quit) {
        //Initialize change in player coordinates
        dx = dy = 0;
        motionEvents();
        update();
        render();
        //If player gets to finish point, break loop
        if ((checkCollision(r, fin))) {
            return;
        }
    }
}

void Game::motionEvents() {
    //Event handler
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            quit = true;
        } else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_LEFT:
                    dx = -SPEED;
                    break;
                case SDLK_RIGHT:
                    dx = SPEED;
                    break;
                case SDLK_UP:
                    dy = -SPEED;
                    break;
                case SDLK_DOWN:
                    dy = SPEED;
                    break;
            }
        }
    }
}

void Game::loadRect() {
    //Player start point at top left corner
    r.x = TILESIZE;
    r.y = TILESIZE;
    r.w = TILESIZE;
    r.h = TILESIZE;
    //Finish point at bottom right corner
    fin.x = (2*m_size-1)*TILESIZE;
    fin.y = (2*m_size-1)*TILESIZE;
    fin.w = TILESIZE;
    fin.h = TILESIZE;
}

bool Game::loadMaze() {
    bool success = true; //Set success flag
    int line_num = 0, len = 2*m_size+1; //Initialize file iterator
    walls_size = 0; //Counter of wall rectangles in walls
    std::string line;
    std::ifstream file("maze.txt");
    if (file.is_open()) {
        while (getline(file, line)) {
            for (int i = 0; i < len; ++i) {
                if (line[i] == 'W') {
                    //Single wall rectangle
                    walls[walls_size].x = i*TILESIZE;
                    walls[walls_size].y = line_num*TILESIZE;
                    walls[walls_size].w = TILESIZE;
                    walls[walls_size].h = TILESIZE;
                    ++walls_size;
                }
            }
            ++line_num;
        }
        return success;
    }
    return !success;
}

void Game::update() {
    //Apply the changes to player coordinates
    r.x += dx;
    r.y += dy;
    for (int i = 0; i < walls_size; ++i) {
        //If player collides with wall, modify coordinates
        if ((checkCollision(r, walls[i]))) {
            if (dx > 0) {
                r.x = walls[i].x - TILESIZE;
            } else if (dx < 0) {
                r.x = walls[i].x + TILESIZE;
            } else if (dy > 0) {
                r.y = walls[i].y - TILESIZE;
            } else if (dy < 0) {
                r.y = walls[i].y + TILESIZE;
            }
        }
    }
}

void Game::render() {
    //Clear the screen to white
    SDL_SetRenderDrawColor(ren, 240, 240, 240, 255);
    SDL_RenderClear(ren);
    //Render player
    SDL_SetRenderDrawColor(ren, 180, 180, 180, 255); // grey
    SDL_RenderFillRect(ren, &r);
    //Render finish point
    SDL_SetRenderDrawColor(ren, 230, 50, 120, 255); // pink
    SDL_RenderFillRect(ren, &fin);
    //Render maze map
    SDL_SetRenderDrawColor(ren, 20, 20, 20, 255); //black
    for (int i = 0; i < walls_size; ++i) {
        SDL_RenderFillRect(ren, walls+i);
    }
    //Update screen
    SDL_RenderPresent(ren);
}

bool Game::checkCollision(const SDL_Rect &r1, const SDL_Rect &r2) {
    int left1, left2, right1, right2, top1, top2, bottom1, bottom2;
    //Calculate the sides of r1
    left1 = r1.x;
    right1 = r1.x + r1.w;
    top1 = r1.y;
    bottom1 = r1.y + r1.h;

    //Calculate the sides of r2
    left2 = r2.x;
    right2 = r2.x + r2.w;
    top2 = r2.y;
    bottom2 = r2.y + r2.h;

    if (bottom1 <= top2) {
        return false;
    }
    if (bottom2 <= top1) {
        return false;
    }
    if (right1 <= left2) {
        return false;
    }
    if (right2 <= left1) {
        return false;
    }
    return true;
}
