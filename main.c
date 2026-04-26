#include "chip8.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_stdinc.h>
#include <stdint.h>
#include <stdio.h>

#define SCALE 10

///////////////////////////////////////////////////////////////////////////
int lecture_ROM(FILE *fichier_jeu, chip8_t *console);
uint8_t mapping(SDL_Event ev);
void render_display(SDL_Renderer* renderer, uint8_t* display);

///////////////////////////////////////////////////////////////////////////
int lecture_ROM(FILE *fichier_jeu, chip8_t *console){
    if (!fichier_jeu){
        fprintf(stderr, "fichier_jeu n'existe pas dans lecture_ROM");
        return 0;
    }

    fread(&console->memory[0x200], sizeof(uint8_t), 3584, fichier_jeu);
    return 1;
}
uint8_t mapping(SDL_Event ev){
    switch (ev.key.keysym.sym) {
        case SDLK_1:
            return 0x1;
        case SDLK_2:
            return 0x2;
        case SDLK_3:
            return 0x3;
        case SDLK_4:
            return 0xC;
        ////////////
        case SDLK_a:
            return 0x4;
        case SDLK_z:
            return 0x5;
        case SDLK_e:
            return 0x6;
        case SDLK_r:
            return 0xD;
        ////////////
        case SDLK_q:
            return 0x7;
        case SDLK_s:
            return 0x8;
        case SDLK_d:
            return 0x9;
        case SDLK_f:
            return 0xE;
        ////////////
        case SDLK_w:
            return 0xA;
        case SDLK_x:
            return 0x0;
        case SDLK_c:
            return 0xB;
        case SDLK_v:
            return 0xF;
        default:
            return 0x20;
    }
}
void render_display(SDL_Renderer* renderer, uint8_t* display) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int y = 0; y < HAUTEUR; y++) {
        for (int x = 0; x < LARGEUR; x++) {
            if (display[y * LARGEUR + x]) {
                SDL_Rect pixel;
                pixel.x = x * SCALE;
                pixel.y = y * SCALE;
                pixel.w = SCALE;
                pixel.h = SCALE;

                SDL_RenderFillRect(renderer, &pixel);
            }
        }
    }
    SDL_RenderPresent(renderer);
}

///////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]){

    SDL_Window* window = NULL;
	SDL_Renderer* rendu = NULL;
	SDL_Event ev;

	chip8_t console;
	FILE *jeu_ROM = fopen(argv[1], "rb");

	int running = 1;

	// Initialisation
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		fprintf(stderr, "Echec Initilisation : %s", SDL_GetError());
		return 1;
	}

	window = SDL_CreateWindow("Simulateur", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, LARGEUR*SCALE, HAUTEUR*SCALE, SDL_WINDOW_SHOWN);
	if (!window) {
	    fprintf(stderr, "Pointeur windows NULL");
		return 1;
	}

	rendu = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!rendu) {
		fprintf(stderr, "Echec Render : %s", SDL_GetError());
		return 1;
	}

	init_emulator(&console);
	lecture_ROM(jeu_ROM, &console);

	// Boucle
	while (running) {
        while (SDL_PollEvent(&ev) != 0) {
            if (ev.type == SDL_QUIT)
                running = 0;
            else if (ev.type == SDL_KEYDOWN || ev.type == SDL_KEYUP){
                uint8_t presse = (ev.type == SDL_KEYDOWN) ? 1 : 0;
                uint8_t touche = mapping(ev);
                if (presse && (touche != 0x20))
                    console.keypad[touche] = 1;
                else if (!presse && (touche != 0x20))
                    console.keypad[touche] = 0;
                else
                    continue;
            }
        }

        uint16_t opcode = fetch(&console);
        instruction_t decomposition_opcode = decode(opcode);
        execute(&decomposition_opcode, &console);
        render_display(rendu, console.display);
        if (console.delay_timer > 0) console.delay_timer--;
        if (console.sound_timer > 0) console.sound_timer--;

        SDL_Delay(2);
    }

	SDL_DestroyRenderer(rendu);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
