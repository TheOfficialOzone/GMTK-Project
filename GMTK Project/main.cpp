
#include <iostream>
#include <filesystem>

#include "SDL2/SDL.h"	//SDL
#undef main

#include "SDL_image.h"	//For loading images
#include "SDL_mixer.h"	//Loading Audio
#include "SDL_ttf.h"


#include "Music/MusicHelper/MusicHelper.h"

#include "MenuManager/MenuManager.h"

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

	if (TTF_Init() == -1)
		std::cout << "SDL_TTF Not Initialized \n";
	SDL_Window* myWindow = SDL_CreateWindow("GMTK Project 2021", 0, 100, 500, 500, SDL_WINDOW_RESIZABLE);
	SDL_Renderer* myRenderer = SDL_CreateRenderer(myWindow, -1, SDL_RENDERER_ACCELERATED);


	ShapeRenderer::setRenderer(myRenderer);
	SDL_Color white;
	white.r = 255;
	white.g = 255;
	white.b = 255;
	white.a = 255;
	ShapeRenderer::setColour(white);


	GraphicRenderer::setRenderer(myRenderer);

	SDL_Texture* testTexture = nullptr;	//C++ gets mad if you don't set it to something
	SDL_Surface* testSurface = nullptr;
	std::string testString = "Images/test.png";

	GraphicRenderer::loadImageToTexture(testTexture, testString);

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
	

	Mix_VolumeMusic(MIX_MAX_VOLUME / 10);
	Mix_Volume(-1, MIX_MAX_VOLUME / 10);

	MusicHelper::loadAndPlayChunk("Music/dog_growl3.wav", -1);
	MusicHelper::loadAndPlayChunk("Music/dolphin.wav", -1);

	Mix_ChannelFinished(channelDone);
	Mix_HookMusicFinished(musicDone);
	
	//End of audio Testing

	//Interactable Testing
	InteractManager* manager = new InteractManager();
	Interactable* testInteract = new Interactable(print, testRect2);
	manager->addInteractable(testInteract);

	//End of Interactable Testing

	//Text Testing
	TTF_Font* Sans = TTF_OpenFont("Fonts/OpenSans-Regular.ttf", 15);

	SDL_Surface* surf = TTF_RenderText_Solid(Sans, "GAmers", white);
	SDL_Texture* myTexture;
	GraphicRenderer::surfaceToTexture(surf, myTexture);
	//End of Text Testing

	bool hasRun = false;
	while (1) {
		SDL_PumpEvents();
		//GraphicRenderer::renderTextureWithAngle(testTexture, testRect2, 180);
		//GraphicRenderer::renderTextureWithFlip(testTexture, testRect, SDL_FLIP_NONE);
		
		GraphicRenderer::renderTexture(myTexture, testRect);
		manager->renderInteractables();
		SDL_RenderPresent(myRenderer);

		SDL_Event event;	//Exits the program
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT) 
			break;
	}
	
	Mix_CloseAudio();	//Closes the Audio
	Mix_Quit();
	
	SDL_DestroyRenderer(myRenderer);
	SDL_DestroyWindow(myWindow);
	SDL_Quit();

	return 0;
}