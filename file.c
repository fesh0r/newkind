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
 * file.c
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "elite.h"
#include "config.h"
#include "file.h"

void write_config_file (void)
{
	FILE *fp;
	
	fp = fopen ("newkind.cfg", "w");
	if (fp == NULL)
		return;

	fprintf (fp, "%d\t\t# Game Speed, the lower the number the faster the game.\n", speed_cap);

	fprintf (fp, "%d\t\t# Graphics: 0 = Solid, 1 = Wireframe\n", wireframe);

	fprintf (fp, "%d\t\t# Anti-Alias Wireframe: 0 = Normal, 1 = Anti-Aliased\n", anti_alias_gfx);

	fprintf (fp, "%d\t\t# Planet style: 0 = Wireframe, 1 = Green, 2 = SNES, 3 = Fractal\n", planet_render_style);
	
	fprintf (fp, "%d\t\t# Planet Descriptions: 0 = Tree Grubs, 1 = Hoopy Casinos\n", hoopy_casinos);

	fprintf (fp, "%d\t\t# Instant dock: 0 = off, 1 = on\n", instant_dock);
	
	fprintf (fp, "newscan.cfg\t# Name of scanner config file to use.\n");

	fclose (fp);
}


/*
 * Read a line from a .cfg file.
 * Ignore blanks, comments and strip white space.
 */

void read_cfg_line (char *str, int max_size, FILE *fp)
{
	char *s;

	do
	{	
		fgets (str, max_size, fp);

		for (s = str; *s; s++)					/* End of line at LF or # */
		{
			if ((*s == '\n') || (*s == '#'))
			{
				*s = '\0';
				break;
			} 		
		}

		if (s != str)							/* Trim any trailing white space */
		{
			s--;
			while (isspace(*s))
			{
				*s = '\0';
				if (s == str)
					break;
				s--;
			}
		}

	} while (*str == '\0');
}


/*
 * Read in the scanner .cfg file.
 */

void read_scanner_config_file (char *filename)
{
	FILE *fp;
	char str[256];
	
	fp = fopen (filename, "r");
	if (fp == NULL)
		return;

	read_cfg_line (str, sizeof(str), fp);
	strcpy (scanner_filename, str);

	read_cfg_line (str, sizeof(str), fp);
	sscanf (str, "%d,%d", &scanner_cx, &scanner_cy);
	scanner_cy += 385;

	read_cfg_line (str, sizeof(str), fp);
	sscanf (str, "%d,%d", &compass_centre_x, &compass_centre_y);
	compass_centre_y += 385;
	
	fclose (fp);
}

/*
 * Read in the newkind.cfg file.
 */

void read_config_file (void)
{
	FILE *fp;
	char str[256];
	
	fp = fopen ("newkind.cfg", "r");
	if (fp == NULL)
		return;

	read_cfg_line (str, sizeof(str), fp);
	sscanf (str, "%d", &speed_cap);

	read_cfg_line (str, sizeof(str), fp);
	sscanf (str, "%d", &wireframe);

	read_cfg_line (str, sizeof(str), fp);
	sscanf (str, "%d", &anti_alias_gfx);

	read_cfg_line (str, sizeof(str), fp);
	sscanf (str, "%d", &planet_render_style);
	
	read_cfg_line (str, sizeof(str), fp);
	sscanf (str, "%d", &hoopy_casinos);

	read_cfg_line (str, sizeof(str), fp);
	sscanf (str, "%d", &instant_dock);

	read_cfg_line (str, sizeof(str), fp);
	read_scanner_config_file (str);
		
	fclose (fp);
}

int checksum (unsigned char *block)
{
	int acc,carry;
	int i;

	acc = 0x49;
	carry = 0;
	for (i = 0x49; i > 0; i--)
	{
		acc += block[i-1] + carry;
		carry = acc >> 8;
		acc &= 255;
		acc ^= block[i];
	}

	return acc;
}


