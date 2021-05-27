/******************************************************************************
**
** SAGACard_VideoCapture.c
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

#ifndef SAGACARD_VIDEOCAPTURE_H
#include "SAGACard_VideoCapture.h"
#endif

/*****************************************************************************/

#define D(x) x

/*****************************************************************************/

ULONG Card_CreateVideoCapture(
    struct BoardInfo    *bi,
    struct VideoCapture *vc,
    struct TagItem      *tags)
{
    ULONG result;
    struct TagItem *tag;
    struct TagItem *tstate = tags;
    
    D(KPrintf("Card_CreateVideoCapture(vc=0x%08lx)\n", vc));
    
    if (!(bi->Flags & BIF_VIDEOCAPTURE))
    {
        return(1);
    }
    
    while (tag = NextTagItem(&tstate))
    {
        ULONG value = tag->ti_Data;
        
        switch (tag->ti_Tag)
        {
        case FA_SourceWidth:
            vc->SourceWidth = value;
            break;
        case FA_SourceHeight:
            vc->SourceHeight = value;
            break;
        case FA_Format:
            vc->Format = value;
            break;
        case FA_Interlace:
            vc->Interlace = value;
            break;
        }
    }
    
    /* Allocate Capture BitMap */
    
    if (vc->BitMap = Card_AllocBitMapTags(bi, 
        vc->SourceWidth, 
        vc->SourceHeight * (vc->Interlace ? 2 : 1),
        ABMA_RGBFormat,   vc->Format,
        ABMA_Clear,       FALSE,
        ABMA_Displayable, TRUE,
        ABMA_Visible,     TRUE,
        TAG_DONE))
    {
        /* Modify hardware */
        
        result = Card_SetVideoCaptureAttrs(bi, vc, tags);
    }
    else
    {
        result = 1;
    }
    
    return(result);
}

/*****************************************************************************/

void Card_DeleteVideoCapture(
    struct BoardInfo    *bi,
    struct VideoCapture *vc)
{
    D(KPrintf("Card_DeleteVideoCapture(vc=0x%08lx)\n", vc));
    {
        /* TODO: HARDWARE IMPLEMENTATION */
    }
    
    Card_FreeBitMapTags(bi, vc->BitMap, TAG_DONE);
}

/*****************************************************************************/

ULONG Card_SetVideoCaptureAttrs(
    struct BoardInfo    *bi,
    struct VideoCapture *vc,
    struct TagItem      *tags)
{
    ULONG result = 0;
    struct TagItem *tag;
    struct TagItem *tstate = tags;
    ULONG oldFlags = vc->Flags;
    
    D(KPrintf("Card_SetVideoCaptureAttrs(vc=0x%08lx)\n", vc));
    
    vc->ChangeFlags = 0;
    
    while (tag = NextTagItem(&tstate))
    {
        ULONG value = tag->ti_Data;
        
        switch (tag->ti_Tag)
        {
        case FA_Active:
            if (value) {
                vc->Flags |= VCF_ACTIVE;
            } else {
                vc->Flags &= ~VCF_ACTIVE;
            }
            break;
        case FA_Onboard:
            if (value) {
                vc->Flags |= VCF_ONBOARD;
            } else {
                vc->Flags &= ~VCF_ONBOARD;
            }
            break;
        }
    }
    
    if ((vc->Flags & (VCF_ACTIVE | VCF_ONBOARD)) == (VCF_ACTIVE | VCF_ONBOARD))
    {
        D(KPrintf("VideoCapture on board and active\n"));
        
        if ((oldFlags & (VCF_ACTIVE | VCF_ONBOARD)) != (VCF_ACTIVE | VCF_ONBOARD))
        {
            vc->ChangeFlags |= VCCF_RESTORE;
        }
        
        if (vc->ChangeFlags & VCCF_RESTORE)
        {
            D(KPrintf("Starting VideoCapture\n"));
            {
                /* TODO: HARDWARE IMPLEMENTATION */
            }
        }
        
        if (vc->ChangeFlags)
        {
            if (vc->ChangeFlags & VCCF_RESTORE)
            {
                D(KPrintf("VideoCapture running\n"));
                {
                    /* TODO: HARDWARE IMPLEMENTATION */
                }
            }
        }
        else
        {
            D(KPrintf("VideoCapture did not change\n"));
        }
    }
    else
    {
        D(KPrintf("VideoCapture off board or inactive\n"));
        
        if ((oldFlags & (VCF_ACTIVE | VCF_ONBOARD)) == (VCF_ACTIVE | VCF_ONBOARD))
        {
            D(KPrintf("Stopping VideoCapture\n"));
            {
                /* TODO: HARDWARE IMPLEMENTATION */
            }
        }
    }
    
    return(result);
}

/*****************************************************************************/

ULONG Card_GetVideoCaptureAttrs(
    struct BoardInfo *bi, 
    struct VideoCapture *vc, 
    struct TagItem *Tags)
{
    ULONG result = 1;
    struct TagItem *tag;
    struct TagItem *tstate = Tags;
    
    D(KPrintf("Card_GetVideoCaptureAttrs(vc=0x%08lx)\n", vc));
    
    while (tag = NextTagItem(&tstate))
    {
        ULONG *ptr = ((ULONG *)tag->ti_Data);
        
        switch (tag->ti_Tag)
        {
        case FA_SourceWidth:
            *ptr = vc->SourceWidth;
            break;
        case FA_SourceHeight:
            *ptr = vc->SourceHeight;
            break;
        case FA_BitMap:
            *ptr = (ULONG)vc->BitMap;
            break;
        }
    }
    
    return(result);
}

/*****************************************************************************/
