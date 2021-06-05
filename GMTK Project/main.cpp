
#include <iostream>

#include "SDL2/SDL.h"	//SDL
#undef main

#include "SDL_image.h"	//For loading images


#include "Rendering/ShapeRenderer/ShapeRenderer.h"

int main() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0)	//Initialize SDL
		std::cout << "SDL2 Not Initialized";

	int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
	if (IMG_Init(imgFlags) != imgFlags)	//Initializing SDL_Image	Loading Images and such
		std::cout << "SDL_Image Not Initialized";
	
	SDL_Window* myWindow = SDL_CreateWindow("GMTK Project 2021", 0, 100, 100, 100, SDL_WINDOW_RESIZABLE);
	SDL_Renderer* myRenderer = SDL_CreateRenderer(myWindow, -1, SDL_RENDERER_ACCELERATED);


	ShapeRenderer* shapeRender = new ShapeRenderer(myRenderer);
	shapeRender->fillEnable();

	SDL_FRect* testRect = new SDL_FRect();
	testRect->x = 50;
	testRect->y = 0;
	testRect->w = 100;
	testRect->h = 100;


	//shapeRender->renderRect(testRect);

	Line* testLine = new Line(5, 5, 100, 300);
	//testLine->point1.x = 5;
	//testLine->point1.y = 5;
	//testLine->point2.x = 100;
	//testLine->point2.y = 500;

	shapeRender->renderLine(testLine);

	while (1) {
		SDL_PumpEvents();
		shapeRender->renderRect(testRect);
		shapeRender->renderLine(testLine);
		SDL_RenderPresent(myRenderer);
	}


	SDL_DestroyRenderer(myRenderer);
	SDL_DestroyWindow(myWindow);
	SDL_Quit();

	return 0;
}