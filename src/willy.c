#include "constants.h"

#define WILLY_START_X 42
#define WILLY_START_Y 189
#define MOD_Y_TARGET 5

// Drawing variables
unsigned char willyXpos;
unsigned char willyYpos;
unsigned char willyFrameInc;////increment for bitmap frame
unsigned char willyFrameNo = 1;//current frame no. for animation
char willyFrameStep = 1;//increment of frame
unsigned char willyDirection = RIGHT;//0 - right, 1 - left
unsigned char willyMaxFrames = 4;//max number of frames
unsigned char maxWillies = 2;//number of willies available

unsigned char jump = FALSE, left  = FALSE, right  = FALSE;//control modes
unsigned char control = TRUE ;//control available 
unsigned char falling = FALSE;//is willy falling
unsigned char ascent = FALSE;//is willy on his way up
unsigned char jumpCounter = 0;//counter for his jump
unsigned char jumpIncrement = 1;//increment for counter
unsigned char xOffset, yOffset;
unsigned char maxJump = 20 ;//max height
unsigned char ZeroYMod = FALSE;
unsigned char disintergrate = 0;
unsigned char disintergrateMax = 5;
unsigned char jumpReady;
unsigned char bugfix;// a real shite solution
unsigned char fallCounter;//return value

//chars - use variables as we have multiple char sets
unsigned char BLANK = 0;
unsigned char EDGE = 1;
unsigned char SOLID = 2;
unsigned char STANDARD1 = 3;
unsigned char STANDARD2 = 4;
unsigned char DISINTERGRATE1 = 5;
unsigned char DISINTERGRATE2 = 6;
unsigned char DISINTERGRATE3 = 7;
unsigned char NOTE = 26;

unsigned char LEFT_TRAV_MIN = 8;
unsigned char LEFT_TRAV_MAX = 16;
unsigned char RIGHT_TRAV_MIN = 17;
unsigned char RIGHT_TRAV_MAX = 25;

unsigned char RightFrames[] = {0, 0, 1, 1, 2, 2, 1, 1, 255};
unsigned char LeftFrames[] = {3, 3, 4, 4, 5, 5, 4, 4, 255};

extern unsigned int disintigratorLocations[16];
extern unsigned char disintegratorCounters[16];
extern unsigned char disintegratorCounter;


// Control Variables
void initWilly(void)
{
    willyXpos = WILLY_START_X;
    willyYpos = WILLY_START_Y;

    POKE(SPRITE_ENABLE_REGISTER, 1);  // Shows willy but also hides all other sprites
    
}
void printWilly(void)
{
    POKE(SPRITE_X_0, willyXpos);
    POKE(SPRITE_Y_0, willyYpos);
    POKE(SPRITE_BASE_POINTER, 112 + willyFrameNo);
}

unsigned int joyStickLeft()
{
    unsigned char port2 = PEEK(JOYSTICK_PORT_2);
    return ((port2 & 4) == 0);
}

unsigned int joyStickRight()
{
    unsigned char port2 = PEEK(JOYSTICK_PORT_2);
    return ((port2 & 8) == 0);
}

unsigned int joyStickFire()
{
    unsigned char port2 = PEEK(JOYSTICK_PORT_2);
    return  ((port2 & 16) == 0);
}

