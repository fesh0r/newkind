/*
 * Elite - The New Kind.
 *
 * Reverse engineered from the BBC disk version of Elite.
 * Additional material by C.J.Pinder.
 *
 * Alterations to vertex ordering by Thomas Harte.
 * (T.Harte@excite.com)
 *
 * The original Elite code is (C) I.Bell & D.Braben 1984.
 * This version re-engineered in C by C.J.Pinder 1999-2001.
 *
 * email: <christian@newkind.co.uk>
 *
 *
 */

/*
 * shipface.c
 *
 * Face information for the ships.
 * Adapted from the Elite ship data published by Ian Bell.
 *
 * Adapted further for clockwise vertex lists.
 */ 

#include <stdlib.h>

#include "config.h"
#include "elite.h"
#include "shipface.h"
#include "gfx.h"




struct ship_face missile_face[] = 
{
	//fins
	{GFX_COL_RED,  0x20, 0x00, 0x00, 3,  5, 9, 15, 0, 0, 0, 0, 0},
	{GFX_COL_RED,  0x00, 0x20, 0x00, 3, 15, 9,  5, 0, 0, 0, 0, 0},

	{GFX_COL_RED, -0x20, 0x00, 0x00, 3, 8, 12, 13, 0, 0, 0, 0, 0},
	{GFX_COL_RED,  0x00, 0x20, 0x00, 3, 13, 12, 8, 0, 0, 0, 0, 0},

	{GFX_COL_RED, -0x20, 0x00, 0x00, 3, 7, 11, 14, 0, 0, 0, 0, 0},
	{GFX_COL_RED,  0x00,-0x20, 0x00, 3, 14, 11, 7, 0, 0, 0, 0, 0},

	{GFX_COL_RED,  0x20, 0x00, 0x00, 3, 6, 10, 16, 0, 0, 0, 0, 0},
	{GFX_COL_RED,  0x00,-0x20, 0x00, 3, 16, 10, 6, 0, 0, 0, 0, 0},

	//nose cone
	{GFX_COL_DARK_RED, -0x40, 0x00, 0x10, 3, 0,  3,  4, 0, 0, 0, 0, 0},
	{GFX_COL_RED,       0x00,-0x40, 0x10, 3, 0,  4,  1, 0, 0, 0, 0, 0},
	{GFX_COL_DARK_RED,  0x40, 0x00, 0x10, 3, 0,  1,  2, 0, 0, 0, 0, 0},
	{GFX_COL_RED,       0x00, 0x40, 0x10, 3, 0,  2,  3, 0, 0, 0, 0, 0},

	//main body
	{GFX_COL_GREY_3,  0x20, 0x00, 0x00, 4, 6,  5,  2, 1, 0, 0, 0, 0},
	{GFX_COL_GREY_1,  0x00, 0x20, 0x00, 4, 5,  8,  3, 2, 0, 0, 0, 0},
	{GFX_COL_GREY_3, -0x20, 0x00, 0x00, 4, 8,  7,  4, 3, 0, 0, 0, 0},
	{GFX_COL_GREY_1,  0x00,-0x20, 0x00, 4, 7,  6,  1, 4, 0, 0, 0, 0},

	//bottom
	{GFX_COL_GREY_2,  0x00, 0x00,-0xB0, 4, 5,  6,  7, 8, 0, 0, 0, 0},
};


struct ship_face coriolis_face[] = 
{
	{GFX_COL_GREY_3,  0x6B,-0x6B, 0x6B, 3,  4,  0,  3,  0,  0,  0, 0, 0},
	{GFX_COL_GREY_3,  0x6B, 0x6B, 0x6B, 3,  0,  5,  1,  0,  0,  0, 0, 0},
	{GFX_COL_GREY_3, -0x6B, 0x6B, 0x6B, 3,  1,  6,  2,  0,  0,  0, 0, 0},
	{GFX_COL_GREY_3, -0x6B,-0x6B, 0x6B, 3,  2,  7,  3,  0,  0,  0, 0, 0},

	{GFX_COL_GREY_2,  0x00,-0xA0, 0x00, 4, 4,  3,  7, 11,  0,  0, 0, 0},
	{GFX_COL_GREY_2,  0xA0, 0x00, 0x00, 4,  8,  5,  0,  4,  0,  0, 0, 0},
	{GFX_COL_GREY_2, -0xA0, 0x00, 0x00, 4, 10,  7,  2,  6,  0,  0, 0, 0},
	{GFX_COL_GREY_2,  0x00, 0xA0, 0x00, 4,  1,  5,  9,  6,  0,  0, 0, 0},
        
	{GFX_COL_GREY_3, -0x6B,-0x6B,-0x6B, 3, 11,  7, 10,  0,  0,  0, 0, 0},
	{GFX_COL_GREY_3,  0x6B,-0x6B,-0x6B, 3, 11,  8,  4,  0,  0,  0, 0, 0},
	{GFX_COL_GREY_3,  0x6B, 0x6B,-0x6B, 3,  9,  5,  8,  0,  0,  0, 0, 0},
	{GFX_COL_GREY_3, -0x6B, 0x6B,-0x6B, 3, 10,  6,  9,  0,  0,  0, 0, 0},

	{GFX_COL_GREY_1, 0x00, 0x00,-0xA0, 4, 11, 10,  9,  8,  0,  0, 0, 0},
	{GFX_COL_GREY_1,  0x00, 0x00, 0xA0, 4,  0,  1,  2,  3,  0,  0, 0, 0},

	{GFX_COL_BLACK,  0x00, 0x00, 0xA0, 4, 15, 12, 13, 14,  0,  0, 0, 0},
};


struct ship_face escape_face[] = 
{
	{GFX_COL_RED,		 0x34, 0x00,-0x7A, 3, 3, 1, 2,  0, 0, 0, 0, 0},
	{GFX_COL_DARK_RED,	 0x27, 0x67, 0x1E, 3, 0, 3, 2,  0, 0, 0, 0, 0},
	{GFX_COL_RED_3,		 0x27,-0x67, 0x1E, 3, 0, 1, 3,  0, 0, 0, 0, 0},
	{GFX_COL_RED_4,		-0x70, 0x00, 0x00, 3, 0, 2, 1,  0, 0, 0, 0, 0},
};


struct ship_face alloy_face[] =
{
	{GFX_COL_GREY_1, 0x00, 0x00, 0x00, 4, 0, 1, 2, 3, 0, 0, 0, 0},
	{GFX_COL_GREY_3, 0x00, 0x00, 0x00, 4, 3, 2, 1, 0, 0, 0, 0, 0},
};

struct ship_face cargo_face[] =
{
	{GFX_COL_GREY_4,  0x60, 0x00, 0x00, 5, 4, 0, 1, 2, 3, 0, 0, 0},

