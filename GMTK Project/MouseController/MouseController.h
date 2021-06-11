

#ifndef MOUSE_CONTROLLER_H
#define MOUSE_CONTROLLER_H

#include <vector>
#include "SDL2/SDL.h"
#include "Piece/Piece.h"
#include "Board/Board.h"

class MouseController {
private:
	static bool mouseIsDown;
	static Piece* heldPiece;
	static std::vector<Piece*> pieces;

	//Code will run when Mouse is pressed
	static void mousePressed() {
		pickupPiece();
	}
	//Code will Run when Mouse released
	static void mouseReleased() {
		dropPiece();
	}
public:
	static void updateInfo() {
		bool newMouseIsDown = checkMouseDown();
		
		//If they do not match!
		if (newMouseIsDown != mouseIsDown) {
			if (newMouseIsDown == true) {	//Mouse just got Pressed
				mousePressed();
			} else {	//Mouse just got released
				mouseReleased();
			}
		}

		mouseIsDown = newMouseIsDown;
	}

	//Gets the mouse position
	static void getMousePosition(int& x, int& y) {
		SDL_GetMouseState(&x, &y);
	}

	//Checks if the mouse is Down or not
	static bool checkMouseDown() {
		if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
			return true;
		return false;
	}

	//Adds a piece to the array list
	static void addPiece(Piece* addedPiece) {
		pieces.push_back(addedPiece);
	}
	//Held Piece
	static void setHeldPiece(Piece* newHeldPiece) { heldPiece = newHeldPiece; }
	static Piece* getHeldPiece() { return heldPiece; }

	static void renderPieces() {
		for (int i = 0; i < pieces.size(); i++) {
			Piece* p = pieces[i];
			
			if (p == heldPiece) {
				SDL_FRect* mouseArea = ChessBoard::getCellRenderArea(p->getX(), p->getY());
				int mouseX, mouseY;
				getMousePosition(mouseX, mouseY);
				mouseArea->x = mouseX;
				mouseArea->y = mouseY;
				pieces[i]->render(mouseArea);
			} else				
				pieces[i]->render(ChessBoard::getCellRenderArea(p->getX(), p->getY()));
		}
	}


};

#endif