#pragma once

#ifndef MUSIC_HELPER_H
#define MUSIC_HELPER_H

#include <string>
#include <vector>
#include <iostream>

#include "SDL_mixer.h"	//Loading Audio


class MusicHelper {
private:
	static float volume;
	static std::vector<Mix_Chunk*> audio;
	static std::vector<Mix_Music*> music;

public:
	
	//Loads and Plays Audio in channels
	// -1 for first available channel
	static int loadAndPlayChunk(std::string file, int channel) {
		Mix_Chunk* song = Mix_LoadWAV(file.c_str());

		if (!song) 
			printf((file + " had an error loading %s \n").c_str(), Mix_GetError());

		if (Mix_PlayChannel(channel, song, 0) == -1) {
			printf("Mix_PlayChannel: %s\n", Mix_GetError());
			return -1;	//Returns -1 on failure
		}

		//Adds the audio to the list
		audio.push_back(song);
		return 1;	//Returns 1 on success
	}

	//Loads and plays music
	static int loadAndPlayMusic(std::string file) {
		Mix_Music* song = Mix_LoadMUS(file.c_str());
		
		if (getMusicPlaying()) {
			deleteMusic();
		}

		if (!song)
			printf((file + " had an error loading %s \n").c_str(), Mix_GetError());

		if (Mix_PlayMusic(song, -1) == -1) {
			printf("Mix_PlayMusic: %s\n", Mix_GetError());
			return -1;	//Returns -1 on failure
		}
		music.push_back(song);
		return 1;	//Returns 1 on success
	}

	//Return true if already playing music
	static bool getMusicPlaying() { return Mix_PlayingMusic(); }

	static void deleteMusic() { 
		std::cout << "Deleting song \n";
		Mix_Music* song = music.at(0);
		music.erase(music.begin()); 
		Mix_FreeMusic(song);
	}

	//Sets the volume for all the music chunks
	static void setVolume(float percent) { 
		Mix_VolumeMusic(MIX_MAX_VOLUME * percent);
		Mix_Volume(-1, MIX_MAX_VOLUME * percent); 
	}

	static Mix_Music* getLastMusic() {
		return music.at(0);
	}

};

//Callback function (called when channel is finished playing
void channelDone(int channel);

void musicDone();

#endif