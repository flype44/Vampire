/******************************************************************************
**
** SAGACard_Bitmap.h
** Picasso96/SAGA.card
** 
******************************************************************************/

#ifndef SAGACARD_BITMAP_H
#define SAGACARD_BITMAP_H

/*****************************************************************************/

#ifndef boardinfo_H
#include "boardinfo.h"
#endif

/*****************************************************************************/

struct BitMap * Card_AllocBitMapTags(
    struct BoardInfo *bi, 
    ULONG             Width,
    ULONG             Height,
    ULONG             Tags,
    ...);

BOOL Card_FreeBitMapTags(
    struct BoardInfo *bi,
    struct BitMap    *bm,
    ULONG            Tags,
    ...);

/*****************************************************************************/

struct BitMap * ASM SAVEDS Card_AllocBitMap(
    __REGA0(struct BoardInfo *bi),
    __REGD0(ULONG             width),
    __REGD1(ULONG             height),
    __REGA1(struct TagItem   *tags));

BOOL ASM SAVEDS Card_FreeBitMap(
    __REGA0(struct BoardInfo *bi),
    __REGA1(struct BitMap    *bm),
    __REGA2(struct TagItem   *tags));

ULONG ASM SAVEDS Card_GetBitMapAttr(
    __REGA0(struct BoardInfo *bi),
    __REGA1(struct BitMap    *bm),
    __REGD0(ULONG             attr));

/*****************************************************************************/

#endif

