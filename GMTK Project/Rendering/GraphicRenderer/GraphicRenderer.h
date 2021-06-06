#pragma once

#ifndef GRAPHIC_RENDERER_H
#define GRAPHIC_RENDERER_H

#include <string>
#include <iostream>

#include "SDL2/SDL.h"
#include "SDL_image.h"





class GraphicRenderer {
private:
	SDL_Renderer* renderer;	//What renderer we are refering to

public:
	//Constructor
	GraphicRenderer(SDL_Renderer* newRenderer) {
		setRenderer(newRenderer);
	}

	//Renderer
	void setRenderer(SDL_Renderer* newRenderer) { renderer = newRenderer; }
	SDL_Renderer* getRenderer() { return renderer; }

	//Returns 1 on success, -1 if image failed to load
	int loadImageToSurface(SDL_Surface*& surface, std::string imageName) {	//These need to use pointer references cause SDL
		surface = IMG_Load(imageName.c_str());	//Loads the surface from the file

		if (!surface) {
			std::cout << imageName << " surface failed to load "<< IMG_GetError() << " \n";
			return -1;
		}
		std::cout << "surface loaded \n";
		return 1;
	}

	//Converts a surface to a texture
	int surfaceToTexture(SDL_Surface*& surface, SDL_Texture*& texture) {	//These need to use pointer references cause SDL
		texture = SDL_CreateTextureFromSurface(renderer, surface);

		if (texture == NULL) {
			std::cout << " texture failed to load \n";
			return -1;
		}
		return 1;
	}

	//Loads an Image to a Texture
	int loadImageToTexture(SDL_Texture*& texture, std::string imageName) {	//These need to use pointer references cause SDL
		SDL_Surface* tempSurface = nullptr;
		
		if (loadImageToSurface(tempSurface, imageName)) {
			if (surfaceToTexture(tempSurface, texture)) {
				SDL_FreeSurface(tempSurface);	//So no memory leaks
				return 1;
			}
			SDL_FreeSurface(tempSurface);	//So no memory leaks
			return -2;	//Texture failed to load
		}
		
		return -1;//Surface failed to load
	}

	//renders the texture to a specific part of the screen
	void renderTexture(SDL_Texture* texture, SDL_FRect* renderRect) {
		SDL_RenderCopyF(renderer, texture, NULL, renderRect);	//Renders the image
	}
};




#endif