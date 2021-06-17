

#include "MouseController.h"

bool MouseController::mouseIsDown = false;
Piece* MouseController::heldPiece = nullptr;

std::vector<Piece*> MouseController::pieces;
SDL_Texture* MouseController::ropeTexture;
