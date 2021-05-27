/******************************************************************************
**
** SAGACard_Settings.h
** Picasso96/SAGA.card
** 
******************************************************************************/

#ifndef SAGACARD_SETTINGS_H
#define SAGACARD_SETTINGS_H

/*****************************************************************************/

#ifndef boardinfo_H
#include "boardinfo.h"
#endif

/*****************************************************************************/

BOOL AddResolution(
    struct BoardInfo *bi,
    ULONG             DisplayID,
    ULONG             Width,
    ULONG             Height);

void AddResolutionList(
    struct BoardInfo *bi);

/*****************************************************************************/

#endif
