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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>

#include "config.h"
#include "elite.h"
#include "gfx.h"
#include "planet.h"
#include "vector.h"
#include "shipdata.h"
#include "shipface.h"
#include "threed.h"
#include "space.h"
#include "random.h"

#define MAX(x,y) (((x) > (y)) ? (x) : (y))


#define LAND_X_MAX	128
#define LAND_Y_MAX	128

static unsigned char landscape[LAND_X_MAX+1][LAND_Y_MAX+1];

static struct point point_list[100];


/*
 * The following routine is used to draw a wireframe represtation of a ship.
 *
 * caveat: it is a work in progress.
 * A number of features (such as not showing detail at distance) have not yet been implemented.
 *
 */

void draw_wireframe_ship (struct univ_object *univ)
{
	Matrix trans_mat;
	int i;
	int sx,sy,ex,ey;
	double rx,ry,rz;
	int visible[32];
	Vector vec;
	Vector camera_vec;
	double cos_angle;
	double tmp;
	struct ship_face_normal *ship_norm;
	int num_faces;
	struct ship_data *ship;
	int lasv;

	ship = ship_list[univ->type];
	
	for (i = 0; i < 3; i++)
		trans_mat[i] = univ->rotmat[i];
		
	camera_vec = univ->location;
	mult_vector (&camera_vec, trans_mat);
	camera_vec = unit_vector (&camera_vec);
	
	num_faces = ship->num_faces;
	
	for (i = 0; i < num_faces; i++)
	{
		ship_norm = ship->normals;

		vec.x = ship_norm[i].x;
		vec.y = ship_norm[i].y;
		vec.z = ship_norm[i].z;

		if ((vec.x == 0) && (vec.y == 0) && (vec.z == 0))
			visible[i] = 1;
		else
		{
			vec = unit_vector (&vec);
			cos_angle = vector_dot_product (&vec, &camera_vec);
			visible[i] = (cos_angle < -0.2);
		}
	}

	tmp = trans_mat[0].y;
	trans_mat[0].y = trans_mat[1].x;
	trans_mat[1].x = tmp;

	tmp = trans_mat[0].z;
	trans_mat[0].z = trans_mat[2].x;
	trans_mat[2].x = tmp;

	tmp = trans_mat[1].z;
	trans_mat[1].z = trans_mat[2].y;
	trans_mat[2].y = tmp;

	for (i = 0; i < ship->num_points; i++)
	{
		vec.x = ship->points[i].x;
		vec.y = ship->points[i].y;
		vec.z = ship->points[i].z;

		mult_vector (&vec, trans_mat);

		rx = vec.x + univ->location.x;
		ry = vec.y + univ->location.y;
		rz = vec.z + univ->location.z;

		sx = (rx * 256) / rz;
		sy = (ry * 256) / rz;

		sy = -sy;

		sx += 128;
		sy += 96;

		sx *= GFX_SCALE;
		sy *= GFX_SCALE;

		point_list[i].x = sx;
		point_list[i].y = sy;

	}

	for (i = 0; i < ship->num_lines; i++)
	{
		if (visible[ship->lines[i].face1] ||
			visible[ship->lines[i].face2])
		{
			sx = point_list[ship->lines[i].start_point].x;
			sy = point_list[ship->lines[i].start_point].y;

			ex = point_list[ship->lines[i].end_point].x;
			ey = point_list[ship->lines[i].end_point].y;

			gfx_draw_line (sx, sy, ex, ey);
		}
	}


	if (univ->flags & FLG_FIRING)
	{
		lasv = ship_list[univ->type]->front_laser;
		gfx_draw_line (point_list[lasv].x, point_list[lasv].y,
					   univ->location.x > 0 ? 0 : 511, rand255() * 2);
	}
}




/*
 * Hacked version of the draw ship routine to display solid ships...
 * This needs a lot of tidying...
 *
 * Check for hidden surface supplied by T.Harte.
 */

