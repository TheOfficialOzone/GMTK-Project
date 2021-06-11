

#include "Board.h"

int ChessBoard::sizeX = 0;
int ChessBoard::sizeY = 0;
Cell** ChessBoard::board = nullptr;

SDL_FRect* ChessBoard::percentArea = nullptr;	//Where it is rendered in Percentage
SDL_FRect* ChessBoard::renderArea = new SDL_FRect();	//Where it is rendered in Percentage