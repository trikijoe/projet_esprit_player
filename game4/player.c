#include "functions.h"
#include <SDL/SDL_image.h>

void loadPlayerAnimations(SDL_Surface **forward, SDL_Surface **backward, SDL_Surface **jump) {
    char *pathsForward[3] = {"forward1.png", "forward2.png", "forward3.png"};
    char *pathsBackward[3] = {"backward1.png", "backward2.png", "backward3.png"};
    char *pathsJump[3] = {"jump1.png", "jump2.png", "jump3.png"};
    for (int i = 0; i < 3; i++) {
        forward[i] = IMG_Load(pathsForward[i]);
        backward[i] = IMG_Load(pathsBackward[i]);
        jump[i] = IMG_Load(pathsJump[i]);
    }
}

void freePlayerAnimations(SDL_Surface **forward, SDL_Surface **backward, SDL_Surface **jump) {
    for (int i = 0; i < 3; i++) {
        SDL_FreeSurface(forward[i]);
        SDL_FreeSurface(backward[i]);
        SDL_FreeSurface(jump[i]);
    }
}

