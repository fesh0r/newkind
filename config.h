/**
 *
 * Elite - The New Kind.
 *
 * The code in this file has not been derived from the original Elite code.
 * Written by C.J.Pinder 1999-2001.
 *
 * email: <christian@newkind.co.uk>
 *
 **/

/*
 * config.h
 *
 * Define the compile time options.
 * Most options are defined at runtime in the newkind.cfg file.
 * However some must be done at compile time either because they are
 * platform dependant or for playing speed.
 */
 
 
#ifndef CONFIG_H
#define CONFIG_H

/*
 * Set the graphics platform we are using...
 */

#define GFX_ALLEGRO

/*
 * #define GFX_WIN32_GDI
 * #define GFX_OPENGL
 * #defime GFX_X_WINDOWS
 * #define GFX_DIRECTX
 */

/*
 * Set the screen resolution...
 * (if nothing is defined, assume 256x256).
 */

#define RES_800_600
// #define RES_512_512

/*
 * #define RES_640_480
 * #define RES_320_240
 */

#endif
