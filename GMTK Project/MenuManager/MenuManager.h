#pragma once


#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include <iostream>
#include <vector>
#include "Rendering/ShapeRenderer/ShapeRenderer.h"

void print() {
	std::cout << "Called Baby!";
}

//The Generic Interactable type
class Interactable {
private:
	static SDL_Renderer* renderTo;	//Where it renders to
	void	(*func)();	//The function it will take in
	SDL_FRect* renderArea;	//Where this will be rendered

public:
	Interactable(void(*functionToCall)(), SDL_FRect* newRenderArea) {
		setFunction(functionToCall);
		setRenderArea(newRenderArea);
	}

	void setFunction(void (*functionToCall)()) { func = functionToCall; }
	void setRenderArea(SDL_FRect* newRenderArea) { renderArea = newRenderArea; }
	void callFunction() { func(); }

	//Checks if a click is within it's boundaries
	virtual bool clickInBounds(int clickX, int clickY) {
		return (clickX > renderArea->x && clickX - renderArea->x < renderArea->w&& clickY > renderArea->y && clickY - renderArea->y < renderArea->h);
	};

	//Renders the Interactable
	virtual void render() {
		ShapeRenderer::renderRect(renderArea);
	}
};

//Managers all the interactables
class InteractManager {
private:
	std::vector<Interactable*> interactables;
public:

	//Loops through all te buttons checking if any clicks are inbound
	int clickInBounds(int clickX, int clickY) {
		for (int i = 0; i < interactables.size(); i++) {
			if (interactables[i]->clickInBounds(clickX, clickY)) {
				interactables[i]->callFunction();
				return 1;	//Returns 1 when there was a button clicked
			}
		}
		return -1;	//No button was clicked
	}

	//Adds an Interactable
	void addInteractable(Interactable* newInteractable) {
		interactables.push_back(newInteractable);
	}

	//removes an interactable to the list
	void removeInteractable(int index) {
		Interactable* deleteInteractable;
		interactables.erase(interactables.begin() + index);
		delete deleteInteractable;
	}

	//Removes all the Interactables
	void removeAllInteractables() {
		int size = interactables.size();
		for (int i = 0; i < size; i++)
			removeInteractable(0);
		interactables.clear();
	}
	
	//Renders the interactables
	void renderInteractables() {
		for (int i = 0; i < interactables.size(); i++) {
			interactables[i]->render();
		}
	}
};





#endif