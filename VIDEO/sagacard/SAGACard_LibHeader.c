/******************************************************************************
**
** SAGACard_LibHeader.c
** Picasso96/SAGA.card
** 
******************************************************************************/

#include <exec/initializers.h>
#include <exec/nodes.h>
#include <exec/resident.h>
#include <exec/libraries.h>

/*****************************************************************************/

#ifndef boardinfo_H
#include "boardinfo.h"
#endif

#ifndef SAGACARD_H
#include "SAGACard.h"
#endif

#ifndef SAGACARD_VERSION_H
#include "SAGACard_Version.h"
#endif

/*****************************************************************************/

ASM LONG LibNull(void)
{
    return(0);
}

/*****************************************************************************/

extern const char LibName[];
extern const char LibVersionString[];
extern const APTR LibInitTab[];

/*****************************************************************************/

static const struct Resident _00RomTag;
static const struct Resident _00RomTag = 
{
    RTC_MATCHWORD,
    (struct Resident*) &_00RomTag,
    (struct Resident*) &_00RomTag + 1,
    RTF_AUTOINIT,
    LIBVERSION,
    NT_LIBRARY,
    0,
    (char*)LibName,
    (char*)LibVersionString,
    (APTR)LibInitTab
};

/*****************************************************************************/
