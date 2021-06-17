

#include <vector>
#include <algorithm>

#include "Globals/Globals.h"
#include "Piece.h"
#include "Board/Board.h"
#include "Rendering/ShapeRenderer/ShapeRenderer.h"
#include "MouseController/MouseController.h"
#include "Music/MusicHelper/MusicHelper.h"

/*
	This is the flow for how the piece picking up works:
	when you click on a piece, a phantom version of the piece gets bound to the mouse
	and for whichever piece is bound to the mouse, it shows valid cells to move to
	when the phantom piece is clicked on a valid cell, 
	the phantom piece dissapears and the real piece is moved to that valid cell
*/

//Draws the valid Moves of a Piece
void renderValidMoves(Piece* p) {
	std::vector<int>* possibleMoves = getValidMoves(p);

	//Goes through the possible moves
	for (int i = 0; i < possibleMoves->size(); i++) {
		int x, y;
		ChessBoard::getCoords(x, y, possibleMoves->at(i));
		SDL_FRect* renderRect = ChessBoard::getCellRenderArea(x, y);

		ShapeRenderer::setColour(light_blue);
		ShapeRenderer::renderRect(renderRect);
		delete renderRect;
	}
}



// STEP 3: DROP THE PIECE DOWN
// CALL THIS ON EVERY MOUSE CLICK - IT WILL CHECK IF A PIECE NEEDS TO BE DROPPED
// AND WILL MAKE ALL RELEVANT CHANGES
// INPUTS -> X, Y COORDINATES OF THE MOUSE
void dropPiece() {
	int x;
	int y;
	MouseController::getMousePosition(x, y);
	if (MouseController::getHeldPiece() != nullptr) {
		std::vector<int>* moves = getValidMoves(MouseController::getHeldPiece());
		if (ChessBoard::mouseToBoard(x, y)) { // converts x and y to board indeces as well as does the check
			int index = ChessBoard::getIndex(x, y);
			if (std::find(moves->begin(), moves->end(), index) != moves->end()) {	//IF it's in the vector
				Cell* origin = ChessBoard::getCell(MouseController::getHeldPiece()->getX(), MouseController::getHeldPiece()->getY());
				Cell* destination = ChessBoard::getCell(x, y);
				origin->removePiece();
				destination->setPiece(MouseController::getHeldPiece());
				MouseController::getHeldPiece()->setX(x);
				MouseController::getHeldPiece()->setY(y);

				std::string mySTR = "Music/PiecePlace";
				mySTR += std::to_string(rand() % 2 + 1);
				mySTR += ".mp3";
				MusicHelper::loadAndPlayChunk(mySTR, -1);
			}
		}
	}
	MouseController::setHeldPiece(nullptr);
}


bool isWithinDistance(int x, int y, Piece* p) {
	std::vector<Piece*>* pieces = MouseController::getPieces();
	Cell* current = ChessBoard::getCell(x, y);

	for (int i = 0; i < pieces->size(); i++) {
		if(p == pieces->at(i)) {
			if(i > 0) {
				Cell* other = ChessBoard::getCell(pieces->at(i - 1)->getX(), pieces->at(i - 1)->getY());
				if(Cell::distBetweenCells(current, other) > distance) {
					return false;
				}
			}
			if(i < pieces->size() - 1) {
				Cell* other = ChessBoard::getCell(pieces->at(i + 1)->getX(), pieces->at(i + 1)->getY());
				if(Cell::distBetweenCells(current, other) > distance) {
					return false;
				}
			}
			return true;
		}
	}
	return false;
}

// STEP 2.5 the actual checking on the move
bool isValid(int startX, int startY, int tempX, int tempY, Piece* p) {
	if (ChessBoard::positionInBounds(tempX, tempY)) {
		Cell* origin = ChessBoard::getCell(startX, startY);
		Cell* destination = ChessBoard::getCell(tempX, tempY);
		if (destination->isEmpty()) {
			return (isWithinDistance(tempX, tempY, p));
		}
	}
	return false;
}
// STEP 2: CHECKS VALID MOVES 
// THIS RETURNS AN INT[] THAT HOLDS INDICES IN THE BOARD WHERE THE PIECE CAN MOVE TO
// USE THIS FUNCTION TO DRAW THOSE CELLS IN A DIFFERENT COLOUR
std::vector<int>* getValidMoves(Piece* p) {
	std::vector<int>* moves = new std::vector<int>;
	int sizeX;
	int sizeY;
	ChessBoard::getBoardSize(sizeX, sizeY);

	if (p != nullptr) {
		int x = p->getX();
		int y = p->getY();

		switch (p->getType()) {
		case Knight:
			//This is a knight
			for (int i = -2; i <= 2; i++) {
				if (i != 0) {
					int j = 3 - abs(i);
					int tempX = x + i;
					int tempY = y + j;
					if (isValid(x, y, tempX, tempY, p)) {
						moves->push_back(ChessBoard::getIndex(tempX, tempY));
					}
					tempY = y - j;
					if (isValid(x, y, tempX, tempY, p)) {
						moves->push_back(ChessBoard::getIndex(tempX, tempY));
					}
				}
			}
			break;
		case Bishop:
			for(int k = 1; k < distance * 2; k++) {
				if(isValid(x, y, x + k, y + k, p)) {
					moves->push_back(ChessBoard::getIndex(x + k, y + k));
				}
				if(isValid(x, y, x - k, y + k, p)) {
					moves->push_back(ChessBoard::getIndex(x - k, y + k));
				}
				if(isValid(x, y, x + k, y - k, p)) {
					moves->push_back(ChessBoard::getIndex(x + k, y - k));
				}
				if(isValid(x, y, x - k, y - k, p)) {
					moves->push_back(ChessBoard::getIndex(x - k, y - k));
				}
			}
			break;
		case Rook:
			for(int k = 1; k < distance; k++) {
				if(isValid(x, y, x + k, y, p)) {
					moves->push_back(ChessBoard::getIndex(x + k, y));
				}
				if(isValid(x, y, x - k, y, p)) {
					moves->push_back(ChessBoard::getIndex(x - k, y));
				}
				if(isValid(x, y, x, y + k, p)) {
					moves->push_back(ChessBoard::getIndex(x, y + k));
				}
				if(isValid(x, y, x, y - k, p)) {
					moves->push_back(ChessBoard::getIndex(x, y - k));
				}
			}
			break;
		default:
			std::cout << "Piece movement not yet made!";
			break;
		}
	}
	return moves;
}

// STEP 1: PICKS UP THE PIECE - BINDS IT TO THE MOUSE
void pickupPiece() {
	if(MouseController::getHeldPiece() == nullptr) {
		int mouseX;
		int mouseY;
		MouseController::getMousePosition(mouseX, mouseY);
		if (ChessBoard::mouseToBoard(mouseX, mouseY)) {
			Cell* current = ChessBoard::getCell(mouseX, mouseY);
			if (current->getPiece() != nullptr) {
				MouseController::setHeldPiece(current->getPiece());
			}
		}
	}
}