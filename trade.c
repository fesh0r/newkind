/*
 * Elite - The New Kind.
 *
 * Reverse engineered from the BBC disk version of Elite.
 * Additional material by C.J.Pinder.
 *
 * The original Elite code is (C) I.Bell & D.Braben 1984.
 * This version re-engineered in C by C.J.Pinder 1999-2001.
 *
 * email: <christian@newkind.co.uk>
 *
 *
 */

/*
 * trade.c
 */

#include <stdlib.h>
 
#include "config.h"
#include "gfx.h"
#include "elite.h"
#include "trade.h"
#include "docked.h"
#include "planet.h"
#include "space.h"
#include "sound.h"
#include "random.h"
#include "main.h"
#include "swat.h"
 
#define SLAVES		3
#define NARCOTICS	6
#define FIREARMS	10

/*
 * The following holds the Elite Planet Stock Market.
 */

#define TONNES		0
#define	KILOGRAMS	1
#define GRAMS		2

struct stock_item stock_market[NO_OF_STOCK_ITEMS]=
{
	{"Food",		 0, 0,  19, -2,   6, 0x01, TONNES},
	{"Textiles",	 0, 0,  20, -1,  10, 0x03, TONNES},
	{"Radioactives", 0, 0,  65, -3,   2, 0x07, TONNES},
	{"Slaves",		 0, 0,  40, -5, 226, 0x1F, TONNES},
	{"Liquor/Wines", 0, 0,  83, -5, 251, 0x0F, TONNES},
	{"Luxuries",	 0, 0, 196,  8,  54, 0x03, TONNES},
	{"Narcotics",	 0, 0, 235, 29,   8, 0x78, TONNES},
	{"Computers",	 0, 0, 154, 14,  56, 0x03, TONNES},
	{"Machinery",	 0, 0, 117,  6,  40, 0x07, TONNES},
	{"Alloys",		 0, 0,  78,  1,  17, 0x1F, TONNES},
	{"Firearms",	 0, 0, 124, 13,  29, 0x07, TONNES},
	{"Furs",		 0, 0, 176, -9, 220, 0x3F, TONNES},
	{"Minerals",	 0, 0,  32, -1,  53, 0x03, TONNES},
	{"Gold",		 0, 0,  97, -1,  66, 0x07, KILOGRAMS},
	{"Platinum",	 0, 0, 171, -2,  55, 0x1F, KILOGRAMS},
	{"Gem-Stones",	 0, 0,  45, -1, 250, 0x0F, GRAMS},
	{"Alien Items",	 0, 0,  53, 15, 192, 0x07, TONNES},
};




/*
 * Generate the Elite stock market.
 * The prices and quantities are affected by the planet's economy.
 * There is also a slight amount of randomness added in.
 * The random value is changed each time we hyperspace.
 */


void generate_stock_market (void)
{
	int quant;
	int price;
	int i;

	for (i = 0; i < NO_OF_STOCK_ITEMS; i++)
	{
		price  = stock_market[i].base_price;								/* Start with the base price	*/
		price += cmdr.market_rnd & stock_market[i].mask;					/* Add in a random amount		*/
		price += current_planet_data.economy * stock_market[i].eco_adjust;	/* Adjust for planet economy	*/
		price &= 255;														/* Only need bottom 8 bits		*/

		quant  = stock_market[i].base_quantity;								/* Start with the base quantity */
		quant += cmdr.market_rnd & stock_market[i].mask;					/* Add in a random amount		*/
		quant -= current_planet_data.economy * stock_market[i].eco_adjust;	/* Adjust for planet economy	*/
		quant &= 255;														/* Only need bottom 8 bits		*/

		if (quant > 127)	/* In an 8-bit environment '>127' would be negative */
			quant = 0;		/* So we set it to a minimum of zero. */

		quant &= 63;		/* Quantities range from 0..63 */

		stock_market[i].current_price = price * 4;
		stock_market[i].current_quantity = quant;
	}


	/* Alien Items are never available for purchase... */

	stock_market[ALIEN_ITEMS_IDX].current_quantity = 0;
}



void set_stock_quantities(int *quant)
{
	int i;

	for (i = 0; i < NO_OF_STOCK_ITEMS; i++)
		stock_market[i].current_quantity = quant[i];

	stock_market[ALIEN_ITEMS_IDX].current_quantity = 0;
}

 
int carrying_contraband (void)
{
	return (cmdr.current_cargo[SLAVES] + cmdr.current_cargo[NARCOTICS]) * 2 +
			cmdr.current_cargo[FIREARMS];
}


int total_cargo (void)
{
	int i;
	int cargo_held;

	cargo_held = 0;
	for (i = 0; i < 17; i++)
	{
		if ((cmdr.current_cargo[i] > 0) &&
			(stock_market[i].units == TONNES))
		{
			cargo_held += cmdr.current_cargo[i];
		}
	}

	return cargo_held;
}


void scoop_item (int un)
{
	int type;
	int trade;

	if (universe[un].flags & FLG_DEAD)
		return;
	
	type = universe[un].type;
	
	if (type == SHIP_MISSILE)
		return;

	if ((cmdr.fuel_scoop == 0) || (universe[un].location.y >= 0) ||
		(total_cargo() == cmdr.cargo_capacity))
	{
		explode_object (un);
		damage_ship (128 + (universe[un].energy / 2), universe[un].location.z > 0);
		return;
	}

	if (type == SHIP_CARGO)
	{
		trade = rand255() & 7;
		cmdr.current_cargo[trade]++;
		info_message (stock_market[trade].name);
		remove_ship (un);
		return;					
	}

	if (ship_list[type]->scoop_type != 0)
	{
		trade = ship_list[type]->scoop_type + 1;
		cmdr.current_cargo[trade]++;
		info_message (stock_market[trade].name);
		remove_ship (un);
		return;					
	}
	
	explode_object (un);
	damage_ship (universe[un].energy / 2, universe[un].location.z > 0);
}

