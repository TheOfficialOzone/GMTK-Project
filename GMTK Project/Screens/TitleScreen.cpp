


#include "TitleScreen.h"


void startGame() {
	loadGameScreen();
}
void startGame2() {
	loadGameScreenRandom();
}
//Loads the Title Screen
void loadTitleScreen() {
	runGame = false;
	displayGame = false;
	levelLoaded = true;

	InteractManager::removeAllInteractables();

	SDL_FRect* background = new SDL_FRect();
	background->x = 0;
	background->y = 0;
	background->w = 1;
	background->h = 1;


	Interactable* bground = new Interactable(nullptr, background);
	bground->setColor(navy);
	InteractManager::addInteractable(bground);

	SDL_FRect* bGTexture = new SDL_FRect();
	bGTexture->x = -0.12;
	bGTexture->y = 0;
	bGTexture->w = 2.1;
	bGTexture->h = 1.23;


	TiltedInteractable* cool = new TiltedInteractable("Images/GMTK back.png", nullptr, bGTexture);
	InteractManager::addInteractable(cool);

	SDL_FRect* ButtonArea = new SDL_FRect();
	ButtonArea->x = 0.25;
	ButtonArea->y = 0.50;
	ButtonArea->w = 0.5;
	ButtonArea->h = 0.25;

	SDL_FRect* RandomButtonArea = new SDL_FRect();
	RandomButtonArea->x = 0.35;
	RandomButtonArea->y = 0.75;
	RandomButtonArea->w = 0.3;
	RandomButtonArea->h = 0.2;

	TextureInteractable* startButton = new TextureInteractable("Images/Start.png", startGame, ButtonArea);
	InteractManager::addInteractable(startButton);

	TextureInteractable* startButton2 = new TextureInteractable("Images/Random.png", startGame2, RandomButtonArea);
	InteractManager::addInteractable(startButton2);


	SDL_FRect* TextArea = new SDL_FRect();
	TextArea->x = 0.2;
	TextArea->y = 0.2;
	TextArea->w = 0.6;
	TextArea->h = 0.3;


	TextureInteractable* titleButton = new TextureInteractable("Images/Title.png", nullptr, TextArea);
	InteractManager::addInteractable(titleButton);

	SDL_FRect* audio = new SDL_FRect();
	audio->x = 0.35;
	audio->y = 0.95;
	audio->w = 0.3;
	audio->h = 0.05;

	AudioInteractable* audioButton = new AudioInteractable(0.3, nullptr, audio);
	InteractManager::addInteractable(audioButton);


	//InteractManager::addInteractable(fground);
}

