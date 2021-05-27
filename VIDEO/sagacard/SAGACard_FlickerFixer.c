/******************************************************************************
**
** SAGACard_FlickerFixer.c
** Picasso96/SAGA.card
** 
******************************************************************************/

#ifndef boardinfo_H
#include "boardinfo.h"
#endif

#ifndef SAGACARD_H
#include "SAGACard.h"
#endif

#ifndef SAGACARD_BITMAP_H
#include "SAGACard_Bitmap.h"
#endif

#ifndef SAGACARD_SPECIAL_H
#include "SAGACard_Special.h"
#endif

#ifndef SAGACARD_FLICKERFIXER_H
#include "SAGACard_FlickerFixer.h"
#endif

/*****************************************************************************/

#define D(x) x

/*****************************************************************************/

ULONG Card_CreateFlickerFixer(
    struct BoardInfo    *bi,
    struct FlickerFixer *ff,
    struct TagItem      *tags)
{
    ULONG result;
    struct TagItem *tag;
    struct TagItem *tstate = tags;
    
    D(KPrintf("Card_CreateFlickerFixer(ff=0x%08lx)\n", ff));
    
    if (!(bi->Flags & BIF_FLICKERFIXER))
    {
        return(1);
    }
    
    while (tag = NextTagItem(&tstate))
    {
        ULONG value = tag->ti_Data;
        
        switch (tag->ti_Tag)
        {
        case FA_Interlace:
            ff->IsInterlace = value;
            break;
            
        case FA_PAL:
            ff->IsPAL = value;
            break;
        }
    }
    
    /* Allocate FlickerFixer BitMap */
    
    if (ff->BitMap = Card_AllocBitMapTags(bi, 
        724,
        283 * (ff->IsInterlace ? 2 : 1),
        ABMA_RGBFormat,   ff->Format,
        ABMA_NoMemory,    TRUE,
        ABMA_Clear,       TRUE,
        ABMA_Displayable, TRUE,
        ABMA_Visible,     TRUE,
        TAG_DONE))
    {
        /* Modify hardware */
        
        result = Card_SetFlickerFixerAttrs(bi, ff, tags);
    }
    else
    {
        result = 1;
    }
    
    return(result);
}

/*****************************************************************************/

void Card_DeleteFlickerFixer(
    struct BoardInfo    *bi,
    struct FlickerFixer *ff)
{
    D(KPrintf("Card_DeleteFlickerFixer(ff=0x%08lx)\n", ff));
    {
        /* TODO: HARDWARE IMPLEMENTATION */
    }
    
    Card_FreeBitMapTags(bi, ff->BitMap, TAG_DONE);
}

/*****************************************************************************/

ULONG Card_SetFlickerFixerAttrs(
    struct BoardInfo    *bi,
    struct FlickerFixer *ff,
    struct TagItem      *tags)
{
    ULONG result = 0;
    struct TagItem *tag;
    struct TagItem *tstate = tags;
    ULONG oldFlags = ff->Flags;
    
    D(KPrintf("Card_SetFlickerFixerAttrs(ff=0x%08lx)\n", ff));
    
    ff->ChangeFlags = 0;
    
    while (tag = NextTagItem(&tstate))
    {
        ULONG value = tag->ti_Data;
        
        switch (tag->ti_Tag)
        {
        case FA_Active:
            if (value) {
                ff->Flags |= FFF_ACTIVE;
            } else {
                ff->Flags &= ~FFF_ACTIVE;
            }
            break;
            
        case FA_Onboard:
            if (value) {
                ff->Flags |= FFF_ONBOARD;
            } else {
                ff->Flags &= ~FFF_ONBOARD;
            }
            break;
        }
    }

    if ((ff->Flags & (FFF_ACTIVE | FFF_ONBOARD)) == (FFF_ACTIVE | FFF_ONBOARD))
    {
        if ((oldFlags & (FFF_ACTIVE | FFF_ONBOARD)) != (FFF_ACTIVE | FFF_ONBOARD))
        {
            D(KPrintf("Starting FlickerFixer\n"));
            {
                /* TODO: HARDWARE IMPLEMENTATION */
            }
        }
    }
    else
    {
        if ((oldFlags & (FFF_ACTIVE | FFF_ONBOARD)) == (FFF_ACTIVE | FFF_ONBOARD))
        {
            D(KPrintf("Stopping FlickerFixer\n"));
            {
                /* TODO: HARDWARE IMPLEMENTATION */
            }
        }
    }
    
    return(result);
}

/*****************************************************************************/

ULONG Card_GetFlickerFixerAttrs(
    struct BoardInfo    *bi,
    struct FlickerFixer *ff,
    struct TagItem      *tags)
{
    ULONG result = 0;
    struct TagItem *tag;
    struct TagItem *tstate = tags;
    
    D(KPrintf("Card_GetFlickerFixerAttrs(ff=0x%08lx)\n", ff));
    
    while (tag = NextTagItem(&tstate))
    {
        ULONG *ptr = ((ULONG *)tag->ti_Data);
        
        switch(tag->ti_Tag)
        {
        case FA_Interlace:
            *ptr = (ULONG)ff->IsInterlace;
            break;
        case FA_PAL:
            *ptr = (ULONG)ff->IsPAL;
            break;
        case FA_BitMap:
            *ptr = (ULONG)ff->BitMap;
            break;
        }
    }
    
    return(result);
}

/*****************************************************************************/
