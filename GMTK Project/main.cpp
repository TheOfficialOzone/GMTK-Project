
#include <iostream>

#include "SDL2/SDL.h"	//SDL
#undef main
#include "Globals.h"

#include "SDL_image.h"	//For loading images
#include "SDL_mixer.h"	//Loading Audio
#include "SDL_ttf.h"


#include "Music/MusicHelper/MusicHelper.h"
#include "MenuManager/MenuManager.h"

#include "Rendering/ShapeRenderer/ShapeRenderer.h"
#include "Rendering/GraphicRenderer/GraphicRenderer.h"

#include "Screens/TitleScreen.h"	//For loading the title screen

#include "Board/Board.h"
#include "MouseController/MouseController.h"


static int resizingEventWatcher(void* data, SDL_Event* event) {
	if (event->type == SDL_WINDOWEVENT &&
		event->window.event == SDL_WINDOWEVENT_RESIZED) {
		InteractManager::updateInteractables();
		InteractManager::renderInteractables();
		SDL_RenderPresent(globalRenderer);
	}
	return 0;
}

/*
	Chess with 2 connected pieces
		- 

*/

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

	globalWindow = myWindow;
	globalRenderer = myRenderer;

	white.r = 255;
	white.g = 255;
	white.b = 255;
	white.a = 255;

	black.r = 0;
	black.g = 0;
	black.b = 0;
	black.a = 255;

	red.r = 255;
	red.g = 100;
	red.b = 100;
	red.a = 255;

	ShapeRenderer::setRenderer(myRenderer);
	ShapeRenderer::setColour(white);

	//Watches for window size changes
	SDL_AddEventWatch(resizingEventWatcher, nullptr);

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

	SDL_FRect* percentRect = new SDL_FRect();
	percentRect->x = 0.5;
	percentRect->y = 0.5;
	percentRect->w = 0.5;
	percentRect->h = 0.5;


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
	//End of Audio Testing

	//Interactable Testing
	Interactable* testInteract = new Interactable(nullptr, percentRect);

	InteractManager::setWindow(myWindow);
	InteractManager::addInteractable(testInteract);
	//End of Interactable Testing

	//Text Testing
	TTF_Font* Sans = TTF_OpenFont("Fonts/FFFFORWA.ttf", 150);
	ShapeRenderer::setFont(Sans);
	//End of Text Testing
	loadTitleScreen();
	//Chess Board 
	SDL_FRect* chessBoardArea = new SDL_FRect();
	chessBoardArea->x = 0;
	chessBoardArea->y = 0;
	chessBoardArea->w = 1;
	chessBoardArea->h = 0.5;
	ChessBoard* myBoard = new ChessBoard(10, 5, chessBoardArea);

	//Piece Testing
	Piece* myPiece = new Piece(Knight, 2, 2, "Images/chess-Knight.png", nullptr);
	MouseController::addPiece(myPiece);

	while (1) {
		SDL_PumpEvents();
		ShapeRenderer::setColour(black);
		SDL_RenderClear(myRenderer);
		InteractManager::updateInteractables();
		InteractManager::renderInteractables();
		

		ChessBoard::updateRenderArea();
		ChessBoard::drawBoard();
		renderValidMoves(myPiece);
		MouseController::renderPieces();
		SDL_RenderPresent(myRenderer);

		MouseController::updateInfo();

		SDL_Event event;	//Exits the program
		SDL_PollEvent(&event);
		int clickX, clickY;
		if (SDL_GetMouseState(&clickX, &clickY)) {
			InteractManager::clickInBounds(clickX, clickY);
		}

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