void draw_solid_ship (struct univ_object *univ)
{
	int i;
	int sx,sy;
	double rx,ry,rz;
	struct vector vec;
	struct vector camera_vec;
	double tmp;
	struct ship_face *face_data;
	int num_faces;
	int num_points;
	int poly_list[16];
	int zavg;
	struct ship_solid *solid_data;
	struct ship_data *ship;
	Matrix trans_mat;
	int lasv;
	int col;

	solid_data = &ship_solids[univ->type];
	ship = ship_list[univ->type];
	
	for (i = 0; i < 3; i++)
		trans_mat[i] = univ->rotmat[i];
		
	camera_vec = univ->location;
	mult_vector (&camera_vec, trans_mat);
	camera_vec = unit_vector (&camera_vec);

	num_faces = solid_data->num_faces;
	face_data = solid_data->face_data;

/*
	for (i = 0; i < num_faces; i++)
	{
		vec.x = face_data[i].norm_x;
		vec.y = face_data[i].norm_y;
		vec.z = face_data[i].norm_z;

		vec = unit_vector (&vec);
		cos_angle = vector_dot_product (&vec, &camera_vec);

		visible[i] = (cos_angle < -0.13);
	}
*/

	tmp = trans_mat[0].y;
	trans_mat[0].y = trans_mat[1].x;
	trans_mat[1].x = tmp;

	tmp = trans_mat[0].z;
	trans_mat[0].z = trans_mat[2].x;
	trans_mat[2].x = tmp;

	tmp = trans_mat[1].z;
	trans_mat[1].z = trans_mat[2].y;
	trans_mat[2].y = tmp;


	for (i = 0; i < ship->num_points; i++)
	{
		vec.x = ship->points[i].x;
		vec.y = ship->points[i].y;
		vec.z = ship->points[i].z;

		mult_vector (&vec, trans_mat);

		rx = vec.x + univ->location.x;
		ry = vec.y + univ->location.y;
		rz = vec.z + univ->location.z;

		if (rz <= 0)
			rz = 1;
		
		sx = (rx * 256) / rz;
		sy = (ry * 256) / rz;

		sy = -sy;

		sx += 128;
		sy += 96;

		sx *= GFX_SCALE;
		sy *= GFX_SCALE;

		point_list[i].x = sx;
		point_list[i].y = sy;
		point_list[i].z = rz;
		
	}

	for (i = 0; i < num_faces; i++)
	{
		if (((point_list[face_data[i].p1].x - point_list[face_data[i].p2].x) * 
		     (point_list[face_data[i].p3].y - point_list[face_data[i].p2].y) -
			 (point_list[face_data[i].p1].y - point_list[face_data[i].p2].y) *
			 (point_list[face_data[i].p3].x - point_list[face_data[i].p2].x)) <= 0)
		{
			num_points = face_data[i].points;

			poly_list[0] = point_list[face_data[i].p1].x;
			poly_list[1] = point_list[face_data[i].p1].y;
			zavg = point_list[face_data[i].p1].z;

			poly_list[2] = point_list[face_data[i].p2].x;
			poly_list[3] = point_list[face_data[i].p2].y;
			zavg = MAX(zavg,point_list[face_data[i].p2].z);

			if (num_points > 2)
			{
				poly_list[4] = point_list[face_data[i].p3].x;
				poly_list[5] = point_list[face_data[i].p3].y;
				zavg = MAX(zavg,point_list[face_data[i].p3].z);
			}

			if (num_points > 3)
			{
				poly_list[6] = point_list[face_data[i].p4].x;
				poly_list[7] = point_list[face_data[i].p4].y;
				zavg = MAX(zavg,point_list[face_data[i].p4].z);
			}

			if (num_points > 4)
			{
				poly_list[8] = point_list[face_data[i].p5].x;
				poly_list[9] = point_list[face_data[i].p5].y;
				zavg = MAX(zavg,point_list[face_data[i].p5].z);
			}

			if (num_points > 5)
			{
				poly_list[10] = point_list[face_data[i].p6].x;
				poly_list[11] = point_list[face_data[i].p6].y;
				zavg = MAX(zavg,point_list[face_data[i].p6].z);
			}
														 
			if (num_points > 6)
			{
				poly_list[12] = point_list[face_data[i].p7].x;
				poly_list[13] = point_list[face_data[i].p7].y;
				zavg = MAX(zavg,point_list[face_data[i].p7].z);
			}

			if (num_points > 7)
			{
				poly_list[14] = point_list[face_data[i].p8].x;
				poly_list[15] = point_list[face_data[i].p8].y;
				zavg = MAX(zavg,point_list[face_data[i].p8].z);
			}
			

			gfx_render_polygon (face_data[i].points, poly_list, face_data[i].colour, zavg);
			
		}
	}

	if (univ->flags & FLG_FIRING)
	{
		lasv = ship_list[univ->type]->front_laser;
		col = (univ->type == SHIP_VIPER) ? GFX_COL_CYAN : GFX_COL_WHITE; 
		
		gfx_render_line (point_list[lasv].x, point_list[lasv].y,
						 univ->location.x > 0 ? 0 : 511, rand255() * 2,
						 point_list[lasv].z, col);
	}
}





