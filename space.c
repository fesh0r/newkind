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
 * space.c
 *
 * This module handles all the flight system and management of the space universe.
 */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "vector.h"

#include "alg_data.h"

#include "config.h"
#include "elite.h"
#include "gfx.h"
#include "docked.h"
#include "intro.h"
#include "shipdata.h"
#include "shipface.h"
#include "space.h" 
#include "threed.h"
#include "sound.h"
#include "main.h"
#include "swat.h"
#include "random.h"
#include "trade.h"
#include "stars.h"
#include "pilot.h"

extern int flight_climb;
extern int flight_roll;
extern int flight_speed;

struct galaxy_seed destination_planet;
int hyper_ready;
int hyper_countdown;
char hyper_name[16];
int hyper_distance;
int hyper_galactic;






void rotate_x_first (double *a, double *b, int direction)
{
	double fx,ux;

	fx = *a;
	ux = *b;

	if (direction < 0)
	{	
		*a = fx - (fx / 512) + (ux / 19);
		*b = ux - (ux / 512) - (fx / 19);
	}
	else
	{
		*a = fx - (fx / 512) - (ux / 19);
		*b = ux - (ux / 512) + (fx / 19);
	}
}


void rotate_vec (struct vector *vec, double alpha, double beta)
{
	double x,y,z;
	
	x = vec->x;
	y = vec->y;
	z = vec->z;

	y = y - alpha * x;
	x = x + alpha * y;
	y = y - beta * z;
	z = z + beta * y;
	
	vec->x = x;
	vec->y = y;
	vec->z = z;
}


/*
 * Update an objects location in the universe.
 */

void move_univ_object (struct univ_object *obj)
{
	double x,y,z;
	double k2;
	double alpha;
	double beta;
	int rotx,rotz;
	double speed;
	
	alpha = flight_roll / 256.0;
	beta = flight_climb / 256.0;
	
	x = obj->location.x;
	y = obj->location.y;
	z = obj->location.z;

	if (!(obj->flags & FLG_DEAD))
	{ 
		if (obj->velocity != 0)
		{
			speed = obj->velocity;
			speed *= 1.5; 	
			x += obj->rotmat[2].x * speed; 
			y += obj->rotmat[2].y * speed; 
			z += obj->rotmat[2].z * speed; 
		}

		if (obj->acceleration != 0)
		{
			obj->velocity += obj->acceleration;
			obj->acceleration = 0;
			if (obj->velocity > ship_list[obj->type]->velocity)
				obj->velocity = ship_list[obj->type]->velocity;
			
			if (obj->velocity <= 0)
				obj->velocity = 1;
		}
	}
	
	k2 = y - alpha * x;
	z = z + beta * k2;
	y = k2 - z * beta;
	x = x + alpha * y;

	z = z - flight_speed;

	obj->location.x = x;
	obj->location.y = y;
	obj->location.z = z;	

	obj->distance = sqrt (x*x + y*y + z*z);
	
	if (obj->type == SHIP_PLANET)
		beta = 0.0;
	
	rotate_vec (&obj->rotmat[2], alpha, beta);
	rotate_vec (&obj->rotmat[1], alpha, beta);
	rotate_vec (&obj->rotmat[0], alpha, beta);

	if (obj->flags & FLG_DEAD)
		return;


	rotx = obj->rotx;
	rotz = obj->rotz;
	
	/* If necessary rotate the object around the X axis... */

	if (rotx != 0)
	{
		rotate_x_first (&obj->rotmat[2].x, &obj->rotmat[1].x, rotx);
		rotate_x_first (&obj->rotmat[2].y, &obj->rotmat[1].y, rotx);	
		rotate_x_first (&obj->rotmat[2].z, &obj->rotmat[1].z, rotx);

		if ((rotx != 127) && (rotx != -127))
			obj->rotx -= (rotx < 0) ? -1 : 1;
	}	

	
	/* If necessary rotate the object around the Z axis... */

	if (rotz != 0)
	{	
		rotate_x_first (&obj->rotmat[0].x, &obj->rotmat[1].x, rotz);
		rotate_x_first (&obj->rotmat[0].y, &obj->rotmat[1].y, rotz);	
		rotate_x_first (&obj->rotmat[0].z, &obj->rotmat[1].z, rotz);	

		if ((rotz != 127) && (rotz != -127))
			obj->rotz -= (rotz < 0) ? -1 : 1;
	}


	/* Orthonormalize the rotation matrix... */

	tidy_matrix (obj->rotmat);
}


