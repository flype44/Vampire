/******************************************************************************
**
** SAGACard_CardMem.h
** Picasso96/SAGA.card
** 
******************************************************************************/

#ifndef SAGACARD_CARDMEM_H
#define SAGACARD_CARDMEM_H

/*****************************************************************************/

#ifndef boardinfo_H
#include "boardinfo.h"
#endif

/*****************************************************************************/

APTR ASM SAVEDS Card_AllocCardMem(
    __REGA0(struct BoardInfo  *bi),
    __REGD0(ULONG              size),
    __REGD1(BOOL               force),
    __REGD2(BOOL               system));

BOOL ASM SAVEDS Card_FreeCardMem(
    __REGA0(struct BoardInfo  *bi),
    __REGA1(APTR               mem));

/*****************************************************************************/

#endif

