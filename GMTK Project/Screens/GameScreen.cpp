

#include "GameScreen.h"
#include "Music/MusicHelper/MusicHelper.h"
#include "MouseController/MouseController.h"

void print() {
	std::cout << "THE GAME SCREEN BUTTON";
}
//Loads the Title Screen
void loadGameScreen() {
	InteractManager::removeAllInteractables();
	resetGame();

	runGame = true;
	displayGame = true;
}
void loadGameScreenRandom() {
	InteractManager::removeAllInteractables();
	resetGame();


	randomGame = true;
	runGame = true;
	displayGame = true;
}
void lostGame() {
	pauseGame();
	levelLoaded = true;
	MouseController::setHeldPiece(nullptr);
	
	
	MusicHelper::loadAndPlayChunk("Music/GameOver.wav", -1);


}

void resetGame() {
	levelLoaded = true;
	InteractManager::removeAllInteractables();
	ChessBoard::resetBoard();
	MouseController::resetMouseController();

	//Chess Board 
	SDL_FRect* chessBoardArea = new SDL_FRect();
	chessBoardArea->x = -0.3;
	chessBoardArea->y = 0.25;
	chessBoardArea->w = 2;
	chessBoardArea->h = 0.5;
	ChessBoard* myBoard = new ChessBoard(24, 6, chessBoardArea);

	Piece* myPiece = new Piece(Knight, 6, 2, "Images/Knight.png", nullptr);
	Piece* myPiece2 = new Piece(Bishop, 9, 2, "Images/Bishop.png", nullptr);
	MouseController::addPiece(myPiece);
	MouseController::addPiece(myPiece2);

	SDL_FRect* scoreBoardArea = new SDL_FRect();
	scoreBoardArea->x = 0.3;
	scoreBoardArea->y = 0;
	scoreBoardArea->w = 0.4;
	scoreBoardArea->h = 0.25;
	ScoreInteractable* ScoreBoard = new ScoreInteractable("invis", nullptr, scoreBoardArea);
	InteractManager::addInteractable(ScoreBoard);

	SDL_FRect* lostRect = new SDL_FRect();
	lostRect->x = 0;
	lostRect->y = 0.75;
	lostRect->w = 0.5;
	lostRect->h = 0.25;

	TextureInteractable* resetButton = new TextureInteractable("Images/Start_1.png", resetGame, lostRect);
	InteractManager::addInteractable(resetButton);


	SDL_FRect* backMenu = new SDL_FRect();
	backMenu->x = 0.5;
	backMenu->y = 0.75;
	backMenu->w = 0.5;
	backMenu->h = 0.25;

	TextureInteractable* backToTitleButton = new TextureInteractable("Images/BackToMenu.png", loadTitleScreen, backMenu);
	InteractManager::addInteractable(backToTitleButton);

	unPauseGame();
}

void pauseGame() {
	runGame = false;
}

void unPauseGame() {
	runGame = true;
}