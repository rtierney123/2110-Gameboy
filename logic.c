#include "logic.h"
#include "gba.h"
#include "stdio.h"
#include "stdlib.h"

void initializeAppState(AppState* appState, const u16* gameImage) {
    // TA-TODO: Initialize everything that's part of this AppState struct here.
    // Suppose the struct contains random values, make sure everything gets
    // the value it should have when the app begins.
	//Store all image info
	initializeTiles(appState, gameImage);
	//start player at bottom right
	appState->playerX = 3;
	appState->playerY = 3;
	appState->isMoved = 0;
	shuffleTiles(appState->gameTiles);

}

// TA-TODO: Add any process functions for sub-elements of your app here.
// For example, for a snake game, you could have a processSnake function
// or a createRandomFood function or a processFoods function.
//
// e.g.:
// static Snake processSnake(Snake* currentSnake);
// static void generateRandomFoods(AppState* currentAppState, AppState* nextAppState);

// This function processes your current app state and returns the new (i.e. next)
// state of your application.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow) {
    /* TA-TODO: Do all of your app processing here. This function gets called
     * every frame.
     *
     * To check for key presses, use the KEY_JUST_PRESSED macro for cases where
     * you want to detect each key press once, or the KEY_DOWN macro for checking
     * if a button is still down.
     *
     * To count time, suppose that the GameBoy runs at a fixed FPS (60fps) and
     * that VBlank is processed once per frame. Use the vBlankCounter variable
     * and the modulus % operator to do things once every (n) frames. Note that
     * you want to process button every frame regardless (otherwise you will
     * miss inputs.)
     *
     * Do not do any drawing here.
     *
     * TA-TODO: VERY IMPORTANT! READ THIS PART.
     * You need to perform all calculations on the currentAppState passed to you,
     * and perform all state updates on the nextAppState state which we define below
     * and return at the end of the function. YOU SHOULD NOT MODIFY THE CURRENTSTATE.
     * Modifying the currentAppState will mean the undraw function will not be able
     * to undraw it later.
     */

    AppState nextAppState = *currentAppState;

	

	if (KEY_JUST_PRESSED(BUTTON_A, keysPressedBefore, keysPressedNow)) {
		nextAppState.isMoved = 1;
	
	} else {
		nextAppState.isMoved = 0;
	}

	//register player keys
	if (KEY_JUST_PRESSED(BUTTON_UP, keysPressedBefore, keysPressedNow)) {
		if (nextAppState.playerY > 0){
			nextAppState.playerY--;
		}
	}
	if (KEY_JUST_PRESSED(BUTTON_DOWN, keysPressedBefore, keysPressedNow)) {
		if (nextAppState.playerY < 3){
			nextAppState.playerY++;
		}
	}
	if (KEY_JUST_PRESSED(BUTTON_RIGHT, keysPressedBefore, keysPressedNow)) {
		if (nextAppState.playerX < 3){
			nextAppState.playerX++;
		}
	}
	if (KEY_JUST_PRESSED(BUTTON_LEFT, keysPressedBefore, keysPressedNow)) {
		if (nextAppState.playerX > 0){
			nextAppState.playerX--;
		}
	}


    return nextAppState;
}

//generate and store all the tiles, in the correct order
void initializeTiles(AppState *appState, const u16 *image){

	for(int x = 0; x < 4; x++){
		for (int y = 0; y < 4; y++){
			if (x != 3 || y != 3){
				appState->gameTiles[x][y].image = getPartialImage(x*60, y*40, 60, 40, 240, 160, image);
				appState->gameTiles[x][y].x = x;
				appState->gameTiles[x][y].y = y;
				appState->gameTiles[x][y].isEmpty = 0;
			}  else {
				appState->gameTiles[x][y].image = getSingleColorImage(60, 40, BLACK);
				appState->gameTiles[x][y].x = x;
				appState->gameTiles[x][y].y = y;
				appState->gameTiles[x][y].isEmpty = 1;
			}
			
		}
	}
}

//return pointer to empty adjacent tile
tile *findAdjacentEmptyTile(int cX, int cY, tile gameTiles[4][4]){
	
		//test for empty left
		if (cX > 0 &&  gameTiles[cX-1][cY].isEmpty == 1) {
			return &gameTiles[cX-1][cY];
		}
		//test for empty right
		else if(cX < 3 &&  gameTiles[cX+1][cY].isEmpty == 1) {
			return &gameTiles[cX+1][cY];
		}
		//test for empty top
		else if (cY > 0 && gameTiles[cX][cY-1].isEmpty == 1) {
			return &gameTiles[cX][cY-1];
		}
		//test for empty bottom
		else if (cY < 3 &&  gameTiles[cX][cY+1].isEmpty == 1) {
			return &gameTiles[cX][cY+1];
		} else {
			return NULL;
		}
}

//return shuffled tile array
void shuffleTiles(tile gameTiles[4][4]){
	int shuffleNum = rand() % 20;
	shuffleNum = shuffleNum + 5;
	
	int emptyX = 3;
	int emptyY = 3;
	while (shuffleNum > 0){

		int chooseX = rand() % 2;
		if (chooseX == 0){
			chooseX = -1;
		}
		int chooseY = rand() % 2;
		if (chooseY == 0){
			chooseY = -1;
		}

		if ((emptyX + chooseX <= 3 && emptyX + chooseX >= 0) &&
				(emptyY + chooseY <= 3 && emptyY + chooseY >= 0)){
			tile empty = gameTiles[emptyX][emptyY];
			empty.x = emptyX + chooseX;
			empty.y = emptyY + chooseY;
			tile swapTile = gameTiles[emptyX + chooseX][emptyY + chooseY];
			swapTile.x = emptyX;
			swapTile.y = emptyY;
			gameTiles[emptyX + chooseX][emptyY + chooseY] = empty;
			gameTiles[emptyX][emptyY] = swapTile;
		}
		shuffleNum--;
	}
}

