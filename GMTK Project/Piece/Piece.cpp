

#include <vector>
#include <algorithm>

#include "Globals.h"
#include "Piece.h"
#include "Board/Board.h"
#include "Rendering/ShapeRenderer/ShapeRenderer.h"
#include "MouseController/MouseController.h"

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

		ShapeRenderer::setColour(red);
		ShapeRenderer::renderRect(renderRect);
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
		std::vector<int>* moves = getValidMoves(MouseController::getPiece());
		if (ChessBoard::mouseToBoard(x, y)) { // converts x and y to board indeces as well as does the check
			int index = ChessBoard::getIndex(x, y);
			if (std::find(moves->begin(), moves->end(), index) != moves->end()) {	//IF it's in the vector
				Cell* origin = ChessBoard::getCell(MouseController::getHeldPiece()->getX(), MouseController::getHeldPiece()->getY());
				Cell* destination = ChessBoard::getCell(x, y);
				origin->removePiece();
				destination->setPiece(MouseController::getHeldPiece());
				MouseController::getHeldPiece()->setX(x);
				MouseController::getHeldPiece()->setY(y);
			}
		}
	}
	MouseController::setHeldPiece(nullptr);
}

// STEP 2.5 the actual checking on the move
bool isValid(int startX, int startY, int tempX, int tempY) {
	if (ChessBoard::positionInBounds(tempX, tempY)) {
		Cell* origin = ChessBoard::getCell(startX, startY);
		Cell* destination = ChessBoard::getCell(tempX, tempY);
		if (destination->isEmpty()) {
			if (Cell::distBetweenCells(origin, destination) <= distance) {
				return true;
			}
		}
	}
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
					if (isValid(x, y, tempX, tempY)) {
						moves->push_back(ChessBoard::getIndex(tempX, tempY));
					}
					tempY = y - j;
					if (isValid(x, y, tempX, tempY)) {
						moves->push_back(ChessBoard::getIndex(tempX, tempY));
					}
				}
			}
			break;
		case Bishop:
			for(int k = 1; k < distance; k++) {
				if(isValid(x, y, x + k, y + k)) {
					moves->push_back(ChessBoard::getIndex(x + k, y + k));
				}
				if(isValid(x, y, x - k, y + k)) {
					moves->push_back(ChessBoard::getIndex(x - k, y + k));
				}
				if(isValid(x, y, x + k, y - k)) {
					moves->push_back(ChessBoard::getIndex(x + k, y - k));
				}
				if(isValid(x, y, x - k, y - k)) {
					moves->push_back(ChessBoard::getIndex(x - k, y - k));
				}
			}
			break;
		case Rook:
			for(int k = 1; k < distance; k++) {
				if(isValid(x, y, x + k, y)) {
					moves->push_back(ChessBoard::getIndex(x + k, y));
				}
				if(isValid(x, y, x - k, y)) {
					moves->push_back(ChessBoard::getIndex(x - k, y));
				}
				if(isValid(x, y, x, y - k)) {
					moves->push_back(ChessBoard::getIndex(x, y - k));
				}
				if(isValid(x, y, x, y - k)) {
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
			std::cout << mouseX << " " << mouseY << std::endl;
			if (current->getPiece() != nullptr) {
				MouseController::setHeldPiece(current->getPiece());
			}
		}
	}
}