

#include "MusicHelper.h"



float MusicHelper::volume = 0.5f;
std::vector<Mix_Chunk*> MusicHelper::audio;
std::vector<Mix_Music*> MusicHelper::music;


void channelDone(int channel) {
	printf("channel %d finished playback. Deleting the chunk \n", channel);

	Mix_Chunk* doneChunk = Mix_GetChunk(channel);	//Gets the last chunk used on this channel
	Mix_FreeChunk(doneChunk);	//Deleted the chunk
}

void musicDone() {
	//Mix_Music* song;
	//Mix_FreeMusic(song);	//Deletes the Music
}