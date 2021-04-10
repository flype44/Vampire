#ifndef	VAMPIRE_SAGA_JOYPAD_H
#define	VAMPIRE_SAGA_JOYPAD_H
/*
**	$VER: saga/joypad.h 0.1 (09.04.2021)
**	
**	Vampire SAGA JOYPAD hardware
**	
**	(C) Copyright 2015-2021 APOLLO-Team
**	    All Right Reserved
*/

#define VREG_JOYP1      0xdff220 // (R) (16-bit)
#define VREG_JOYP2      0xdff222 // (R) (16-bit)

#define VJOYB_PLUGGED   0        // RIGHT
#define VJOYB_A         1        // LEFT
#define VJOYB_B         2        // DOWN
#define VJOYB_X         3        // UP
#define VJOYB_Y         4        // RESERVED
#define VJOYB_BACK      5        // START
#define VJOYB_PLAY      6        // BACK
#define VJOYB_LB        7        // FIRE8
#define VJOYB_RB        8        // FIRE7
#define VJOYB_LT        9        // FIRE6
#define VJOYB_RT        10       // FIRE5
#define VJOYB_FREE      11       // FIRE4
#define VJOYB_UP        12       // FIRE3
#define VJOYB_DOWN      13       // FIRE2
#define VJOYB_LEFT      14       // FIRE1
#define VJOYB_RIGHT     15       // PLUG

#define VJOYF_PLUGGED   (1 << VJOYB_PLUGGED)
#define VJOYF_A         (1 << VJOYB_A)
#define VJOYF_B         (1 << VJOYB_B)
#define VJOYF_X         (1 << VJOYB_X)
#define VJOYF_Y         (1 << VJOYB_Y)
#define VJOYF_BACK      (1 << VJOYB_BACK)
#define VJOYF_PLAY      (1 << VJOYB_PLAY)
#define VJOYF_LB        (1 << VJOYB_LB)
#define VJOYF_RB        (1 << VJOYB_RB)
#define VJOYF_LT        (1 << VJOYB_LT)
#define VJOYF_RT        (1 << VJOYB_RT)
#define VJOYF_FREE      (1 << VJOYB_FREE)
#define VJOYF_UP        (1 << VJOYB_UP)
#define VJOYF_DOWN      (1 << VJOYB_DOWN)
#define VJOYF_LEFT      (1 << VJOYB_LEFT)
#define VJOYF_RIGHT     (1 << VJOYB_RIGHT)

#endif	/* VAMPIRE_SAGA_JOYPAD_H */