/*
 * Colour map used to generate a SNES Elite style planet.
 * This is a quick hack and needs tidying up.
 */

int snes_planet_colour[] =
{
	102, 102,
	134, 134, 134, 134,
	167, 167, 167, 167,
	213, 213,
	255,
	83,83,83,83,
	122,
	83,83,
	249,249,249,249, 
	83,
	122,
	249,249,249,249,249,249,
	83, 83,
	122,
	83,83, 83, 83,
	255,
	213, 213,
	167,167, 167, 167,
	134,134, 134, 134,
	102, 102
}; 


/*
 * Generate a landscape map for a SNES Elite style planet.
 */

void generate_snes_landscape (void)
{
	int x,y;
	int colour;
	
	for (y = 0; y <= LAND_Y_MAX; y++)
	{
		colour = snes_planet_colour[y * (sizeof(snes_planet_colour)/sizeof(int)) / LAND_Y_MAX];  
		for (x = 0; x <= LAND_X_MAX; x++)
		{
			landscape[x][y] = colour;		
		}
	}	
}




/*
 * Guassian random number generator.
 * Returns a number between -7 and +8 with Gaussian distribution.
 */

int grand (void)
{
	int i;
	int r;
	
	r = 0;
	for (i = 0; i < 12; i++)
		r += randint() & 15;
	
	r /= 12;
	r -= 7;

	return r;
}


/*
 * Calculate the midpoint between two given points.
 */

int calc_midpoint (int sx, int sy, int ex, int ey)
{
	int a,b,n;

	a = landscape[sx][sy];
	b = landscape[ex][ey];
	
	n = ((a + b) / 2) + grand();
	if (n < 0)
		n = 0;
	if (n > 255)
		n = 255;
	
	return n;
} 


/*
 * Calculate a square on the midpoint map.
 */

void midpoint_square (int tx, int ty, int w)
{
	int mx,my;
	int bx,by;
	int d;

	d = w / 2;	
	mx = tx + d;
	my = ty + d;
	bx = tx + w;
	by = ty + w;
	
	landscape[mx][ty] = calc_midpoint(tx,ty,bx,ty);
	landscape[mx][by] = calc_midpoint(tx,by,bx,by);
	landscape[tx][my] = calc_midpoint(tx,ty,tx,by);
	landscape[bx][my] = calc_midpoint(bx,ty,bx,by);
	landscape[mx][my] = calc_midpoint(tx,my,bx,my); 

	if (d == 1)
		return;
	
	midpoint_square (tx,ty,d);
	midpoint_square (mx,ty,d);
	midpoint_square (tx,my,d);
	midpoint_square (mx,my,d);
}


/*
 * Generate a fractal landscape.
 * Uses midpoint displacement method.
 */

void generate_fractal_landscape (int rnd_seed)
{
	int x,y,d,h;
	double dist;
	int dark;
	int old_seed;
	
	old_seed = get_rand_seed();
	set_rand_seed(rnd_seed);
	
	d = LAND_X_MAX / 8;
	
	for (y = 0; y <= LAND_Y_MAX; y += d)
		for (x = 0; x <= LAND_X_MAX; x += d)
			landscape[x][y] = randint() & 255;

	for (y = 0; y < LAND_Y_MAX; y += d)
		for (x = 0; x < LAND_X_MAX; x += d)	
			midpoint_square (x,y,d);

	for (y = 0; y <= LAND_Y_MAX; y++)
	{
		for (x = 0; x <= LAND_X_MAX; x++)
		{
			dist = x*x + y*y;
			dark = dist > 10000;
			h = landscape[x][y];
			if (h > 166)
				landscape[x][y] = dark ? GFX_COL_GREEN_1 : GFX_COL_GREEN_2;
			else 
				landscape[x][y] = dark ? GFX_COL_BLUE_2 : GFX_COL_BLUE_1;

		}
	}

	set_rand_seed (old_seed);
}