	{GFX_COL_GREY_2,  0x00, 0x29, 0x1E, 4, 5, 6, 1, 0, 0, 0, 0, 0},
	{GFX_COL_GREY_1,  0x00,-0x12, 0x30, 4, 6, 7, 2, 1, 0, 0, 0, 0},
	{GFX_COL_GREY_3,  0x00,-0x33, 0x00, 4, 7, 8, 3, 2, 0, 0, 0, 0},
	{GFX_COL_GREY_1,  0x00,-0x12,-0x30, 4, 8, 9, 4, 3, 0, 0, 0, 0},
	{GFX_COL_GREY_3,  0x00, 0x29,-0x1E, 4, 9, 5, 0, 4, 0, 0, 0, 0},

	{GFX_COL_GREY_4, -0x60, 0x00, 0x00, 5, 8, 7, 6, 5, 9, 0, 0, 0},
};


struct ship_face boulder_face[] =
{
	{GFX_COL_GREY_3, -0x0F,-0x03, 0x08, 3, 0, 4, 5,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_1, -0x07, 0x0C, 0x1E, 3, 0, 5, 1,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_2,  0x20,-0x2F, 0x18, 3, 1, 5, 2,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_3, -0x03,-0x27,-0x07, 3, 2, 5, 3,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_1, -0x05,-0x04,-0x01, 3, 3, 5, 4,  0, 0, 0, 0, 0},

	{GFX_COL_GREY_2,  0x31, 0x54, 0x08, 3, 1, 6, 0,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_3,  0x70, 0x15,-0x15, 3, 2, 6, 1,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_1,  0x4C,-0x23,-0x52, 3, 3, 6, 2,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_2,  0x16, 0x38,-0x89, 3, 4, 6, 3,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_1,  0x28, 0x6E,-0x26, 3, 6, 4, 0,  0, 0, 0, 0, 0},
};


struct ship_face asteroid_face[] = 
{
	{GFX_COL_GREY_3,  0x09, 0x42, 0x51, 3, 5, 0, 6,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_1,  0x09,-0x42, 0x51, 3, 2, 5, 6,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_2, -0x48, 0x40, 0x1F, 3, 6, 0, 1,  0, 0, 0, 0, 0},

	{GFX_COL_GREY_3, -0x40,-0x49, 0x2F, 3, 2, 6, 1,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_2,  0x2D,-0x4F, 0x41, 3, 3, 5, 2,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_1,  0x87, 0x0F, 0x23, 3, 4, 5, 3,  0, 0, 0, 0, 0},

	{GFX_COL_GREY_2,  0x26, 0x4C, 0x46, 3, 0, 5, 4,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_3, -0x42, 0x3B,-0x27, 3, 1, 0, 7,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_1, -0x43,-0x0F,-0x50, 3, 1, 7, 8,  0, 0, 0, 0, 0},

	{GFX_COL_GREY_2,  0x42,-0x0E,-0x4B, 3, 3, 8, 7,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_2, -0x46,-0x50,-0x28, 3, 1, 8, 2,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_3,  0x3A,-0x66,-0x33, 3, 3, 2, 8,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_3,  0x51, 0x09,-0x43, 3, 4, 3, 7,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_1,  0x2F, 0x5E,-0x3F, 3, 4, 7, 0,  0, 0, 0, 0, 0},
};


struct ship_face rock_face[] =
{
	{GFX_COL_GREY_1, 0x00, 0x00, 0x00, 3, 3, 2, 1, 0, 0, 0, 0, 0},
	{GFX_COL_GREY_2, 0x00, 0x00, 0x00, 3, 0, 2, 3, 0, 0, 0, 0, 0},
	{GFX_COL_GREY_3, 0x00, 0x00, 0x00, 3, 3, 1, 0, 0, 0, 0, 0, 0},
	{GFX_COL_GREY_4, 0x00, 0x00, 0x00, 3, 0, 1, 2, 0, 0, 0, 0, 0},
};



struct ship_face shuttle_face[] =
{
	{GFX_COL_GREY_1,  0x00,-0x4A, 0x04, 3,  0,  4,  7,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_2, -0x33,-0x33, 0x17, 3,  1,  4,  0,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_1, -0x4A, 0x00, 0x04, 3,  1,  5,  4,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_2, -0x33, 0x33, 0x17, 3,  2,  5,  1,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_1,  0x00, 0x4A, 0x04, 3,  2,  6,  5,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_2,  0x33, 0x33, 0x17, 3,  3,  6,  2,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_1,  0x4A, 0x00, 0x04, 3,  3,  7,  6,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_2,  0x33,-0x33, 0x17, 3,  0,  7,  3,  0, 0, 0, 0, 0},

	{GFX_COL_GREY_3,	0x00, 0x00,-0x6B, 4,  7,  4,  5,  6, 0, 0, 0, 0},
	{GFX_COL_RED,		0x00, 0x00,-0x6B, 4, 11,  8,  9, 10, 0, 0, 0, 0},

	{GFX_COL_GREY_4, -0x37, -0x37, 0x28, 3,  0, 12,  1,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_1, -0x29,  0x29, 0x5A, 3,  1, 12,  2,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_4,  0x29,  0x29, 0x5A, 3,  2, 12,  3,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_1,  0x37, -0x37, 0x28, 3,  3, 12,  0,  0, 0, 0, 0, 0},

	{GFX_COL_BLUE_1,  0x29, 0x29, 0x5A, 3, 14, 13, 15,  0, 0, 0, 0, 0},
	{GFX_COL_BLUE_1, -0x29, 0x29, 0x5A, 3, 18, 16, 17,  0, 0, 0, 0, 0},
};



/* Two lines on base & detail of IB DB initials added back in :-) */

struct ship_face transporter_face[] =
{
	{GFX_COL_GREY_3,  0x00, 0x00,-0x67, 7,  5,  4,  3, 2,  1 ,0, 6, 0},

	{GFX_COL_BLUE_1, -0x6F, 0x30,-0x07, 4,  9,  8,  1,  2, 0,  0, 0, 0},
	{GFX_COL_BLUE_2, -0x69,-0x3F,-0x15, 3,  3,  9,  2,  0, 0,  0, 0, 0},
	{GFX_COL_BLUE_4,  0x00,-0x22, 0x00, 6, 14, 13,  9,  3, 4, 10, 0, 0},
	{GFX_COL_BLUE_2,  0x69,-0x3F,-0x15, 3,  5, 10,  4,  0, 0,  0, 0, 0},
	{GFX_COL_BLUE_1,  0x6F, 0x30,-0x07, 4, 11, 10,  5,  6, 0,  0, 0, 0},

	{GFX_COL_GREY_1,  0x08, 0x20, 0x03, 4, 6,  0,  7, 11, 0,  0, 0, 0},
	{GFX_COL_GREY_2, -0x08, 0x20, 0x03, 4,  8,  7,  0,  1, 0,  0, 0, 0},

	{GFX_COL_BLUE_1, -0x4B, 0x20, 0x4F, 4, 13, 12,  8,  9, 0,  0, 0, 0},
	{GFX_COL_BLUE_1,  0x4B, 0x20, 0x4F, 4, 15, 14, 10, 11, 0,  0, 0, 0},

