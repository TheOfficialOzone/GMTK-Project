



#include "Globals.h"

SDL_Renderer* globalRenderer = nullptr;
SDL_Window* globalWindow = nullptr;
SDL_Color white;
SDL_Color gray;
SDL_Color black;
SDL_Color red;
SDL_Color navy;
SDL_Color light_blue;
SDL_Color clear;

float distance = 4;
bool runGame = false;
bool displayGame = false;
bool randomGame = false;
bool levelLoaded = false;
int levelOn = 0;
SDL_Texture* backGround;