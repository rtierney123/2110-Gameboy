#include "graphics.h"
#include "gba.h"
#include "stdio.h"
// TA-TODO: Include any header files for images generated by nin10kit.
// Example for the provided garbage image:
//#include "images/garbage.h"

// TA-TODO: Add any draw/undraw functions for sub-elements of your app here.
// For example, for a snake game, you could have a drawSnake function
// or a drawFood function
//
// e.g.:
// static void drawSnake(Snake* snake);
// static void drawFood(Food* food);

// This function will be used to draw everything about the app
// including the background and whatnot.
void fullDrawAppState(AppState *state) {
	drawAllTiles(state);
	drawGrid(3, 3, 60, 40, 4);
	drawRectDMA(0, 0, 4, 160, BLACK);
	drawRectDMA(236, 0, 4, 160, BLACK);
	drawPlayer(state->playerX, state->playerY);

	
}

// This function will be used to undraw (i.e. erase) things that might
// move in a frame. E.g. in a Snake game, erase the Snake, the food & the score.
void undrawAppState(AppState *state) {
	drawGrid(3, 3, 60, 40, 4);
	drawEmptyRect(60 * (state->playerX), 40 * (state->playerY), 60, 40, 4, BLACK);
}

// This function will be used to draw things that might have moved in a frame.
// For example, in a Snake game, draw the snake, the food, the score.
void drawAppState(AppState *state) {
	drawPlayer(state->playerX, state->playerY);
	if (state->isMoved == 1){
		int pX = state->playerX;
		int pY = state->playerY;
		tile* empty = findAdjacentEmptyTile(pX, pY, state->gameTiles);

		if(empty != NULL){

			tile currentTile = state->gameTiles[pX][pY];
			int oldX = currentTile.x;
			int oldY = currentTile.y;
			int newX = empty->x;
			int newY = empty->y;
		
			drawImageDMA(60*newX, 40*newY, 60, 40, currentTile.image);
			drawRectDMA(60*oldX, 40*oldY, 60, 40, BLACK);
			currentTile.x = newX;
			currentTile.y = newY;
			empty->x = oldX;
			empty->y = oldY;
			tile temp = *empty;
			state->gameTiles[newX][newY] = currentTile;
			state->gameTiles[oldX][oldY] = temp;
		}
	
	}
	
}

//undraw player and updates player position
void drawPlayer(int pX, int pY){
	drawEmptyRect(60 * pX, 40 * pY, 60, 40, 4, WHITE);
}

void drawAllTiles(AppState *appState){
	for(int x = 0; x < 4; x++){
		for (int y = 0; y < 4; y++){
			u16 *image = appState->gameTiles[x][y].image;
			drawImageDMA(60*x, 40*y, 60, 40, image);
		}
	}
}





