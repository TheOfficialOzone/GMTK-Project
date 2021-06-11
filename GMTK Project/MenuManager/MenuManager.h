#pragma once


#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include <iostream>
#include <vector>
#include "Rendering/ShapeRenderer/ShapeRenderer.h"
#include "Globals.h"

//The Generic Interactable type
class Interactable {
protected:
	static SDL_Renderer* renderTo;	//Where it renders to
	void	(*func)();	//The function it will take in
	SDL_FRect* renderArea = new SDL_FRect();	//Where this will be rendered
	SDL_FRect* percentArea;	//Where it is rendered in Percentage
	SDL_Color myColour = white;	//The Colour it is
public:
	Interactable(void(*functionToCall)(), SDL_FRect* newRenderArea) {
		setFunction(functionToCall);
		setRenderArea(newRenderArea);
	}

	virtual void setFunction(void (*functionToCall)()) { func = functionToCall; }
	void setRenderArea(SDL_FRect* newRenderArea) { percentArea = newRenderArea; }
	void updateRenderArea(int width, int height) {
		renderArea->x = width * percentArea->x;
		renderArea->y = height * percentArea->y;
		renderArea->w = width * percentArea->w;
		renderArea->h = height * percentArea->h;
	}
	void callFunction() { 
		if (func != nullptr)
			func(); 
	}
	void setColor(SDL_Color newColour) { myColour = newColour; }

	//Checks if a click is within it's boundaries
	virtual bool clickInBounds(int clickX, int clickY) {
		return (clickX > renderArea->x && clickX - renderArea->x < renderArea->w&& clickY > renderArea->y && clickY - renderArea->y < renderArea->h);
	};

	//Renders the Interactable
	virtual void render() {
		ShapeRenderer::fillEnable();
		ShapeRenderer::setColour(myColour);
		ShapeRenderer::renderRect(renderArea);
	}
};


class TextInteractable : public Interactable {
private:
	SDL_Texture* myTexture;
	SDL_Color textColour = black;
	std::string text;
public:
	TextInteractable(std::string newText, void(*functionToCall)(), SDL_FRect* newRenderArea) : Interactable(functionToCall, newRenderArea) {
		setText(newText);
		updateTexture();
	}

	void setTextColor(SDL_Color newColour) {
		textColour = newColour;
		updateTexture();
	}

	void setText(std::string newText){ text = newText; }

	void updateTexture() {
		ShapeRenderer::setColour(textColour);
		ShapeRenderer::textToTexture(text, myTexture);
	}

	void render() {
		Interactable::render();
		GraphicRenderer::renderTexture(myTexture, renderArea);
	}
};


class TextureInteractable : public Interactable {
protected:
	SDL_Texture* myTexture = nullptr;
public:
	TextureInteractable(std::string textureToLoad, void(*functionToCall)(), SDL_FRect* newRenderArea) : Interactable(functionToCall, newRenderArea) {
		GraphicRenderer::loadImageToTexture(myTexture, textureToLoad);
	}

	void setTexture(std::string textureToLoad) {
		SDL_Texture* deleteTexture = myTexture;
		SDL_DestroyTexture(deleteTexture);
		GraphicRenderer::loadImageToTexture(myTexture, textureToLoad);

	}

	void render() {
		GraphicRenderer::renderTexture(myTexture, renderArea);
	}
};


//Managers all the interactables
class InteractManager {
private:
	static std::vector<Interactable*> interactables;
	static SDL_Window* myWindow;
public:
	//Sets the window
	static void setWindow(SDL_Window* newWindow) { myWindow = newWindow; }

	//Loops through all te buttons checking if any clicks are inbound
	static int clickInBounds(int clickX, int clickY) {
		for (int i = 0; i < interactables.size(); i++) {
			if (interactables[i]->clickInBounds(clickX, clickY)) {
				interactables[i]->callFunction();
				return 1;	//Returns 1 when there was a button clicked
			}
		}
		return -1;	//No button was clicked
	}

	//Adds an Interactable
	static void addInteractable(Interactable* newInteractable) {
		interactables.push_back(newInteractable);
	}

	//removes an interactable to the list
	static void removeInteractable(int index) {
		Interactable* deleteInteractable = interactables[index];
		interactables.erase(interactables.begin() + index);
		delete deleteInteractable;
	}

	//Removes all the Interactables
	static void removeAllInteractables() {
		int size = interactables.size();
		for (int i = 0; i < size; i++)
			removeInteractable(0);
		interactables.clear();
	}
	
	//Renders the interactables
	static void renderInteractables() {
		for (int i = 0; i < interactables.size(); i++) {
			interactables[i]->render();
		}
	}

	//Updates the interactables 
	static void updateInteractables() {
		int width, height;
		SDL_GetWindowSize(myWindow, &width, &height);

		for (int i = 0; i < interactables.size(); i++) {
			interactables[i]->updateRenderArea(width, height);
		}
	}
};





#endif