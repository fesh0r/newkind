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
 * swat.c
 *
 * Special Weapons And Tactics.
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "gfx.h"
#include "elite.h"
#include "vector.h"
#include "swat.h"
#include "shipdata.h"
#include "space.h"
#include "main.h"
#include "sound.h"
#include "random.h"
#include "trade.h"
#include "pilot.h" 

int laser_counter;
int laser;
int laser2;
int laser_x;
int laser_y;

int ecm_active;
int missile_target;
int ecm_ours;
int in_battle;

struct univ_object universe[MAX_UNIV_OBJECTS];
int ship_count[NO_OF_SHIPS + 1];  /* many */


int initial_flags[NO_OF_SHIPS + 1] =
{
	0,											// NULL,
	0,											// missile 
	0,											// coriolis
	FLG_SLOW | FLG_FLY_TO_PLANET,				// escape
	FLG_INACTIVE,								// alloy
	FLG_INACTIVE,								// cargo
	FLG_INACTIVE,								// boulder
	FLG_INACTIVE,								// asteroid
	FLG_INACTIVE,								// rock
	FLG_FLY_TO_PLANET | FLG_SLOW,				// shuttle
	FLG_FLY_TO_PLANET | FLG_SLOW,				// transporter
	0,											// cobra3
	0,											// python
	0,											// boa
	FLG_SLOW,									// anaconda
	FLG_SLOW,									// hermit
	FLG_BOLD | FLG_POLICE,						// viper
	FLG_BOLD | FLG_ANGRY,						// sidewinder
	FLG_BOLD | FLG_ANGRY,						// mamba
	FLG_BOLD | FLG_ANGRY,						// krait
	FLG_BOLD | FLG_ANGRY,						// adder
	FLG_BOLD | FLG_ANGRY,						// gecko
	FLG_BOLD | FLG_ANGRY,						// cobra1
	FLG_SLOW | FLG_ANGRY,						// worm
	FLG_BOLD | FLG_ANGRY,						// cobra3
	FLG_BOLD | FLG_ANGRY,						// asp2
	FLG_BOLD | FLG_ANGRY,						// python
	FLG_POLICE,									// fer_de_lance
	FLG_BOLD | FLG_ANGRY,						// moray
	FLG_BOLD | FLG_ANGRY,						// thargoid
	FLG_ANGRY,									// thargon
	FLG_ANGRY,									// constrictor
	FLG_POLICE | FLG_CLOAKED,					// cougar
	0											// dodec
};




void clear_universe (void)
{
	int i;

	for (i = 0; i < MAX_UNIV_OBJECTS; i++)
		universe[i].type = 0;

	for (i = 0; i <= NO_OF_SHIPS; i++)
		ship_count[i] = 0;

	in_battle = 0;
}


int add_new_ship (int ship_type, int x, int y, int z, struct vector *rotmat, int rotx, int rotz)
{
	int i;

	for (i = 0; i < MAX_UNIV_OBJECTS; i++)
	{
		if (universe[i].type == 0)
		{
			universe[i].type = ship_type;
			universe[i].location.x = x;
			universe[i].location.y = y;
			universe[i].location.z = z;
			
			universe[i].distance = sqrt(x*x + y*y + z*z);

			universe[i].rotmat[0] = rotmat[0];
			universe[i].rotmat[1] = rotmat[1];
			universe[i].rotmat[2] = rotmat[2];

			universe[i].rotx = rotx;
			universe[i].rotz = rotz;
			
			universe[i].velocity = 0;
			universe[i].acceleration = 0;
			universe[i].bravery = 0;
			universe[i].target = 0;
			
			universe[i].flags = initial_flags[ship_type];

			if ((ship_type != SHIP_PLANET) && (ship_type != SHIP_SUN))
			{
				universe[i].energy = ship_list[ship_type]->energy;
				universe[i].missiles = ship_list[ship_type]->missiles;
				ship_count[ship_type]++;
			}
			
			return i;
		}
	}

	return -1;
}




void check_missiles (int un)
{
	int i;
	
	if (missile_target == un)
	{
		missile_target = MISSILE_UNARMED;
		info_message ("Target Lost");
	}

	for (i = 0; i < MAX_UNIV_OBJECTS; i++)
	{
		if ((universe[i].type == SHIP_MISSILE) && (universe[i].target == un))
			universe[i].flags |= FLG_DEAD;
	}
}


