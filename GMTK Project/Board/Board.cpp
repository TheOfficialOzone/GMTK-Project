

#include "Board.h"

bool Cell::flipColour = true;
SDL_Texture* Cell::lavaFull = nullptr;
std::vector<SDL_Texture*> Cell::lavaPieces;

int ChessBoard::sizeX = 0;
int ChessBoard::sizeY = 0;
int ChessBoard::loadX = 0;
int ChessBoard::lavaPosition = 0;
int ChessBoard::lavaWait = 1000;
int ChessBoard::lastLavaMove = 0;
float ChessBoard::cameraX = 0;


std::ifstream* ChessBoard::levelFile = loadFile("Level/level.txt");
Cell** ChessBoard::board = nullptr;

SDL_FRect* ChessBoard::percentArea = nullptr;	//Where it is rendered in Percentage
SDL_FRect* ChessBoard::renderArea = new SDL_FRect();	//Where it is rendered in Percentage

void Cell::render(SDL_FRect* renderArea) {


	if ((x + y + flipColour) % 2)
		ShapeRenderer::setColour(white);
	else
		ShapeRenderer::setColour(gray);

	if (ChessBoard::getLavaPos() > x) {
		ShapeRenderer::setColour(red);
		GraphicRenderer::renderTexture(lavaFull, renderArea);
		enableLava();
	}
	else if (ChessBoard::getLavaPos() == x) {
		ShapeRenderer::renderRect(renderArea);
		GraphicRenderer::renderTexture(lavaPieces[rand() % 6], renderArea);
		enableLava();
	} else {
		ShapeRenderer::renderRect(renderArea);
	}

	if (myPiece != nullptr) {
		if (myPiece->hasEnemy()) {
			myPiece->render(renderArea);
		}
	}
}