int save_commander_file (char *path)
{
	FILE *fp;
	unsigned char block[256];
	int i;
	int chk;
	
	fp = fopen (path, "wb");
	if (fp == NULL)
		return 1;
	
	block[0]  = cmdr.mission;
	block[1]  = docked_planet.d;
	block[2]  = docked_planet.b;
	block[3]  = cmdr.galaxy.a;
	block[4]  = cmdr.galaxy.b;
	block[5]  = cmdr.galaxy.c;
	block[6]  = cmdr.galaxy.d;
	block[7]  = cmdr.galaxy.e;
	block[8]  = cmdr.galaxy.f;
	block[9]  = (cmdr.credits >> 24) & 255;
	block[10] = (cmdr.credits >> 16) & 255;
	block[11] = (cmdr.credits >> 8) & 255;
	block[12] = cmdr.credits & 255;
	block[13] = cmdr.fuel;
	block[14] = 4;
	block[15] = cmdr.galaxy_number;
	block[16] = cmdr.front_laser;
	block[17] = cmdr.rear_laser;
	block[18] = cmdr.left_laser;
	block[19] = cmdr.right_laser;
	block[20] = 0;
	block[21] = 0;
	block[22] = cmdr.cargo_capacity + 2;

	for (i = 0; i < NO_OF_STOCK_ITEMS; i++)
		block[23+i] = cmdr.current_cargo[i];
	
	block[40] = cmdr.ecm ? 255 : 0;
	block[41] = cmdr.fuel_scoop ? 255 : 0;
	block[42] = cmdr.energy_bomb ? 0x7F : 0;
	block[43] = cmdr.energy_unit;
	block[44] = cmdr.docking_computer ? 255 : 0;
	block[45] = cmdr.galactic_hyperdrive ? 255 : 0;
	block[46] = cmdr.escape_pod ? 255 : 0;
	block[47] = 0;
	block[48] = 0;
	block[49] = 0;
	block[50] = 0;
	block[51] = cmdr.missiles;
	block[52] = cmdr.legal_status;
	
	for (i = 0; i < NO_OF_STOCK_ITEMS; i++)
		block[53+i] = stock_market[i].current_quantity;
	
	block[70] = cmdr.market_rnd;
	block[71] = cmdr.score & 255;
	block[72] = cmdr.score >> 8;
	block[73] = 0x20;

	chk = checksum (block);
	
	block[74] = chk ^ 0xA9;
	block[75] = chk;
	
	for (i = 76; i < 256; i++)
		block[i] = 0;

	if (fwrite (block, 256, 1, fp) != 1)
		return 1;
		
	if (fclose (fp) == EOF)
		return 1;	

	return 0;
}


int load_commander_file (char *path)
{
	FILE *fp;
	unsigned char block[256];
	int i;
	int chk;
	
	fp = fopen (path, "rb");
	if (fp == NULL)
		return 1;

	if (fread (block, 256, 1, fp) != 1)
		return 1;

	chk = checksum (block);

	if ((block[74] != (chk ^ 0xA9)) || (block[75] != chk))
		return 1;
	
	saved_cmdr.mission = block[0];

	saved_cmdr.ship_x = block[1];
	saved_cmdr.ship_y = block[2];
	
	saved_cmdr.galaxy.a = block[3];
	saved_cmdr.galaxy.b = block[4];
	saved_cmdr.galaxy.c = block[5];
	saved_cmdr.galaxy.d = block[6];
	saved_cmdr.galaxy.e = block[7];
	saved_cmdr.galaxy.f = block[8];;
	
	saved_cmdr.credits = block[9] << 24;
	saved_cmdr.credits += block[10] << 16;
	saved_cmdr.credits += block[11] << 8;
	saved_cmdr.credits += block[12];

	saved_cmdr.fuel = block[13];

	saved_cmdr.galaxy_number = block[15];
	saved_cmdr.front_laser = block[16];
	saved_cmdr.rear_laser = block[17];
	saved_cmdr.left_laser = block[18];
	saved_cmdr.right_laser = block[19];

	saved_cmdr.cargo_capacity = block[22] - 2;

	for (i = 0; i < NO_OF_STOCK_ITEMS; i++)
		saved_cmdr.current_cargo[i] = block[23+i];
	
	saved_cmdr.ecm = block[40];
	saved_cmdr.fuel_scoop = block[41];
	saved_cmdr.energy_bomb = block[42];
	saved_cmdr.energy_unit = block[43];
	saved_cmdr.docking_computer = block[44];
	saved_cmdr.galactic_hyperdrive = block[45];
	saved_cmdr.escape_pod = block[46];
	saved_cmdr.missiles = block[51];
	saved_cmdr.legal_status = block[52];
	
	for (i = 0; i < NO_OF_STOCK_ITEMS; i++)
		saved_cmdr.station_stock[i] = block[53+i];
	
	saved_cmdr.market_rnd = block[70];

	saved_cmdr.score = block[71];
	saved_cmdr.score += block[72] << 8;

	if (fclose (fp) == EOF)
		return 1;	

	return 0;
}