void remove_ship (int un)
{
	int type;
	Matrix rotmat;
	int px,py,pz;
	
	type = universe[un].type;
	
	if (type == 0)
		return;

	if (type > 0)
		ship_count[type]--;

	universe[un].type = 0;		

	check_missiles (un);

	if ((type == SHIP_CORIOLIS) || (type == SHIP_DODEC))
	{
		set_init_matrix (rotmat);
		px = universe[un].location.x;
		py = universe[un].location.y;
		pz = universe[un].location.z;
		
		py &= 0xFFFF;
		py |= 0x60000;
		
		add_new_ship (SHIP_SUN, px, py, pz, rotmat, 0, 0);
	}
}


void add_new_station (double sx, double sy, double sz, Matrix rotmat)
{
	int station;
	
	station = (current_planet_data.techlevel >= 10) ? SHIP_DODEC : SHIP_CORIOLIS;
	universe[1].type = 0;
	add_new_ship (station, sx, sy, sz, rotmat, 0, -127);					
}
	


	
void reset_weapons (void)
{
	laser_temp = 0;
	laser_counter = 0;
	laser = 0;
	ecm_active = 0;
	missile_target = MISSILE_UNARMED;
}

 
void launch_enemy (int un, int type, int flags, int bravery)
{
	int newship;
	struct univ_object *ns;
	
	newship = add_new_ship (type, universe[un].location.x, universe[un].location.y,
							universe[un].location.z, universe[un].rotmat,
							universe[un].rotx, universe[un].rotz);

	if (newship == -1)
	{
		return;
	}

	ns = &universe[newship];
	
	if ((universe[un].type == SHIP_CORIOLIS) || (universe[un].type == SHIP_DODEC))
	{
		ns->velocity = 32;
		ns->location.x += ns->rotmat[2].x * 2; 		
		ns->location.y += ns->rotmat[2].y * 2; 		
		ns->location.z += ns->rotmat[2].z * 2;
	}

	ns->flags |= flags;
	ns->rotz /= 2;
	ns->rotz *= 2;
	ns->bravery = bravery;

	if ((type == SHIP_CARGO) || (type == SHIP_ALLOY) || (type == SHIP_ROCK))
	{
		ns->rotz = ((rand255() * 2) & 255) - 128;
		ns->rotx = ((rand255() * 2) & 255) - 128;
		ns->velocity = rand255() & 15;
	}
}


void launch_loot (int un, int loot)
{
	int i,cnt;

	if (loot == SHIP_ROCK)
	{
		cnt = rand255() & 3;
	}
	else
	{
		cnt = rand255();
		if (cnt >= 128)
			return;

		cnt &= ship_list[universe[un].type]->max_loot;
		cnt &= 15;
	}

	for (i = 0; i < cnt; i++)
	{
		launch_enemy (un, loot, 0,0);
	}
}




int in_target (int type, double x, double y, double z)
{
	double size;
	
	if (z < 0)
		return 0;
		
	size = ship_list[type]->size;

	return ((x*x + y*y) <= size);	
}



void make_angry (int un)
{
	int type;
	int flags;
	
	type = universe[un].type;
	flags = universe[un].flags;

	if (flags & FLG_INACTIVE)
		return;
	
	if ((type == SHIP_CORIOLIS) || (type == SHIP_DODEC))
	{
		universe[un].flags |= FLG_ANGRY;
		return;
	}
	
	if (type > SHIP_ROCK)
	{
		universe[un].rotx = 4;
		universe[un].acceleration = 2;
		universe[un].flags |= FLG_ANGRY;
	}
}


void explode_object (int un)
{

	cmdr.score++;

	if ((cmdr.score & 255) == 0)
		info_message ("Right On Commander!");
	
	snd_play_sample (SND_EXPLODE);
	universe[un].flags |= FLG_DEAD;

	if (universe[un].type == SHIP_CONSTRICTOR)
		cmdr.mission = 2;
}


