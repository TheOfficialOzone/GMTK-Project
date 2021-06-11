

#ifndef PIECE_H
#define PIECE_H

#include "MenuManager/MenuManager.h"

enum ChessPieces{
	Pawn = 0,
	Knight = 1,
	Bishop = 2,
	Rook = 3,
	Queen = 4,
	King = 5
};


class Piece : public TextureInteractable {
private:
	// 1: KNIGHT
	// 2: BISHOP
	int pieceType;

	// These are equal to the location ON THE GRID
	int posX;
	int posY;

	void (*pieceFunc)(Piece*);
public:
	Piece(int type, int x, int y, std::string textureString, SDL_FRect* newRenderArea) : TextureInteractable(textureString, nullptr, newRenderArea){
		this->pieceType = type;
		this->posX = x;
		this->posY = y;
	}

	void setFunction(void (*functionToCall)(Piece*)) { pieceFunc = functionToCall; }
	
	int getType() { return this->pieceType; }
	int getX() { return this->posX; }
	int getY() { return this->posY; }
	void setX(int newX) { this->posX = newX; }
	void setY(int newY) { this->posY = newY; }

	//Renders the Piece
	void render(SDL_FRect* renderArea) {
		GraphicRenderer::renderTexture(myTexture, renderArea);
	}
};

void dropPiece();
std::vector<int>* getValidMoves(Piece* p);
void pickupPiece();
void renderValidMoves(Piece* p);

#endif