/*
 * Dock the player into the space station.
 */

void dock_player (void)
{
	disengage_auto_pilot();
	docked = 1;
	flight_speed = 0;
	flight_roll = 0;
	flight_climb = 0;
	front_shield = 255;
	aft_shield = 255;
	energy = 255;
	myship.altitude = 255;
	myship.cabtemp = 30;
	reset_weapons();
}


/*
 * Check if we are correctly aligned to dock.
 */

int is_docking (int sn)
{
	struct vector vec;
	double fz;
	double ux;

	if (auto_pilot)		// Don't want it to kill anyone!
		return 1;
	
	fz = universe[sn].rotmat[2].z;

	if (fz > -0.90)
		return 0;
	
	vec = unit_vector (&universe[sn].location);

	if (vec.z < 0.927)
		return 0;
	
	ux = universe[sn].rotmat[1].x;
	if (ux < 0)
		ux = -ux;
	
	if (ux < 0.84)
		return 0;
	 
	return 1;
}


/*
 * Game Over...
 */

void do_game_over (void)
{
	snd_play_sample (SND_GAMEOVER);
	game_over = 1;
}


void update_altitude (void)
{
	double x,y,z;
	double dist;
	
	myship.altitude = 255;

	if (witchspace)
		return;
	
	x = fabs(universe[0].location.x);
	y = fabs(universe[0].location.y);
	z = fabs(universe[0].location.z);
	
	if ((x > 65535) || (y > 65535) || (z > 65535))
		return;

	x /= 256;
	y /= 256;
	z /= 256;
	
	dist = (x * x) + (y * y) + (z * z);

	if (dist > 65535)
		return;
	
	dist -= 9472;
	if (dist < 1)
	{
		myship.altitude = 0;
		do_game_over ();
		return;
	}

	dist = sqrt (dist);
	if (dist < 1)
	{
		myship.altitude = 0;
		do_game_over ();
		return;
	}

	myship.altitude = dist;	
}


void update_cabin_temp (void)
{
	int x,y,z;
	int dist;
	
	myship.cabtemp = 30;

	if (witchspace)
		return;
	
	if (ship_count[SHIP_CORIOLIS] || ship_count[SHIP_DODEC])
		return;
	
	x = abs((int)universe[1].location.x);
	y = abs((int)universe[1].location.y);
	z = abs((int)universe[1].location.z);
	
	if ((x > 65535) || (y > 65535) || (z > 65535))
		return;

	x /= 256;
	y /= 256;
	z /= 256;
	
	dist = ((x * x) + (y * y) + (z * z)) / 256;

	if (dist > 255)
		return;

  	dist ^=  255;

	myship.cabtemp = dist + 30;

	if (myship.cabtemp > 255)
	{
		myship.cabtemp = 255;
		do_game_over ();
		return;
	}
	
	if ((myship.cabtemp < 224) || (cmdr.fuel_scoop == 0))
		return;

	cmdr.fuel += flight_speed / 2;
	if (cmdr.fuel > myship.max_fuel)
		cmdr.fuel = myship.max_fuel;

	info_message ("Fuel Scoop On");	
}



/*
 * Regenerate the shields and the energy banks.
 */

void regenerate_shields (void)
{
	if (energy > 127)
	{
		if (front_shield < 255)
		{
			front_shield++;
			energy--;
		}
	
		if (aft_shield < 255)
		{
			aft_shield++;
			energy--;
		}
	}
		
	energy++;
	energy += cmdr.energy_unit;
	if (energy > 255)
		energy = 255;
}


void decrease_energy (int amount)
{
	energy += amount;

	if (energy <= 0)
		do_game_over();
}


/*
 * Deplete the shields.  Drain the energy banks if the shields fail.
 */

void damage_ship (int damage, int front)
{
	int shield;

	if (damage <= 0)	/* sanity check */
		return;
	
	shield = front ? front_shield : aft_shield;
	
	shield -= damage;
	if (shield < 0)
	{
		decrease_energy (shield);
		shield = 0;
	}
	
	if (front)
		front_shield = shield;
	else
		aft_shield = shield;
}




