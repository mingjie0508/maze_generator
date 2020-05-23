#include <iostream>
#include <fstream>
#include <algorithm>    // std::random_shuffle
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include "maze.h"
using namespace std;

int NORTH = 0;
int EAST = 1;
int SOUTH = 2;
int WEST = 3;

void shuffle_array(int arr[], int n) {
    random_shuffle(arr, arr + n);
}

int in_bound(int x, int y, int w, int h) {
    if (x < 0 || x >= w) return false;
    if (y < 0 || y >= h) return false;
    return true;
}

int xy_to_index (int x, int y, int w) {
    return y * w + x;
}

char *create_grid (int w, int h) {
    int size = w * h;
    char *grid = new char[size];
    for (int i = 0; i < size; ++i) {
        grid[i] = 'W';
    }
    return grid;
}

void destroy_grid (char *maze) {
    delete[] maze;
}

void visit (char *maze, int x, int y, int w, int h) {
    maze[xy_to_index(x, y, w)] = ' ';
    int dirs[4] = {NORTH, EAST, SOUTH, WEST};
    int dirs_size = 4;
    shuffle_array(dirs, dirs_size);
    for (int i = 0; i < dirs_size; ++i) {
        int dx = 0;
        int dy = 0;
        if (dirs[i] == NORTH) {
            dy = -1;
        } else if (dirs[i] == EAST) {
            dx = 1;
        } else if (dirs[i] == SOUTH) {
            dy = 1;
        } else if (dirs[i] == WEST) {
            dx = -1;
        }
        int new_x = x + 2 * dx;
        int new_y = y + 2 * dy;
        if (in_bound(new_x, new_y, w, h) &&
        maze[xy_to_index(new_x, new_y, w)] == 'W') {
            maze[xy_to_index(x+dx, y+dy, w)] = ' ';
            visit(maze, new_x, new_y, w, h);
        }
    }
}

void print_maze (char *maze, int w, int h) {
    maze[xy_to_index(w-2, h-2, w)] = 'E';

    ofstream file("maze.txt");
    if (file.is_open()) {
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                file << maze[xy_to_index(j, i, w)];
            }
            file << endl;
        }
    } else {
        cout << "cannot overwrite the file, something went wrong"
        << endl;
    }
    file.close();
}

void make_maze(int width, int height) {
    srand(time(0));
    int w = 2 * width + 1;
    int h = 2 * height + 1;
    char *maze = create_grid(w, h);
    visit(maze, 1, 1, w, h);
    print_maze(maze, w, h);
    destroy_grid(maze);
}
