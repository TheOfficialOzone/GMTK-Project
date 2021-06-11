#pragma once


#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <algorithm>
#include <math.h>

#include "SDL2/SDL.h"
#include "Piece/Piece.h"

#include "Rendering/ShapeRenderer/ShapeRenderer.h"

class Cell {
private:
	Piece* myPiece = nullptr;
	int x, y;
public:
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

	bool isEmpty() { return true; }
	void render(SDL_FRect* renderArea) {
		if ((x + y) % 2)
			ShapeRenderer::setColour(white);
		else 
			ShapeRenderer::setColour(black);

		ShapeRenderer::renderRect(renderArea);
	}

	//Compares the distance between 2 cells
	static float distBetweenCells(Cell* firstCell, Cell* secondCell) {
		int x1, y1, x2, y2;
		firstCell->getPos(x1, y1);
		secondCell->getPos(x2, y2);

		float dist = std::sqrt(std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2));
		return dist;
	}
};

class ChessBoard {
private:
	static int sizeX;
	static int sizeY;

	static Cell** board;
	static SDL_FRect* percentArea;	//Where it is rendered in Percentage
	static SDL_FRect* renderArea;	//Where it is rendered in Percentage


public:
	ChessBoard(int newSizeX, int newSizeY, SDL_FRect* newPercentArea) {
		setRenderArea(newPercentArea);

		sizeX = newSizeX;
		sizeY = newSizeY;

		board = new Cell * [sizeX];
		for (int i = 0; i < sizeX; i++) {
			board[i] = new Cell[sizeY];
		}

		for (int y = 0; y < sizeY; y++) {
			for (int x = 0; x < sizeX; x++) {
				board[x][y].setPos(x, y);
			}
		}
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
		cellRenderArea->x = std::roundf(renderArea->x + renderArea->w * (float)x / sizeX);
		cellRenderArea->y = std::roundf(renderArea->y + renderArea->h * (float)y / sizeY);

		cellRenderArea->w = renderArea->w / sizeX;
		cellRenderArea->h = renderArea->h / sizeY;

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

	//Converts mouse coordinates into a single index in the grid. returns -1 if not in grid
	static bool mouseToBoard(int& x, int& y) {
		int startX = renderArea->x;
		int startY = renderArea->y;
		int width = renderArea->w;
		int height = renderArea->h;

		if(ChessBoard::clickInBounds(x, y)) {
			int sizeX;
			int sizeY;
			ChessBoard::getBoardSize(sizeX, sizeY);

			SDL_FRect* cellArea = getCellRenderArea(0, 0);
			x = (x - startX) / cellArea->w;
			y = (y - startY) / cellArea->h;

			return true;
		} else {
			return false;
		}
	}

	//Checks if a position is legal
	static bool positionInBounds(int x, int y) { return (x >= 0 && y >= 0 && x < sizeX&& y < sizeY); }
	//Checks if the click is in the renderers bounds
	static bool clickInBounds(int x, int y) { return (x >= renderArea->x && y >= renderArea->y && x < renderArea->x + renderArea->w && y < renderArea->y + renderArea->h); }
	//Draws the board
	static void drawBoard() {
		for (int y = 0; y < sizeY; y++) {
			for (int x = 0; x < sizeX; x++) {
				board[x][y].render(getCellRenderArea(x, y));
			}
		}
	}

};



#endif