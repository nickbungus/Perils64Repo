#ifndef CONSTANTS_HEADER
#define CONSTANTS_HEADER

#define POKE(addr,val)     (*(unsigned char*) (addr) = (val))
#define POKEW(addr,val)    (*(unsigned*) (addr) = (val))
#define PEEK(addr)         (*(unsigned char*) (addr))
#define PEEKW(addr)        (*(unsigned*) (addr))

#define DIRECTIONAL_REGISTER  56578
#define DATA_REGISTER  56576
#define GRAPHICS_POINTER  53272
#define COLOUR_MODE_REGISTER  53270
#define MULTI_COLOUR1 53282
#define MULTI_COLOUR2 53283
#define COLOUR_RAM 55296
#define RASTER_REGISTER 0xD012
#define SPRITE_ENABLE_REGISTER 0xD015 
#define SPRITE_ENABLE_MULTICOLOUR D01C
#define SPRITE_BASE_POINTER 0x43F8

#define SPRITE_X_0 0xD000
#define SPRITE_X_1 0xD002
#define SPRITE_X_2 0xD004
#define SPRITE_X_3 0xD006
#define SPRITE_X_4 0xD008
#define SPRITE_X_5 0xD00A
#define SPRITE_X_6 0xD00C
#define SPRITE_X_7 0xD00E

#define SPRITE_Y_0 0xD001
#define SPRITE_Y_1 0xD003
#define SPRITE_Y_2 0xD005
#define SPRITE_Y_3 0xD007
#define SPRITE_Y_4 0xD008
#define SPRITE_Y_5 0xD00B
#define SPRITE_Y_6 0xD00D
#define SPRITE_Y_7 0xD00F

#define BLACK  0
#define WHITE  1
#define RED  2
#define CYAN  3
#define VIOLET  4
#define GREEN  5
#define BLUE  6
#define YELLOW  7
#define ORANGE  8
#define BROWN  9
#define LIGHT_RED  10
#define DARK_GREY  11
#define GREY  12
#define LIGHT_GREEN  13
#define LIGHT_BLUE  14
#define LIGHT_GREY  15

#endif