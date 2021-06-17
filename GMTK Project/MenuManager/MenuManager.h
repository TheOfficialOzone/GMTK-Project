#pragma once


#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include <iostream>
#include <vector>
#include "Rendering/ShapeRenderer/ShapeRenderer.h"
#include "Globals/Globals.h"
#include "Music/MusicHelper/MusicHelper.h"

//The Generic Interactable type
class Interactable {
protected:
	static SDL_Renderer* renderTo;	//Where it renders to
	void	(*func)() = nullptr;	//The function it will take in
	SDL_FRect* renderArea = new SDL_FRect();	//Where this will be rendered
	SDL_FRect* percentArea;	//Where it is rendered in Percentage
	SDL_Color myColour = white;	//The Colour it is
public:
	Interactable(void(*functionToCall)(), SDL_FRect* newRenderArea) {
		setFunction(functionToCall);
		setRenderArea(newRenderArea);

		if (newRenderArea != nullptr) {
			int width, height;
			SDL_GetWindowSize(globalWindow, &width, &height);
			updateRenderArea(width, height);
		}
	}

	virtual void setFunction(void (*functionToCall)()) { func = functionToCall; }
	void setRenderArea(SDL_FRect* newRenderArea) { percentArea = newRenderArea; }
	void updateRenderArea(int width, int height) {
		renderArea->x = width * percentArea->x;
		renderArea->y = height * percentArea->y;
		renderArea->w = width * percentArea->w;
		renderArea->h = height * percentArea->h;
	}
	bool hasFunction() { return func != nullptr; }
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
protected:
	SDL_Texture* myTexture = nullptr;
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

class ScoreInteractable : public TextInteractable {

public:
	ScoreInteractable(std::string newText, void(*functionToCall)(), SDL_FRect* newRenderArea) : TextInteractable(newText, functionToCall, newRenderArea) {
		textColour = white;
	}

	void updateTexture() {
		if (myTexture != nullptr)
			SDL_DestroyTexture(myTexture);
		ShapeRenderer::setColour(textColour);
		ShapeRenderer::textToTexture(text, myTexture);
	}

	void render() {
		int score = levelOn;
		text = std::to_string(score);
		updateTexture();
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

class TiltedInteractable : public TextureInteractable {
public:
	TiltedInteractable(std::string textureToLoad, void(*functionToCall)(), SDL_FRect* newRenderArea) : TextureInteractable(textureToLoad, functionToCall, newRenderArea) {
	}
	void render() {
		GraphicRenderer::renderTextureWithAngle(myTexture, renderArea, 15);
	}
};

class AudioInteractable : public Interactable {
private:
	float audioPercent;
	SDL_Texture* myTexture;

	float xClick;
public:
	AudioInteractable(float newAudioPercent, void(*functionToCall)(), SDL_FRect* newRenderArea) : Interactable(functionToCall, newRenderArea) {
		audioPercent = newAudioPercent;
		MusicHelper::setVolume(std::pow(audioPercent, 2));
		
		GraphicRenderer::loadImageToTexture(myTexture, "Images/note.png");
	}

	void callFunction() {
		if (func != nullptr)
			func();
	}

	void updateRenderArea(int width, int height) {
		Interactable::updateRenderArea(width, height);
		xClick = renderArea->x + audioPercent / renderArea->w;
	}
	
	bool clickInBounds(int clickX, int clickY) {
		bool clickIn = (clickX > renderArea->x && clickX - renderArea->x < renderArea->w&& clickY > renderArea->y && clickY - renderArea->y < renderArea->h);

		if (clickIn) {
			xClick = clickX;
			audioPercent = (xClick - renderArea->x) / renderArea->w;
			MusicHelper::setVolume(std::pow(audioPercent, 2));
		}
		return clickIn;
	};

	bool hasFunction() { return true; }

	void render() {
		Line* newLine = new Line(renderArea->x, renderArea->y + renderArea->h / 2, renderArea->x + renderArea->w, renderArea->y + renderArea->h / 2);
		SDL_FRect* grabX = new SDL_FRect();
		grabX->x = renderArea->x + renderArea->w * audioPercent - renderArea->w / 10;
		grabX->y = renderArea->y;
		grabX->w = renderArea->w / 5;
		grabX->h = renderArea->h;

		ShapeRenderer::setColour(navy);
		ShapeRenderer::renderLine(newLine);
		GraphicRenderer::renderTexture(myTexture, grabX);
		delete newLine;
		delete grabX;
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
				if (interactables[i]->hasFunction()) {
					interactables[i]->callFunction();
					return 1;	//Returns 1 when there was a button clicked
				}
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