	{GFX_COL_GREY_1, -0x08, 0x22, 0x0B, 3,  8, 12,  7,  0, 0,  0, 0, 0},
	{GFX_COL_GREY_2,  0x08, 0x22, 0x0B, 3, 7, 15,  11,  0, 0,  0, 0, 0},
	{GFX_COL_GREY_4,  0x00, 0x26, 0x11, 3, 7, 12,  15,  0, 0,  0, 0, 0},

	{GFX_COL_WHITE_2,  0x00, 0x00, 0x79, 4, 15, 12, 13, 14, 0,  0, 0, 0},
	{GFX_COL_DARK_RED,  0x00, 0x00,-0x67, 4, 35, 34, 33, 36, 0,  0, 0, 0},

	{GFX_COL_WHITE,  0x00,-0x22, 0x00, 2, 30, 29, 31,  0, 0,  0, 0, 0},
	{GFX_COL_WHITE,  0x00,-0x22, 0x00, 2, 31, 32, 29,  0, 0,  0, 0, 0},

	{GFX_COL_WHITE, -0x08, 0x20, 0x03, 2, 17, 16, 18,  0, 0,  0, 0, 0},
	{GFX_COL_WHITE, -0x08, 0x20, 0x03, 2, 18, 19, 16,  0, 0,  0, 0, 0},
	{GFX_COL_WHITE, -0x08, 0x20, 0x03, 2, 18, 20, 19,  0, 0,  0, 0, 0},
	{GFX_COL_WHITE, -0x08, 0x20, 0x03, 2, 20, 21, 18,  0, 0,  0, 0, 0},
	{GFX_COL_WHITE, -0x08, 0x20, 0x03, 2, 20, 19, 21,  0, 0,  0, 0, 0},

	{GFX_COL_WHITE,  0x08, 0x20, 0x03, 2, 23, 22, 26,  0, 0,  0, 0, 0},
	{GFX_COL_WHITE,  0x08, 0x20, 0x03, 2, 25, 26, 23,  0, 0,  0, 0, 0},
	{GFX_COL_WHITE,  0x08, 0x20, 0x03, 2, 24, 22, 25,  0, 0,  0, 0, 0},
	{GFX_COL_WHITE,  0x08, 0x20, 0x03, 2, 24, 23, 22,  0, 0,  0, 0, 0},
	{GFX_COL_WHITE,  0x08, 0x20, 0x03, 2, 28, 27, 23,  0, 0,  0, 0, 0},
	{GFX_COL_WHITE,  0x08, 0x20, 0x03, 2, 25, 27, 22,  0, 0,  0, 0, 0},
	{GFX_COL_WHITE,  0x08, 0x20, 0x03, 2, 27, 26, 22,  0, 0,  0, 0, 0},
};


struct ship_face cobra3_face[] = 
{
	{GFX_COL_GREY_2,	0x00, 0x3E, 0x1F, 3,  1,  0,  2,  0, 0, 0, 0, 0},
	{GFX_COL_BLUE_1,   -0x12, 0x37, 0x10, 3,  5,  1,  2,  0, 0, 0, 0, 0},
	{GFX_COL_BLUE_1,	0x12, 0x37, 0x10, 3,  2,  0,  6,  0, 0, 0, 0, 0},
	{GFX_COL_BLUE_3,   -0x10, 0x34, 0x0E, 3,  3,  1,  5,  0, 0, 0, 0, 0},
	{GFX_COL_BLUE_3,	0x10, 0x34, 0x0E, 3,  6,  0,  4,  0, 0, 0, 0, 0},

	{GFX_COL_GREY_1,   -0x0E, 0x2F, 0x00, 3,  5,  2,  9,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_1,	0x0E, 0x2F, 0x00, 3,  9,  2,  6,  0, 0, 0, 0, 0},

	{GFX_COL_BLUE_2,   -0x3D, 0x66, 0x00, 3,  8,  3,  5,  0, 0, 0, 0, 0},
	{GFX_COL_BLUE_2,	0x3D, 0x66, 0x00, 3,  6,  4,  7,  0, 0, 0, 0, 0},
        
	{GFX_COL_GREY_2,	0x00, 0x00,-0x50, 7,  6,  7, 11, 10, 8, 5, 9, 0},

	{GFX_COL_GREY_3,   -0x07,-0x2A, 0x09, 4, 10,  1,  3,  8, 0, 0, 0, 0},
	{GFX_COL_DARK_RED,	0x00,-0x1E, 0x06, 4, 10, 11,  0,  1, 0, 0, 0, 0},
	{GFX_COL_GREY_3,	0x07,-0x2A, 0x09, 4,  7,  4,  0, 11, 0, 0, 0, 0},

	{GFX_COL_RED,		0x00, 0x00,-0x50, 4, 17, 14, 15, 16, 0, 0, 0, 0},
	{GFX_COL_RED,		0x00, 0x00,-0x50, 4, 19, 12, 13, 18, 0, 0, 0, 0},
	{GFX_COL_DARK_RED,  0x00, 0x00,-0x50, 3, 23, 22, 24,  0, 0, 0, 0, 0},
	{GFX_COL_DARK_RED,  0x00, 0x00,-0x50, 3, 27, 25, 26,  0, 0, 0, 0, 0},

	{GFX_COL_WHITE,		0x00, 0x3E, 0x1F, 2, 20, 21,  0,  0, 0, 0, 0, 0},
	{GFX_COL_WHITE,		0x00,-0x1E, 0x06, 2, 21, 20,  0,  0, 0, 0, 0, 0},
};


struct ship_face python_face[] =
{
	{GFX_COL_GREY_2, -0x1B, 0x28, 0x0B, 3, 0,  1,  3, 0, 0, 0, 0, 0},
	{GFX_COL_GREY_1,  0x1B, 0x28, 0x0B, 3, 2,  1,  0, 0, 0, 0, 0, 0},
	{GFX_COL_GREY_1, -0x1B,-0x28, 0x0B, 3, 0,  3,  8, 0, 0, 0, 0, 0},
	{GFX_COL_GREY_2,  0x1B,-0x28, 0x0B, 3, 8,  2,  0, 0, 0, 0, 0, 0},

	{GFX_COL_YELLOW_1, -0x13, 0x26, 0x00, 3, 3,  1,  4, 0, 0, 0, 0, 0},
	{GFX_COL_YELLOW_2,  0x13, 0x26, 0x00, 3, 4,  1,  2, 0, 0, 0, 0, 0},
	{GFX_COL_YELLOW_2, -0x13,-0x26, 0x00, 3, 3,  9,  8, 0, 0, 0, 0, 0},
	{GFX_COL_YELLOW_1,  0x13,-0x26, 0x00, 3, 8,  9,  2, 0, 0, 0, 0, 0},

	{GFX_COL_GREY_2, -0x19, 0x25,-0x0B, 4, 3,  4,  5, 6, 0, 0, 0, 0},
	{GFX_COL_GREY_1,  0x19, 0x25,-0x0B, 4, 2,  7,  5, 4, 0, 0, 0, 0},
	{GFX_COL_GREY_2,  0x19,-0x25,-0x0B, 4, 2,  9, 10, 7, 0, 0, 0, 0},
	{GFX_COL_GREY_1, -0x19,-0x25,-0x0B, 4, 3,  6, 10, 9, 0, 0, 0, 0},

