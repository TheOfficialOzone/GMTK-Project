
#include <iostream>

#include "SDL2/SDL.h"	//SDL
#undef main

#include "SDL_image.h"	//For loading images

#include "Rendering/ShapeRenderer/ShapeRenderer.h"
#include "Rendering/GraphicRenderer/GraphicRenderer.h"

int main() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0)	//Initialize SDL
		std::cout << "SDL2 Not Initialized \n";

	int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
	if (IMG_Init(imgFlags) != imgFlags)	//Initializing SDL_Image	Loading Images and such
		std::cout << "SDL_Image Not Initialized \n";
	
	SDL_Window* myWindow = SDL_CreateWindow("GMTK Project 2021", 0, 100, 500, 500, SDL_WINDOW_RESIZABLE);
	SDL_Renderer* myRenderer = SDL_CreateRenderer(myWindow, -1, SDL_RENDERER_ACCELERATED);


	ShapeRenderer* shapeRender = new ShapeRenderer(myRenderer);
	GraphicRenderer* graphicRenderer = new GraphicRenderer(myRenderer);

	SDL_Texture* testTexture = nullptr;	//C++ gets mad if you don't set it to something
	SDL_Surface* testSurface = nullptr;
	std::string testString = "Images/test.png";

	graphicRenderer->loadImageToTexture(testTexture, testString);

	SDL_FRect* testRect = new SDL_FRect();
	testRect->x = 10;
	testRect->y = 10;
	testRect->w = 400;
	testRect->h = 400;

	while (1) {
		SDL_PumpEvents();
		graphicRenderer->renderTexture(testTexture, testRect);
		SDL_RenderPresent(myRenderer);
	}
	
	
	SDL_DestroyRenderer(myRenderer);
	SDL_DestroyWindow(myWindow);
	SDL_Quit();

	return 0;
}