void check_target (int un, struct univ_object *flip)
{
	struct univ_object *univ;
	
	univ = &universe[un];
	
	if (in_target (univ->type, flip->location.x, flip->location.y, flip->location.z))
	{
		if ((missile_target == MISSILE_ARMED) && (univ->type >= 0))
		{
			missile_target = un;
			info_message ("Target Locked");
			snd_play_sample (SND_BEEP);
		}
	
		if (laser)
		{
			snd_play_sample (SND_HIT_ENEMY);

			if ((univ->type != SHIP_CORIOLIS) && (univ->type != SHIP_DODEC))
			{			
				if ((univ->type == SHIP_CONSTRICTOR) || (univ->type == SHIP_COUGAR))
				{
					if (laser == (MILITARY_LASER & 127))
						univ->energy -= laser / 4;
				}
				else
				{
					univ->energy -= laser;
				}
			}

			if (univ->energy <= 0)
			{
				explode_object (un);
				
				if (univ->type == SHIP_ASTEROID)
				{
					if (laser == (MINING_LASER & 127))
					    launch_loot (un, SHIP_ROCK);
				}
				else
				{
					launch_loot (un, SHIP_ALLOY);
					launch_loot (un, SHIP_CARGO); 
				}
			}
					
			make_angry (un);
		}
	}
}



void activate_ecm (int ours)
{
	if (ecm_active == 0)
	{
		ecm_active = 32;
		ecm_ours = ours;
		snd_play_sample (SND_ECM);
	}
}


void time_ecm (void)
{
	if (ecm_active != 0)
	{
		ecm_active--;
		if (ecm_ours)
			decrease_energy (-1);
	}
}


void arm_missile (void)
{
	if ((cmdr.missiles != 0) && (missile_target == MISSILE_UNARMED))
		missile_target = MISSILE_ARMED;
}


void unarm_missile (void)
{
	missile_target = MISSILE_UNARMED;
	snd_play_sample (SND_BOOP);
}

void fire_missile (void)
{
	int newship;
	struct univ_object *ns;
	Matrix rotmat;

	if (missile_target < 0)
		return;
	
	set_init_matrix (rotmat);
	rotmat[2].z = 1.0;
	rotmat[0].x = -1.0;
	
	newship = add_new_ship (SHIP_MISSILE, 0, -28, 14, rotmat, 0, 0);

	if (newship == -1)
	{
		info_message ("Missile Jammed");
		return;
	}

	ns = &universe[newship];
	
	ns->velocity = flight_speed * 2;
	ns->flags = FLG_ANGRY;
	ns->target = missile_target;

	if (universe[missile_target].type > SHIP_ROCK)
		universe[missile_target].flags |= FLG_ANGRY;
	
	cmdr.missiles--;
	missile_target = MISSILE_UNARMED;
	
	snd_play_sample (SND_MISSILE);
}



void track_object (struct univ_object *ship, double direction, Vector nvec)
{	
	double dir;
	int rat;
	double rat2;
	
	rat = 3;
	rat2 = 0.111;
	
	dir = vector_dot_product (&nvec, &ship->rotmat[1]);

	if (direction < -0.861)
	{
		ship->rotx = (dir < 0) ? 7 : -7;
		ship->rotz = 0;
		return; 
	}
	
	ship->rotx = 0;
	
	if ((fabs(dir) * 2) >= rat2)
	{
		ship->rotx = (dir < 0) ? rat : -rat;
	}
		
	if (abs(ship->rotz) < 16)
	{
		dir = vector_dot_product (&nvec, &ship->rotmat[0]);

		ship->rotz = 0;

		if ((fabs(dir) * 2) > rat2)
		{
			ship->rotz = (dir < 0) ? rat : -rat;

			if (ship->rotx < 0)
				ship->rotz = -ship->rotz;
		}		
	}
}