	{GFX_COL_GREY_3,  0x00, 0x00,-0x70, 4, 10, 6 , 5, 7, 0, 0, 0, 0},
};


struct ship_face boa_face[] =
{
	{GFX_COL_BLUE_4,  0x2B, 0x25,-0x3C, 6, 11, 10,  1, 5, 6, 2, 0, 0},
	{GFX_COL_BLUE_2,  0x00,-0x2D,-0x59, 6, 12, 11,  2, 7, 8, 3, 0, 0},
	{GFX_COL_BLUE_3, -0x2B, 0x25,-0x3C, 6, 3, 9,  4, 1, 10, 12, 0, 0},

	{GFX_COL_BLUE_4,  0x00, 0x28, 0x00, 3,  5,  1,  4, 0, 0, 0, 0, 0},
	{GFX_COL_BLUE_2,  0x3E,-0x20,-0x14, 3,  7,  2,  6, 0, 0, 0, 0, 0},
	{GFX_COL_BLUE_3, -0x3E,-0x20,-0x14, 3,  3,  8,  9, 0, 0, 0, 0, 0},

	{GFX_COL_GREY_1,  0x00, 0x17, 0x06, 3,  5,  4,  0, 0, 0, 0, 0, 0},
	{GFX_COL_GREY_1, -0x17,-0x0F, 0x09, 3,  9,  8,  0, 0, 0, 0, 0, 0},
	{GFX_COL_GREY_1,  0x17,-0x0F, 0x09, 3,  7,  6,  0, 0, 0, 0, 0, 0},
	{GFX_COL_GREY_2, -0x1A, 0x0D, 0x0A, 3,  0,  4,  9, 0, 0, 0, 0, 0},
	{GFX_COL_GREY_2,  0x00,-0x1F, 0x0C, 3,  0,  8,  7, 0, 0, 0, 0, 0},
	{GFX_COL_GREY_2,  0x1A, 0x0D, 0x0A, 3,  0,  6,  5, 0, 0, 0, 0, 0},

	{GFX_COL_DARK_RED,  0x00, 0x00,-0x6B, 3, 12, 10, 11, 0, 0, 0, 0, 0},
};


struct ship_face anaconda_face[] =
{
	{GFX_COL_GREEN_1,  0x00,-0x33,-0x31, 5,  3,  2,  1,  0, 4, 0, 0, 0},
	{GFX_COL_GREEN_2, -0x33, 0x12,-0x57, 5,  6, 10,  5,  0, 1, 0, 0, 0},
	{GFX_COL_GREEN_3, -0x4D,-0x39,-0x13, 5,  7, 11,  6,  1, 2, 0, 0, 0},

	{GFX_COL_GREY_2,  0x00,-0x5A, 0x10, 5,  8, 12,  7,  2, 3, 0, 0, 0},

	{GFX_COL_GREEN_2,  0x4D,-0x39,-0x13, 5,  9, 13,  8,  3, 4, 0, 0, 0},
	{GFX_COL_GREEN_3,  0x33, 0x12,-0x57, 5, 9,  4,  0,  5, 14, 0, 0, 0},
	{GFX_COL_GREEN_1,  0x00, 0x6F,-0x14, 3, 10, 14,  5,  0, 0, 0, 0, 0},

	{GFX_COL_GREY_2, -0x61, 0x48, 0x18, 4, 10, 6,  11, 12, 0, 0, 0, 0},
	{GFX_COL_GREY_1, -0x6C,-0x44, 0x22, 3, 7, 12,  11,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_1,  0x6C,-0x44, 0x22, 3, 8, 13,  12,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_2,  0x61, 0x48, 0x18, 4, 9, 14,  12, 13, 0, 0, 0, 0},
	{GFX_COL_GREY_1,  0x00, 0x5E, 0x12, 3, 10, 12, 14,  0, 0, 0, 0, 0},
};


struct ship_face hermit_face[] = 
{
	{GFX_COL_PINK_1,  0x09, 0x42, 0x51, 3, 5, 0, 6,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_1,  0x09,-0x42, 0x51, 3, 2, 5, 6,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_2, -0x48, 0x40, 0x1F, 3, 6, 0, 1,  0, 0, 0, 0, 0},
	{GFX_COL_PINK_1, -0x40,-0x49, 0x2F, 3, 2, 6, 1,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_2,  0x2D,-0x4F, 0x41, 3, 3, 5, 2,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_1,  0x87, 0x0F, 0x23, 3, 4, 5, 3,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_2,  0x26, 0x4C, 0x46, 3, 0, 5, 4,  0, 0, 0, 0, 0},
	{GFX_COL_PINK_1, -0x42, 0x3B,-0x27, 3, 1, 0, 7,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_1, -0x43,-0x0F,-0x50, 3, 1, 7, 8,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_2,  0x42,-0x0E,-0x4B, 3, 3, 8, 7,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_2, -0x46,-0x50,-0x28, 3, 1, 8, 2,  0, 0, 0, 0, 0},
	{GFX_COL_PINK_1,  0x3A,-0x66,-0x33, 3, 3, 2, 8,  0, 0, 0, 0, 0},
	{GFX_COL_PINK_1,  0x51, 0x09,-0x43, 3, 4, 3, 7,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_1,  0x2F, 0x5E,-0x3F, 3, 4, 7, 0,  0, 0, 0, 0, 0},
};


struct ship_face viper_face[] = 
{
	{GFX_COL_GREY_2,  0x00, 0x20, 0x00, 3,  7,  8,  1, 0, 0, 0, 0, 0},
        
	{GFX_COL_BLUE_3, -0x16, 0x21, 0x0B, 4,  8,  4,  0, 1, 0, 0, 0, 0},
	{GFX_COL_BLUE_2,  0x16, 0x21, 0x0B, 4,  3,  7,  1, 0, 0, 0, 0, 0},
        
	{GFX_COL_BLUE_2, -0x16,-0x21, 0x0B, 4,  2,  0,  4, 6, 0, 0, 0, 0},
	{GFX_COL_BLUE_3,  0x16,-0x21, 0x0B, 4,  0,  2,  5, 3, 0, 0, 0, 0},
        
	{GFX_COL_GREY_2,  0x00,-0x20, 0x00, 3,  2,  6,  5, 0, 0, 0, 0, 0},
	{GFX_COL_GREY_1,  0x00, 0x00,-0x30, 6,  4,  8,  7, 3, 5, 6, 0, 0},
	{GFX_COL_RED,  0x00, 0x00,-0x30, 3, 12, 13,  9, 0, 0, 0, 0, 0},
	{GFX_COL_RED,  0x00, 0x00,-0x30, 3, 10, 14, 11, 0, 0, 0, 0, 0},
};



struct ship_face sidewinder_face[] =
{
	{GFX_COL_YELLOW_1,  0x00, 0x20, 0x08, 3, 4, 0, 1, 0, 0, 0, 0, 0},
	{GFX_COL_YELLOW_2, -0x0C, 0x2F, 0x06, 3, 4, 3, 0, 0, 0, 0, 0, 0},
	{GFX_COL_YELLOW_2,  0x0C, 0x2F, 0x06, 3, 2, 4, 1, 0, 0, 0, 0, 0},

