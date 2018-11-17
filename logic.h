#ifndef LOGIC_SEEN
#define LOGIC_SEEN

#include "gba.h"

typedef struct{
	u16 *image;
	int x;
	int y;
	int correctX;
	int correctY;
	int isEmpty;
} tile;

typedef struct {
    // Store whether or not the game is over in this member:
    int gameOver;
	tile gameTiles[4][4];
	u16 *gameImage;
	int playerX;
	int playerY;

	int isMoved;
    /*
    * TA-TODO: Add any logical elements you need to keep track of in your app.
    *
    * For example, for a Snake game, those could be:
    *
    * Snake snake;
    * Food foods[10];
    * int points;
    *
    */

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
void shuffleTiles(tile gameTiles[4][4]);
#endif
