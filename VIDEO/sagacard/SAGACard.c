/******************************************************************************
**
** SAGACard.c
** Picasso96/SAGA.card
** 
******************************************************************************/

#include <exec/types.h>

#ifndef SAGACARD_H
#include "SAGACard.h"
#endif

/*****************************************************************************/

void Write16(int reg, UWORD value)
{
    /*
    D(KPrintf("Write16(reg=0x%08lx, value=0x%08lx)\n", reg, (ULONG)value));
    */
    *(volatile UWORD *)reg = value;
}

/*****************************************************************************/

void Write32(int reg, ULONG value)
{
    /*
    D(KPrintf("Write32(reg=0x%08lx, value=0x%08lx)\n", reg, value));
    */
    
    *(volatile ULONG *)reg = value;
}

/*****************************************************************************/
