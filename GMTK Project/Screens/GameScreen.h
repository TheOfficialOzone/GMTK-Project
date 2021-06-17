#pragma once


#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include "MenuManager/MenuManager.h"
#include "Globals/Globals.h"
#include "TitleScreen.h"

void print();
//Loads the Title Screen
void loadGameScreen();
void loadGameScreenRandom();
//What happens when the game is lost
void lostGame();
//Resets the game
void resetGame();
//Pauses the game
void pauseGame();
void unPauseGame();

#endif