void generate_landscape (int rnd_seed)
{
	switch (planet_render_style)
	{
		case 0:		/* Wireframe... do nothing for now... */
			break;
		
		case 1:
			/* generate_green_landscape (); */
			break;
		
		case 2:
			generate_snes_landscape();
			break;
		
		case 3:
			generate_fractal_landscape (rnd_seed);
			break;
	}
}

 
 
/*
 * Draw a line of the planet with appropriate rotation.
 */


void render_planet_line (int xo, int yo, int x, int y, int radius, int vx, int vy)
{
	int lx, ly;
	int rx, ry;
	int colour;
	int sx,sy;
	int ex;
	int div;

	sy = y + yo;
	
	if ((sy < GFX_VIEW_TY + GFX_Y_OFFSET) ||
		(sy > GFX_VIEW_BY + GFX_Y_OFFSET))
		return;
					   
	sx = xo - x;
	ex = xo + x;
	
	rx = -x * vx - y * vy;
	ry = -x * vy + y * vx;
	rx += radius << 16;
	ry += radius << 16;
	div = radius << 10;	 /* radius * 2 * LAND_X_MAX >> 16 */
	
		
	for (; sx <= ex; sx++)
	{
		if ((sx >= (GFX_VIEW_TX + GFX_X_OFFSET)) && (sx <= (GFX_VIEW_BX + GFX_X_OFFSET)))
		{
			lx = rx / div;
			ly = ry / div;
			colour = landscape[lx][ly];
 
			gfx_fast_plot_pixel (sx, sy, colour);
		}
		rx += vx;
		ry += vy;
	}
}


/*
 * Draw a solid planet.  Based on Doros circle drawing alogorithm.
 */

void render_planet (int xo, int yo, int radius, struct vector *vec)
{
	int x,y;
	int s;
	int vx,vy;

	xo += GFX_X_OFFSET;
	yo += GFX_Y_OFFSET;
	
	vx = vec[1].x * 65536;
	vy = vec[1].y * 65536;	
	
	s = radius;
	x = radius;
	y = 0;

	s -= x + x;
	while (y <= x)
	{
		render_planet_line (xo, yo, x, y, radius, vx, vy);
		render_planet_line (xo, yo, x,-y, radius, vx, vy);
		render_planet_line (xo, yo, y, x, radius, vx, vy);
		render_planet_line (xo, yo, y,-x, radius, vx, vy);
		
		s += y + y + 1;
		y++;
		if (s >= 0)
		{
			s -= x + x + 2;
			x--;
		}				
	}
}


/*
 * Draw a wireframe planet.
 * At the moment we just draw a circle.
 * Need to add in the two arcs that the original Elite had.
 */

void draw_wireframe_planet (int xo, int yo, int radius, struct vector *vec)
{
	gfx_draw_circle (xo, yo, radius, GFX_COL_WHITE);
}


/*
 * Draw a planet.
 * We can currently do three different types of planet...
 * - Wireframe.
 * - Fractal landscape.
 * - SNES Elite style.
 */

void draw_planet (struct univ_object *planet)
{
	int x,y;
	int radius;
	
	x = (planet->location.x * 256) / planet->location.z;
	y = (planet->location.y * 256) / planet->location.z;

	y = -y;
	
	x += 128;
	y += 96;

	x *= GFX_SCALE;
	y *= GFX_SCALE;
	
	radius = 6291456 / planet->distance;
//	radius = 6291456 / ship_vec.z;   /* Planets are BIG! */

	radius *= GFX_SCALE;

	if ((x + radius <  0) ||
		(x - radius > 511) ||
		(y + radius < 0) ||
		(y - radius > 383))
		return; 

	switch (planet_render_style)
	{
		case 0:
			draw_wireframe_planet (x, y, radius, planet->rotmat);
			break;
		
		case 1:
			gfx_draw_filled_circle (x, y, radius, GFX_COL_GREEN_1);
			break;

		case 2:
		case 3:
			render_planet (x, y, radius, planet->rotmat);
			break;
	}
}


