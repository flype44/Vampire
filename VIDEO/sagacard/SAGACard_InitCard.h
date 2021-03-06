/******************************************************************************
**
** SAGACard_InitCard.h
** Picasso96/SAGA.card
** 
******************************************************************************/

#ifndef SAGACARD_INITCARD_H
#define SAGACARD_INITCARD_H

/*****************************************************************************/

#ifndef boardinfo_H
#include "boardinfo.h"
#endif

/*****************************************************************************/

BOOL SAVEDS ASM InitCard(
    __REGA0(struct BoardInfo *bi),
    __REGA1(char **toolTypes));

/*****************************************************************************/

#endif
