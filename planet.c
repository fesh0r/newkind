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
 */

/*
 *
 * Handle the generation of planet info...
 */

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "config.h"
#include "gfx.h"
#include "elite.h"
#include "planet.h"
#include "missions.h"


extern struct galaxy_seed hyperspace_planet;

struct random_seed
{
	int a;
	int b;
	int c;
	int d;
};


static struct random_seed rnd_seed;

static char *digrams="ABOUSEITILETSTONLONUTHNOALLEXEGEZACEBISOUSESARMAINDIREA?ERATENBERALAVETIEDORQUANTEISRION";

static char *inhabitant_desc1[] = {"Large ", "Fierce ", "Small "};

static char *inhabitant_desc2[] = {"Green ", "Red ", "Yellow ", "Blue ", "Black ", "Harmless "};

static char *inhabitant_desc3[] = {"Slimy ", "Bug-Eyed ", "Horned ", "Bony ", "Fat ", "Furry "};

static char *inhabitant_desc4[] = {"Rodent", "Frog", "Lizard", "Lobster", "Bird", "Humanoid", "Feline", "Insect"};


static char planet_description[300];
static char *desc_ptr;




static char *desc_list[36][5] =
{
/*  0	*/	{"fabled", "notable", "well known", "famous", "noted"},
/*  1	*/	{"very", "mildly", "most", "reasonably", ""},
/*  2	*/	{"ancient", "<20>", "great", "vast", "pink"},
/*  3	*/	{"<29> <28> plantations", "mountains", "<27>", "<19> forests", "oceans"},
/*  4	*/	{"shyness", "silliness", "mating traditions", "loathing of <5>", "love for <5>"},
/*  5	*/	{"food blenders", "tourists", "poetry", "discos", "<13>"},
/*  6	*/	{"talking tree", "crab", "bat", "lobst", "%R"},
/*  7	*/	{"beset", "plagued", "ravaged", "cursed", "scourged"},
/*  8	*/	{"<21> civil war", "<26> <23> <24>s", "a <26> disease", "<21> earthquakes", "<21> solar activity"},
/*  9	*/	{"its <2> <3>", "the %I <23> <24>","its inhabitants' <25> <4>", "<32>", "its <12> <13>"},
/* 10	*/	{"juice", "brandy", "water", "brew", "gargle blasters"},
/* 11	*/	{"%R", "%I <24>", "%I %R", "%I <26>", "<26> %R"},
/* 12	*/	{"fabulous", "exotic", "hoopy", "unusual", "exciting"},
/* 13	*/	{"cuisine", "night life", "casinos", "sit coms", " <32> "},
/* 14	*/	{"%H", "The planet %H", "The world %H", "This planet", "This world"},
/* 15	*/	{"n unremarkable", " boring", " dull", " tedious", " revolting"},
/* 16	*/	{"planet", "world", "place", "little planet", "dump"},
/* 17	*/	{"wasp", "moth", "grub", "ant", "%R"},
/* 18	*/	{"poet", "arts graduate", "yak", "snail", "slug"},
/* 19	*/	{"tropical", "dense", "rain", "impenetrable", "exuberant"},
/* 20	*/	{"funny", "wierd", "unusual", "strange", "peculiar"},
/* 21	*/	{"frequent", "occasional", "unpredictable", "dreadful", "deadly"},
/* 22	*/	{"<1> <0> for <9>", "<1> <0> for <9> and <9>", "<7> by <8>", "<1> <0> for <9> but <7> by <8>"," a<15> <16>"},
/* 23	*/	{"<26>", "mountain", "edible", "tree", "spotted"},
/* 24	*/	{"<30>", "<31>", "<6>oid", "<18>", "<17>"},
/* 25	*/	{"ancient", "exceptional", "eccentric", "ingrained", "<20>"},
/* 26	*/	{"killer", "deadly", "evil", "lethal", "vicious"},
/* 27	*/	{"parking meters", "dust clouds", "ice bergs", "rock formations", "volcanoes"},
/* 28	*/	{"plant", "tulip", "banana", "corn", "%Rweed"},
/* 29	*/	{"%R", "%I %R", "%I <26>", "inhabitant", "%I %R"},
/* 30	*/	{"shrew", "beast", "bison", "snake", "wolf"},
/* 31	*/	{"leopard", "cat", "monkey", "goat", "fish"},
/* 32	*/	{"<11> <10>", "%I <30> <33>","its <12> <31> <33>", "<34> <35>", "<11> <10>"},
/* 33	*/	{"meat", "cutlet", "steak", "burgers", "soup"},
/* 34	*/	{"ice", "mud", "Zero-G", "vacuum", "%I ultra"},
/* 35	*/	{"hockey", "cricket", "karate", "polo", "tennis"}
};








