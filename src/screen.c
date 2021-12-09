#include "constants.h"

extern unsigned char* charAttributes;
extern void animamteNotes(void);
extern void animateTravelators(void);

unsigned char temp = 0;
unsigned char i = 0;
unsigned int currentScreenLoc;
unsigned int travelatorLocations[64];
unsigned char travelatorChars[64];

unsigned int disintigratorLocations[16];
unsigned char disintegratorCounters[16];
unsigned char disintegratorCounter = 0;

unsigned int noteColourLocations[8];
unsigned char noteAvailable[64];
unsigned char noteCounter = 0;

unsigned char travelatorCount = 0;
unsigned char travelatorFrame = 0;
unsigned char charRead = 0; 
unsigned char screenAnimCounter = 0;

unsigned char level1[] = {1,23 ,0,3 ,26,1 ,0,4 ,26,1 ,0,6 ,26,1 ,0,4 ,1,2 ,0,20 ,1,2 ,0,2 ,2,15 ,5,2 ,3,1 ,1,2 ,0,15 ,26,1 ,0,4 ,1,2 ,5,1 ,0,19 ,1,2 ,26,1 ,0,9 ,8,1 ,11,8 ,14,1 ,1,2 ,4,1 ,0,19 ,1,2 ,0,12 ,26,1 ,0,5 ,4,2 ,1,2 ,3,11 ,0,9 ,1,2 ,26,1 ,0,17 ,4,2 ,1,2 ,0,20 ,1,2 ,20,4 ,23,1 ,0,3 ,4,10 ,0,2 ,1,2 ,0,20 ,1,2 ,3,1 ,0,19 ,1,2 ,0,19 ,26,1 ,1,2 ,4,3 ,0,5 ,4,7 ,0,5 ,1,2 ,0,19 ,4,1 ,1,2 ,4,2 ,0,18 ,1,2 ,0,16 ,3,4 ,1,23,255};
unsigned char level2[] = {1,23 ,0,9 ,26,1 ,0,10 ,1,2 ,0,20 ,1,2 ,0,20 ,1,2 ,0,20 ,1,2 ,0,15 ,26,1 ,0,4 ,1,2 ,0,6 ,8,1 ,11,6 ,14,1 ,4,1 ,0,5 ,1,2 ,0,9 ,26,1 ,0,10 ,1,2 ,0,2 ,3,3 ,0,15 ,1,2 ,0,19 ,26,1 ,1,2 ,3,1 ,0,6 ,4,6 ,0,7 ,1,2 ,26,1 ,0,19 ,1,2 ,0,13 ,8,1 ,11,3 ,14,1 ,0,2 ,1,2 ,0,20 ,1,2 ,3,4 ,0,15 ,3,1 ,1,2 ,0,20 ,1,2 ,26,1 ,0,3 ,3,9 ,0,6 ,3,1 ,1,2 ,0,9 ,2,1 ,0,10 ,1,2 ,3,1 ,0,8 ,2,1 ,0,3 ,3,4 ,0,3 ,1,2 ,0,9 ,2,1 ,26,1 ,0,9 ,1,23,255};

unsigned char* levels[] = {level1, level2};
unsigned char levelIndex = 1;

unsigned char hiScore[] = {61, 36, 34, 35, 0, 72, 30, 42, 45, 32};

unsigned char logo[] = { 0xF8,0xF7,0x9D,0x9E,0x9F,0xA0,0xF8,0xF8,0xF8,0xF8,0xF8,0xA1,0xA2,0xA3,0xA4,0xA5,
 0xA6,0xF8,0xF8,0xF8,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xF8,0xF8,0xAF,0xB0,
 0xB1,0xB2,0xB3,0xF8,0xB4,0xB5,0xF8,0xF8,0xB6,0xF8,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,
 0xBD,0xF8,0xF8,0xF8,0xBE,0xBF,0xC0,0xC1,0xC2,0xC3,0xC4,0xF8,0xF8,0xF8,0xC5,0xC6,
 0xC7,0xC8,0xC9,0xCA,0xCB,0xF8,0xF8,0xF8,0xF8,0xCC,0xCD,0xCE,0xCF,0xF8,0xF8,0xF8,
 0xF8,0xF8,0xF8,0xD0,0xD1,0xD2,0xD3,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xD4,0xD5,0xD6,
 0xD7,0xF8,0xF8,0xF8,0xF8,0xF8,0xD8,0xD9,0xDA,0xDB,0xF8,0xF8,0xF8,0xF8,0xF8,0xDC,
 0xDD,0xDE,0xDF,0xE0,0xE1,0xE2,0xF8,0xF8,0xF8,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,
 0xEA,0xF8,0xF8,0xEB,0xEC,0xED,0xEE,0xEF,0xF0,0xF1,0xF2,0xF8,0xF8,0xF3,0xF4,0xF8,
 0xF8,0xF8,0xF5,0xF6,0x9C,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0x9B,0xF8};
    unsigned char x = 0, y = 0;

    unsigned char sharedDrawingIndex = 0;
    unsigned int lineAddress = 0x4000;
    unsigned int colourAddress = COLOUR_RAM;

    unsigned char xIndex = 0;
    
    unsigned char charCount = 0;
    unsigned char* ptr = &charAttributes ;
    unsigned char* logoData = 0;// &logo_data;

