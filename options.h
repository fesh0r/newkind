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
 * options.h
 */
 
#ifndef OPTIONS_H
#define OPTIONS_H

void display_options (void);
void select_previous_option (void);
void select_next_option (void);
void do_option (void);

void select_left_setting (void);
void select_right_setting (void);
void select_up_setting (void);
void select_down_setting (void);
void toggle_setting (void);

#endif
