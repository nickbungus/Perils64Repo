#include "constants.h"

#define WILLY_START_X 42
#define WILLY_START_Y 189

unsigned char willyXpos;
unsigned char willyYpos;
unsigned char willyFrameInc;////increment for bitmap frame
unsigned char willyFrameNo;//current frame no. for animation
unsigned char willyFrameStep;//increment of frame
unsigned char willyDirection;//0 - right, 1 - left
unsigned char willyMaxFrames;//max number of frames
unsigned char maxWillies;//number of willies available

void initWilly(void)
{
    POKE(SPRITE_ENABLE_REGISTER, 1);
    POKE(SPRITE_X_0, WILLY_START_X);
    POKE(SPRITE_Y_0, WILLY_START_Y);
    POKE(SPRITE_BASE_POINTER, 112);
}
void printWilly(void)
{


}