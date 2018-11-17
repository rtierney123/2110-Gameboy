#include "logic.h"
#include "gba.h"
#include "stdio.h"

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
	appState->emptyX = 3;
	appState->emptyY = 3;

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
	/*
		int pX = nextAppState.playerX;
		int pY = nextAppState.playerY;
		
		//test for empty left
		if (nextAppState.emptyX == pX - 1 && nextAppState.emptyY == pY) {
			nextAppState.isMoved = 1;
		}
		//test for empty right
		else if(nextAppState.emptyX == pX + 1 && nextAppState.emptyY == pY) {
			nextAppState.isMoved = 1;
		}
		//test for empty top
		else if (nextAppState.emptyX == pX && nextAppState.emptyY == pY - 1) {
			nextAppState.isMoved = 1;
		}
		//test for empty bottom
		else if (nextAppState.emptyX == pX && nextAppState.emptyY == pY + 1) {
			nextAppState.isMoved = 1;
		} else {
			nextAppState.isMoved = 0;
		}
		*/
		
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
	//nextAppState.emptyX = 3;
	//nextAppState.emptyY = 3;

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
			} 
			
		}
	}
}

tile *findAdjacentEmptyTile(int cX, int cY, tile *gameTiles[4][4]){
	
		//test for empty left
		if (cX > 0 &&  gameTiles[cX-1][cY]->isEmpty == 1) {
			return gameTiles[cX-1][cY];
		}
		//test for empty right
		else if(cX < 3 &&  gameTiles[cX+1][cY]->isEmpty == 1) {
			return gameTiles[cX+1][cY];
		}
		//test for empty top
		else if (cY > 0 && gameTiles[cX][cY-1]->isEmpty == 1) {
			return gameTiles[cX][cY-1];
		}
		//test for empty bottom
		else if (cY < 3 &&  gameTiles[cX][cY+1]->isEmpty == 1) {
			return gameTiles[cX][cY+1];
		} else {
			return NULL;
		}
}

