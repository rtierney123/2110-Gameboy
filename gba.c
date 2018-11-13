#include "gba.h"

volatile unsigned short *videoBuffer = (volatile unsigned short *)0x6000000;
u32 vBlankCounter = 0;

void waitForVBlank(void) {

    // Write a while loop that loops until we're NOT in vBlank anymore:
    // (This prevents counting one VBlank more than once if your app is too fast)
	while(*SCANLINECOUNTER > 160){
	
	}
  
    // Write a while loop that keeps going until we're in vBlank:
	while(*SCANLINECOUNTER < 160){
	
	}

    // Finally, increment the vBlank counter:
	vBlankCounter++;

}

static int __qran_seed= 42;
static int qran(void) {
    __qran_seed= 1664525*__qran_seed+1013904223;
    return (__qran_seed>>16) & 0x7FFF;
}

int randint(int min, int max) {
    return (qran()*(max-min)>>15)+min;
}

void setPixel(int x, int y, u16 color) {
	// check if coord is out of bounds
	int maxX = 240;
	int maxY = 160;
	if (x < 0 || x >= maxX || y < 0 || y >= maxY){
		return;
	}

	//find index to place Pixel
	//1) calculate index of complete rows
	int completeRows = maxX * y;
	//2) Add remaining indexes
	int pixelIndex = completeRows + x;

	videoBuffer[pixelIndex] = color;
 
}

void drawRectDMA(int x, int y, int width, int height, volatile u16 color) {

     for (int r = 0; r < height; r++) {
		DMA[3].src = &color;
		DMA[3].dst = &videoBuffer[OFFSET(r + y, x, 240)];
		DMA[3].cnt = width | DMA_ON | DMA_SOURCE_FIXED;
     }
}

void drawFullScreenImageDMA(u16 *image) {
    // TA-TODO: IMPLEMENT
    UNUSED(image);
}

void drawImageDMA(int x, int y, int width, int height, u16 *image) {
    // TA-TODO: IMPLEMENT
    UNUSED(x);
    UNUSED(y);
    UNUSED(width);
    UNUSED(height);
    UNUSED(image);
}

void fillScreenDMA(volatile u16 color) {
    drawRectDMA(0, 0, 240, 160, color);
}

void drawChar(int col, int row, char ch, u16 color) {
    for(int r = 0; r<8; r++) {
        for(int c=0; c<6; c++) {
            if(fontdata_6x8[OFFSET(r, c, 6) + ch*48]) {
                setPixel(col+c, row+r, color);
            }
        }
    }
}

void drawString(int col, int row, char *str, u16 color) {
    while(*str) {
        drawChar(col, row, *str++, color);
        col += 6;
    }
}

void drawCenteredString(int x, int y, int width, int height, char *str, u16 color) {
    u32 len = 0;
    char *strCpy = str;
    while (*strCpy) {
        len++;
        strCpy++;
    }

    u32 strWidth = 6 * len;
    u32 strHeight = 8;

    int col = x + ((width - strWidth) >> 1);
    int row = y + ((height - strHeight) >> 1);
    drawString(col, row, str, color);
}


