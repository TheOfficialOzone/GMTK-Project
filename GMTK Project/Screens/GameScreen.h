#pragma once


#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include "MenuManager/MenuManager.h"

void print() {
	std::cout << "THE GAME SCREEN BUTTON";
}
//Loads the Title Screen
void loadGameScreen() {
	InteractManager::removeAllInteractables();
	
}



#endif