void make_station_appear (void)
{
	double px,py,pz;
	double sx,sy,sz;
	Vector vec;
	Matrix rotmat;
	
	px = universe[0].location.x;
	py = universe[0].location.y;
	pz = universe[0].location.z;
	
	vec.x = (rand() & 32767) - 16384;	
	vec.y = (rand() & 32767) - 16384;	
	vec.z = rand() & 32767;	

	vec = unit_vector (&vec);

	sx = px - vec.x * 65792;
	sy = py - vec.y * 65792;
	sz = pz - vec.z * 65792;

//	set_init_matrix (rotmat);
	
	rotmat[0].x = 1.0;
	rotmat[0].y = 0.0;
	rotmat[0].z = 0.0;

	rotmat[1].x = vec.x;
	rotmat[1].y = vec.z;
	rotmat[1].z = -vec.y;
	
	rotmat[2].x = vec.x;
	rotmat[2].y = vec.y;
	rotmat[2].z = vec.z;

	tidy_matrix (rotmat);
	
	add_new_station (sx, sy, sz, rotmat);
}



void check_docking (int i)
{
	if (is_docking(i))
	{
		snd_play_sample (SND_DOCK);					
		dock_player();
		current_screen = SCR_BREAK_PATTERN;
		return;
	}
					
	if (flight_speed >= 5)
	{
		do_game_over();
		return;
	}

	flight_speed = 1;
	damage_ship (5, universe[i].location.z > 0);
	snd_play_sample (SND_CRASH);
}


void switch_to_view (struct univ_object *flip)
{
	double tmp;
	
	if ((current_screen == SCR_REAR_VIEW) ||
		(current_screen == SCR_GAME_OVER))
	{
		flip->location.x = -flip->location.x;
		flip->location.z = -flip->location.z;

		flip->rotmat[0].x = -flip->rotmat[0].x;
		flip->rotmat[0].z = -flip->rotmat[0].z;

		flip->rotmat[1].x = -flip->rotmat[1].x;
		flip->rotmat[1].z = -flip->rotmat[1].z;

		flip->rotmat[2].x = -flip->rotmat[2].x;
		flip->rotmat[2].z = -flip->rotmat[2].z;
		return;
	}


	if (current_screen == SCR_LEFT_VIEW)
	{
		tmp = flip->location.x;
		flip->location.x = flip->location.z;
		flip->location.z = -tmp;

		if (flip->type < 0)
			return;
		
		tmp = flip->rotmat[0].x;
		flip->rotmat[0].x = flip->rotmat[0].z;
		flip->rotmat[0].z = -tmp;		

		tmp = flip->rotmat[1].x;
		flip->rotmat[1].x = flip->rotmat[1].z;
		flip->rotmat[1].z = -tmp;		

		tmp = flip->rotmat[2].x;
		flip->rotmat[2].x = flip->rotmat[2].z;
		flip->rotmat[2].z = -tmp;		
		return;
	}

	if (current_screen == SCR_RIGHT_VIEW)
	{
		tmp = flip->location.x;
		flip->location.x = -flip->location.z;
		flip->location.z = tmp;

		if (flip->type < 0)
			return;
		
		tmp = flip->rotmat[0].x;
		flip->rotmat[0].x = -flip->rotmat[0].z;
		flip->rotmat[0].z = tmp;		

		tmp = flip->rotmat[1].x;
		flip->rotmat[1].x = -flip->rotmat[1].z;
		flip->rotmat[1].z = tmp;		

		tmp = flip->rotmat[2].x;
		flip->rotmat[2].x = -flip->rotmat[2].z;
		flip->rotmat[2].z = tmp;		

	}
}


/*
 * Update all the objects in the universe and render them.
 */

