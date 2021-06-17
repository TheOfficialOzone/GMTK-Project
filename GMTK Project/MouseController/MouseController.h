

#ifndef MOUSE_CONTROLLER_H
#define MOUSE_CONTROLLER_H

#include <vector>
#include "SDL2/SDL.h"
#include "Piece/Piece.h"
#include "Board/Board.h"
#include "Screens/GameScreen.h"

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
	static SDL_Texture* ropeTexture;
	//Updates the mouse info
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

		checkPiecesInLava();
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
		Cell* piecesCell = ChessBoard::getCell(addedPiece->getX(), addedPiece->getY());
		piecesCell->setPiece(addedPiece);
	}

	static void checkPiecesInLava() {
		for (int i = 0; i < pieces.size(); i++) {
			int lavaPos = ChessBoard::getLavaPos();
			if (pieces[i]->getX() < lavaPos) {
				lostGame();
			}
		}
	}
	//Held Piece
	static void resetMouseController() {
		pieces.clear();
		heldPiece = nullptr;
		mouseIsDown = false;
	}
	static void setHeldPiece(Piece* newHeldPiece) { heldPiece = newHeldPiece; }
	static Piece* getHeldPiece() { return heldPiece; }

	//Gets the pieces vector
	static std::vector<Piece*>* getPieces() {
		return &pieces;
	}

	//Renders the Pieces
	static void renderPieces() {
		bool overridePiecePos = false;
		float overX = 0, overY = 0;
		for (int i = 1; i < pieces.size(); i++) {
			Line* line = new Line();

			line->point1.x = pieces[i - 1]->getX();
			line->point1.y = pieces[i - 1]->getY();

			line->point2.x = pieces[i]->getX();
			line->point2.y = pieces[i]->getY();

			ChessBoard::boardToPixel(line->point1.x, line->point1.y);
			ChessBoard::boardToPixel(line->point2.x, line->point2.y);

			float xDif = line->point1.x - line->point2.x;
			float yDif = line->point1.y - line->point2.y;

			SDL_FRect* ropeRect = new SDL_FRect();

			float dist = std::sqrt(std::pow(xDif, 2) + std::pow(yDif, 2));

			ropeRect->x = line->point1.x;
			ropeRect->y = line->point1.y - 10;
			ropeRect->w = dist;
			ropeRect->h = 20;

			float angle = std::atan2(-yDif, -xDif) * 180 / M_PI;
			SDL_FPoint* point = new SDL_FPoint();
			point->x = 0;
			point->y = 10;
			GraphicRenderer::renderTextureWithAngleAroundAxis(ropeTexture, ropeRect, angle, point);

			delete ropeRect;
			delete point;
			delete line;
		}

		for (int i = 0; i < pieces.size(); i++) {
			Piece* p = pieces[i];
			
			if (p == heldPiece) {
				SDL_FRect* mouseArea = ChessBoard::getCellRenderArea(p->getX(), p->getY());
				int mouseX, mouseY;
				getMousePosition(mouseX, mouseY);
				mouseArea->x = mouseX;
				mouseArea->y = mouseY;

				for (int i = 0; i < pieces.size(); i++) {
					if(heldPiece == pieces.at(i)) {
						if(i > 0) {
							Line* phantom1 = new Line();

							phantom1->point1.x = pieces[i - 1]->getX();
							phantom1->point1.y = pieces[i - 1]->getY();

							phantom1->point2.x = mouseX;
							phantom1->point2.y = mouseY;

							ChessBoard::boardToPixel(phantom1->point1.x, phantom1->point1.y);


							float length = std::sqrt(std::pow(phantom1->point1.x - phantom1->point2.x, 2) + std::pow(phantom1->point1.y - phantom1->point2.y, 2));
							int pX = pieces[i - 1]->getX();
							int pY = pieces[i - 1]->getY();

							float mX = mouseX, mY = mouseY;
							ChessBoard::boardToPixel(pX, pY);
							length = std::sqrt(std::pow(pX - mX, 2) + std::pow(pY - mY, 2));

							SDL_FRect* cellSize = ChessBoard::getCellRenderArea(0, 0);
							if (length > distance * cellSize->w) {
								float angle = std::atan2(mX - pX, mY - pY);

								
								overX = sin(angle) * distance * cellSize->w + pX;
								overY = cos(angle) * distance * cellSize->h + pY;
								overridePiecePos = true;
								
								phantom1->point2.x = overX;
								phantom1->point2.y = overY;
								
							}
							delete cellSize;

							ShapeRenderer::setColour(red);
							ShapeRenderer::renderLine(phantom1);

							delete phantom1;
						}
						if(i < pieces.size() - 1) {
							Line* phantom2 = new Line();

							phantom2->point1.x = pieces[i + 1]->getX();
							phantom2->point1.y = pieces[i + 1]->getY();

							phantom2->point2.x = mouseX;
							phantom2->point2.y = mouseY;

							ChessBoard::boardToPixel(phantom2->point1.x, phantom2->point1.y);

							float length = std::sqrt(std::pow(phantom2->point1.x - phantom2->point2.x, 2) + std::pow(phantom2->point1.y - phantom2->point2.y, 2));
							int pX = pieces[i + 1]->getX();
							int pY = pieces[i + 1]->getY();

							float mX = mouseX, mY = mouseY;
							ChessBoard::boardToPixel(pX, pY);
							length = std::sqrt(std::pow(pX - mX, 2) + std::pow(pY - mY, 2));

							SDL_FRect* cellSize = ChessBoard::getCellRenderArea(0, 0);
							if (length > distance * cellSize->w) {
								float angle = std::atan2(mX - pX, mY - pY);


								overX = sin(angle) * distance * cellSize->w + pX;
								overY = cos(angle) * distance * cellSize->h + pY;
								overridePiecePos = true;

								phantom2->point2.x = overX;
								phantom2->point2.y = overY;

							}
							delete cellSize;



							ShapeRenderer::setColour(red);
							ShapeRenderer::renderLine(phantom2);

							delete phantom2;
						}
					}
				}	
				if (overridePiecePos) {
					mouseArea->x = overX;
					mouseArea->y = overY;
				}

				pieces[i]->render(mouseArea);
				delete mouseArea;
			}			

			SDL_FRect* delRect = ChessBoard::getCellRenderArea(p->getX(), p->getY());
			pieces[i]->render(delRect);
			delete delRect;
		}
	}
};

#endif