void missile_tactics (int un)
{
	struct univ_object *missile;
	struct univ_object *target;
	Vector vec;
	Vector nvec;
	double direction;
	double cnt2 = 0.223;
	
	missile = &universe[un];
	
	if (ecm_active)
	{
		snd_play_sample (SND_EXPLODE);
		missile->flags |= FLG_DEAD;		
		return;
	}

	if (missile->target == 0)
	{
		if (missile->distance < 256)
		{
			missile->flags |= FLG_DEAD;
			snd_play_sample (SND_EXPLODE);
			damage_ship (250, missile->location.z >= 0.0);
			return;
		}

		vec.x = missile->location.x;
		vec.y = missile->location.y;
		vec.z = missile->location.z;
	}
	else
	{
		target = &universe[missile->target];

		vec.x = missile->location.x - target->location.x;
		vec.y = missile->location.y - target->location.y;
		vec.z = missile->location.z - target->location.z;
	
		if ((fabs(vec.x) < 256) && (fabs(vec.y) < 256) && (fabs(vec.z) < 256))
		{
			missile->flags |= FLG_DEAD;		

			if ((target->type != SHIP_CORIOLIS) && (target->type != SHIP_DODEC))
				explode_object (missile->target);
			else
				snd_play_sample (SND_EXPLODE);

			return;
		}

		if ((rand255() < 16) && (target->flags & FLG_HAS_ECM))
		{
			activate_ecm (0);
			return;
		}
	}	

	nvec = unit_vector(&vec);
	direction = vector_dot_product (&nvec, &missile->rotmat[2]); 
	nvec.x = -nvec.x;
	nvec.y = -nvec.y;
	nvec.z = -nvec.z;
	direction = -direction;

	track_object (missile, direction, nvec);

	if (direction <= -0.167)
	{
		missile->acceleration = -2;
		return;
	}

	if (direction >= cnt2)
	{
		missile->acceleration = 3;
		return;
	}

	if (missile->velocity < 6)
		missile->acceleration = 3;
	else
		if (rand255() >= 200)
			missile->acceleration = -2;
	return;
}



void launch_shuttle (void)
{
	int type;

	if ((ship_count[SHIP_TRANSPORTER] != 0) ||
		(ship_count[SHIP_SHUTTLE] != 0) ||
		(rand255() < 253) || (auto_pilot))
		return;

	type = rand255() & 1 ? SHIP_SHUTTLE : SHIP_TRANSPORTER; 
	launch_enemy (1, type, FLG_HAS_ECM | FLG_FLY_TO_PLANET, 113);
}