void update_universe (void)
{
	int i;
	int type;
	int bounty;
	char str[80];
	struct univ_object flip;
	
	
	gfx_start_render();
				 	
	for (i = 0; i < MAX_UNIV_OBJECTS; i++)
	{
		type = universe[i].type;
		
		if (type != 0)
		{
			if (universe[i].flags & FLG_REMOVE)
			{
				if (type == SHIP_VIPER)
					cmdr.legal_status |= 64;
			
				bounty = ship_list[type]->bounty;
				
				if ((bounty != 0) && (!witchspace))
				{
					cmdr.credits += bounty;
					sprintf (str, "%d.%d CR", cmdr.credits / 10, cmdr.credits % 10);
					info_message (str);
				}
				
				remove_ship (i);
				continue;
			}

			if ((detonate_bomb) && ((universe[i].flags & FLG_DEAD) == 0) &&
				(type != SHIP_PLANET) && (type != SHIP_SUN) &&
				(type != SHIP_CONSTRICTOR) && (type != SHIP_COUGAR) &&
				(type != SHIP_CORIOLIS) && (type != SHIP_DODEC))
			{
				snd_play_sample (SND_EXPLODE);
				universe[i].flags |= FLG_DEAD;		
			}

			if ((current_screen != SCR_INTRO_ONE) &&
				(current_screen != SCR_INTRO_TWO) &&
				(current_screen != SCR_GAME_OVER) &&
				(current_screen != SCR_ESCAPE_POD))
			{
				tactics (i);
			} 
		
			move_univ_object (&universe[i]);

			flip = universe[i];
			switch_to_view (&flip);
			
			if (type == SHIP_PLANET)
			{
				if ((ship_count[SHIP_CORIOLIS] == 0) &&
					(ship_count[SHIP_DODEC] == 0) &&
					(universe[i].distance < 65792)) // was 49152
				{
					make_station_appear();
				}				

				draw_ship (&flip);
				continue;
			}

			if (type == SHIP_SUN)
			{
				draw_ship (&flip);
				continue;
			}
			
			
			if (universe[i].distance < 170)
			{
				if ((type == SHIP_CORIOLIS) || (type == SHIP_DODEC))
					check_docking (i);
				else
					scoop_item(i);
				
				continue;
			}

			if (universe[i].distance > 57344)
			{
				remove_ship (i);
				continue;
			}

			draw_ship (&flip);

			universe[i].flags = flip.flags;
			universe[i].exp_seed = flip.exp_seed;
			universe[i].exp_delta = flip.exp_delta;
			
			universe[i].flags &= ~FLG_FIRING;
			
			if (universe[i].flags & FLG_DEAD)
				continue;

			check_target (i, &flip);
		}
	}

	gfx_finish_render();
	detonate_bomb = 0;
}




/*
 * Update the scanner and draw all the lollipops.
 */

void update_scanner (void)
{
	int i;
	int x,y,z;
	int x1,y1,y2;
	int colour;
	
	for (i = 0; i < MAX_UNIV_OBJECTS; i++)
	{
		if ((universe[i].type <= 0) ||
			(universe[i].flags & FLG_DEAD) ||
			(universe[i].flags & FLG_CLOAKED))
			continue;
	
		x = universe[i].location.x / 256;
		y = universe[i].location.y / 256;
		z = universe[i].location.z / 256;

		x1 = x;
		y1 = -z / 4;
		y2 = y1 - y / 2;

		if ((y2 < -28) || (y2 > 28) ||
			(x1 < -50) || (x1 > 50))
			continue;

		x1 += scanner_cx;
		y1 += scanner_cy;
		y2 += scanner_cy;

		colour = (universe[i].flags & FLG_HOSTILE) ? GFX_COL_YELLOW_5 : GFX_COL_WHITE;
			
		switch (universe[i].type)
		{
			case SHIP_MISSILE:
				colour = 137;
				break;

			case SHIP_DODEC:
			case SHIP_CORIOLIS:
				colour = GFX_COL_GREEN_1;
				break;
				
			case SHIP_VIPER:
				colour = 252;
				break;
		}
			
		gfx_draw_colour_line (x1+2, y2,   x1-3, y2, colour);
		gfx_draw_colour_line (x1+2, y2+1, x1-3, y2+1, colour);
		gfx_draw_colour_line (x1+2, y2+2, x1-3, y2+2, colour);
		gfx_draw_colour_line (x1+2, y2+3, x1-3, y2+3, colour);


		gfx_draw_colour_line (x1,   y1, x1,   y2, colour);
		gfx_draw_colour_line (x1+1, y1, x1+1, y2, colour);
		gfx_draw_colour_line (x1+2, y1, x1+2, y2, colour);
	}
}


/*
 * Update the compass which tracks the space station / planet.
 */

