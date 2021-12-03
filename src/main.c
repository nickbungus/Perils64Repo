
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
        animateCurrentScreen();
        willyControl();
        printWilly();
    }
    return 0;
}