	{GFX_COL_GREY_1,  0x00, 0x00,-0x70, 4, 2, 5, 3, 4, 0, 0, 0, 0},

	{GFX_COL_YELLOW_1, -0x0C,-0x2F, 0x06, 3, 5, 0, 3, 0, 0, 0, 0, 0},
	{GFX_COL_YELLOW_2,  0x00,-0x20, 0x08, 3, 1, 0, 5, 0, 0, 0, 0, 0},
	{GFX_COL_YELLOW_1,  0x0C,-0x2F, 0x06, 3, 2, 1, 5, 0, 0, 0, 0, 0},
	{GFX_COL_RED,  0x00, 0x00,-0x70, 4, 8, 9, 6, 7, 0, 0, 0, 0},
};


struct ship_face mamba_face[] =
{
	{GFX_COL_GREEN_1,  0x00,-0x18, 0x02, 3,  1,  4,  0,  0, 0, 0, 0, 0},
	{GFX_COL_GREEN_3,  0x00, 0x18, 0x02, 3,  2,  0,  3,  0, 0, 0, 0, 0},
	{GFX_COL_GREEN_2, -0x20, 0x40, 0x10, 3,  1,  0,  2,  0, 0, 0, 0, 0},
	{GFX_COL_GREEN_2,  0x20, 0x40, 0x10, 3,  3,  0,  4,  0, 0, 0, 0, 0},

	{GFX_COL_GREY_1,  0x00, 0x00,-0x7F, 4,  1,  2,  3,  4, 0, 0, 0, 0},
	{GFX_COL_BLUE_1,  0x00,-0x18, 0x02, 3, 11, 12,  9,  0, 0, 0, 0, 0},
	{GFX_COL_RED,  0x00, 0x00,-0x7F, 4, 17, 18, 15, 16, 0, 0, 0, 0},
	{GFX_COL_BLUE_2,  0x00, 0x18, 0x02, 4,  7,  6,  5,  8, 0, 0, 0, 0},
	{GFX_COL_BLUE_1,  0x00,-0x18, 0x02, 3, 13, 14, 10,  0, 0, 0, 0, 0},
	{GFX_COL_RED,  0x00, 0x00,-0x7F, 3, 20, 24, 21,  0, 0, 0, 0, 0},
	{GFX_COL_RED,  0x00, 0x00,-0x7F, 3, 22, 23, 19,  0, 0, 0, 0, 0},
};


struct ship_face krait_face[] = 
{
	{GFX_COL_BLUE_3,  0x03, 0x18, 0x03, 3,  0,  3,  1, 0, 0, 0, 0, 0},
	{GFX_COL_BLUE_2,  0x03,-0x18, 0x03, 3,  2,  3,  0, 0, 0, 0, 0, 0},

	{GFX_COL_BLUE_3, -0x03,-0x18, 0x03, 3,  0,  4,  2, 0, 0, 0, 0, 0},
	{GFX_COL_BLUE_2, -0x03, 0x18, 0x03, 3,  1,  4,  0, 0, 0, 0, 0, 0},

	{GFX_COL_GREY_3,  0x26, 0x00,-0x4D, 3,  3,  2,  1, 0, 0, 0, 0, 0},
	{GFX_COL_GREY_1, -0x26, 0x00,-0x4D, 3,  4,  1,  2, 0, 0, 0, 0, 0},

	{GFX_COL_WHITE,  0x03,-0x18, 0x03, 2,  3,  5,  0, 0, 0, 0, 0, 0},
	{GFX_COL_WHITE,  0x03, 0x18, 0x03, 2,  5,  3,  0, 0, 0, 0, 0, 0},
	{GFX_COL_WHITE, -0x03, 0x18, 0x03, 2,  4,  6,  0, 0, 0, 0, 0, 0},
	{GFX_COL_WHITE, -0x03,-0x18, 0x03, 2,  6,  4,  0, 0, 0, 0, 0, 0},

	{GFX_COL_RED,	  0x26, 0x00,-0x4D, 3, 12, 11, 13, 0, 0, 0, 0, 0},
	{GFX_COL_RED,	 -0x26, 0x00,-0x4D, 3, 16, 14, 15, 0, 0, 0, 0, 0},
	{GFX_COL_WHITE,	  0x03, 0x18, 0x03, 3,  7, 10,  8, 0, 0, 0, 0, 0},
	{GFX_COL_WHITE,	 -0x03, 0x18, 0x03, 3,  8,  9,  7, 0, 0, 0, 0, 0},
};


struct ship_face adder_face[] =
{
	{GFX_COL_GREY_1,	0x00, 0x27, 0x0A, 4, 0, 1,  11,  10, 0, 0, 0, 0},
	{GFX_COL_GREY_1,	0x00,-0x27, 0x0A, 4, 1, 0,  12,  13, 0, 0, 0, 0},

	{GFX_COL_RED_4,  	0x45, 0x32, 0x0D, 3,  2, 11,  1,  0, 0, 0, 0, 0},
	{GFX_COL_RED,		0x45,-0x32, 0x0D, 3,  1, 13,  2,  0, 0, 0, 0, 0},
	{GFX_COL_DARK_RED,  0x1E, 0x34, 0x00, 4,  9, 11,  2,  3, 0, 0, 0, 0},
	{GFX_COL_RED_3,     0x1E,-0x34, 0x00, 4,  3, 2, 13,  4, 0, 0, 0, 0},

	{GFX_COL_DARK_RED, -0x1E, 0x34, 0x00, 4, 10,  8,  6,  7, 0, 0, 0, 0},
	{GFX_COL_RED_3,	   -0x1E,-0x34, 0x00, 4, 7,  6,  5,  12, 0, 0, 0, 0},
	{GFX_COL_RED_4,	   -0x45, 0x32, 0x0D, 3, 10,  7,  0,  0, 0, 0, 0, 0},
	{GFX_COL_RED,      -0x45,-0x32, 0x0D, 3, 0,  7,  12,  0, 0, 0, 0, 0},

	{GFX_COL_GREY_3,  0x00, 0x00,-0xA0, 6,  3,  4,  5,  6, 8, 9, 0, 0},
	{GFX_COL_GREY_2,  0x00, 0x1C, 0x00, 4, 10, 11,  9,  8, 0, 0, 0, 0},
	{GFX_COL_GREY_2,  0x00,-0x1C, 0x00, 4, 5, 4,  13,  12, 0, 0, 0, 0},
	{GFX_COL_BLUE_1,  0x00, 0x27, 0x0A, 4, 17, 14, 15, 16, 0, 0, 0, 0},
};


struct ship_face gecko_face[] =
{
	{GFX_COL_GREY_2,  0x00, 0x1F, 0x05, 4,  3,  2, 0, 1, 0, 0, 0, 0},
	{GFX_COL_GREY_1,  0x04, 0x2D, 0x08, 3,  3,  1, 5, 0, 0, 0, 0, 0},
	{GFX_COL_GREY_3,  0x19,-0x6C, 0x13, 3,  5,  1, 7, 0, 0, 0, 0, 0},

