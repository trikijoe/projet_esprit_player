#include "functions.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        fprintf(stderr, "SDL_image initialization failed: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "SDL_mixer initialization failed: %s\n", Mix_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    if (TTF_Init() == -1) {
        fprintf(stderr, "SDL_ttf initialization failed: %s\n", TTF_GetError());
        Mix_CloseAudio();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Surface *screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (!screen) {
        fprintf(stderr, "Unable to set %dx%d video: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
        Mix_CloseAudio();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Surface *background = IMG_Load("background.png");
    if (!background) {
        fprintf(stderr, "Unable to load background image: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Surface *playerForward[3], *playerBackward[3], *playerJump[3];
    loadPlayerAnimations(playerForward, playerBackward, playerJump);

    int playerX = SCREEN_WIDTH / 2;
    int playerY = SCREEN_HEIGHT - 200; // Player's initial position on the ground
    bool movingRight = false, movingLeft = false, isJumping = false;
    float verticalVelocity = 1, gravity = 0.4;
    int playerSpeed = 1;
    bool quit = false;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_d: movingRight = true; break;
                    case SDLK_q: movingLeft = true; break;
                    case SDLK_z: 
                        if (!isJumping) {
                            verticalVelocity = -10;
                            isJumping = true;
                        }
                        break;
                }
            } else if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                    case SDLK_d: movingRight = false; break;
                    case SDLK_q: movingLeft = false; break;
                }
            }
        }

        SDL_BlitSurface(background, NULL, screen, NULL); // Redraw background to clear old frames

        // Jumping physics
        if (isJumping) {
            playerY += (int) verticalVelocity;
            verticalVelocity += gravity;
            if (playerY >= SCREEN_HEIGHT - 200) { 
                playerY = SCREEN_HEIGHT - 200; 
                verticalVelocity = 0; 
                isJumping = false; 
            }
        }

        // Horizontal movement and boundary checks
        if (movingRight) {
            playerX += playerSpeed;
            if (playerX > SCREEN_WIDTH - playerForward[0]->w) {
                playerX = SCREEN_WIDTH - playerForward[0]->w;
            }
        }
        if (movingLeft) {
            playerX -= playerSpeed;
            if (playerX < 0) {
                playerX = 0;
            }
        }

        int currentPlayerFrame = (SDL_GetTicks() / 100) % 3; // Cycle through the 3 images based on time
        SDL_Surface *currentImage; // Determine the current image based on the state
        if (isJumping) {
            currentImage = playerJump[currentPlayerFrame];
        } else if (movingRight) {
            currentImage = playerForward[currentPlayerFrame];
        } else if (movingLeft) {
            currentImage = playerBackward[currentPlayerFrame];
        } else {
            currentImage = playerForward[0];
        }

        SDL_Rect playerRect = {playerX, playerY, currentImage->w, currentImage->h};
        SDL_BlitSurface(currentImage, NULL, screen, &playerRect);

        SDL_Flip(screen);
    }

    // Cleanup and exit
    freePlayerAnimations(playerForward, playerBackward, playerJump);
    SDL_FreeSurface(background);
    TTF_Quit();
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();

    return 0;
}

