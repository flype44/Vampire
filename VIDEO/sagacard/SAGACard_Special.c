/******************************************************************************
**
** SAGACard_Special.c
** Picasso96/SAGA.card
** 
******************************************************************************/

#ifndef boardinfo_H
#include "boardinfo.h"
#endif

#ifndef SAGACARD_H
#include "SAGACard.h"
#endif

#ifndef SAGACARD_SPECIAL_H
#include "SAGACard_Special.h"
#endif

#ifndef SAGACARD_MEMORYWINDOW_H
#include "SAGACard_MemoryWindow.h"
#endif

#ifndef SAGACARD_VIDEOWINDOW_H
#include "SAGACard_VideoWindow.h"
#endif

#ifndef SAGACARD_VIDEOCAPTURE_H
#include "SAGACard_VideoCapture.h"
#endif

#ifndef SAGACARD_FLICKERFIXER_H
#include "SAGACard_FlickerFixer.h"
#endif

/*****************************************************************************/

#define	D(x) x

/*****************************************************************************/

APTR SAVEDS ASM Card_CreateFeature(
    __REGA0(struct BoardInfo *bi),
    __REGD0(ULONG             type),
    __REGA1(struct TagItem   *tags))
{
    APTR result = NULL;
    
    D(KPrintf("Card_CreateFeature(type=0x%08lx)\n", type));
    
    switch (type)
    {
        case SFT_MEMORYWINDOW:
            
            if (result = AllocVec(sizeof(struct MemoryWindow), MEMF_CLEAR | MEMF_ANY))
            {
                if (Card_CreateMemoryWindow(bi, 
                    (struct MemoryWindow *)result, tags))
                {
                    FreeVec(result);
                    result = NULL;
                }
            }
            
            break;
            
        case SFT_VIDEOWINDOW:
            
            if (result = AllocVec(sizeof(struct VideoWindow), MEMF_CLEAR | MEMF_ANY))
            {
                if (Card_CreateVideoWindow(bi, 
                    (struct VideoWindow *)result, tags))
                {
                    FreeVec(result);
                    result = NULL;
                }
            }
            
            break;
            
        case SFT_VIDEOCAPTURE:
            
            if (result = AllocVec(sizeof(struct VideoCapture), MEMF_CLEAR | MEMF_ANY))
            {
                if (Card_CreateVideoCapture(bi, 
                    (struct VideoCapture *)result, tags))
                {
                    FreeVec(result);
                    result = NULL;
                }
            }
            
            break;
            
        case SFT_FLICKERFIXER:
            
            if (result = AllocVec(sizeof(struct FlickerFixer), MEMF_CLEAR | MEMF_ANY))
            {
                if (Card_CreateFlickerFixer(bi, 
                    (struct FlickerFixer *)result, tags))
                {
                    FreeVec(result);
                    result = NULL;
                }
            }
            
            break;
    }
    
    return(result);
}

/*****************************************************************************/

ULONG ASM SAVEDS Card_SetFeatureAttrs(
    __REGA0(struct BoardInfo *bi),
    __REGA1(APTR              featureData),
    __REGD0(ULONG             type),
    __REGA2(struct TagItem   *tags))
{
    ULONG result = 0;
    
    D(KPrintf("Card_SetFeatureAttrs(type=0x%08lx)\n", type));
    
    switch (type)
    {
        case SFT_MEMORYWINDOW:
            result = Card_SetMemoryWindowAttrs(bi, 
                (struct MemoryWindow *)featureData, tags);
            break;
            
        case SFT_VIDEOWINDOW:
            result = Card_SetVideoWindowAttrs(bi, 
                (struct VideoWindow *)featureData, tags);
            break;
            
        case SFT_VIDEOCAPTURE:
            result = Card_SetVideoCaptureAttrs(bi, 
                (struct VideoCapture *)featureData, tags);
            break;
            
        case SFT_FLICKERFIXER:
            result = Card_SetFlickerFixerAttrs(bi, 
                (struct FlickerFixer *)featureData, tags);
            break;
    }
    
    return(result);
}

/*****************************************************************************/

ULONG ASM SAVEDS Card_GetFeatureAttrs(
    __REGA0(struct BoardInfo *bi),
    __REGA1(APTR              featureData),
    __REGD0(ULONG             type),
    __REGA2(struct TagItem   *tags))
{
    ULONG result = 0;
    
    D(KPrintf("Card_GetFeatureAttrs(type=0x%08lx)\n", type));
    
    switch (type)
    {
        case SFT_MEMORYWINDOW:
            result = Card_GetMemoryWindowAttrs(bi, 
                (struct MemoryWindow *)featureData, tags);
            break;
            
        case SFT_VIDEOWINDOW:
            result = Card_GetVideoWindowAttrs(bi, 
                (struct VideoWindow *)featureData, tags);
            break;
            
        case SFT_VIDEOCAPTURE:
            result = Card_GetVideoCaptureAttrs(bi, 
                (struct VideoCapture *)featureData, tags);
            break;
            
        case SFT_FLICKERFIXER:
            result = Card_GetFlickerFixerAttrs(bi, 
                (struct FlickerFixer *)featureData, tags);
            break;
    }
    
    return(result);
}

/*****************************************************************************/

BOOL ASM SAVEDS Card_DeleteFeature(
    __REGA0(struct BoardInfo *bi),
    __REGA1(APTR              featureData),
    __REGD0(ULONG             type))
{
    D(KPrintf("Card_DeleteFeature(type=0x%08lx)\n", type));
    
    switch (type)
    {
        case SFT_MEMORYWINDOW:
            Card_DeleteMemoryWindow(bi, 
                (struct MemoryWindow *)featureData);
            break;
            
        case SFT_VIDEOWINDOW:
            Card_DeleteVideoWindow(bi, 
                (struct VideoWindow *)featureData);
            break;
            
        case SFT_VIDEOCAPTURE:
            Card_DeleteVideoCapture(bi, 
                (struct VideoCapture *)featureData);
            break;
            
        case SFT_FLICKERFIXER:
            Card_DeleteFlickerFixer(bi, 
                (struct FlickerFixer *)featureData);
            break;
    }
    
    FreeVec(featureData);
    
    return(TRUE);
}

/*****************************************************************************/
