


//	Leave this Here
#include "GraphicRenderer/GraphicRenderer.h"
#include "ShapeRenderer/ShapeRenderer.h"
#include "MenuManager/MenuManager.h"

SDL_Renderer* GraphicRenderer::renderer = nullptr;	//Basically just defines the static values

SDL_Renderer* ShapeRenderer::renderer = nullptr;	//I hate how C++ does this...
bool ShapeRenderer::fill = false;
SDL_Color defaultColour;
SDL_Color ShapeRenderer::renderColour = defaultColour;
TTF_Font* ShapeRenderer::textFont = nullptr;


SDL_Window* InteractManager::myWindow = nullptr;
std::vector<Interactable*> InteractManager::interactables;

