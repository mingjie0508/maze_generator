#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <SDL2/SDL.h>

class Game {
    public:
        Game(int maze_size);
        ~Game();
        void mainloop();
        void gameloop();
        void render();
        void loadRect();
        bool loadMaze();
        void update();
        void motionEvents();
        bool checkCollision(const SDL_Rect &r1, const SDL_Rect &r2);
    private:
        SDL_Window *win;
        SDL_Renderer *ren;
        bool quit;
        int SCREEN_WIDTH, SCREEN_HEIGHT;
        int dx, dy;
        int walls_size, m_size;
        SDL_Rect r, fin;
        SDL_Rect *walls;
};

#endif //GAME_H