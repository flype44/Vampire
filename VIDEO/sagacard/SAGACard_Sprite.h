/******************************************************************************
**
** SAGACard_Sprite.h
** Picasso96/SAGA.card
** 
******************************************************************************/

#ifndef SAGACARD_SPRITE_H
#define SAGACARD_SPRITE_H

/*****************************************************************************/

#ifndef boardinfo_H
#include "boardinfo.h"
#endif

/*****************************************************************************/

BOOL ASM SAVEDS Card_SetSprite(
    __REGA0(struct BoardInfo  *bi),
    __REGD0(BOOL               activate),
    __REGD7(RGBFTYPE           rgbFormat));

void ASM SAVEDS Card_SetSpriteColor(
    __REGA0(struct BoardInfo  *bi),
    __REGD0(UBYTE              index),
    __REGD1(UBYTE              r),
    __REGD2(UBYTE              g),
    __REGD3(UBYTE              b),
    __REGD7(RGBFTYPE           rgbFormat));

void ASM SAVEDS Card_SetSpriteImage(
    __REGA0(struct BoardInfo  *bi),
    __REGD7(RGBFTYPE           rgbFormat));

void ASM SAVEDS Card_SetSpritePosition(
    __REGA0(struct BoardInfo  *bi),
    __REGD0(WORD               x),
    __REGD1(WORD               y),
    __REGD7(RGBFTYPE           rgbFormat));

/*****************************************************************************/

#endif

