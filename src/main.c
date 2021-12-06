
#include "screen.h"
#include "willy.h"

unsigned char main(void) 
{
    
    initScreen();
    drawLogo();

    initWilly();
    while (1)
    {
        while(PEEK(RASTER_REGISTER) != 250 )
        {
            
        }
        POKE(53280, 1);
        animateCurrentScreen();
        willyControl();
        printWilly();
        POKE(53280, 0);
    }
    return 0;
}
