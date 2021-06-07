


//	Leave this Here
#include "GraphicRenderer/GraphicRenderer.h"
#include "ShapeRenderer/ShapeRenderer.h"

SDL_Renderer* GraphicRenderer::renderer = nullptr;	//Basically just defines the static values

SDL_Renderer* ShapeRenderer::renderer = nullptr;	//I hate how C++ does this...
bool ShapeRenderer::fill = false;
SDL_Color defaultColour;
SDL_Color ShapeRenderer::renderColour = defaultColour;

