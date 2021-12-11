
#include "screen.h"
#include "willy.h"

extern void initInterrupt(void);
extern void sidTune(void);
extern void initTime(void);
extern void updateTime(void);
unsigned char main(void) 
{
    sidTune();
    initInterrupt();
    initScreen();
    initTime();
    drawLogo();

    initWilly();
    while (1)
    {
        while(PEEK(RASTER_REGISTER) != 250 )
        {
            
        }
        POKE(53280, 1);
        animateCurrentScreen();
        updateTime();
        willyControl();
        printWilly();
        POKE(53280, 0);
    }
    return 0;
}
