#pragma once

#ifndef GRAPHIC_RENDERER_H
#define GRAPHIC_RENDERER_H

#include <string>
#include <iostream>

#include "SDL2/SDL.h"
#include "SDL_image.h"





class GraphicRenderer {
private:
	static SDL_Renderer* renderer;	//What renderer we are refering to

public:
	//Renderer
	static void setRenderer(SDL_Renderer* newRenderer) { renderer = newRenderer; }
	static SDL_Renderer* getRenderer() { return renderer; }

	//Returns 1 on success, -1 if image failed to load
	static int loadImageToSurface(SDL_Surface*& surface, std::string imageName) {	//These need to use pointer references cause SDL
		surface = IMG_Load(imageName.c_str());	//Loads the surface from the file

		if (!surface) {
			std::cout << imageName << " surface failed to load "<< IMG_GetError() << " \n";
			return -1;
		}
		std::cout << "surface loaded \n";
		return 1;
	}

	//Converts a surface to a texture
	static int surfaceToTexture(SDL_Surface*& surface, SDL_Texture*& texture) {	//These need to use pointer references cause SDL
		texture = SDL_CreateTextureFromSurface(renderer, surface);

		if (texture == NULL) {
			std::cout << " texture failed to load \n";
			return -1;
		}
		return 1;
	}

	//Loads an Image to a Texture
	static int loadImageToTexture(SDL_Texture*& texture, std::string imageName) {	//These need to use pointer references cause SDL
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
	static void renderTexture(SDL_Texture* texture, SDL_FRect* renderRect) {
		SDL_RenderCopyF(renderer, texture, NULL, renderRect);	//Renders the image
	}

	//Renders the Texture With an Angle
	//	Angle is in degrees!
	static void renderTextureWithAngle(SDL_Texture* texture, SDL_FRect* renderRect, double angle) {
		SDL_RenderCopyExF(renderer, texture, NULL, renderRect, angle, NULL, SDL_FLIP_NONE); 
	}

	//Renders the Texture with an Angle about an Axis
	static void renderTextureWithAngleAroundAxis(SDL_Texture* texture, SDL_FRect* renderRect, double angle, SDL_FPoint* pointOnTexture) {
		SDL_RenderCopyExF(renderer, texture, NULL, renderRect, angle, pointOnTexture, SDL_FLIP_NONE);
	}

	//Renders the Texture with a Flip applied
	static void renderTextureWithFlip(SDL_Texture* texture, SDL_FRect* renderRect, SDL_RendererFlip flipType) {
		SDL_RenderCopyExF(renderer, texture, NULL, renderRect, 0, NULL, flipType);
	}

	//With all commands
	static void renderTextureEx(SDL_Texture* texture, SDL_FRect* renderRect, double angle, SDL_FPoint* pointOnTexture, SDL_RendererFlip flipType) {
		SDL_RenderCopyExF(renderer, texture, NULL, renderRect, angle, pointOnTexture, flipType);
	}

};




#endif