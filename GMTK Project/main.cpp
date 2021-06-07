
#include <iostream>

#include "SDL2/SDL.h"	//SDL
#undef main

#include "SDL_image.h"	//For loading images
#include "SDL_mixer.h"	//Loading Audio
#include "Music/MusicHelper/MusicHelper.h"

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


	ShapeRenderer::setRenderer(myRenderer);
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

	MusicHelper::loadAndPlayMusic("Music/Soda City Funk.mp3");
	MusicHelper::loadAndPlayMusic("Music/Soda City Funk.mp3");
	MusicHelper::loadAndPlayChunk("Music/Co-Champions (Official Lyric Video).mp3", 5);
	MusicHelper::loadAndPlayChunk("Music/dolphin.wav", -1);

	Mix_AllocateChannels(8);

	Mix_VolumeMusic(MIX_MAX_VOLUME / 10);
	Mix_Volume(-1, MIX_MAX_VOLUME / 10);

	Mix_ChannelFinished(channelDone);
	Mix_HookMusicFinished(musicDone);
	
	//End of audio Testing
	while (1) {
		SDL_PumpEvents();
		GraphicRenderer::renderTextureWithAngle(testTexture, testRect2, 180);
		GraphicRenderer::renderTextureWithFlip(testTexture, testRect, SDL_FLIP_NONE);
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