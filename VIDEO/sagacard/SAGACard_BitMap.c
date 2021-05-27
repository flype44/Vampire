/******************************************************************************
**
** SAGACard_Bitmap.c
** Picasso96/SAGA.card
** 
******************************************************************************/

#include <exec/exec.h>
#include <proto/exec.h>

/*****************************************************************************/

#ifndef boardinfo_H
#include "boardinfo.h"
#endif

#ifndef SAGACARD_H
#include "SAGACard.h"
#endif

/*****************************************************************************/

#define	D(x) x

/*****************************************************************************/

struct BitMap * Card_AllocBitMapTags(
    struct BoardInfo *bi,
    ULONG             width,
    ULONG             height,
    ULONG             tags, ...)
{
    D(KPrintf("Card_AllocBitMapTags(w=%ld, h=%ld)\n", width, height));
    
    return(bi->AllocBitMap(bi, width, height, (struct TagItem *)&tags));
}

/*****************************************************************************/

BOOL Card_FreeBitMapTags(
    struct BoardInfo *bi,
    struct BitMap    *bm,
    ULONG            tags, ...)
{
    D(KPrintf("Card_FreeBitMapTags(bm=0x%08lx)\n", bm));
    
    return(bi->FreeBitMap(bi, bm, (struct TagItem *)&tags));
}

/*****************************************************************************/

struct BitMap * ASM SAVEDS Card_AllocBitMap(
    __REGA0(struct BoardInfo *bi),
    __REGD0(ULONG             width),
    __REGD1(ULONG             height),
    __REGA1(struct TagItem   *tags))
{
    D(KPrintf("Card_AllocBitMap(w=%ld, h=%ld)\n", width, height));
    
    return(bi->AllocBitMap(bi, width, height, tags));
}

/*****************************************************************************/

BOOL ASM SAVEDS Card_FreeBitMap(
    __REGA0(struct BoardInfo *bi),
    __REGA1(struct BitMap    *bm),
    __REGA2(struct TagItem   *tags))
{
    D(KPrintf("Card_FreeBitMap(bm=0x%08lx)\n", bm));
    
    return(bi->FreeBitMap(bi, bm, tags));
}

/*****************************************************************************/

ULONG ASM SAVEDS Card_GetBitMapAttr(
    __REGA0(struct BoardInfo *bi),
    __REGA1(struct BitMap    *bm),
    __REGD0(ULONG             attr))
{
    D(KPrintf("Card_GetBitMapAttr(bm=0x%08lx, attr=0x%08lx)\n", bm, attr));
    
    return(bi->GetBitMapAttr(bi, bm, attr));
}

/*****************************************************************************/
