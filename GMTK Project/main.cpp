
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include <iostream>

#include "SDL2/SDL.h"	//SDL
#undef main
#include "Globals/Globals.h"

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
		int width, height;
		SDL_GetWindowSize(globalWindow, &width, &height);
		if (width > height) {
			SDL_SetWindowSize(globalWindow, height, height);
			return 0;
		}
		if (height > width) {
			SDL_SetWindowSize(globalWindow, width, width);
			return 0;
		}
	}
	return 0;
}

/*
	Chess with 2 connected pieces
*/

int main() {
	srand(time(NULL));

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

	SDL_Window* myWindow = SDL_CreateWindow("Reventador", 0, 100, 500, 500, SDL_WINDOW_RESIZABLE);
	SDL_Renderer* myRenderer = SDL_CreateRenderer(myWindow, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawBlendMode(myRenderer, SDL_BLENDMODE_BLEND);
	globalWindow = myWindow;
	globalRenderer = myRenderer;

	white.r = 255;
	white.g = 255;
	white.b = 255;
	white.a = 255;

	clear.r = 255;
	clear.g = 255;
	clear.b = 255;
	clear.a = 0;

	gray.r = 150;
	gray.g = 150;
	gray.b = 150;
	gray.a = 255;

	black.r = 0;
	black.g = 0;
	black.b = 0;
	black.a = 255;

	navy.r = 0;
	navy.g = 0;
	navy.b = 128;
	navy.a = 255;

	light_blue.r = 128;
	light_blue.g = 128;
	light_blue.b = 255;
	light_blue.a = 255;

	red.r = 255;
	red.g = 100;
	red.b = 100;
	red.a = 255;

	ShapeRenderer::setRenderer(myRenderer);
	ShapeRenderer::setColour(white);

	//Watches for window size changes
	SDL_AddEventWatch(resizingEventWatcher, nullptr);



	GraphicRenderer::setRenderer(myRenderer);


	//Loading the lava Pieces
	GraphicRenderer::loadImageToTexture(Cell::lavaFull, "Images/Lava/lava_full.png");
	GraphicRenderer::loadImageToTexture(MouseController::ropeTexture, "Images/rope.png");
	GraphicRenderer::loadImageToTexture(backGround, "Images/GMTK back.png");
	for (int i = 0; i < 6; i++) {
		SDL_Texture* tempTexture;
		std::string mySTR = "Images/Lava/LavaEdge_";
		mySTR += std::to_string(rand() % 5);
		mySTR += ".png";

		GraphicRenderer::loadImageToTexture(tempTexture, mySTR);
		Cell::lavaPieces.push_back(tempTexture);
	}

	//Audio Setup
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		printf("Mix_OpenAudio: %s\n", Mix_GetError());
		exit(2);
	}
	Mix_VolumeMusic(MIX_MAX_VOLUME / 7);
	Mix_Volume(-1, MIX_MAX_VOLUME / 3);

	Mix_ChannelFinished(channelDone);
	Mix_HookMusicFinished(musicDone);

	MusicHelper::loadAndPlayMusic("Music/GMTK_Jam.mp3");
	//End of Audio Setup

	//Interactable Setup
	InteractManager::setWindow(myWindow);

	//Text Setup
	TTF_Font* Sans = TTF_OpenFont("Fonts/OpenSans-Bold.ttf", 150);
	ShapeRenderer::setFont(Sans);
	//End of Text Setup
	loadTitleScreen();

	ChessBoard::loadNextChunk();




	int startTick, endTick;
	startTick = SDL_GetTicks();
	endTick = SDL_GetTicks();


	while (1) {
		startTick = SDL_GetTicks();	//For timing

		SDL_PumpEvents();
		ShapeRenderer::setColour(black);
		SDL_RenderClear(myRenderer);
		InteractManager::updateInteractables();
		InteractManager::renderInteractables();
		
		if (runGame) {
			MouseController::updateInfo();
			ChessBoard::calculateCameraX(MouseController::getPieces());
			ChessBoard::updateChessBoard();
		}
		if (displayGame) {
			ChessBoard::updateRenderArea();
			ChessBoard::drawBoard();
			renderValidMoves(MouseController::getHeldPiece());
			MouseController::renderPieces();
		}
		

		SDL_RenderPresent(myRenderer);
		SDL_Event event;	//Exits the program
		SDL_PollEvent(&event);
		int clickX, clickY;
		if (SDL_GetMouseState(&clickX, &clickY) && MouseController::checkMouseDown()) {
			if (!levelLoaded)
				if (MouseController::getHeldPiece() == nullptr)
					InteractManager::clickInBounds(clickX, clickY);
		}
		else {
			levelLoaded = false;
		}


		endTick = SDL_GetTicks();	//For timing
		int dif = endTick - startTick;
		if (dif > 10)
			dif = 10;
		SDL_Delay(10 - dif);
		

		
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