void tactics (int un)
{
	int type;
	int energy;
	int maxeng;
	int flags;
	struct univ_object *ship;
	Vector nvec;
	double cnt2 = 0.223;
	double direction;
	int attacking;
	
	ship = &universe[un];
	type = ship->type;
	flags = ship->flags;

	if ((type == SHIP_PLANET) || (type == SHIP_SUN))
		return;
	
	if (flags & FLG_DEAD)
		return;

	if (flags & FLG_INACTIVE)
		return;
	
	if (type == SHIP_MISSILE)
	{
		if (flags & FLG_ANGRY)
			missile_tactics (un);
		return;
	}

	if (((un ^ mcount) & 7) != 0)
		return;

	if ((type == SHIP_CORIOLIS) || (type == SHIP_DODEC))
	{
 		if (flags & FLG_ANGRY) 
		{
			if ((rand() & 255) < 240)
				return;
		
			if (ship_count[SHIP_VIPER] >= 4)
				return; 

			launch_enemy (un, SHIP_VIPER, FLG_ANGRY | FLG_HAS_ECM, 113);
			return;
		}

		launch_shuttle ();
		return;
	}

	if (type == SHIP_HERMIT)
	{
		if (rand255() > 200)
		{
			launch_enemy (un, SHIP_SIDEWINDER + (rand255() & 3), FLG_ANGRY | FLG_HAS_ECM, 113);
			ship->flags |= FLG_INACTIVE;
		}

		return;
	}
	
	
	if (ship->energy < ship_list[type]->energy)
		ship->energy++;

	if ((type == SHIP_THARGLET) && (ship_count[SHIP_THARGOID] == 0))
	{
		ship->flags = 0;
		ship->velocity /= 2;
		return;
	}

	if (flags & FLG_SLOW)
	{
		if (rand255() > 50)
			return;
	}

	if (flags & FLG_POLICE)
	{
		if (cmdr.legal_status >= 64)
		{
			flags |= FLG_ANGRY;
			ship->flags = flags;
		}
	}
	
	if ((flags & FLG_ANGRY) == 0)
	{
		if ((flags & FLG_FLY_TO_PLANET) || (flags & FLG_FLY_TO_STATION))
		{
			auto_pilot_ship (&universe[un]);
		}

		return;
	}

	
	/* If we get to here then the ship is angry so start attacking... */

	if (ship_count[SHIP_CORIOLIS] || ship_count[SHIP_DODEC])
	{
		if ((flags & FLG_BOLD) == 0)
			ship->bravery = 0;
	}

	
	if (type == SHIP_ANACONDA)
	{
		if (rand255() > 200)
		{
			launch_enemy (un, rand255() > 100 ? SHIP_WORM : SHIP_SIDEWINDER,
						  FLG_ANGRY | FLG_HAS_ECM, 113);
			return;
		}
	}

	
	if (rand255() >= 250)
	{
		ship->rotz = rand255() | 0x68;
		if (ship->rotz > 127)
			ship->rotz = -(ship->rotz & 127);
	}
	
	maxeng = ship_list[type]->energy;
	energy = ship->energy;

	if (energy < (maxeng / 2))
	{
		if ((energy < (maxeng / 8)) && (rand255() > 230) && (type != SHIP_THARGOID))
		{
			ship->flags &= ~FLG_ANGRY;
			ship->flags |= FLG_INACTIVE;
			launch_enemy (un, SHIP_ESCAPE_CAPSULE, 0, 126);
			return;				
		}

		if ((ship->missiles != 0) && (ecm_active == 0) &&
			(ship->missiles >= (rand255() & 31)))
		{
			ship->missiles--;
			if (type == SHIP_THARGOID)
				launch_enemy (un, SHIP_THARGLET, FLG_ANGRY, ship->bravery);
			else
			{
				launch_enemy (un, SHIP_MISSILE, FLG_ANGRY, 126);
				info_message ("INCOMING MISSILE");
			}
			return;
		}
	}

	nvec = unit_vector(&universe[un].location);
	direction = vector_dot_product (&nvec, &ship->rotmat[2]); 
	
	if 	((ship->distance < 8192) && (direction <= -0.833) &&
		 (ship_list[type]->laser_strength != 0))
	{
		if (direction <= -0.917)
			ship->flags |= FLG_FIRING | FLG_HOSTILE;		

		if (direction <= -0.972)
		{
			damage_ship (ship_list[type]->laser_strength, ship->location.z >= 0.0);
			ship->acceleration--;
			if (((ship->location.z >= 0.0) && (front_shield == 0)) ||
				((ship->location.z < 0.0) && (aft_shield == 0)))
				snd_play_sample (SND_INCOMMING_FIRE_2);
			else
				snd_play_sample (SND_INCOMMING_FIRE_1);
		}				
		else
		{
			nvec.x = -nvec.x;
			nvec.y = -nvec.y;
			nvec.z = -nvec.z;
			direction = -direction;
			track_object (&universe[un], direction, nvec);
		}

//		if ((fabs(ship->location.z) < 768) && (ship->bravery <= ((rand255() & 127) | 64)))
		if (fabs(ship->location.z) < 768)
		{
			ship->rotx = rand255() & 0x87;
			if (ship->rotx > 127)
				ship->rotx = -(ship->rotx & 127);

			ship->acceleration = 3;
			return;
		}

		if (ship->distance < 8192)
			ship->acceleration = -1;
		else
			ship->acceleration = 3;
		return;
	} 

	attacking = 0;

	if ((fabs(ship->location.z) >= 768) ||
		(fabs(ship->location.x) >= 512) ||
		(fabs(ship->location.y) >= 512))
	{
		if (ship->bravery > (rand255() & 127))
		{
			attacking = 1;
			nvec.x = -nvec.x;
			nvec.y = -nvec.y;
			nvec.z = -nvec.z;
			direction = -direction;
		}
	}

	track_object (&universe[un], direction, nvec);

	if ((attacking == 1) && (ship->distance < 2048))
	{
		if (direction >= cnt2)
		{
			ship->acceleration = -1;
			return;
		}

		if (ship->velocity < 6)
			ship->acceleration = 3;
		else
			if (rand255() >= 200)
				ship->acceleration = -1;
		return;
	}
	
	if (direction <= -0.167)
	{
		ship->acceleration = -1;
		return;
	}

	if (direction >= cnt2)
	{
		ship->acceleration = 3;
		return;
	}
		 
	if (ship->velocity < 6)
		ship->acceleration = 3;
	else
		if (rand255() >= 200)
			ship->acceleration = -1;
}


