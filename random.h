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
 * random.h
 */

#ifndef RANDOM_H
#define RANDOM_H

int randint (void);
void set_rand_seed (int seed);
int get_rand_seed (void);
int rand255 (void);

#endif