	{GFX_COL_GREY_1,  0x00,-0x54, 0x0C, 4,  1,  0, 6, 7, 0, 0, 0, 0},
	{GFX_COL_GREY_3, -0x19,-0x6C, 0x13, 3,  4,  6, 0, 0, 0, 0, 0, 0},
	{GFX_COL_GREY_1, -0x04, 0x2D, 0x08, 3,  0,  2, 4, 0, 0, 0, 0, 0},

	{GFX_COL_DARK_RED, -0x58, 0x10,-0xD6, 3,  4,  2, 6, 0, 0, 0, 0, 0},
	{GFX_COL_RED,  0x00, 0x00,-0xBB, 4,  2,  3, 7, 6, 0, 0, 0, 0},
	{GFX_COL_DARK_RED,  0x58, 0x10,-0xD6, 3,  5,  7, 3, 0, 0, 0, 0, 0},

	{GFX_COL_WHITE,  0x00,-0x54, 0x0C, 2,  8, 10, 9, 0, 0, 0, 0, 0},
	{GFX_COL_WHITE,  0x00,-0x54, 0x0C, 2, 11,  9, 8, 0, 0, 0, 0, 0},
};



struct ship_face cobra1_face[] =
{
	{GFX_COL_BLUE_2,  0x00, 0x29, 0x0A, 3,  0, 1, 8, 0, 0, 0, 0, 0},
	{GFX_COL_BLUE_2,  0x00,-0x1B, 0x03, 4,  6, 7, 1, 0, 0, 0, 0, 0},

	{GFX_COL_BLUE_3, -0x08, 0x2E, 0x08, 4,  2, 0, 8, 4, 0, 0, 0, 0},
	{GFX_COL_BLUE_4, -0x0C,-0x39, 0x0C, 3,  6, 0, 2, 0, 0, 0, 0, 0},
	{GFX_COL_BLUE_3,  0x08, 0x2E, 0x08, 4,  1, 3, 5, 8, 0, 0, 0, 0},
	{GFX_COL_BLUE_4,  0x0C,-0x39, 0x0C, 3,  1, 7, 3, 0, 0, 0, 0, 0},

	{GFX_COL_BLUE_2,  0x00, 0x31, 0x00, 3,  4, 8, 5, 0, 0, 0, 0, 0},
	{GFX_COL_BLUE_4,  0x00, 0x00,-0x9A, 4,  7, 6, 4, 5, 0, 0, 0, 0},

	{GFX_COL_BLUE_2, -0x79, 0x6F,-0x3E, 3,  2, 4, 6, 0, 0, 0, 0, 0},
	{GFX_COL_BLUE_2,  0x79, 0x6F,-0x3E, 3,  3, 7, 5, 0, 0, 0, 0, 0},

	{GFX_COL_WHITE,  0x00, 0x29, 0x0A, 2, 9, 10, 0, 0, 0, 0, 0, 0},
	{GFX_COL_WHITE,  0x00,-0x1B, 0x03, 2, 10, 9, 0, 0, 0, 0, 0, 0},
};


struct ship_face worm_face[] = 
{
	{GFX_COL_GREY_4,  0x00, 0x58, 0x46, 4, 1, 0, 2, 3, 0, 0, 0, 0},
	{GFX_COL_GREY_1,  0x46, 0x42, 0x23, 3, 0, 4, 2, 0, 0, 0, 0, 0},
	{GFX_COL_GREY_1, -0x46, 0x42, 0x23, 3, 1, 3, 5, 0, 0, 0, 0, 0},

	{GFX_COL_GREY_2,  0x40, 0x31, 0x0E, 4, 2, 4, 6, 8, 0, 0, 0, 0},
	{GFX_COL_GREY_2, -0x40, 0x31, 0x0E, 4, 5, 3, 9, 7, 0, 0, 0, 0},
	{GFX_COL_GREY_1,  0x00, 0x00,-0xC8, 4, 6, 7, 9, 8, 0, 0, 0, 0},

	{GFX_COL_GREY_3,  0x00,-0x50, 0x00, 6, 4, 0, 1, 5, 7, 6, 0, 0},
	{GFX_COL_GREY_1,  0x00, 0x45, 0x0E, 4, 9, 3, 2, 8, 0, 0, 0, 0},
};


struct ship_face asp2_face[] = 
{
	{GFX_COL_GREY_4,  0x00,-0x23, 0x05, 5,  8,  9,  7,  0, 4, 0, 0, 0},
	{GFX_COL_GREY_2,  0x08,-0x26,-0x07, 5,  3,  4,  0,  1, 2, 0, 0, 0},
	{GFX_COL_GREY_1, -0x08,-0x26,-0x07, 5,  1,  0,  7,  6, 5, 0, 0, 0},
	{GFX_COL_GREY_3,  0x3B,-0x40, 0x1F, 3,  8,  4,  3,  0, 0, 0, 0, 0},
	{GFX_COL_GREY_3, -0x3B,-0x40, 0x1F, 3,  6,  7,  9,  0, 0, 0, 0, 0},

	{GFX_COL_BLUE_2,  0x00, 0x18,-0x01, 3, 11, 10, 12,  0, 0, 0, 0, 0},
	{GFX_COL_BLUE_1,  0x00, 0x2B, 0x13, 4,  9,  8, 10, 11, 0, 0, 0, 0},
	{GFX_COL_BLUE_4, -0x06, 0x1C,-0x02, 4,  6, 11, 12,  5, 0, 0, 0, 0},
	{GFX_COL_BLUE_4,  0x06, 0x1C,-0x02, 4,  2, 12, 10,  3, 0, 0, 0, 0},
	{GFX_COL_BLUE_3,  0x50, 0x2E, 0x32, 3,  3, 10,  8,  0, 0, 0, 0, 0},
	{GFX_COL_BLUE_3, -0x50, 0x2E, 0x32, 3,  9, 11,  6,  0, 0, 0, 0, 0},

	{GFX_COL_DARK_RED,  0x00, 0x00,-0x5A, 4, 2,  1,  5,  12, 0, 0, 0, 0},
	{GFX_COL_RED,  0x00, 0x00,-0x5A, 4, 14, 15, 13, 16, 0, 0, 0, 0},

	{GFX_COL_WHITE,  0x00, 0x2B, 0x13, 2, 18, 17,  0,  0, 0, 0, 0, 0},
	{GFX_COL_WHITE,  0x00,-0x23, 0x05, 2, 17, 18,  0,  0, 0, 0, 0, 0},
};


struct ship_face fer_de_lance_face[] = 
{
	{GFX_COL_GREY_1,  0x00, 0x18, 0x06, 4,  5,  0,  8, 9, 0, 0, 0, 0},
	{GFX_COL_GREY_2, -0x44, 0x00, 0x18, 3,  0,  5,  1, 0, 0, 0, 0, 0},

	{GFX_COL_BLUE_2, -0x3F, 0x00,-0x25, 4,  2,  1,  5, 6, 0, 0, 0, 0},