void update_compass (void)
{
	struct vector dest;
	int compass_x;
	int compass_y;
	int un = 0;

	if (witchspace)
		return;
	
	if (ship_count[SHIP_CORIOLIS] || ship_count[SHIP_DODEC])
		un = 1;
	
	dest = unit_vector (&universe[un].location);
	
	compass_x = compass_centre_x + (dest.x * 16);
	compass_y = compass_centre_y + (dest.y * -16);
	
	if (dest.z < 0)
	{
		gfx_draw_sprite (IMG_RED_DOT, compass_x, compass_y);
	}
	else
	{
		gfx_draw_sprite (IMG_GREEN_DOT, compass_x, compass_y);
	}
				
}


/*
 * Display the speed bar.
 */

void display_speed (void)
{
	int sx,sy;
	int i;
	int len;
	int colour;

	sx = 417;
	sy = 384 + 9;

	len = ((flight_speed * 64) / myship.max_speed) - 1;

	colour = (flight_speed > (myship.max_speed * 2 / 3)) ? GFX_COL_DARK_RED : GFX_COL_GOLD;

	for (i = 0; i < 6; i++)
	{
		gfx_draw_colour_line (sx, sy + i, sx + len, sy + i, colour);
	}
}


/*
 * Draw an indicator bar.
 * Used for shields and energy banks.
 */

void display_dial_bar (int len, int x, int y)
{
	int i = 0;

	gfx_draw_colour_line (x, y + 384, x + len, y + 384, GFX_COL_GOLD);
	i++;
	gfx_draw_colour_line (x, y + i + 384, x + len, y + i + 384, GFX_COL_GOLD);
	
	for (i = 2; i < 7; i++)
		gfx_draw_colour_line (x, y + i + 384, x + len, y + i + 384, GFX_COL_YELLOW_1);

	gfx_draw_colour_line (x, y + i + 384, x + len, y + i + 384, GFX_COL_DARK_RED);
}


/*
 * Display the current shield strengths.
 */

void display_shields (void)
{
	if (front_shield > 3)
		display_dial_bar (front_shield / 4, 31, 7);

	if (aft_shield > 3)
		display_dial_bar (aft_shield / 4, 31, 23);
}


void display_altitude (void)
{
	if (myship.altitude > 3)
		display_dial_bar (myship.altitude / 4, 31, 92);
}

void display_cabin_temp (void)
{
	if (myship.cabtemp > 3)
		display_dial_bar (myship.cabtemp / 4, 31, 60);
}


void display_laser_temp (void)
{
	if (laser_temp > 0)
		display_dial_bar (laser_temp / 4, 31, 76);
}


/*
 * Display the energy banks.
 */

void display_energy (void)
{
	int e1,e2,e3,e4;

	e1 = energy > 64 ? 64 : energy;
	e2 = energy > 128 ? 64 : energy - 64;
	e3 = energy > 192 ? 64 : energy - 128;
	e4 = energy - 192;  	
	
	if (e4 > 0)
		display_dial_bar (e4, 416, 61);

	if (e3 > 0)
		display_dial_bar (e3, 416, 79);

	if (e2 > 0)
		display_dial_bar (e2, 416, 97);

	if (e1 > 0)
		display_dial_bar (e1, 416, 115);
}



void display_flight_roll (void)
{
	int sx,sy;
	int i;
	int pos;

	sx = 416;
	sy = 384 + 9 + 14;

	pos = sx - ((flight_roll * 28) / myship.max_roll);
	pos += 32;

	for (i = 0; i < 4; i++)
	{
		gfx_draw_colour_line (pos + i, sy, pos + i, sy + 7, GFX_COL_GOLD);
	}
}

void display_flight_climb (void)
{
	int sx,sy;
	int i;
	int pos;

	sx = 416;
	sy = 384 + 9 + 14 + 16;

	pos = sx + ((flight_climb * 28) / myship.max_climb);
	pos += 32;

	for (i = 0; i < 4; i++)
	{
		gfx_draw_colour_line (pos + i, sy, pos + i, sy + 7, GFX_COL_GOLD);
	}
}


void display_fuel (void)
{
	if (cmdr.fuel > 0)
		display_dial_bar ((cmdr.fuel * 64) / myship.max_fuel, 31, 44);
}


void display_missiles (void)
{
	int nomiss;
	int x,y;

	if (cmdr.missiles == 0)
		return;
	
	nomiss = cmdr.missiles > 4 ? 4 : cmdr.missiles;

	x = (4 - nomiss) * 16 + 35;
	y = 113 + 385;
	
	if (missile_target != MISSILE_UNARMED)
	{
		gfx_draw_sprite ((missile_target < 0) ? IMG_MISSILE_YELLOW :
											    IMG_MISSILE_RED, x, y);
		x += 16;
		nomiss--;
	}

	for (; nomiss > 0; nomiss--)
	{
		gfx_draw_sprite (IMG_MISSILE_GREEN, x, y);
		x += 16;
	}
}