void draw_laser_lines (void)
{
	if (wireframe)
	{
		gfx_draw_colour_line (32 * GFX_SCALE, GFX_VIEW_BY, laser_x, laser_y, GFX_COL_WHITE);
		gfx_draw_colour_line (48 * GFX_SCALE, GFX_VIEW_BY, laser_x, laser_y, GFX_COL_WHITE);
		gfx_draw_colour_line (208 * GFX_SCALE, GFX_VIEW_BY, laser_x, laser_y, GFX_COL_WHITE);
		gfx_draw_colour_line (224 * GFX_SCALE, GFX_VIEW_BY, laser_x, laser_y, GFX_COL_WHITE);
	}
	else
	{
		gfx_draw_triangle (32 * GFX_SCALE, GFX_VIEW_BY, laser_x, laser_y,  48 * GFX_SCALE, GFX_VIEW_BY, GFX_COL_RED);
		gfx_draw_triangle (208 * GFX_SCALE, GFX_VIEW_BY, laser_x, laser_y, 224 * GFX_SCALE, GFX_VIEW_BY, GFX_COL_RED);
	}		 
}


int fire_laser (void)
{
	if ((laser_counter == 0) && (laser_temp < 242))
	{
		switch (current_screen)
		{
			case SCR_FRONT_VIEW:
				laser = cmdr.front_laser;
				break;
			
			case SCR_REAR_VIEW:
				laser = cmdr.rear_laser;
				break;
					
			case SCR_RIGHT_VIEW:
				laser = cmdr.right_laser;
				break;
					
			case SCR_LEFT_VIEW:
				laser = cmdr.left_laser;
				break;
				
			default:
				laser = 0;
		}

		if (laser != 0)
		{
			laser_counter = (laser > 127) ? 0 : (laser & 0xFA);
			laser &= 127;
			laser2 = laser;

			snd_play_sample (SND_PULSE);
			laser_temp += 8;
			if (energy > 1)
				energy--;
			
			laser_x = ((rand() & 3) + 128 - 2) * GFX_SCALE;
			laser_y = ((rand() & 3) + 96 - 2) * GFX_SCALE;
			
			return 2;
		}
	}

	return 0;
}


void cool_laser (void)
{
	laser = 0;

	if (laser_temp > 0)
		laser_temp--;
					
	if (laser_counter > 0)
		laser_counter--;
				
	if (laser_counter > 0)
		laser_counter--;
}


int create_other_ship (int type)
{
	Matrix rotmat;
	int x,y,z;
	int newship;
	
	set_init_matrix (rotmat);

	z = 12000;
	x = 1000 + (randint() & 8191);
	y = 1000 + (randint() & 8191);

	if (rand255() > 127)
		x = -x;
	if (rand255() > 127)
		y = -y;

	newship = add_new_ship (type, x, y, z, rotmat, 0, 0);

	return newship;
}


void create_thargoid (void)
{
	int newship;
	
	newship = create_other_ship (SHIP_THARGOID);
	if (newship != -1)
	{
		universe[newship].flags = FLG_ANGRY | FLG_HAS_ECM;
		universe[newship].bravery = 113;

		if (rand255() > 64)
			launch_enemy (newship, SHIP_THARGLET, FLG_ANGRY | FLG_HAS_ECM, 96);
	}	
}



void create_cougar (void)
{
	int newship;

	if (ship_count[SHIP_COUGAR] != 0)
		return;
	
	newship = create_other_ship (SHIP_COUGAR);
	if (newship != -1)
	{
		universe[newship].flags = FLG_HAS_ECM; // | FLG_CLOAKED;
		universe[newship].bravery = 121;
		universe[newship].velocity = 18;
	}	
}



void create_trader (void)
{
	int newship;
	int rnd;
	int type;

	type = SHIP_COBRA3 + (rand255() & 3);

	newship = create_other_ship (type);
	
	if (newship != -1)
	{
		universe[newship].rotmat[2].z = -1.0;
		universe[newship].rotz = rand255() & 7;
		
		rnd = rand255();
		universe[newship].velocity = (rnd & 31) | 16;
		universe[newship].bravery = rnd / 2;

		if (rnd & 1)
			universe[newship].flags |= FLG_HAS_ECM;

//		if (rnd & 2)
//			universe[newship].flags |= FLG_ANGRY; 
	}
}