	{GFX_COL_RED,  0x00, 0x00,-0x68, 4,  3,  2,  6, 7, 0, 0, 0, 0},

	{GFX_COL_BLUE_2,  0x3F, 0x00,-0x25, 4,  4,  3,  7, 8, 0, 0, 0, 0},
	{GFX_COL_GREY_2,  0x44, 0x00, 0x18, 3,  4,  8,  0, 0, 0, 0, 0, 0},

	{GFX_COL_BLUE_3, -0x0C, 0x2E,-0x13, 3,  5,  9,  6, 0, 0, 0, 0, 0},
	{GFX_COL_BLUE_2,  0x00, 0x2D,-0x16, 3,  6,  9,  7, 0, 0, 0, 0, 0},
	{GFX_COL_BLUE_3,  0x0C, 0x2E,-0x13, 3,  7,  9,  8, 0, 0, 0, 0, 0},
	{GFX_COL_GREY_3,  0x00,-0x1C, 0x00, 5,  4,  0,  1, 2, 3, 0, 0, 0},

	{GFX_COL_DARK_RED,  0x00,-0x1C, 0x00, 3, 16, 18, 17, 0, 0, 0, 0, 0},
	{GFX_COL_DARK_RED,  0x00, 0x18, 0x06, 3, 11, 10, 12, 0, 0, 0, 0, 0},
	{GFX_COL_DARK_RED,  0x00, 0x18, 0x06, 3, 15, 13, 14, 0, 0, 0, 0, 0},
};


struct ship_face moray_face[] = 
{
	{GFX_COL_BLUE_4,  0x00, 0x2B, 0x07, 3,  0,  2, 1, 0, 0, 0, 0, 0},
	{GFX_COL_BLUE_3, -0x0A, 0x31, 0x07, 3,  1,  2, 3, 0, 0, 0, 0, 0},
	{GFX_COL_BLUE_3,  0x0A, 0x31, 0x07, 3,  4,  2, 0, 0, 0, 0, 0, 0},

	{GFX_COL_GREY_1,-0x3B,-0x1C,-0x65, 3,  3,  2, 6, 0, 0, 0, 0, 0},
	{GFX_COL_GREY_3,  0x00,-0x34,-0x4E, 3,  6,  2, 5, 0, 0, 0, 0, 0},
	{GFX_COL_GREY_1, 0x3B,-0x1C,-0x65, 3,  5,  2, 4, 0, 0, 0, 0, 0},

	{GFX_COL_BLUE_1, -0x48,-0x63, 0x32, 3,  6,  1, 3, 0, 0, 0, 0, 0},
	{GFX_COL_BLUE_2,  0x00,-0x53, 0x1E, 4,  6,  5, 0, 1, 0, 0, 0, 0},
	{GFX_COL_BLUE_1,  0x48,-0x63, 0x32, 3,  4,  0, 5, 0, 0, 0, 0, 0},

	{GFX_COL_DARK_RED, 0x00,-0x34,-0x4E, 3,  8,  9, 7, 0, 0, 0, 0, 0},

	{GFX_COL_WHITE,  0x00, 0x2B, 0x07, 2, 11, 10, 12, 0, 0, 0, 0, 0},
	{GFX_COL_WHITE,  0x00, 0x2B, 0x07, 2, 12, 13, 10, 0, 0, 0, 0, 0},
};


struct ship_face thargoid_face[] = 
{
	{GFX_COL_DARK_RED,	 0x67,-0x3C, 0x19, 4,  1,  0,  8,  9,  0,  0, 0, 0},
	{GFX_COL_GREY_2,	 0x67,-0x3C,-0x19, 4,  2,  1,  9, 10,  0,  0, 0, 0},
	{GFX_COL_DARK_RED,	 0x67,-0x19,-0x3C, 4,  3,  2, 10, 11,  0,  0, 0, 0},
	{GFX_COL_GREY_2,	 0x67, 0x19,-0x3C, 4,  4,  3, 11, 12,  0,  0, 0, 0},
/*
	{GFX_COL_GREY_3,	 0x40, 0x00, 0x00, 8,  7,  6,  5,  4,  3,  2, 1, 0},
*/
	{GFX_COL_GREY_3,	 0x40, 0x00, 0x00, 4,  0,  1,  2,  7,  0,  0, 0, 0},
	{GFX_COL_GREY_3,	 0x40, 0x00, 0x00, 4,  2,  3,  6,  7,  0,  0, 0, 0},
	{GFX_COL_GREY_3,	 0x40, 0x00, 0x00, 4,  3,  4,  5,  6,  0,  0, 0, 0},

	{GFX_COL_DARK_RED,	 0x67, 0x3C,-0x19, 4,  5,  4, 12, 13,  0,  0, 0, 0},
	{GFX_COL_GREY_2,	 0x67, 0x3C, 0x19, 4,  6,  5, 13, 14,  0,  0, 0, 0},
	{GFX_COL_DARK_RED,	 0x67, 0x19, 0x3C, 4,  7,  6, 14, 15,  0,  0, 0, 0},
	{GFX_COL_GREY_2,	 0x67,-0x19, 0x3C, 4,  0,  7, 15,  8,  0,  0, 0, 0},
/*
	{GFX_COL_GREY_3,	-0x30, 0x00, 0x00, 8, 15, 14, 13, 12, 11, 10, 9, 8},
*/
	{GFX_COL_GREY_3,	-0x30, 0x00, 0x00, 4,  9,  8, 15, 10, 0, 0, 0, 0},
	{GFX_COL_GREY_3,	-0x30, 0x00, 0x00, 4, 11, 10, 15, 14, 0, 0, 0, 0},
	{GFX_COL_GREY_3,	-0x30, 0x00, 0x00, 4, 12, 11, 14, 13, 0, 0, 0, 0},

	{GFX_COL_WHITE,		-0x30, 0x00, 0x00, 2, 16, 17, 19,  0,  0,  0, 0, 0},
	{GFX_COL_WHITE,		-0x30, 0x00, 0x00, 2, 18, 19, 16,  0,  0,  0, 0, 0},
};


struct ship_face thargon_face[] = 
{
	{GFX_COL_DARK_RED, -0x24, 0x00, 0x00, 5, 3, 2, 1, 0, 4, 0, 0, 0},

	{GFX_COL_GREY_1,	0x14,-0x05, 0x07, 4, 6, 5, 0, 1, 0, 0, 0, 0},
	{GFX_COL_GREY_2,	0x2E,-0x2A,-0x0E, 4, 7, 6, 1, 2, 0, 0, 0, 0},
	{GFX_COL_GREY_4,	0x24, 0x00,-0x68, 4, 8, 7, 2, 3, 0, 0, 0, 0},
	{GFX_COL_GREY_2,	0x2E, 0x2A,-0x0E, 4, 9, 8, 3, 4, 0, 0, 0, 0},
	{GFX_COL_GREY_3,	0x14, 0x05, 0x07, 4, 4, 0, 5, 9, 0, 0, 0, 0},

	{GFX_COL_DARK_RED,  0x24, 0x00, 0x00, 5, 9, 5, 6, 7, 8, 0, 0, 0},
};


