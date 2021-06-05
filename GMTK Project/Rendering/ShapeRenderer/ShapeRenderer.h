#pragma once


#ifndef SHAPE_RENDERER_H
#define SHAPE_RENDERER_H


#include "SDL2/SDL.h"

//The Line Shape
struct Line {
	SDL_FPoint point1;
	SDL_FPoint point2;
};



class ShapeRenderer {
private:
	SDL_Color renderColour;	//The Colour objects will be rendered at
	SDL_Renderer* renderer;	//What renderer we are refering to

	bool fill;	//If it is filled or not
public:
	//Constructor 
	ShapeRenderer(SDL_Renderer* newRenderer) {
		renderer = newRenderer;
	}

	///	Shape Conversion
	SDL_FPoint* linesToPoints(Line** lines, int amount) {
		SDL_FPoint* points = new SDL_FPoint[amount + 1];

		points[0].x = lines[0]->point1.x;	//Gets the first point
		points[0].y = lines[0]->point1.y;

		//Now only has to connect to the last position of each line
		for (int i = 0; i < amount; i++) {
			points[i + 1].x = lines[i]->point2.x;
			points[i + 1].y = lines[i]->point2.y;
		}

		return points;
	}

	//Colors!
	void setColour(SDL_Color newColour) {
		renderColour = newColour; 
		SDL_SetRenderDrawColor(renderer, renderColour.r, renderColour.g, renderColour.b, renderColour.a); 
	}
	SDL_Color getColour() { return renderColour; }

	//Renderer
	void setRenderer(SDL_Renderer* newRenderer) { renderer = newRenderer; }
	SDL_Renderer* getRenderer() { return renderer; }

	//Filling
	void fillEnable() { fill = true; }
	void fillDisable() { fill = false; }

	////	Shape Drawing

	//Rect
	void renderRect(SDL_FRect* drawRect) {
		if (fill)
			SDL_RenderFillRectF(renderer, drawRect);
		else
			SDL_RenderDrawRectF(renderer, drawRect);
	}

	//Lines
	void renderLine(Line* drawLine) { SDL_RenderDrawLineF(renderer, drawLine->point1.x, drawLine->point1.y, drawLine->point2.x, drawLine->point2.y); }
	void renderLines(Line** drawLines, int amount) {
		SDL_RenderDrawLinesF(renderer, linesToPoints(drawLines, amount), amount + 1);
	}


};


#endif