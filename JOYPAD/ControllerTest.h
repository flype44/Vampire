#ifndef __CONTROLLERTEST_H__
#define __CONTROLLERTEST_H__

/*****************************************************************
 *
 * MACROS
 * 
 *****************************************************************/

#define BTST(a, b) (((a) & b) == b)

const STRPTR TITLE = "V4 JoyPad Tester 1.0 @ APOLLO-Team";
const STRPTR VERSTRING = "$VER: V4JoyTest 1.0 (7.2.2021) (C) APOLLO-Team";

/*****************************************************************
 *
 * Vampire AttnFlags (V2/V4)
 * 
 *****************************************************************/

#ifndef AFB_68080
#define AFB_68080 (10)
#endif

#ifndef AFF_68080
#define AFF_68080 (1<<AFB_68080)
#endif

/*****************************************************************
 *
 * Vampire Board Register (MODEL & XFREQ) (V2/V4)
 * 
 *****************************************************************/

#define VREG_BOARD		0xdff3fc // (R) (16-bit)

#define VBOARD_MODEL(a) (((a) >> 8) && 0xff)
#define VBOARD_XFREQ(a) ((a) && 0xff)

#define VMODEL_Unknown  0x00
#define VMODEL_V600     0x01
#define VMODEL_V500     0x02
#define VMODEL_V4       0x03
#define VMODEL_V666     0x04
#define VMODEL_V4SA     0x05
#define VMODEL_V1200    0x06
#define VMODEL_Future   0x07

/*****************************************************************
 *
 * Vampire Joypad Registers (JOYxBTN) (V4)
 * 
 *****************************************************************/

#define VREG_JOYP1      0xdff220 // (R) (16-bit)
#define VREG_JOYP2      0xdff222 // (R) (16-bit)

#define VJOYB_PLUGGED   0
#define VJOYB_A         1
#define VJOYB_B         2
#define VJOYB_X         3
#define VJOYB_Y         4
#define VJOYB_BACK      5
#define VJOYB_PLAY      6
#define VJOYB_LB        7
#define VJOYB_RB        8
#define VJOYB_LT        9
#define VJOYB_RT        10
#define VJOYB_FREE      11
#define VJOYB_UP        12
#define VJOYB_DOWN      13
#define VJOYB_LEFT      14
#define VJOYB_RIGHT     15

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

#endif