struct ship_face constrictor_face[]= 
{
	{GFX_COL_GREY_4,    0x00,  0x37,  0x0F, 4,  1,  0,  8, 9, 0, 0, 0, 0},
	{GFX_COL_GREY_1,   -0x18,  0x4B,  0x14, 3,  1,  9,  2, 0, 0, 0, 0, 0},
	{GFX_COL_GREY_1,    0x18,  0x4B,  0x14, 3,  0,  7,  8, 0, 0, 0, 0, 0},

	{GFX_COL_YELLOW_2,    0x2C,  0x4B,  0x00, 3,  7,  6,  8, 0, 0, 0, 0, 0},
	{GFX_COL_YELLOW_2,   -0x2C,  0x4B,  0x00, 3,  9,  3,  2, 0, 0, 0, 0, 0},

	{GFX_COL_YELLOW_1,   -0x2C,  0x4B,  0x00, 3,  9,  4,  3, 0, 0, 0, 0, 0},
	{GFX_COL_GREY_1,    0x00,  0x35,  0x00, 4,  8,  5,  4, 9, 0, 0, 0, 0},
	{GFX_COL_YELLOW_1,    0x2C,  0x4B,  0x00, 3,  8,  6,  5, 0, 0, 0, 0, 0},

	{GFX_COL_GREY_2,    0x00,  0x00, -0xA0, 4,  6,  3,  4, 5, 0, 0, 0, 0},
	{GFX_COL_GREY_3,    0x00, -0x1B,  0x00, 6,  3,  6,  7, 0, 1, 2, 0, 0},
	{GFX_COL_DARK_RED,    0x00, -0x1B,  0x00, 3, 12, 10, 14, 0, 0, 0, 0, 0},
	{GFX_COL_DARK_RED,    0x00, -0x1B,  0x00, 3, 15, 11, 13, 0, 0, 0, 0, 0},
};

struct ship_face cougar_face[]= 
{
	{GFX_COL_GREY_1,	-0x10,  0x2E,  0x04, 4,  2,  1,  0, 3, 0, 0, 0, 0},
	{GFX_COL_GREY_2,	-0x10, -0x2E,  0x04, 3,  4,  1,  2, 0, 0, 0, 0, 0},
	{GFX_COL_GREY_4,	 0x00, -0x1B,  0x05, 4,  4,  5,  0, 1, 0, 0, 0, 0},
	{GFX_COL_GREY_2,	 0x10, -0x2E,  0x04, 3,  6,  5,  4, 0, 0, 0, 0, 0},
	{GFX_COL_GREY_2,	 0x10,  0x2E,  0x04, 4,  5,  6,  3, 0, 0, 0, 0, 0},
	{GFX_COL_GREY_3,	 0x00,  0x00, -0xA0, 4,  6,  4,  2, 3, 0, 0, 0, 0},

	{GFX_COL_YELLOW_1,	-0x10, -0x2E,  0x04, 4,  1,  2,  8, 7, 0, 0, 0, 0},
	{GFX_COL_YELLOW_1,	-0x10,  0x2E,  0x04, 4,  7,  8,  2, 1, 0, 0, 0, 0},
	{GFX_COL_YELLOW_1,	 0x10,  0x2E,  0x04, 4,  5,  6, 10, 9, 0, 0, 0, 0},
	{GFX_COL_YELLOW_1,	 0x10, -0x2E,  0x04, 4,  9, 10,  6, 5, 0, 0, 0, 0},

	{GFX_COL_BLUE_3,	-0x10,  0x2E,  0x04, 3, 12, 13, 11, 0, 0, 0, 0, 0},
	{GFX_COL_BLUE_2,	 0x10,  0x2E,  0x04, 3, 11, 14, 12, 0, 0, 0, 0, 0},
/*
	{8,	 0x00,  0x00, -0xA0, 3, 15, 16, 19, 0, 0, 0, 0, 0},
	{8,	 0x00,  0x00, -0xA0, 3, 19, 18, 17, 0, 0, 0, 0, 0},
*/
};

struct ship_face dodec_face[]= 
{
	{GFX_COL_GREY_4,	 0x00,  0x00,  0xC4, 5,  3,  2,  1,  0,  4, 0, 0, 0},
	{GFX_COL_GREY_1,	 0x67,  0x8E,  0x58, 5,  6, 10,  5,  0,  1, 0, 0, 0},
	{GFX_COL_GREY_2,	 0xA9, -0x37,  0x59, 5,  7, 11,  6,  1,  2, 0, 0, 0},
	{GFX_COL_GREY_3,	 0x00, -0xB0,  0x58, 5,  8, 12,  7,  2,  3, 0, 0, 0},
	{GFX_COL_GREY_1,	-0xA9, -0x37,  0x59, 5,  9, 13,  8,  3,  4, 0, 0, 0},
	{GFX_COL_GREY_3,	-0x67,  0x8E,  0x58, 5,  5, 14,  9,  4,  0, 0, 0, 0},
 	{GFX_COL_GREY_1,	 0x00,  0xB0, -0x58, 5, 15, 19, 14,  5, 10, 0, 0, 0},
	{GFX_COL_GREY_2,	 0xA9,  0x37, -0x59, 5, 16, 15, 10,  6, 11, 0, 0, 0},
	{GFX_COL_GREY_1,	 0x67, -0x8E, -0x58, 5, 17, 16, 11,  7, 12, 0, 0, 0},
	{GFX_COL_GREY_3,	-0x67, -0x8E, -0x58, 5, 18, 17, 12,  8, 13, 0, 0, 0},
	{GFX_COL_GREY_2,	-0xA9,  0x37, -0x59, 5, 19, 18, 13,  9, 14, 0, 0, 0},
	{GFX_COL_GREY_4,	 0x00,  0x00, -0xC4, 5, 19, 15, 16, 17, 18, 0, 0, 0},
	{GFX_COL_BLACK,		 0x00,  0x00,  0xC4, 4, 22, 20, 21, 23,  0, 0, 0, 0},
};


struct ship_solid ship_solids[] =
{
	{ 0, NULL},
	{17, missile_face}, 
	{15, coriolis_face},
	{ 4, escape_face},
	{ 2, alloy_face},
	{ 7, cargo_face},
	{10, boulder_face},
	{14, asteroid_face},
	{ 4, rock_face},
	{16, shuttle_face},
	{29, transporter_face},
	{19, cobra3_face},
	{13, python_face},
	{13, boa_face},
	{12, anaconda_face},
	{14, hermit_face},
	{ 9, viper_face},
	{ 8, sidewinder_face},
	{11, mamba_face},
	{14, krait_face},
	{14, adder_face},
	{11, gecko_face},
	{12, cobra1_face},
	{ 8, worm_face},
	{19, cobra3_face},
	{15, asp2_face},
	{13, python_face},
	{13, fer_de_lance_face},
	{12, moray_face},
	{16, thargoid_face},
	{ 7, thargon_face},
	{12, constrictor_face},
	{12, cougar_face},
	{13, dodec_face},
};