void update_console (void)
{
	gfx_set_clip_region (0, 0, 512, 512);
	gfx_draw_scanner();
	
	display_speed();
	display_flight_climb();
	display_flight_roll();
	display_shields();
	display_altitude();
	display_energy();
	display_cabin_temp();
	display_laser_temp();
	display_fuel();
	display_missiles();
	
	if (docked)
		return;

	update_scanner();
	update_compass();

	if (ship_count[SHIP_CORIOLIS] || ship_count[SHIP_DODEC])
		gfx_draw_sprite (IMG_BIG_S, 387, 490);

	if (ecm_active)
		gfx_draw_sprite (IMG_BIG_E, 115, 490);
}

void increase_flight_roll (void)
{
	if (flight_roll < myship.max_roll)
		flight_roll++;
}


void decrease_flight_roll (void)
{
	if (flight_roll > -myship.max_roll)
		flight_roll--;
}


void increase_flight_climb (void)
{
	if (flight_climb < myship.max_climb)
		flight_climb++;
}

void decrease_flight_climb (void)
{
	if (flight_climb > -myship.max_climb)
		flight_climb--;
}


void start_hyperspace (void)
{
	if (hyper_ready)
		return;
		
	hyper_distance = calc_distance_to_planet (docked_planet, hyperspace_planet);

	if ((hyper_distance == 0) || (hyper_distance > cmdr.fuel))
		return;

	destination_planet = hyperspace_planet;
	name_planet (hyper_name, destination_planet);
	capitalise_name (hyper_name);
	
	hyper_ready = 1;
	hyper_countdown = 15;
	hyper_galactic = 0;

	disengage_auto_pilot();
}

void start_galactic_hyperspace (void)
{
	if (hyper_ready)
		return;

	if (cmdr.galactic_hyperdrive == 0)
		return;
		
	hyper_ready = 1;
	hyper_countdown = 2;
	hyper_galactic = 1;
	disengage_auto_pilot();
}



void display_hyper_status (void)
{
	char str[80];

	sprintf (str, "%d", hyper_countdown);	

	if ((current_screen == SCR_FRONT_VIEW) || (current_screen == SCR_REAR_VIEW) ||
		(current_screen == SCR_LEFT_VIEW) || (current_screen == SCR_RIGHT_VIEW))
	{
		gfx_display_text (5, 5, str);
		if (hyper_galactic)
		{
			gfx_display_centre_text (358, "Galactic Hyperspace", 120, GFX_COL_WHITE);
		}
		else
		{
			sprintf (str, "Hyperspace - %s", hyper_name);
			gfx_display_centre_text (358, str, 120, GFX_COL_WHITE);
		} 	
	}
	else
	{
		gfx_clear_area (5, 5, 25, 34);
		gfx_display_text (5, 5, str);
	}
}


int rotate_byte_left (int x)
{
	return ((x << 1) | (x >> 7)) & 255;
}

void enter_next_galaxy (void)
{
	cmdr.galaxy_number++;
	cmdr.galaxy_number &= 7;
	
	cmdr.galaxy.a = rotate_byte_left (cmdr.galaxy.a);
	cmdr.galaxy.b = rotate_byte_left (cmdr.galaxy.b);
	cmdr.galaxy.c = rotate_byte_left (cmdr.galaxy.c);
	cmdr.galaxy.d = rotate_byte_left (cmdr.galaxy.d);
	cmdr.galaxy.e = rotate_byte_left (cmdr.galaxy.e);
	cmdr.galaxy.f = rotate_byte_left (cmdr.galaxy.f);

	docked_planet = find_planet (0x60, 0x60);
	hyperspace_planet = docked_planet;
}





void enter_witchspace (void)
{
	int i;
	int nthg;

	witchspace = 1;
	docked_planet.b ^= 31;
	in_battle = 1;  

	flight_speed = 12;
	flight_roll = 0;
	flight_climb = 0;
	create_new_stars();
	clear_universe();

	nthg = (randint() & 3) + 1;
	
	for (i = 0; i < nthg; i++)
		create_thargoid();	
	
	current_screen = SCR_BREAK_PATTERN;
	snd_play_sample (SND_HYPERSPACE);
}


