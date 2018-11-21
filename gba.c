#include "gba.h"
#include "stdlib.h"
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

void seed_qran(int seed) {
    __qran_seed = seed;
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

void drawFullScreenImageDMA(const u16 *image) {

	DMA[3].src = image;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = ((240*160) | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | DMA_ON);

}

void drawImageDMA(int x, int y, int width, int height, const u16 *image) {
	 for (int r = 0; r < height; r++) {
		DMA[3].src = &image[OFFSET(r, 0, width)];
		DMA[3].dst = &videoBuffer[OFFSET(r + y, x, 240)];
		DMA[3].cnt = width | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | DMA_ON;
     }
  
}

void drawEmptyRect(int x , int y, int width, int height, int thickness, volatile u16 color ) {
	
	//left
	drawRectDMA(x, y, thickness, height, color);

	//right
	if(x + width + thickness >=240){
		drawRectDMA(240 - thickness, y, thickness, height, color);
	} else {
		
		drawRectDMA(x + width, y, thickness, height + thickness, color);
	}
	
	
	//top
	drawRectDMA(x, y, width, thickness, color);

	//bottom
	if (y + height + thickness >= 160){
		drawRectDMA(x, 160 - thickness, width, thickness, color);
	} else {
		drawRectDMA(x, y + height, width, thickness, color);
	}
	
}

void drawGrid(int numVert, int numHorz, int width, int height, int thickness){
	for (int c = 1; c <= numVert; c++){
		drawRectDMA(c*width, 0, thickness, 160, BLACK);
	}

	for (int r = 1; r <= numHorz; r++){
		drawRectDMA(0, r*height, 240, thickness, BLACK);
	}

}


u16 *getPartialImage(int x, int y, int width, int height, int imageW, int imageH, const u16 *original){

	if (width > imageW || height > imageH){
		return 0;
	}
	
	u16* newImage = malloc(width*height*sizeof(u16));
	for (int r = 0; r < height; r++) {
		for(int c = 0; c < width; c++){
			newImage[OFFSET(r, c, width)] = original[OFFSET(r + y, c + x, imageW)];
		}
     }
	 return newImage;
}

u16 *getSingleColorImage(int width, int height, volatile u16 color){
	if (width > 240 || height > 160){
		return 0;
	}
	u16* newImage = malloc(width*height*sizeof(u16));
	for (int r = 0; r < height; r++) {
		for(int c = 0; c < width; c++){
			newImage[OFFSET(r, c, width)] = color;
		}
     }
	 return newImage;

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