void render_sun_line (int xo, int yo, int x, int y, int radius)
{
	int sy = yo + y;
	int sx,ex;
	int colour;
	int dx,dy;
	int distance;
	int inner,outer;
	int inner2;
	int mix;

	if ((sy < GFX_VIEW_TY + GFX_Y_OFFSET) ||
		(sy > GFX_VIEW_BY + GFX_Y_OFFSET))
		return;
	
	sx = xo - x;
	ex = xo + x;

	sx -= (radius * (2 + (randint() & 7))) >> 8;
	ex += (radius * (2 + (randint() & 7))) >> 8;
	
	if ((sx > GFX_VIEW_BX + GFX_X_OFFSET) ||
		(ex < GFX_VIEW_TX + GFX_X_OFFSET))
		return;
	
	if (sx < GFX_VIEW_TX + GFX_X_OFFSET)
		sx = GFX_VIEW_TX + GFX_X_OFFSET;
	
	if (ex > GFX_VIEW_BX + GFX_X_OFFSET)
		ex = GFX_VIEW_BX + GFX_X_OFFSET;

	inner = (radius * (200 + (randint() & 7))) >> 8;
	inner *= inner;
	
	inner2 = (radius * (220 + (randint() & 7))) >> 8;
	inner2 *= inner2;
	
	outer = (radius * (239 + (randint() & 7))) >> 8;
	outer *= outer;	

	dy = y * y;
	dx = sx - xo;
	
	for (; sx <= ex; sx++,dx++)
	{
		mix = (sx ^ y) & 1;
		distance = dx * dx + dy;

		if (distance < inner)
			colour = GFX_COL_WHITE;
		else if (distance < inner2)
			colour = GFX_COL_YELLOW_4;
		else if (distance < outer)
			colour = GFX_ORANGE_3;
		else
			colour = mix ? GFX_ORANGE_1 : GFX_ORANGE_2;
		
		gfx_fast_plot_pixel (sx, sy, colour);
	} 	
}


void render_sun (int xo, int yo, int radius)
{
	int x,y;
	int s;
	
	xo += GFX_X_OFFSET;
	yo += GFX_Y_OFFSET;
	
	s = -radius;
	x = radius;
	y = 0;

	// s -= x + x;
	while (y <= x)
	{
		render_sun_line (xo, yo, x, y, radius);
		render_sun_line (xo, yo, x,-y, radius);
		render_sun_line (xo, yo, y, x, radius);
		render_sun_line (xo, yo, y,-x, radius);
		
		s += y + y + 1;
		y++;
		if (s >= 0)
		{
			s -= x + x + 2;
			x--;
		}				
	}
}



void draw_sun (struct univ_object *planet)
{
	int x,y;
	int radius;
	
	x = (planet->location.x * 256) / planet->location.z;
	y = (planet->location.y * 256) / planet->location.z;

	y = -y;
	
	x += 128;
	y += 96;

	x *= GFX_SCALE;
	y *= GFX_SCALE;
	
	radius = 6291456 / planet->distance;

	radius *= GFX_SCALE;

	if ((x + radius <  0) ||
		(x - radius > 511) ||
		(y + radius < 0) ||
		(y - radius > 383))
		return; 

	render_sun (x, y, radius);
}