unsigned char willyControl(void)
{
    unsigned char i = 0;
    unsigned char modY; // 
    unsigned int rightAvailable = TRUE;
    unsigned int leftAvailable = TRUE;
    
    unsigned int onLeftTravelator = FALSE;
    unsigned int onRightTravelator = FALSE;

    unsigned char topLeftChar, topRightChar, middleLeftChar, middleRightChar, bottomLeftChar, bottomRightChar;
    unsigned char oldXpos, oldYpos;
    unsigned int peekAddress;
    unsigned int temp;

    unsigned char y = (willyYpos - SPRITE_Y_OFFSET + 5) >> 3;
    unsigned char x = (willyXpos - SPRITE_X_OFFSET + 8) >> 3;

    peekAddress = 0x4000 + (y * 40) + x;

    topLeftChar = (PEEK(peekAddress));
    if (topLeftChar == NOTE)
    {
        POKE(peekAddress, 0);
        topLeftChar = BLANK;
    }

    ++peekAddress;
    topRightChar = (PEEK(peekAddress));
    if (topRightChar == NOTE)
    {
        POKE(peekAddress, 0);
        topRightChar = BLANK;
    }

    peekAddress += 39;

    middleLeftChar = (PEEK(peekAddress));
    if (middleLeftChar == NOTE)
    {
        POKE(peekAddress, 0);
        middleLeftChar = BLANK;
    }

    ++peekAddress;
    middleRightChar = (PEEK(peekAddress));
    if (middleRightChar == NOTE)
    {
        POKE(peekAddress, 0);
        middleRightChar = BLANK;
    }
    peekAddress += 39;

    bottomLeftChar = (PEEK(peekAddress));
    if (bottomLeftChar == NOTE)
    {
        POKE(peekAddress, 0);
        bottomLeftChar = BLANK;
    }
    ++peekAddress;
    bottomRightChar = (PEEK(peekAddress));
    if (bottomRightChar == NOTE)
    {
        POKE(peekAddress, 0);
        bottomRightChar = BLANK;
    }

    modY = willyYpos & 7;
    oldXpos = willyXpos;
    oldYpos = willyYpos;

    if (ascent == FALSE && modY == MOD_Y_TARGET) //test for falling
    {
        if ((bottomLeftChar == 0) && (bottomRightChar == 0)) //your not standing on anything
        {
            if (jump == FALSE)
                left = right = FALSE;

            control = FALSE;
            falling = TRUE;
        }
        else
        {
            falling = FALSE;
            control = TRUE;
            jumpIncrement = 2;

            if (fallCounter > 24)
            {
                fallCounter = 0; //reset the counter

                //  THIS NEEDS TO COME BACK!!
                //return DEAD;
                //
            } //Willy is dead
            fallCounter = 0;

            if (bottomRightChar >= DISINTERGRATE1 && bottomRightChar <= DISINTERGRATE3  )
            {
                // theres a disintegrator
                for (i = 0; i < disintegratorCounter; i++)
                {
                    if (disintigratorLocations[i] == peekAddress)
                    {
                        --disintegratorCounters[i];
                        if (disintegratorCounters[i] < DISINTIGRATE_4 )
                        {
                            POKE(peekAddress, 0);
                            disintigratorLocations[i] =0 ;
                            
                        }
                        else if (disintegratorCounters[i] < DISINTIGRATE_3 )
                            POKE(peekAddress, DISINTERGRATE3);
                        else if (disintegratorCounters[i] < DISINTIGRATE_2 )
                            POKE(peekAddress, DISINTERGRATE2);
                    }
                }
            }

            if (bottomLeftChar >= DISINTERGRATE1 && bottomLeftChar <= DISINTERGRATE3  )
            {
                --peekAddress;
                // theres a disintegrator
                for (i = 0; i < disintegratorCounter; i++)
                {
                    if (disintigratorLocations[i] == peekAddress)
                    {
                        --disintegratorCounters[i];
                        if (disintegratorCounters[i] < DISINTIGRATE_4 )
                        {
                            POKE(peekAddress, 0);
                            disintigratorLocations[i] =0 ;
                            
                        }
                        else if (disintegratorCounters[i] < DISINTIGRATE_3 )
                            POKE(peekAddress, DISINTERGRATE3);
                        else if (disintegratorCounters[i] < DISINTIGRATE_2 )
                            POKE(peekAddress, DISINTERGRATE2);
                    }
                }
            }

            if (bottomLeftChar >= DISINTERGRATE1 && bottomLeftChar <= DISINTERGRATE3  )
            {
                // theres a disintegrator
                
            }

            
        }
    }

    if (falling == TRUE && jump == FALSE)
    {
        willyYpos += 1;
        fallCounter += 1;
    }

    if (control) // is willy in a state where player has control
    {
        jump =left = right = FALSE;

        if (joyStickLeft()) // is player selecting to go left
            left = TRUE;

        if (joyStickRight()) //right
            right = TRUE;

        //jumpReady = TRUE;

        if (joyStickFire()) //or trying to jump
        {
            jump = TRUE;     //start jump
            control = FALSE; //give up control
            ascent = TRUE;   //willy will be going up in his jump
            jumpCounter = 0;
        } //intialise the jump

    } //end control

    oldYpos = willyYpos;
    if (jump)
    {
        ++jumpCounter;
        if (jumpCounter <= 18)
        {
            //if (jumpCounter & 1 == 1)
            --willyYpos;
        }      
        else if (jumpCounter >= 24 && jumpCounter <= 42)
        {
            ascent = FALSE;
            //if (jumpCounter & 1 == 1)
            ++willyYpos;
        }
        else if (jumpCounter > 42)
            jump = FALSE;

        // Ensure Willy can land the jump at the top
        if (jumpCounter >= 18)
            ascent = FALSE;
    } //end if jump

    if (ascent == FALSE && modY == MOD_Y_TARGET)
        if (( bottomLeftChar >= RIGHT_TRAV_MIN && bottomLeftChar <= RIGHT_TRAV_MAX) || ( bottomRightChar >= RIGHT_TRAV_MIN && bottomRightChar <= RIGHT_TRAV_MAX))
        {    
            willyXpos += 1;
            onRightTravelator = TRUE;
        }
        else if (( bottomLeftChar >= LEFT_TRAV_MIN && bottomLeftChar <= LEFT_TRAV_MAX) || ( bottomRightChar >= LEFT_TRAV_MIN && bottomRightChar <= LEFT_TRAV_MAX))
        {    
            willyXpos -= 1;
            onLeftTravelator  = TRUE;
        }

    //check for right direction
    if (right)
        willyXpos += 1;

    //Have we gone too far right
    if (willyXpos > oldXpos)
    {
        y = (willyYpos - SPRITE_Y_OFFSET + 5 + jump)  >> 3;
        x = (willyXpos - SPRITE_X_OFFSET + 16)  >> 3;

        peekAddress = 0x4000 + (y * 40) + x;

        topRightChar = (PEEK(peekAddress));
        peekAddress += 40;
        middleRightChar = (PEEK(peekAddress));
        
        y = (willyYpos - SPRITE_Y_OFFSET + 20 + jump)  >> 3;
        peekAddress = 0x4000 + (y * 40) + x;

        bottomRightChar = (PEEK(peekAddress));

        if (topRightChar == 1 || topRightChar == 2 || middleRightChar == 1 || middleRightChar == 2  || bottomRightChar == 1 || bottomRightChar == 2 )
            willyXpos = oldXpos;
    }
    if (right)
    {
        if  (onRightTravelator)
            willyFrameNo = RightFrames[6 + (willyXpos>>1) & 7];
        else    
            willyFrameNo = RightFrames[6 + willyXpos & 7];
    }
        

    //check for left direction
    if (left)
        willyXpos -= 1;

    //Have we gone too far left
    if (willyXpos < oldXpos)
    {
        y = (willyYpos - SPRITE_Y_OFFSET + 5 + jump)  >> 3;
        x = (willyXpos - SPRITE_X_OFFSET + 8)  >> 3;

        peekAddress = 0x4000 + (y * 40) + x;

        topLeftChar = (PEEK(peekAddress));
        peekAddress += 40;
        middleLeftChar = (PEEK(peekAddress));
        peekAddress += 40;
        bottomLeftChar = (PEEK(peekAddress));

        y = (willyYpos - SPRITE_Y_OFFSET + 20 + jump)  >> 3;
        peekAddress = 0x4000 + (y * 40) + x;

        bottomLeftChar = (PEEK(peekAddress));

        if (topLeftChar == 1 || topLeftChar == 2 || middleLeftChar == 1 || middleLeftChar == 2 || bottomLeftChar == 1 || bottomLeftChar == 2 )
            willyXpos = oldXpos;
    }
    if (left)
    {
        if (onLeftTravelator)
            willyFrameNo = LeftFrames[5 + (willyXpos >> 1) & 7];
        else
            willyFrameNo = LeftFrames[5 + willyXpos & 7];
    }
        

    if (willyYpos < oldYpos)
    {
        unsigned char y = (willyYpos - SPRITE_Y_OFFSET + 5) >> 3;
        unsigned char x = (willyXpos - SPRITE_X_OFFSET + 8) >> 3;

        peekAddress = 0x4000 + (y * 40) + x;

        topLeftChar = (PEEK(peekAddress));
        ++peekAddress;
        topRightChar = (PEEK(peekAddress));

        if (topLeftChar == 1 || topLeftChar == 2 || topRightChar == 1 || topRightChar == 2)
            willyYpos = oldYpos;
    }

    // if (willyYpos < oldYpos)
    //     if (topLeftChar == 1 || topLeftChar == 2 || topRightChar == 1 || topRightChar == 2)
    //         willyYpos = oldYpos;
}