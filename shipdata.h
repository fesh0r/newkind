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


#ifndef SHIPDATA_H
#define SHIPDATA_H

#include "vector.h"

#define NO_OF_SHIPS		33



struct ship_point
{
	int x;
	int y;
	int z;
	int dist;
	int face1;
	int face2;
	int face3;
	int face4;
};


struct ship_line
{
	int dist;
	int face1;
	int face2;
	int start_point;
	int end_point;
};


struct ship_face_normal
{
	int dist;
	int x;
	int y;
	int z;
};



struct ship_data
{
	char name[32];
	int num_points;
	int num_lines;
	int num_faces;
	int max_loot;
	int scoop_type;
	double size;
	int front_laser;
	int bounty;
	int vanish_point;
	int energy;
	int velocity;
	int missiles;
	int laser_strength;
	struct ship_point *points;
	struct ship_line *lines;
	struct ship_face_normal *normals;
};


#define SHIP_SUN				-2
#define SHIP_PLANET				-1
#define SHIP_MISSILE			1
#define SHIP_CORIOLIS			2
#define SHIP_ESCAPE_CAPSULE		3
#define SHIP_ALLOY				4
#define SHIP_CARGO				5
#define SHIP_BOULDER			6
#define SHIP_ASTEROID			7
#define SHIP_ROCK				8
#define SHIP_SHUTTLE			9
#define SHIP_TRANSPORTER		10
#define SHIP_COBRA3				11
#define SHIP_PYTHON				12
#define SHIP_BOA				13
#define SHIP_ANACONDA			14
#define SHIP_HERMIT				15
#define SHIP_VIPER				16
#define SHIP_SIDEWINDER			17
#define SHIP_MAMBA				18
#define SHIP_KRAIT				19
#define SHIP_ADDER				20
#define SHIP_GECKO				21
#define SHIP_COBRA1				22
#define SHIP_WORM				23
#define SHIP_COBRA3_LONE		24
#define SHIP_ASP2				25
#define SHIP_PYTHON_LONE		26
#define SHIP_FER_DE_LANCE		27
#define SHIP_MORAY				28
#define SHIP_THARGOID			29
#define SHIP_THARGLET			30
#define SHIP_CONSTRICTOR		31
#define SHIP_COUGAR				32
#define SHIP_DODEC				33

extern struct ship_data missile_data;
extern struct ship_data coriolis_data;
extern struct ship_data esccaps_data;
extern struct ship_data alloy_data;
extern struct ship_data cargo_data;
extern struct ship_data boulder_data;
extern struct ship_data asteroid_data;
extern struct ship_data rock_data;
extern struct ship_data orbit_data;
extern struct ship_data transp_data;
extern struct ship_data cobra3a_data;
extern struct ship_data pythona_data;
extern struct ship_data boa_data;
extern struct ship_data anacnda_data;
extern struct ship_data hermit_data;
extern struct ship_data viper_data;
extern struct ship_data sidewnd_data;
extern struct ship_data mamba_data;
extern struct ship_data krait_data;
extern struct ship_data adder_data;
extern struct ship_data gecko_data;
extern struct ship_data cobra1_data;
extern struct ship_data worm_data;
extern struct ship_data cobra3b_data;
extern struct ship_data asp2_data;
extern struct ship_data pythonb_data;
extern struct ship_data ferdlce_data;
extern struct ship_data moray_data;
extern struct ship_data thargoid_data;
extern struct ship_data thargon_data;
extern struct ship_data constrct_data;
extern struct ship_data cougar_data;
extern struct ship_data dodec_data;


#endif