void draw_explosion (struct univ_object *univ)
{
	int i;
	int z;
	int q;
	int pr;
	int px,py;
	int cnt;
	int sizex,sizey,psx,psy;
	Matrix trans_mat;
	int sx,sy;
	double rx,ry,rz;
	int visible[32];
	struct vector vec;
	struct vector camera_vec;
	double cos_angle;
	double tmp;
	struct ship_face_normal *ship_norm;
	struct ship_point *sp;
	struct ship_data *ship;
	int np;
	int old_seed;
	
	
	if (univ->exp_delta > 251)
	{
		univ->flags |= FLG_REMOVE;
		return;
	}
	
	univ->exp_delta += 4;

	if (univ->location.z <= 0)
		return;

	ship = ship_list[univ->type];
	
	for (i = 0; i < 3; i++)
		trans_mat[i] = univ->rotmat[i];
		
	camera_vec = univ->location;
	mult_vector (&camera_vec, trans_mat);
	camera_vec = unit_vector (&camera_vec);
	
	ship_norm = ship->normals;
	
	for (i = 0; i < ship->num_faces; i++)
	{
		vec.x = ship_norm[i].x;
		vec.y = ship_norm[i].y;
		vec.z = ship_norm[i].z;

		vec = unit_vector (&vec);
		cos_angle = vector_dot_product (&vec, &camera_vec);

		visible[i] = (cos_angle < -0.13);
	}

	tmp = trans_mat[0].y;
	trans_mat[0].y = trans_mat[1].x;
	trans_mat[1].x = tmp;

	tmp = trans_mat[0].z;
	trans_mat[0].z = trans_mat[2].x;
	trans_mat[2].x = tmp;

	tmp = trans_mat[1].z;
	trans_mat[1].z = trans_mat[2].y;
	trans_mat[2].y = tmp;
	
	sp = ship->points;
	np = 0;
	
	for (i = 0; i < ship->num_points; i++)
	{
		if (visible[sp[i].face1] || visible[sp[i].face2] ||
			visible[sp[i].face3] || visible[sp[i].face4])
		{
			vec.x = sp[i].x;
			vec.y = sp[i].y;
			vec.z = sp[i].z;

			mult_vector (&vec, trans_mat);

			rx = vec.x + univ->location.x;
			ry = vec.y + univ->location.y;
			rz = vec.z + univ->location.z;

			sx = (rx * 256) / rz;
			sy = (ry * 256) / rz;

			sy = -sy;

			sx += 128;
			sy += 96;

			sx *= GFX_SCALE;
			sy *= GFX_SCALE;

			point_list[np].x = sx;
			point_list[np].y = sy;
			np++;
		}
	}

	
	z = (int)univ->location.z;
	
	if (z >= 0x2000)
		q = 254;
	else
		q = (z / 32) | 1;

	pr = (univ->exp_delta * 256) / q;
	
//	if (pr > 0x1C00)
//		q = 254;
//	else

	q = pr / 32;	
		
	old_seed = get_rand_seed();
	set_rand_seed (univ->exp_seed);

	for (cnt = 0; cnt < np; cnt++)
	{
		sx = point_list[cnt].x;
		sy = point_list[cnt].y;
	
		for (i = 0; i < 16; i++)
		{
			px = rand255() - 128;
			py = rand255() - 128;		

			px = (px * q) / 256;
			py = (py * q) / 256;
		
			px = px + px + sx;
			py = py + py + sy;

			sizex = (randint() & 1) + 1;
			sizey = (randint() & 1) + 1;

			for (psy = 0; psy < sizey; psy++)
				for (psx = 0; psx < sizex; psx++)		
					gfx_plot_pixel (px+psx, py+psy, GFX_COL_WHITE);
		}
	}

	set_rand_seed (old_seed);
}



/*
 * Draws an object in the universe.
 * (Ship, Planet, Sun etc).
 */

void draw_ship (struct univ_object *ship)
{

	if ((current_screen != SCR_FRONT_VIEW) && (current_screen != SCR_REAR_VIEW) && 
		(current_screen != SCR_LEFT_VIEW) && (current_screen != SCR_RIGHT_VIEW) &&
		(current_screen != SCR_INTRO_ONE) && (current_screen != SCR_INTRO_TWO) &&
		(current_screen != SCR_GAME_OVER) && (current_screen != SCR_ESCAPE_POD))
		return;
	
	if ((ship->flags & FLG_DEAD) && !(ship->flags & FLG_EXPLOSION))
	{
		ship->flags |= FLG_EXPLOSION;
		ship->exp_seed = randint();
		ship->exp_delta = 18; 
	}

	if (ship->flags & FLG_EXPLOSION)
	{
		draw_explosion (ship);
		return;
	}
	
	if (ship->location.z <= 0)	/* Only display ships in front of us. */
		return;

	if (ship->type == SHIP_PLANET)
	{
		draw_planet (ship);
		return;
	}

	if (ship->type == SHIP_SUN)
	{
		draw_sun (ship);
		return;
	}
	
	if ((fabs(ship->location.x) > ship->location.z) ||	/* Check for field of vision. */
		(fabs(ship->location.y) > ship->location.z))
		return;
		
	if (wireframe)
		draw_wireframe_ship (ship);
	else
		draw_solid_ship (ship);
}

