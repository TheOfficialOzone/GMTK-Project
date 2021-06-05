
#include <iostream>

#include "SDL2/SDL.h"	//SDL
#undef main

#include "SDL_image.h"	//For loading images


#include "Rendering/ShapeRenderer/ShapeRenderer.h"

int main() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0)	//Initialize SDL
		std::cout << "SDL2 Not Initialized";

	int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
	if (IMG_Init(imgFlags) != imgFlags)	//Initializing SDL_Image
		std::cout << "SDL_Image Not Initialized";
	
	SDL_Window* myWindow = SDL_CreateWindow("Cool title", 0, 100, 100, 100, SDL_WINDOW_RESIZABLE);
	SDL_Renderer* myRenderer = SDL_CreateRenderer(myWindow, -1, SDL_RENDERER_ACCELERATED);

	ShapeRenderer* shapeRender = new ShapeRenderer(myRenderer);
	shapeRender->fillEnable();
	SDL_Color testColour;

	testColour.r = 255;
	testColour.g = 100;
	testColour.b = 100;
	testColour.a = SDL_ALPHA_OPAQUE;
	shapeRender->setColour(testColour);

	SDL_FRect* testRect = new SDL_FRect();
	testRect->x = 50;
	testRect->y = 0;
	testRect->w = 100;
	testRect->h = 100;


	shapeRender->renderRect(testRect);

	while (1) {
		SDL_PumpEvents();
		shapeRender->renderRect(testRect);
		SDL_RenderPresent(myRenderer);
	}


	SDL_DestroyRenderer(myRenderer);
	SDL_DestroyWindow(myWindow);
	SDL_Quit();

	return 0;
}