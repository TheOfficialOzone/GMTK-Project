
#include <iostream>

#include "SDL2/SDL.h"	//SDL
#undef main

#include "SDL_image.h"	//For loading images
#include "SDL_mixer.h"	//Load*ing Audio

#include "Rendering/ShapeRenderer/ShapeRenderer.h"
#include "Rendering/GraphicRenderer/GraphicRenderer.h"

int main() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0)	//Initialize SDL
		std::cout << "SDL2 Not Initialized \n";

	int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
	if (IMG_Init(imgFlags) != imgFlags)	//Initializing SDL_Image	Loading Images and such
		std::cout << "SDL_Image Not Initialized \n";

	int musicFlags = MIX_INIT_MP3;
	if (Mix_Init(musicFlags)!= musicFlags)
		std::cout << "SDL_Mixer Not Initialized \n";
	
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
	testRect->w = 200;
	testRect->h = 200;

	SDL_FRect* testRect2 = new SDL_FRect();
	testRect2->x = 300;
	testRect2->y = 10;
	testRect2->w = 200;
	testRect2->h = 200;


	//Testing Audio Garbage
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		printf("Mix_OpenAudio: %s\n", Mix_GetError());
		exit(2);
	}

	Mix_Music* music = Mix_LoadMUS("Music/Co-Champions (Official Lyric Video).mp3");
	if (!music)
		std::cout << printf("Mix_LoadMUS(\"Music/Soda City Funk.mp3\"): %s\n", Mix_GetError());

	Mix_PlayMusic(music, 1);
	//End of audio Testing
	while (1) {
		SDL_PumpEvents();
		graphicRenderer->renderTextureWithAngle(testTexture, testRect2, 180);
		graphicRenderer->renderTextureWithFlip(testTexture, testRect, SDL_FLIP_NONE);
		SDL_RenderPresent(myRenderer);
	}
	
	Mix_CloseAudio();	//Closes the Audio
	
	SDL_DestroyRenderer(myRenderer);
	SDL_DestroyWindow(myWindow);
	SDL_Quit();

	return 0;
}