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

#ifndef PLANET_H
#define PLANET_H


struct galaxy_seed
{
	unsigned char a;	/* 6c */
	unsigned char b;	/* 6d */
	unsigned char c;	/* 6e */
	unsigned char d;	/* 6f */
	unsigned char e;	/* 70 */
	unsigned char f;	/* 71 */
};


struct planet_data
{
	int government;
	int economy;
	int techlevel;
	int population;
	int productivity;
	int radius;
};





char *describe_planet (struct galaxy_seed);
void capitalise_name (char *name);
void name_planet (char *gname, struct galaxy_seed glx);
struct galaxy_seed find_planet (int cx, int cy);
int find_planet_number (struct galaxy_seed planet);
void waggle_galaxy (struct galaxy_seed *glx_ptr);
void describe_inhabitants (char *str, struct galaxy_seed planet);
void generate_planet_data (struct planet_data *pl, struct galaxy_seed planet_seed);
void set_current_planet (struct galaxy_seed new_planet);

#endif

