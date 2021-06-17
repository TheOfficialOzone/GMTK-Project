#pragma once


#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <algorithm>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <sstream>      // std::stringstream

#include "SDL2/SDL.h"
#include "Piece/Piece.h"
#include "Level/Level.h"

#include "Rendering/ShapeRenderer/ShapeRenderer.h"

class Cell {
private:
	Piece* myPiece = nullptr;
	int x, y;
	bool isLava = false;

	static bool flipColour;

	
public:
	static SDL_Texture* lavaFull;
	static std::vector<SDL_Texture*> lavaPieces;

	Cell() {}

	Cell(Piece* newPiece, int newX, int newY) {
		setPiece(newPiece);
		setPos(newX, newY);
	}

	//Piece functions
	Piece* getPiece() { return myPiece; }
	void setPiece(Piece* newPiece) { myPiece = newPiece; }
	void removePiece() { myPiece = nullptr; }
	
	void setPos(int newX, int newY) {
		x = newX;
		y = newY;
	}
	void getPos(int& getX, int& getY) { getX = x; getY = y; }
	void decrementX() { x--; }
	void enableLava() { isLava = true; }

	//All the math the Cell has to do when it rotates down
	void rotateDown() {
		decrementX();
		if (myPiece != nullptr) {
			myPiece->rotateDown();
		}
	}
	//Flips the Colours
	static void flipColours() { flipColour = !flipColour; }

	bool isEmpty() { return this->myPiece == nullptr && !isLava; }

	void render(SDL_FRect* rect);

	//Compares the distance between 2 cells
	static float distBetweenCells(Cell* firstCell, Cell* secondCell) {
		int x1, y1, x2, y2;
		firstCell->getPos(x1, y1);
		secondCell->getPos(x2, y2);

		float dist = std::sqrt(std::pow((float)x1 - x2, 2.0f) + std::pow((float)y1 - y2, 2.0f));
		return dist;
	}
};

class ChessBoard {
private:
	static int sizeX;
	static int sizeY;
	static int lavaPosition;
	static int loadX;

	static int lavaWait;
	static int lastLavaMove;

	static float cameraX;

	static Cell** board;
	static SDL_FRect* percentArea;	//Where it is rendered in Percentage
	static SDL_FRect* renderArea;	//Where it is rendered in Percentage

	static std::ifstream* levelFile;
public:
	ChessBoard(int newSizeX, int newSizeY, SDL_FRect* newPercentArea) {
		setRenderArea(newPercentArea);

		sizeX = newSizeX;
		sizeY = newSizeY;

		resetBoard();
	}

	//Call this when changing window size and setting the renderArea fixes where the Board is on screen
	static void updateRenderArea() {
		int width, height;
		SDL_GetWindowSize(globalWindow, &width, &height);

		renderArea->x = width * percentArea->x;
		renderArea->y = height * percentArea->y;
		renderArea->w = width * percentArea->w;
		renderArea->h = height * percentArea->h;
	}

	//Sets the new renderArea of the ChessBoard
	static void setRenderArea(SDL_FRect* newPercentArea) {
		percentArea = newPercentArea;
		updateRenderArea();
	}

	//Gets the cell
	static Cell* getCell(int x, int y) {
		return &board[x][y];
	}

	//Gets where a cell will be rendered to on the screen
	static SDL_FRect* getCellRenderArea(int x, int y) {
		SDL_FRect* cellRenderArea = new SDL_FRect();
		cellRenderArea->x = renderArea->x + renderArea->w * (float)x / sizeX;
		cellRenderArea->y = renderArea->y + renderArea->h * (float)y / sizeY;

		cellRenderArea->w = renderArea->w / sizeX;
		cellRenderArea->h = renderArea->h / sizeY;

		cellRenderArea->x -= cellRenderArea->w * cameraX;

		return cellRenderArea;
	}

	//Gets the size of the board
	static void getBoardSize(int& getSizeX, int& getSizeY) {
		getSizeX = sizeX;
		getSizeY = sizeY;
	}

	//Returns a single int for the index
	static int getIndex(int x, int y) {
		return y * sizeX + x;
	}

	//Converts a single index into x,y coordinates
	static void getCoords(int& x, int& y, int index) {
		x = index % sizeX;
		y = index / sizeX;
	}

	//Gets the distance of the closest piece to the left edge
	static int getPieceDistanceFromEdge() {
		int closestEdge = sizeX;
		for (int x = 0; x < sizeX; x++) {
			if (x >= closestEdge)	//Optimization
				break;
			for (int y = 0; y < sizeY; y++) {
				Cell* c = &board[x][y];

				if (!c->isEmpty()) {
					if (c->getPiece() != nullptr) {
						if (!c->getPiece()->hasEnemy())
							closestEdge = x;
					}
				}
			}
		}

		return closestEdge;
	}

	//Gets a bunch of pieces and get the center point
	static void calculateCameraX(std::vector<Piece*>* pieces) {
		float addedTotal = 0;
		float amount = pieces->size();
		for (int i = 0; i < amount; i++) {	//Loops through all the pieces
			Piece* p = pieces->at(i);
			addedTotal += p->getX();
		}
		addedTotal = std::round(addedTotal / amount);	//Gets the average
		

		float lerpCameraX = addedTotal - 7;
		float xDif = lerpCameraX - cameraX;
		if (xDif > 1)
			cameraX += xDif * 0.01;
	}

