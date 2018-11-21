#ifndef LOGIC_SEEN
#define LOGIC_SEEN

#include "gba.h"

typedef struct{
	u16 *image;
	int x;
	int y;
	int isEmpty;
} tile;

typedef struct{
	int x;
	int y;
} vector;

typedef struct {
    // Store whether or not the game is over in this member:
    int gameOver;
	tile gameTiles[4][4];
	int imageChosen;
	int playerX;
	int playerY;
	int isMoved;
	vector arrayPositions[4][4];

} AppState;

/*
* TA-TODO: Add any additional structs that you need for your app.
*
* For example, for a Snake game, one could be:
*
* typedef struct {
*   int heading;
*   int length;
*   int x;
*   int y;
* } Snake;
*
*/




//intialize the gameTile map
void initializeTiles(AppState *appState, const u16 *image);

// This function can initialize an unused AppState struct.
void initializeAppState(AppState *appState, const u16* gameImage);

// This function will be used to process app frames.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow);

//return pointer to empty adjacent tile
tile *findAdjacentEmptyTile(int cX, int cY, tile gameTiles[4][4]);

//return shuffled tile array
void shuffleTiles(AppState *state);




int checkForGameOver(AppState *state);
#endif
