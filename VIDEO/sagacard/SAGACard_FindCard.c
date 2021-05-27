/******************************************************************************
**
** SAGACard_FindCard.c
** Picasso96/SAGA.card
** 
******************************************************************************/

#include <exec/exec.h>
#include <devices/inputevent.h>
#include <libraries/expansion.h>

#include <proto/exec.h>
#include <proto/expansion.h>
#include <proto/input.h>

/*****************************************************************************/

#ifndef boardinfo_H
#include "boardinfo.h"
#endif

#ifndef SAGACARD_H
#include "SAGACard.h"
#endif

/*****************************************************************************/

BOOL SAVEDS ASM FindCard(
    __REGA0(struct BoardInfo *bi))
{
    struct IORequest io;
    struct Library *ExpansionBase;
    
    /* Card detection */
    
    if (ExpansionBase = OpenLibrary(EXPANSIONNAME, 0L))
    {
        if (FindConfigDev(NULL, SAGA_MANUFACTURER_ID, SAGA_PRODUCT_ID) == NULL)
        {
            CloseLibrary(ExpansionBase);
            return(FALSE);
        }
        
        CloseLibrary(ExpansionBase);
    }
    else
    {
        return(FALSE);
    }
    
    /* Skip if SHIFT-key is pressed */
    
    if (OpenDevice("input.device", 0, &io, 0) == 0)
    {
        struct Library *InputBase = (struct Library *)io.io_Device;
        UWORD qual = PeekQualifier();
        CloseDevice(&io);
        
        if (qual & (IEQUALIFIER_LSHIFT | IEQUALIFIER_RSHIFT))
        {
            return(FALSE);
        }
    }
    
    return(TRUE);
}

/*****************************************************************************/