void initScreen(void)
{    
    unsigned char *currentLevelChar = levels[levelIndex];// levels+levelIndex;
    
    for (i = 0; i < 64;++i)
    {
        travelatorChars[i]= 0;    
        travelatorLocations[i] = 0;
    }   

    for (i = 0; i < 16;++i)
    {
        disintegratorCounters[i]= 0;    
        disintigratorLocations[i] = 0;
    }   

    travelatorCount = 0;

    POKE(53280, 0);
    POKE(53281, 0);

    POKE(DIRECTIONAL_REGISTER,PEEK(DIRECTIONAL_REGISTER) | 3);
    POKE(DATA_REGISTER, (PEEK(DATA_REGISTER) & 252) | 2);

    POKE(GRAPHICS_POINTER, 2);

    //Tell the Kernal where to new screen
    POKE(648, 0x40);

    //SET UP Multi Colour Mode
    POKE(COLOUR_MODE_REGISTER, PEEK(COLOUR_MODE_REGISTER) | 16 );
    
    POKE(MULTI_COLOUR1, DARK_GREY);
    POKE(MULTI_COLOUR2, ORANGE);

    charRead = *(currentLevelChar); 
    
    currentLevelChar++;
    
    while (charRead != 255)
    {           
        charCount = *(currentLevelChar);
        currentLevelChar++;
        for (i = 0 ; i < charCount ; i++)
        {
            currentScreenLoc = lineAddress + xIndex;
            if (charRead >= 8 && charRead <=25)
            {    
                travelatorLocations[travelatorCount] = currentScreenLoc;
                travelatorChars[travelatorCount++] = charRead;
            }
            else if (charRead == 26)
            {
                noteAvailable[noteCounter] = 1;
                noteColourLocations[noteCounter++] = colourAddress + xIndex;
            }
            else if (charRead == 5)
            {
                disintigratorLocations[disintegratorCounter] = currentScreenLoc;
                disintegratorCounters[disintegratorCounter++] = DISINTIGRATE_1;
            }

            POKE(currentScreenLoc, charRead);
            POKE(colourAddress + xIndex, *(ptr+charRead));
            ++xIndex;

            if (xIndex == 22)
            {
                xIndex = 0;
                lineAddress = lineAddress + 40;
                colourAddress = colourAddress + 40;
            }
        }

        charRead = *(currentLevelChar);
        currentLevelChar++;
    }
}

void drawLogo(void)
{
    colourAddress = 55376+64;
    lineAddress = 0x4050 + 64;
    xIndex = 0;
    sharedDrawingIndex = 0;
    for (y = 0; y < 16; y++)
    {
          for (x = 0; x < 10; x++) 
          {
              charRead = logo[sharedDrawingIndex];
              POKE(lineAddress + xIndex, charRead);
              POKE(colourAddress + xIndex, *(ptr+charRead));
              sharedDrawingIndex++;
              xIndex++;
          }
          lineAddress = lineAddress + 40;
          colourAddress = colourAddress + 40;
          xIndex = 0;
    }

    // Print 'Highsrore'
    lineAddress = 0x4000 + 24;
    colourAddress = COLOUR_RAM + 24;
    for (i = 0; i < 10; i++)
    {
        charRead = hiScore[i];
        POKE(lineAddress + i, charRead);
        POKE(colourAddress + i, *(ptr+charRead));
    }

    // Put in 0000000
    lineAddress = 0x4000 + 65;
    colourAddress = COLOUR_RAM + 65;

    for (i = 0; i < 7; i++)
    {
        charRead = 80;
        POKE(lineAddress + i, charRead);
        POKE(colourAddress + i, *(ptr+charRead));
    }

    // Print 'score'
    lineAddress = 0x4000 + 760 + 21;
    colourAddress = COLOUR_RAM + 760 + 21;
    for (i = 5; i < 10; i++)
    {
        charRead = hiScore[i];
        POKE(lineAddress + i, charRead);
        POKE(colourAddress + i, *(ptr+charRead));
    }

    // Put in 0000000
    lineAddress = 0x4000 + 800 + 25;
    colourAddress = COLOUR_RAM + 800 + 25;

    for (i = 0; i < 7; i++)
    {
        charRead = 80;
        POKE(lineAddress + i, charRead);
        POKE(colourAddress + i, *(ptr+charRead));
    }
}

void animateCurrentScreen(void)
{
    screenAnimCounter++;
    if (screenAnimCounter == 3)
    {
        animateTravelators();
        // ++travelatorFrame;
        // if (travelatorFrame > 2)
        //     travelatorFrame = 0;

        // for (i = 0; i < travelatorCount; i++)
        // {
        //     currentScreenLoc = travelatorLocations[i];
        //     charRead = travelatorChars[i];

        //     POKE(currentScreenLoc, charRead + travelatorFrame);
        // }
    }
    else if (screenAnimCounter == 6)
    {
        animamteNotes();
        // for (i = 0; i < 8; i++)
        // {
        //     if (noteAvailable[i] == 1)
        //     {
        //         // resusing charRead for colour
        //         charRead = PEEK(noteColourLocations[i]);
        //         ++charRead;
        //         if (charRead == 4)
        //             charRead = 0;

        //         POKE(noteColourLocations[i], charRead);
        //     }
        // }
        screenAnimCounter = 0;
    }
}