void create_lone_hunter (void)
{
	int rnd;
	int type;
	int newship;

	if ((cmdr.mission == 1) && (cmdr.galaxy_number == 1) &&
		(docked_planet.d == 144) && (docked_planet.b == 33) &&
		(ship_count[SHIP_CONSTRICTOR] == 0))
	{
		type = SHIP_CONSTRICTOR;
	}
	else
	{
		rnd = rand255();
		type = SHIP_COBRA3_LONE + (rnd & 3) + (rnd > 127);
	}
		
	newship = create_other_ship (type);

	if (newship != -1)
	{
		universe[newship].flags = FLG_ANGRY;
		if ((rand255() > 200) || (type == SHIP_CONSTRICTOR))
			universe[newship].flags |= FLG_HAS_ECM;
		
		universe[newship].bravery = ((rand255() * 2) | 64) & 127;
		in_battle = 1;  
	}	
}



/* Check for a random asteroid encounter... */

void check_for_asteroids (void)
{
	int newship;
	int type;

	if ((rand255() >= 35) || (ship_count[SHIP_ASTEROID] >= 3))
		return;

	if (rand255() > 253)
		type = SHIP_HERMIT;
	else
		type = SHIP_ASTEROID;
		
	newship = create_other_ship (type);
	
	if (newship != -1)
	{
//		universe[newship].velocity = (rand255() & 31) | 16; 
		universe[newship].velocity = 8;
		universe[newship].rotz = rand255() > 127 ? -127 : 127; 
		universe[newship].rotx = 16; 
	}
}



/* If we've been a bad boy then send the cops after us... */

void check_for_cops (void)
{
	int newship;
	int offense;

	offense = carrying_contraband() * 2;
	if (ship_count[SHIP_VIPER] == 0)
		offense |= cmdr.legal_status;

	if (rand255() >= offense)
		return;

	newship = create_other_ship (SHIP_VIPER);
	
	if (newship != -1)
	{
		universe[newship].flags = FLG_ANGRY;
		if (rand255() > 245)
			universe[newship].flags |= FLG_HAS_ECM;
		
		universe[newship].bravery = ((rand255() * 2) | 64) & 127;  
	}
}


void check_for_others (void)
{
	int x,y,z;
	int newship;
	Matrix rotmat;
	int gov;
	int rnd;
	int type;
	int i;

	gov = current_planet_data.government; 
	rnd = rand255();

	if ((gov != 0) && ((rnd >= 90) || ((rnd & 7) < gov)))
		return;	

	if (rand255() < 100)
	{
		create_lone_hunter();
		return;
	}	

	/* Pack hunters... */
	
	set_init_matrix (rotmat);

	z = 12000;
	x = 1000 + (randint() & 8191);
	y = 1000 + (randint() & 8191);

	if (rand255() > 127)
		x = -x;
	if (rand255() > 127)
		y = -y;

	rnd = rand255() & 3;
	
	for (i = 0; i <= rnd; i++)
	{
		type = SHIP_SIDEWINDER + (rand255() & rand255() & 7);
		newship = add_new_ship (type, x, y, z, rotmat, 0, 0);
		if (newship != -1)
		{
			universe[newship].flags = FLG_ANGRY;
			if (rand255() > 245)
				universe[newship].flags |= FLG_HAS_ECM;
		
			universe[newship].bravery = ((rand255() * 2) | 64) & 127;
			in_battle++;  
		}
	}
	
}


void random_encounter (void)
{
	if ((ship_count[SHIP_CORIOLIS] != 0) || (ship_count[SHIP_DODEC] != 0))
		return;

	if (rand255() == 136)
	{
		if (((int)(universe[0].location.z) & 0x3e) != 0)
			create_thargoid ();
		else
			create_cougar();			

		return;
	}		

	if ((rand255() & 7) == 0)
	{
		create_trader();
		return;
	}
		
	check_for_asteroids();

	check_for_cops();	

	if (ship_count[SHIP_VIPER] != 0)
		return;

	if (in_battle)
		return;

	if ((cmdr.mission == 5) && (rand255() >= 200))
		create_thargoid ();
		
	check_for_others();	
}


void abandon_ship (void)
{
	int i;

	cmdr.escape_pod = 0;
	cmdr.legal_status = 0;
	cmdr.fuel = myship.max_fuel;
	
	for (i = 0; i < NO_OF_STOCK_ITEMS; i++)
		cmdr.current_cargo[i] = 0;
	
	snd_play_sample (SND_DOCK);					
	dock_player();
	current_screen = SCR_BREAK_PATTERN;
}