	//Will rotate all the Cells down 1 to the left
	static void rotateCellsDown() {
		Cell::flipColours();
		lavaPosition -= 1;
		if (lavaPosition < 0) {
			lavaPosition = 0;
		}
		cameraX -= 1;	//Lowers the camera by 1

		for (int x = 0; x < sizeX - 1; x++) {
			for (int y = 0; y < sizeY; y++) {
				Cell c = board[x + 1][y];
				c.rotateDown();
				board[x][y] = c;
				

				if (!c.isEmpty()) {
					int getX, getY;
					c.getPos(getX, getY);
				}
			}
		}

		loadNextChunk();
	}

	static int getLoadX() { return loadX; }

	static void loadNextChunk() {
		bool runLoop = true;
		std::string text;

		std::ifstream* file = loadFile("Level/level.txt");

		for (int y = 0; y < sizeY; y++) {	//Makes the next Chunk of the program
			Cell c;
			c.setPos(sizeX - 1, y);
			board[sizeX - 1][y] = c;
		}
		if (randomGame) {
			for (int i = 0; i < 6; i++) {
				int rand = std::rand() % 1000;

				if (rand < loadX * 5 + 100) {
					int rY = std::rand() % 6;
					EnemyPiece* p = new EnemyPiece(None, sizeX - 1, rY, "Images/Block.png", nullptr);
					ChessBoard::addPiece(p);
				}
			}
		} else {
			while (std::getline(*file, text)) {
				std::vector<std::string> tokens;
				std::stringstream check1(text);
				std::string word;

				while (std::getline(check1, word, ' ')) {	//Loops through each word
					tokens.push_back(word);
				}

				if (tokens.size() != 0) {
					int cX = std::stoi(tokens[0]);
					int cY = std::stoi(tokens[1]);
					if (cX > loadX)
						break;
					if (cX < loadX)
						continue;
					if (cX == loadX) {
						EnemyPiece* p = new EnemyPiece(None, sizeX - 1, cY, "Images/Block.png", nullptr);
						ChessBoard::addPiece(p);
					}
				}

				text = "";
			}
		}
		loadX++;
		levelOn = loadX;
		file->close();
	}

	//Updates every frame
	static void updateChessBoard() {
		updateLavaPos();
		if (getPieceDistanceFromEdge() > 10) {
			rotateCellsDown();
		}
	}

	//Converts mouse coordinates into a single index in the grid. returns -1 if not in grid
	static bool mouseToBoard(int& x, int& y) {
		int startX = renderArea->x;
		int startY = renderArea->y;
		int width = renderArea->w;
		int height = renderArea->h;

		if (ChessBoard::clickInBounds(x, y)) {
			int sizeX;
			int sizeY;
			ChessBoard::getBoardSize(sizeX, sizeY);

			int tempX = x;

			SDL_FRect* cellArea = getCellRenderArea(0, 0);

			x = (x - startX + cellArea->w * cameraX) / cellArea->w;
			y = (y - startY) / cellArea->h;

			delete cellArea;
			return true;
		} else {
			return false;
		}
	}
	//Floats instead
	static bool mouseToBoard(float& x, float& y) {
		int startX = renderArea->x;
		int startY = renderArea->y;
		int width = renderArea->w;
		int height = renderArea->h;

		if (ChessBoard::clickInBounds(x, y)) {
			int sizeX;
			int sizeY;
			ChessBoard::getBoardSize(sizeX, sizeY);

			int tempX = x;

			SDL_FRect* cellArea = getCellRenderArea(0, 0);

			x = (x - startX + cellArea->w * cameraX) / cellArea->w;
			y = (y - startY) / cellArea->h;

			delete cellArea;
			return true;
		}
		else {
			return false;
		}
	}

	//Converts a board position to Pixel position
	static void boardToPixel(int& x, int& y) {
		SDL_FRect* drawnArea = getCellRenderArea(x, y);

		x = drawnArea->x + drawnArea->w / 2;
		y = drawnArea->y + drawnArea->h / 2;

		delete drawnArea;
	}
	//Overload for floats
	static void boardToPixel(float& x, float& y) {
		SDL_FRect* drawnArea = getCellRenderArea(x, y);

		x = drawnArea->x + drawnArea->w / 2.0f;
		y = drawnArea->y + drawnArea->h / 2.0f;

		delete drawnArea;
	}

	//Checks if a position is legal
	static bool positionInBounds(int x, int y) { return (x >= 0 && y >= 0 && x < sizeX && y < sizeY); }

	//Puts a Piece at this cell
	static void addPiece(Piece* p) {
		Cell* piecesCell = ChessBoard::getCell(p->getX(), p->getY());
		piecesCell->setPiece(p);
	}

	//Checks if the click is in the renderers bounds
	static bool clickInBounds(int x, int y) { return (x >= renderArea->x && y >= renderArea->y && x < renderArea->x + renderArea->w && y < renderArea->y + renderArea->h); }

	//Gets the X of the lava
	static int getLavaPos() { return lavaPosition; }
	static void updateLavaPos() {
		if (SDL_GetTicks() - lastLavaMove > lavaWait) {
			lavaPosition++;
			lastLavaMove = SDL_GetTicks();
		}
	}

	//Draws the board
	static void drawBoard() {
		for (int y = 0; y < sizeY; y++) {
			for (int x = 0; x < sizeX; x++) {
				SDL_FRect* drawArea = getCellRenderArea(x, y);
				board[x][y].render(drawArea);
				delete drawArea;
			}
		}
	}

	//Resets the board
	static void resetBoard() {
		board = new Cell * [sizeX];
		for (int i = 0; i < sizeX; i++) {
			board[i] = new Cell[sizeY];
		}

		for (int y = 0; y < sizeY; y++) {
			for (int x = 0; x < sizeX; x++) {
				board[x][y].setPos(x, y);
			}
		}

		lavaPosition = 1;
		cameraX = 0;
		loadX = 0;
		levelOn = loadX;
	}

};



#endif