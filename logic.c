#include "logic.h"
#include "gba.h"
#include "stdio.h"
#include "stdlib.h"
#include "images/doggie.h"
#include "images/path.h"

void initializeAppState(AppState* appState, const u16* gameImage) {
    // TA-TODO: Initialize everything that's part of this AppState struct here.
    // Suppose the struct contains random values, make sure everything gets
    // the value it should have when the app begins.
	//Store all image info
	initializeTiles(appState, gameImage);
	shuffleTiles(appState);
	//start player at bottom right
	appState->playerX = 3;
	appState->playerY = 3;
	appState->isMoved = 0;
	
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
	nextAppState.gameOver = 0;

	if (KEY_JUST_PRESSED(BUTTON_A, keysPressedBefore, keysPressedNow)) {
		nextAppState.isMoved = 1;
	
	} else {
		nextAppState.isMoved = 0;
	}
	
	if (KEY_JUST_PRESSED(BUTTON_B, keysPressedBefore, keysPressedNow)) {
		nextAppState.gameOver = 1;
	
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
			vector position;
			position.x = x;
			position.y = y;
			appState->arrayPositions[x][y] = position;
			
		}
	}
}


//return pointer to empty adjacent tile
tile *findAdjacentEmptyTile(AppState *appState, int cX, int cY){
		//test for empty left
		if (cX > 0 &&  appState->gameTiles[cX-1][cY].isEmpty == 1) {
			return &appState->gameTiles[cX-1][cY];
		}
		//test for empty right
		else if(cX < 3 &&  appState->gameTiles[cX+1][cY].isEmpty == 1) {
			return &appState->gameTiles[cX+1][cY];
		}
		//test for empty top
		else if (cY > 0 && appState->gameTiles[cX][cY-1].isEmpty == 1) {
			return &appState->gameTiles[cX][cY-1];
		}
		//test for empty bottom
		else if (cY < 3 &&  appState->gameTiles[cX][cY+1].isEmpty == 1) {
			return &appState->gameTiles[cX][cY+1];
		} else {
			return NULL;
		}
}
//shuffles the tiles around 
void shuffleTiles(AppState *state){
	srand(50 * vBlankCounter);
	int shuffleNum = rand() % 500;
	shuffleNum = shuffleNum + 200;

	while (shuffleNum > 0){
		int testX = rand() % 4;
		int testY = rand() % 4;
		tile* empty = findAdjacentEmptyTile(state, testX, testY);
		if(empty != NULL){

			tile currentTile = state->gameTiles[testX][testY];
			int oldX = currentTile.x;
			int oldY = currentTile.y;
			int newX = empty->x;
			int newY = empty->y;
		
			currentTile.x = newX;
			currentTile.y = newY;
			empty->x = oldX;
			empty->y = oldY;

			tile temp = *empty;
			state->gameTiles[newX][newY] = currentTile;
			state->gameTiles[oldX][oldY] = temp;

			vector tempPosition = state->arrayPositions[oldX][oldY];
			state->arrayPositions[oldX][oldY] =  state->arrayPositions[newX][newY];
			state->arrayPositions[newX][newY] =  tempPosition;
		}
		shuffleNum--;
	}
}
//returns whether the picture is completed
int checkForGameOver(AppState *state) {
	for(int x = 0; x < 4; x++){
		for (int y = 0; y < 4; y++){
			if(state->arrayPositions[x][y].x != x || state->arrayPositions[x][y].y != y) {
				return 0;
			}	
		}
	}
	return 1;
}




