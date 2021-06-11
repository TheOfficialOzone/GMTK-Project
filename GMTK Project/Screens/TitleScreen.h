#pragma once

#ifndef TITLE_SCREEN_H
#define TITLE_SCREEN_H

#include "MenuManager/MenuManager.h"
#include "GameScreen.h"
void startGame() {
	loadGameScreen();
}
//Loads the Title Screen
void loadTitleScreen() {
	InteractManager::removeAllInteractables();

	SDL_FRect* ButtonArea = new SDL_FRect();
	ButtonArea->x = 0.3;
	ButtonArea->y = 0.55;
	ButtonArea->w = 0.4;
	ButtonArea->h = 0.2;

	TextInteractable* startButton = new TextInteractable("START GAME", startGame, ButtonArea);
	InteractManager::addInteractable(startButton);

	SDL_FRect* TextArea = new SDL_FRect();
	TextArea->x = 0.25;
	TextArea->y = 0.3;
	TextArea->w = 0.5;
	TextArea->h = 0.2;

	TextInteractable* textButton = new TextInteractable("WELCOME TO GAMER GAME", nullptr, TextArea);
	InteractManager::addInteractable(textButton);

	TextureInteractable* textureButton = new TextureInteractable("Images/test.png", nullptr, TextArea);
	InteractManager::addInteractable(textureButton);

}



#endif