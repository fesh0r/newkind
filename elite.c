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

#include <stdlib.h>

#include "config.h"
#include "elite.h"
#include "vector.h"
#include "planet.h"
#include "shipdata.h"


struct galaxy_seed docked_planet;

struct galaxy_seed hyperspace_planet;

struct planet_data current_planet_data;

int curr_galaxy_num = 1;
int curr_fuel = 70;
int carry_flag = 0;
int current_screen = 0;
int witchspace;

int wireframe = 0;
int anti_alias_gfx = 0;
int hoopy_casinos = 0;
int speed_cap = 75;
int instant_dock = 0;


char scanner_filename[256];
int scanner_cx;
int scanner_cy;
int compass_centre_x;
int compass_centre_y;

int planet_render_style = 0;

int game_over;
int docked;
int finish;
int flight_speed;
int flight_roll;
int flight_climb;
int front_shield;
int aft_shield;
int energy;
int laser_temp;
int detonate_bomb;
int auto_pilot;


struct commander saved_cmdr =
{
	"JAMESON",									/* Name 			*/
	0,											/* Mission Number 	*/
	0x14,0xAD,									/* Ship X,Y			*/
	{0x4a, 0x5a, 0x48, 0x02, 0x53, 0xb7},		/* Galaxy Seed		*/
	1000,										/* Credits * 10		*/
	70,											/* Fuel	* 10		*/
	0,
	0,											/* Galaxy - 1		*/
	PULSE_LASER,								/* Front Laser		*/
	0,											/* Rear Laser		*/
	0,											/* Left Laser		*/
	0,											/* Right Laser		*/
	0, 0,
	20,											/* Cargo Capacity	*/
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},		/* Current Cargo	*/
	0,											/* ECM				*/
	0,											/* Fuel Scoop		*/
	0,											/* Energy Bomb		*/
	0,											/* Energy Unit		*/
	0,											/* Docking Computer */
	0,											/* Galactic H'Drive	*/
	0,											/* Escape Pod		*/
	0,0,0,0,
	3,											/* No. of Missiles	*/
	0,											/* Legal Status		*/
	{0x10, 0x0F, 0x11, 0x00, 0x03, 0x1C,		/* Station Stock	*/
	 0x0E, 0x00, 0x00, 0x0A, 0x00, 0x11,
	 0x3A, 0x07, 0x09, 0x08, 0x00},
	0,											/* Fluctuation		*/
	0,											/* Score			*/
	0x80										/* Saved			*/
};

struct commander cmdr;

struct player_ship myship;


struct ship_data *ship_list[NO_OF_SHIPS + 1] =
{
	NULL,
	&missile_data,
	&coriolis_data,
	&esccaps_data,
	&alloy_data,
	&cargo_data,
	&boulder_data,
	&asteroid_data,
	&rock_data,
	&orbit_data,
	&transp_data,
	&cobra3a_data,
	&pythona_data,
	&boa_data,
	&anacnda_data,
	&hermit_data,
	&viper_data,
	&sidewnd_data,
	&mamba_data,
	&krait_data,
	&adder_data,
	&gecko_data,
	&cobra1_data,
	&worm_data,
	&cobra3b_data,
	&asp2_data,
	&pythonb_data,
	&ferdlce_data,
	&moray_data,
	&thargoid_data,
	&thargon_data,
	&constrct_data,
	&cougar_data,
	&dodec_data
};



void restore_saved_commander (void)
{
	cmdr = saved_cmdr;

	docked_planet = find_planet (cmdr.ship_x, cmdr.ship_y);
	hyperspace_planet = docked_planet;

	generate_planet_data (&current_planet_data, docked_planet);
	generate_stock_market ();
	set_stock_quantities (cmdr.station_stock);
}

