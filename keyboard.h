/*
 * Elite - The New Kind.
 *
 * Allegro version of the keyboard routines.
 *
 * The code in this file has not been derived from the original Elite code.
 * Written by C.J.Pinder 1999-2001.
 * email: <christian@newkind.co.uk>
 *
 */

/*
 * keyboard.h
 *
 * Code to handle keyboard input.
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H
 
extern int kbd_F1_pressed;
extern int kbd_F2_pressed;
extern int kbd_F3_pressed;
extern int kbd_F4_pressed;
extern int kbd_F5_pressed;
extern int kbd_F6_pressed;
extern int kbd_F7_pressed;
extern int kbd_F8_pressed;
extern int kbd_F9_pressed;
extern int kbd_F10_pressed;
extern int kbd_F11_pressed;
extern int kbd_F12_pressed;
extern int kbd_y_pressed;
extern int kbd_n_pressed;
extern int kbd_fire_pressed;
extern int kbd_ecm_pressed;
extern int kbd_energy_bomb_pressed;
extern int kbd_hyperspace_pressed;
extern int kbd_ctrl_pressed;
extern int kbd_jump_pressed;
extern int kbd_escape_pressed;
extern int kbd_dock_pressed;
extern int kbd_d_pressed;
extern int kbd_origin_pressed;
extern int kbd_find_pressed;
extern int kbd_fire_missile_pressed;
extern int kbd_target_missile_pressed;
extern int kbd_unarm_missile_pressed;
extern int kbd_pause_pressed;
extern int kbd_resume_pressed;
extern int kbd_inc_speed_pressed;
extern int kbd_dec_speed_pressed;
extern int kbd_up_pressed;
extern int kbd_down_pressed;
extern int kbd_left_pressed;
extern int kbd_right_pressed;
extern int kbd_enter_pressed;
extern int kbd_backspace_pressed;
extern int kbd_space_pressed;


int kbd_keyboard_startup (void);
int kbd_keyboard_shutdown (void);
void kbd_poll_keyboard (void);
int kbd_read_key (void);
void kbd_clear_key_buffer (void);

#endif
 
