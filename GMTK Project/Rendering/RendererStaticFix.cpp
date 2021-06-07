


#include "GraphicRenderer/GraphicRenderer.h"
#include "ShapeRenderer/ShapeRenderer.h"

SDL_Renderer* GraphicRenderer::renderer = nullptr;

SDL_Renderer* ShapeRenderer::renderer = nullptr;
bool ShapeRenderer::fill = false;
SDL_Color defaultColour;
SDL_Color ShapeRenderer::renderColour = defaultColour;

