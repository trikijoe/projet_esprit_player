#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <SDL/SDL.h>

// Constant definitions
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define ANIMATION_SPEED 1  // Adjust as needed for your animation speed
#define BUTTON_WIDTH 100
#define BUTTON_HEIGHT 5  // Standard button height across your application

void loadPlayerAnimations(SDL_Surface **forward, SDL_Surface **backward, SDL_Surface **jump);
void freePlayerAnimations(SDL_Surface **forward, SDL_Surface **backward, SDL_Surface **jump);

#endif

