#include "chip8.h"
#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdio.h>

int main(void){

    SDL_Window* window = NULL;
	SDL_Renderer* rendu = NULL;
	SDL_Event ev;

	int running = 1;

	// Initialisation
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		fprintf(stderr, "Echec Initilisation : %s", SDL_GetError());
		return 1;
	}

	window = SDL_CreateWindow("Simulateur", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, LARGEUR, HAUTEUR, SDL_WINDOW_SHOWN);
	if (!window) {
	    fprintf(stderr, "Pointeur windows NULL");
		return 1;
	}

	rendu = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!rendu) {
		fprintf(stderr, "Echec Render : %s", SDL_GetError());
		return 1;
	}

	while (running) {
        while (SDL_PollEvent(&ev) != 0) {
            switch (ev.type) {
                case SDL_QUIT:
                running = 0;
                break;
            }
        }

        SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
        SDL_RenderClear(rendu);
        SDL_RenderPresent(rendu);
    }

	SDL_DestroyRenderer(rendu);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
