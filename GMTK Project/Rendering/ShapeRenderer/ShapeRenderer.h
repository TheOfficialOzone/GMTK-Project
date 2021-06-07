#pragma once


#ifndef SHAPE_RENDERER_H
#define SHAPE_RENDERER_H


#include "SDL2/SDL.h"

//The Line Shape
struct Line {
	Line(float x1, float y1, float x2, float y2) {
		point1.x = x1;
		point1.y = y1;
		point2.x = x2;
		point2.y = y2;
	}

	SDL_FPoint point1;
	SDL_FPoint point2;
};



class ShapeRenderer {
private:
	static SDL_Color renderColour;	//The Colour objects will be rendered at
	static SDL_Renderer* renderer;	//What renderer we are refering to

	static bool fill;	//If it is filled or not
public:
	//Constructor enter the values you want
	ShapeRenderer(SDL_Renderer* newRenderer, SDL_Color newColour) {
		setRenderer(newRenderer);
		setColour(newColour);
	}

	//Defaults the Color to white
	ShapeRenderer(SDL_Renderer* newRenderer) {
		setRenderer(newRenderer);

		SDL_Color newColour;
		newColour.r = 255;
		newColour.g = 255;
		newColour.b = 255;
		newColour.a = SDL_ALPHA_OPAQUE;
		setColour(newColour);
	}

	///	Shape Conversion
	static SDL_FPoint* linesToPoints(Line** lines, int amount) {
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
	static void setColour(SDL_Color newColour) {
		renderColour = newColour; 
		SDL_SetRenderDrawColor(renderer, renderColour.r, renderColour.g, renderColour.b, renderColour.a); 
	}
	static SDL_Color getColour() { return renderColour; }

	//Renderer
	static void setRenderer(SDL_Renderer* newRenderer) { renderer = newRenderer; }
	static SDL_Renderer* getRenderer() { return renderer; }

	//Filling
	static void fillEnable() { fill = true; }
	static void fillDisable() { fill = false; }

	////	Shape Drawing

	//Rect
	static void renderRect(SDL_FRect* drawRect) {
		if (fill)
			SDL_RenderFillRectF(renderer, drawRect);
		else
			SDL_RenderDrawRectF(renderer, drawRect);
	}

	//Lines
	static void renderLine(Line* drawLine) { SDL_RenderDrawLineF(renderer, drawLine->point1.x, drawLine->point1.y, drawLine->point2.x, drawLine->point2.y); }
	static void renderLines(Line** drawLines, int amount) {
		SDL_RenderDrawLinesF(renderer, linesToPoints(drawLines, amount), amount + 1);
	}

	//Point
	static void renderPoint(SDL_FPoint* drawPoint) { SDL_RenderDrawPointF(renderer, drawPoint->x, drawPoint->y); }
	static void renderPoints(SDL_FPoint* drawPoints, int amount) { SDL_RenderDrawPointsF(renderer, drawPoints, amount); }

};

#endif