/*
 * Generate a random number between 0 and 255.
 * This is the version used in the 6502 Elites.
 */

int gen_rnd_number (void)
{
	int a,x;

	x = (rnd_seed.a * 2) & 0xFF;
	a = x + rnd_seed.c;
	if (rnd_seed.a > 127)
		a++;
	rnd_seed.a = a & 0xFF;
	rnd_seed.c = x;

	a = a / 256;	/* a = any carry left from above */
	x = rnd_seed.b;
	a = (a + x + rnd_seed.d) & 0xFF;
	rnd_seed.b = a;
	rnd_seed.d = x;
	return a;
}


/*
 * Generate a random number between 0 and 255.
 * This is the version used in the MSX and 16bit Elites.
 */


int gen_msx_rnd_number (void)
{
    int a,b;

	a = rnd_seed.a;
	b = rnd_seed.b;
	
	rnd_seed.a = rnd_seed.c;
	rnd_seed.b = rnd_seed.d;
	
	a += rnd_seed.c;
	b = (b + rnd_seed.d) & 255;
	if (a > 255)
	{
		a &= 255;
		b++;
	}
	
	rnd_seed.c = a;
	rnd_seed.d = b;
	
	return rnd_seed.c / 0x34;
}


void waggle_galaxy (struct galaxy_seed *glx_ptr)
{
    unsigned int x;
	unsigned int y;
	extern int carry_flag;

	x = glx_ptr->a + glx_ptr->c;
    y = glx_ptr->b + glx_ptr->d;


	if (x > 0xFF)
	    y++;

	x &= 0xFF;
	y &= 0xFF;

	glx_ptr->a = glx_ptr->c;
	glx_ptr->b = glx_ptr->d;
	glx_ptr->c = glx_ptr->e;
	glx_ptr->d = glx_ptr->f;

    x += glx_ptr->c;
	y += glx_ptr->d;


	if (x > 0xFF)
		y++;

	if (y > 0xFF)
		carry_flag = 1;
	else
		carry_flag = 0;

    x &= 0xFF;
	y &= 0xFF;

	glx_ptr->e = x;
	glx_ptr->f = y;
}




struct galaxy_seed find_planet (int cx, int cy)
{
    int min_dist = 10000;
	struct galaxy_seed glx;
	struct galaxy_seed planet;
	int distance;
	int dx, dy;
	int i;

	glx = cmdr.galaxy;

	for (i = 0; i < 256; i++)
	{

		dx = abs(cx - glx.d);
		dy = abs(cy - glx.b);

		if (dx > dy)
			distance = (dx + dx + dy) / 2;
		else
			distance = (dx + dy + dy) / 2;

		if (distance < min_dist)
		{
			min_dist = distance;
			planet = glx;
		}

		waggle_galaxy (&glx);
		waggle_galaxy (&glx);
		waggle_galaxy (&glx);
		waggle_galaxy (&glx);
	}

	return planet;
}


int find_planet_number (struct galaxy_seed planet)
{
	struct galaxy_seed glx;
	int i;

	glx = cmdr.galaxy;

	for (i = 0; i < 256; i++)
	{

		if ((planet.a == glx.a) &&
			(planet.b == glx.b) &&
			(planet.c == glx.c) &&
			(planet.d == glx.d) &&
			(planet.e == glx.e) &&
			(planet.f == glx.f))
			return i;
	
		waggle_galaxy (&glx);
		waggle_galaxy (&glx);
		waggle_galaxy (&glx);
		waggle_galaxy (&glx);
	}

	return -1;
}



void name_planet (char *gname, struct galaxy_seed glx)
{
    int size;
	int i;
	char *gp;
	unsigned int x;


	gp = gname;

	if ((glx.a & 0x40) == 0)
		size = 3;
	else
		size = 4;

	for (i = 0; i < size; i++)
	{
		x = glx.f & 0x1F;
		if (x != 0)
		{
			x += 12;
			x *= 2;
			*gp++ = digrams[x];
			if (digrams[x+1] != '?')
				*gp++ = digrams[x+1];
		}

		waggle_galaxy (&glx);
	}

	*gp = '\0';
}