void complete_hyperspace (void)
{
	Matrix rotmat;
	int px,py,pz;
	
	hyper_ready = 0;
	witchspace = 0;
	
	if (hyper_galactic)
	{
		cmdr.galactic_hyperdrive = 0;
		enter_next_galaxy();
		cmdr.legal_status = 0;
	}
	else
	{
		cmdr.fuel -= hyper_distance;
		cmdr.legal_status /= 2;

		if ((rand255() > 253) || (flight_climb == myship.max_climb))
		{
			enter_witchspace();
			return;
		}

		docked_planet = destination_planet; 
	}

	cmdr.market_rnd = rand255();
	generate_planet_data (&current_planet_data, docked_planet);
	generate_stock_market ();
	
	flight_speed = 12;
	flight_roll = 0;
	flight_climb = 0;
	create_new_stars();
	clear_universe();

	generate_landscape(docked_planet.a * 251 + docked_planet.b);
	set_init_matrix (rotmat);

	pz = (((docked_planet.b) & 7) + 7) / 2;
	px = pz / 2;
	py = px;

	px <<= 16;
	py <<= 16;
	pz <<= 16;
	
	if ((docked_planet.b & 1) == 0)
	{
		px = -px;
		py = -py;
	}

	add_new_ship (SHIP_PLANET, px, py, pz, rotmat, 0, 0);


	pz = -(((docked_planet.d & 7) | 1) << 16);
	px = ((docked_planet.f & 3) << 16) | ((docked_planet.f & 3) << 8);

	add_new_ship (SHIP_SUN, px, py, pz, rotmat, 0, 0);

	current_screen = SCR_BREAK_PATTERN;
	snd_play_sample (SND_HYPERSPACE);
}


void countdown_hyperspace (void)
{
	if (hyper_countdown == 0)
	{
		complete_hyperspace();
		return;
	}

	hyper_countdown--;
}



void jump_warp (void)
{
	int i;
	int type;
	int jump;
	
	for (i = 0; i < MAX_UNIV_OBJECTS; i++)
	{
		type = universe[i].type;
		
		if ((type > 0) && (type != SHIP_ASTEROID) && (type != SHIP_CARGO) &&
			(type != SHIP_ALLOY) && (type != SHIP_ROCK) &&
			(type != SHIP_BOULDER) && (type != SHIP_ESCAPE_CAPSULE))
		{
			info_message ("Mass Locked");
			return;
		}
	}

	if ((universe[0].distance < 75001) || (universe[1].distance < 75001))
	{
		info_message ("Mass Locked");
		return;
	}


	if (universe[0].distance < universe[1].distance)
		jump = universe[0].distance - 75000;
	else
		jump = universe[1].distance - 75000;	

	if (jump > 1024)
		jump = 1024;
	
	for (i = 0; i < MAX_UNIV_OBJECTS; i++)
	{
		if (universe[i].type != 0)
			universe[i].location.z -= jump;
	}

	warp_stars = 1;
	mcount &= 63;
	in_battle = 0;
}


void launch_player (void)
{
	Matrix rotmat;

	docked = 0;
	flight_speed = 12;
	flight_roll = -15;
	flight_climb = 0;
	cmdr.legal_status |= carrying_contraband();
	create_new_stars();
	clear_universe();
	generate_landscape(docked_planet.a * 251 + docked_planet.b);
	set_init_matrix (rotmat);
	add_new_ship (SHIP_PLANET, 0, 0, 65536, rotmat, 0, 0);

	rotmat[2].x = -rotmat[2].x;
	rotmat[2].y = -rotmat[2].y;
	rotmat[2].z = -rotmat[2].z;
	add_new_station (0, 0, -256, rotmat);

	current_screen = SCR_BREAK_PATTERN;
	snd_play_sample (SND_LAUNCH);
}



/*
 * Engage the docking computer.
 * For the moment we just do an instant dock if we are in the safe zone.
 */

void engage_docking_computer (void)
{
	if (ship_count[SHIP_CORIOLIS] || ship_count[SHIP_DODEC])
	{
		snd_play_sample (SND_DOCK);					
		dock_player();
		current_screen = SCR_BREAK_PATTERN;
	}
}

