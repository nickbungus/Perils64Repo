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

unsigned char RightFrames[] = {0, 0, 1, 1, 2, 2, 1, 1, 255};
unsigned char LeftFrames[] = {3, 3, 4, 4, 5, 5, 4, 4, 255};
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
    unsigned modY; // 
    unsigned int rightAvailable = TRUE;
    unsigned int leftAvailable = TRUE;
    
    unsigned char topLeftChar, topRightChar, middleLeftChar, middleRightChar, bottomLeftChar, bottomRightChar;
    unsigned int peekAddress;

    unsigned char y = (willyYpos - SPRITE_Y_OFFSET + 5) / 8;
    unsigned char x = (willyXpos - SPRITE_X_OFFSET + 8) / 8;

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

    modY = willyYpos & MOD_Y_TARGET;
    // Windows version checked the mod to only test for falling every 8 Y Pos using the modY - removed for now - might be needed for speed later
    if ( ascent == FALSE  && modY == MOD_Y_TARGET)//test for falling
    {
	    if  ( ( bottomLeftChar == 0) && ( bottomRightChar == 0) )//your not standing on anything
        {
			if (jump==FALSE)	
				left = right = FALSE;
			
			control = FALSE;				
            falling = TRUE;
				
        }
		else
        {	
            falling  = FALSE;
			control = TRUE;
            jumpIncrement = 2;
                
            if (fallCounter > 24)
            {
                fallCounter = 0;//reset the counter


                //  THIS NEEDS TO COME BACK!!
				//return DEAD;
                //  


            }//Willy is dead

            fallCounter = 0;
        }
    }

	if (falling == TRUE && jump == FALSE)
	{	
        willyYpos += 1;
        fallCounter += 1;
	}

    if (control) // is willy in a state where player has control
    {
        //look for a change of direction and make sure willy
        //   sprite frame is true to original
        if ((joyStickLeft() && willyDirection == RIGHT) || (joyStickRight() && willyDirection == LEFT))
        {
            
            if (willyFrameNo <= 0)
                willyFrameNo = 1;

            if (willyFrameNo >= 4)
                willyFrameNo = 1;
        }

        left = right = jump = 0; //reset the modes

        if (joyStickLeft()) // is player selecting to go left
            left = TRUE;

        if (joyStickRight()) //right
            right = TRUE;     

        
        /////////////Jump Not Available
        jumpReady = TRUE;
        
        //if ((l[lev][getX][getY - 3] == 1) || (l[lev][getX][getY - 3] == 2) || (l[lev][getX - 1][getY - 3] == 1) || (l[lev][getX - 1][getY - 3] == 2) && modY == 0)
        if ((modY == 5) && (topLeftChar == 1 || topLeftChar == 2 || topRightChar == 1 || topRightChar == 2))        
            jumpReady = FALSE;

        // //but what if i wanna jump left and I`m hemmed in
        // if (topRightChar >= 1 && l[lev][getX][getY - 3] <= 2 && l[lev][getX - 1][getY - 3] != 2 && left)
        //     jumpReady = TRUE;

        // //and what if a wanna go right
        // if (l[lev][getX - 1][getY - 3] >= 1 && l[lev][getX - 1][getY - 3] <= 2 && l[lev][getX][getY - 3] != 2 && right)
        //     jumpReady = TRUE;

        if (joyStickFire() && jumpReady) //or trying to jump
        {
            jump = TRUE;     //start jump
            control = FALSE; //give up control
            ascent = TRUE;   //willy will be going up in his jump
            jumpCounter = 0;
        } //intialise the jump

    } //end control

    if (jump)
    {   
        ++jumpCounter;
        if (jumpCounter <= 18)
        {
            //if (jumpCounter & 1 == 1)
                --willyYpos;
        }
        else if(jumpCounter >=24 && jumpCounter <= 42)
        {
            ascent = FALSE;
            //if (jumpCounter & 1 == 1)
                ++willyYpos;
        }
        else if (jumpCounter > 42)
            jump = FALSE;
        


        // if (jumpCounter < maxJump - 1)
        //     willyYpos-= jumpIncrement * 2;//move the y

        // jumpCounter += jumpIncrement;//inc the counter
	

        // if (jumpCounter>=maxJump)//do the descent if willy has climaxed
        // {
        //     jumpIncrement = -jumpIncrement;
        //     jumpCounter=maxJump;
        //     ascent = FALSE;
        // }

        // //check for the jump being completed
        // if (jumpCounter < 0)
        // {
        //     //	control = true;
        //     jump = FALSE;
        //     jumpIncrement =2;
        // }

        // //check for ceiling block - first 1
        // // if (ascent && (l[lev][getX][getY-2] ==1) || (l[lev][getX][getY-2] ==2) )
        // // {	
        // //     willyYpos+=jumpIncrement * 4;
        // // }

        // // if (ascent && (l[lev][getX-1][getY-2] ==1) || 	(l[lev][getX-1][getY-2] ==2) )
        // // {	
        // //     willyYpos+=jumpIncrement * 4;
        // // }	
    }               //end if jump

    //     //+2 is standing on the level
    //     ZeroYMod = ( (willyYpos +16 - yOffset)%16 == 0 ? 0 : 1);

    //check for willies body hitting right side walls
       

    if (topRightChar == 1 || topRightChar == 2 || middleRightChar == 1 || middleRightChar == 2)//changed from 0 
	    rightAvailable = FALSE;

    //     //fix the right bug
    //     if (l[lev][getX][getY-1] ==1 || l[lev][getX][getY-1] ==2)
	//         rightAvailable = FALSE;

    //     //fix the right bug
    //     if (l[lev][getX][getY-2] ==1 || l[lev][getX][getY-2] ==2)
	//         rightAvailable = FALSE;


    //     //////Fix the "Julie Bug"
    //     if (modX ==12 && rightAvailable == false && right && l[lev][getX][getY]==0 && jump==false && falling==false)
    //     {	
    //         willyYPos +=4;
	//         falling = TRUE;
    //         control = FALSE;
    //         willyDirection= 0;
	//         willyFrameNo=2;
    //         right=false;
    //         willyXpos+=4;
	//     }
	
    //     //do the travaltor business
    //     if (rightAvailable && jump==FALSE)
    //     {
    //         if  ( ( l[lev][getX][getY] >= TRAV_RIGHT_MIN) && ( l[lev][getX][getY] <= TRAV_RIGHT_MAX) )
	// 	        willyXpos+=4;
    //         else if  ( ( l[lev][getX-1][getY] >= TRAV_RIGHT_MIN) && ( l[lev][getX-1][getY] <= TRAV_RIGHT_MAX) )
	// 	        willyXpos+=4;
    //     }

    //     //make sure willy is in the correct place
    //     if (willyXpos>456) 
    //         willyXpos = 456;

    if(right) 
        willyDirection = RIGHT;

    //check for right direction
    if (right && rightAvailable)
    {
        //move the guy
        willyXpos += 1;

        ++willyFrameStep;

        willyFrameNo = RightFrames[willyFrameStep];

        if (willyFrameNo == 255)
        {
            willyFrameStep = 0;
            willyFrameNo = 0;
        }
        // if (willyFrameNo >=2)
        //     willyFrameStep = - willyFrameStep;

        // willyFrameNo += willyFrameStep;

        // if (willyFrameNo ==0)
        // {
        //     willyFrameStep = -willyFrameStep;  
        //     willyFrameNo = 0;
        // }
    }

    //     ///////////////////////////////////
    //     //check for willies body hitting left side walls
    //     unsigned char leftAvailable = TRUE;
    if (topLeftChar == 1 || topLeftChar == 2 || middleLeftChar == 1 || middleLeftChar == 2)//changed from 0 
        leftAvailable = FALSE;
    //     if ( (l[lev][getX-2][getY - 1+ ZeroYMod]==1 || l[lev][getX-2][getY - 1 + ZeroYMod]==2 ) && 	(willyXpos+16 - xOffset)%16==0) 
	//         leftAvailable = FALSE;

    //     if ( (l[lev][getX-2][getY - 2  + ZeroYMod]==1 || l[lev][getX-2][getY - 2+ ZeroYMod]==2 ) &&  (willyXpos+16 - xOffset)%16==0) 
	//         leftAvailable = FALSE;

    //     //left bug fix
    //     if (l[lev][getX-1][getY-1]==1 ||l[lev][getX-1][getY-1]==2)
	//         leftAvailable= FALSE;

    //     if (l[lev][getX-1][getY-2]==1 ||l[lev][getX-1][getY-2]==2)
	//         leftAvailable= FALSE;


    //////Fix the "Julie Bug"
    // if (modX ==0 && leftAvailable == FALSE && left && l[lev][getX-1][getY]==0 && jump==FALSE && falling==FALSE)
    // {
    //     willyXpos+=4;
    //     falling = TRUE;
    //     willyDirection= 1;
    //     control = FALSE;
    //     willyFrameNo=2;
    //     left=FALSE;
    //     willyXpos-=4;
    //     }

	//     //do the travaltor business
    //     if (leftAvailable && jump==FALSE)//note the 16 +
    //     {
    //         if  ( ( l[lev][getX][getY] >= TRAV_LEFT_MIN) && ( l[lev][getX][getY] <= TRAV_LEFT_MAX) || ( l[lev][getX-1][getY] >= TRAV_LEFT_MIN)	&& ( l[lev][getX-1][getY] <= TRAV_LEFT_MAX) )//travelator
    //         {
    //             willyXpos-=4;//if so move him

    //             if (right && rightAvailable == FALSE)
    //             {
    //                 //if he`s trying to go right and he can`t move him back
    //                 willyXpos+=4;}
    //             }

    //             //make sue willy is in the correct place
    //             if (willyXpos<156) 
    //                 willyXpos = 156;

    //             if (left) 
    //                 willyDirection = 1;

    //check for left direction 
    if (left && leftAvailable)
    {	
        willyXpos-=1;
	
        ++willyFrameStep;

        willyFrameNo = LeftFrames[willyFrameStep];

        if (willyFrameNo == 255)
        {
            willyFrameStep = 0;
            willyFrameNo = 3;
        }

    }

    //             //check for disintergrating blocks
    //             if ( ( l[lev][getX][getY] >= DISINTERGRATE_MIN )  && ( l[lev][getX][getY] <= DISINTERGRATE_MAX)  && modY  == 0 )//is he standing on a disintergrator
	//                 l[lev][getX][getY]--;//if so register it with level object

	
    //             //check for disintergrating blocksII
    //             if ( ( l[lev][getX-1][getY] >= DISINTERGRATE_MIN )  && 	( l[lev][getX-1][getY] <= DISINTERGRATE_MAX)  && modY  == 0 )//is he standing on a disintergrator
	//                 l[lev][getX-1][getY]--;//if so register it with level object
	
    //         //printWilly(source, dest);


    //         ////This bit fixes a nasty bug it fixes the symptoms but not the cause....NASTY!!!
    //         if (modY==12)
	//             bugfix++;
	//         else 
    //             bugfix = 0;
                
    //         if (bugfix>4)
    //             wllyYpos+=4;

    //         return ALIVE;//willy lives
    //     }
}