void capitalise_name (char *name)
{
    char *ptr = name;

	if (*ptr == '\0')
		return;

	*ptr = toupper(*ptr);
	ptr++;

	while (*ptr != '\0')
	{
	    *ptr = tolower(*ptr);
		ptr++;
	}
}


void describe_inhabitants (char *str, struct galaxy_seed planet)
{
	int inhab;

	strcpy (str, "(");

	if (planet.e < 128)
	{
		strcat (str, "Human Colonial");
	}
	else
	{
		inhab = (planet.f / 4) & 7;
		if (inhab < 3)
			strcat (str, inhabitant_desc1[inhab]);

		inhab = planet.f / 32;
		if (inhab < 6)
			strcat (str, inhabitant_desc2[inhab]);

		inhab = (planet.d ^ planet.b) & 7;
		if (inhab < 6)
			strcat (str, inhabitant_desc3[inhab]);

		inhab = (inhab + (planet.f & 3)) & 7;
		strcat (str, inhabitant_desc4[inhab]);
	}

	strcat (str, "s)");
}



void expand_description (char *source)
{
	char str[32];
	char *ptr;
	int num;
	int rnd;
	int option;
	int i, len, x;

	while (*source != '\0')
	{
		if (*source == '<')
		{
			source++;
			ptr = str;
			while (*source != '>')
				*ptr++ = *source++;
			*ptr = '\0';
			source++;
			num = atoi(str);
			
			if (hoopy_casinos)
			{
				option = gen_msx_rnd_number();
			}
			else
			{
				rnd = gen_rnd_number();
				option = 0;
				if (rnd >= 0x33) option++;
				if (rnd >= 0x66) option++;
				if (rnd >= 0x99) option++;
				if (rnd >= 0xCC) option++;
			}
			
			expand_description (desc_list[num][option]);
			continue;
		}

		if (*source == '%')
		{
			source++;
			switch (*source)
			{
				case 'H':
					name_planet (str, hyperspace_planet);
					capitalise_name (str);
					for (ptr = str; *ptr != '\0';)
						*desc_ptr++ = *ptr++;
					break;

				case 'I':
					name_planet (str, hyperspace_planet);
					capitalise_name (str);
					for (ptr = str; *ptr != '\0';)
						*desc_ptr++ = *ptr++;
						desc_ptr--;
					strcpy (desc_ptr, "ian");
					desc_ptr += 3;
					break;

				case 'R':
					len = gen_rnd_number() & 3;
					for (i = 0; i <= len; i++)
					{
						x = gen_rnd_number() & 0x3e;
						if (i == 0)
						    *desc_ptr++ = digrams[x];
						else
							*desc_ptr++ = tolower(digrams[x]);
						*desc_ptr++ = tolower(digrams[x+1]);
					}

			}

			source++;
			continue;
		}

		*desc_ptr++ = *source++;
	}



	*desc_ptr = '\0';
}



char *describe_planet (struct galaxy_seed planet)
{
	char *mission_text;
	
	if (cmdr.mission == 1)
	{
		mission_text = mission_planet_desc (planet);
		if (mission_text != NULL)
			return mission_text;
	}
	
	rnd_seed.a = planet.c;
	rnd_seed.b = planet.d;
	rnd_seed.c = planet.e;
	rnd_seed.d = planet.f;

	if (hoopy_casinos)
	{
		rnd_seed.a ^= planet.a;
		rnd_seed.b ^= planet.b;
		rnd_seed.c ^= rnd_seed.a;
		rnd_seed.d ^= rnd_seed.b;
	}
	
	desc_ptr = planet_description;

	expand_description ("<14> is <22>.");

	return planet_description;
}



void generate_planet_data (struct planet_data *pl, struct galaxy_seed planet_seed)
{

	pl->government = (planet_seed.c / 8) & 7;

	pl->economy = planet_seed.b & 7;

	if (pl->government < 2)
		pl->economy = pl->economy | 2;

	pl->techlevel = pl->economy ^ 7;
	pl->techlevel += planet_seed.d & 3;
	pl->techlevel += (pl->government / 2) + (pl->government & 1);


	pl->population = pl->techlevel * 4;
	pl->population += pl->government;
	pl->population += pl->economy;
	pl->population++;

	pl->productivity = (pl->economy ^ 7) + 3;
	pl->productivity *= pl->government + 4;
	pl->productivity *= pl->population;
	pl->productivity *= 8;

	pl->radius = (((planet_seed.f & 15) + 11) * 256) + planet_seed.d;
}



