/******************************************************************************
**
** SAGACard_InitCard.c
** Picasso96/SAGA.card
** 
******************************************************************************/

#include <exec/exec.h>
#include <proto/exec.h>
#include <proto/utility.h>

/*****************************************************************************/

#ifndef boardinfo_H
#include "boardinfo.h"
#endif

#ifndef SAGACARD_H
#include "SAGACard.h"
#endif

#ifndef SAGACARD_BITMAP_H
#include "SAGACard_Bitmap.h"
#endif

#ifndef SAGACARD_CARDMEM_H
#include "SAGACard_CardMem.h"
#endif

#ifndef SAGACARD_Settings_H
#include "SAGACard_Settings.h"
#endif

#ifndef SAGACARD_SPECIAL_H
#include "SAGACard_Special.h"
#endif

#ifndef SAGACARD_SPRITE_H
#include "SAGACard_Sprite.h"
#endif

#ifndef SAGACARD_SUPPORT_H
#include "SAGACard_Support.h"
#endif

/*****************************************************************************/

ULONG LIBDEBUG = FALSE;

struct ExecBase *SysBase;
struct Library  *UtilityBase;

/*****************************************************************************/

BOOL SAVEDS ASM InitCard(
    __REGA0(struct BoardInfo *bi),
    __REGA1(char **toolTypes))
{
    int  i;
    APTR membase;
    
    /* Reset CardData fields */
    
    for (i = 0; i < 16; i++)
    {
        bi->CardData[i] = 0L;
    }
    
    /* ToolTypes */
    
    if (UtilityBase = (struct Library*)bi->UtilBase)
    {
        char **tt;
        
        for (tt = toolTypes; tt && *tt; tt++)
        {
            if (Strnicmp(*tt, "LIBDEBUG=YES", 12) == 0)
            {
                bi->CardData[CARDDATA_FLAGS] |= CARDD_FLG_DEBUG;
            }
        }
    }
    
    /* Allocate video memory */
    
    if (!(membase = AllocMem(SAGA_VIDEO_SIZE + 31, 
        MEMF_FAST | MEMF_LOCAL | MEMF_PUBLIC)))
    {
        return(FALSE);
    }
    
    /* Properties */
    
    bi->BoardName              = "SAGA";
    bi->BoardType              = 1;
    bi->PaletteChipType        = PCT_Unknown;
    bi->GraphicsControllerType = GCT_Unknown;
    bi->BitsPerCannon          = 8;
    bi->SoftSpriteFlags        = 0;
    bi->RegisterBase           = 0; /* 0xDFF000 on V4 */
    bi->MemoryIOBase           = 0;
    bi->MemoryBase             = (APTR)(((ULONG)(membase) + 31) & ~31);
    bi->MemorySpaceBase        = (APTR)(((ULONG)(membase) + 31) & ~31);
    bi->MemorySize             = SAGA_VIDEO_SIZE;
    bi->MemorySpaceSize        = SAGA_VIDEO_SIZE;
    
    /* Flags */
    
    bi->Flags = (
        BIF_GRANTDIRECTACCESS | 
        BIF_INDISPLAYCHAIN    | 
        BIF_INTERNALMODESONLY | 
        BIF_NEEDSALIGNMENT    | 
        BIF_NOMEMORYMODEMIX   | 
        BIF_NOBLITTER         | 
        BIF_HARDWARESPRITE    | 
        BIF_VIDEOWINDOW       
      //BIF_VIDEOCAPTURE      | /* Not avail  on V4 */
      //BIF_HASSPRITEBUFFER   | /* Not avail  on V4 */
      //BIF_DBLSCANDBLSPRITEY | /* Not avail  on V4 */
      //BIF_ILACEHALFSPRITEY  | /* Not avail  on V4 */
      //BIF_ILACEDBLROWOFFSET | /* Not avail  on V4 */
      //BIF_HIRESSPRITE       | /* Not avail  on V4 */
      //BIF_BIGSPRITE         | /* Not avail  on V4 */
      //BIF_CACHEMODECHANGE   | /* Not needed on V4 */
      //BIF_VBLANKINTERRUPT   | /* Not avail  on V4 */
      //BIF_FLICKERFIXER      | /* Not avail  on V4 */
      //BIF_VGASCREENSPLIT    | /* Not avail  on V4 */
    );
    
    /* RGBFormats */
    
    bi->RGBFormats = (
        RGBFF_CLUT      |  //  8-bit
        RGBFF_R5G5B5    |  // 15-bit
        RGBFF_R5G6B5    |  // 16-bit
        RGBFF_R8G8B8    |  // 24-bit
        RGBFF_A8R8G8B8  |  // 32-bit
        RGBFF_A8B8G8R8     // 32-bit
    );
    
    bi->EssentialFormats = (
        RGBFF_CLUT      | //  8-bit
        RGBFF_R5G6B5    | // 16-bit
        RGBFF_R8G8B8    | // 24-bit
        RGBFF_A8R8G8B8    // 32-bit
    );
    
    /* Maximum displayable and scrollable sizes */
    
    for (i = 0; i < MAXMODES; i++)
    {
        bi->MaxHorValue[i]      = SAGA_VIDEO_MAXHV;
        bi->MaxVerValue[i]      = SAGA_VIDEO_MAXVV;
        bi->MaxHorResolution[i] = SAGA_VIDEO_MAXHR;
        bi->MaxVerResolution[i] = SAGA_VIDEO_MAXVR;
        bi->PixelClockCount[i]  = SAGA_PIXEL_CLOCK;
    }
    
    /* Internal resolutions */
    
    if (TRUE)
    {
        AddResolutionList(bi);
    }
    
    /* Bitmap functions */
    
    if (FALSE)
    {
        bi->AllocBitMap            = Card_AllocBitMap;
        bi->FreeBitMap             = Card_FreeBitMap;
        bi->GetBitMapAttr          = Card_GetBitMapAttr;
    }
    
    /* Memory functions */
    
    if (TRUE)
    {
        bi->AllocCardMem           = Card_AllocCardMem;
        bi->FreeCardMem            = Card_FreeCardMem;
    }
    
    /* Support functions */
    
    if (TRUE)
    {
        bi->CalculateBytesPerRow   = Card_CalculateBytesPerRow;
        bi->CalculateMemory        = Card_CalculateMemory;
        bi->GetCompatibleFormats   = Card_GetCompatibleFormats;
        bi->GetPixelClock          = Card_GetPixelClock;
        bi->GetVBeamPos            = Card_GetVBeamPos;
        bi->GetVSyncState          = Card_GetVSyncState;
        bi->ResetChip              = Card_ResetChip;
        bi->ResolvePixelClock      = Card_ResolvePixelClock;
        bi->SetClearMask           = Card_SetClearMask;
        bi->SetClock               = Card_SetClock;
        bi->SetColorArray          = Card_SetColorArray;
        bi->SetDAC                 = Card_SetDAC;
        bi->SetDisplay             = Card_SetDisplay;
        bi->SetDPMSLevel           = Card_SetDPMSLevel;
        bi->SetGC                  = Card_SetGC;
        bi->SetMemoryMode          = Card_SetMemoryMode;
        bi->SetPanning             = Card_SetPanning;
        bi->SetReadPlane           = Card_SetReadPlane;
        bi->SetSwitch              = Card_SetSwitch;
        bi->SetWriteMask           = Card_SetWriteMask;
        bi->WaitBlitter            = Card_WaitBlitter;
        bi->WaitVerticalSync       = Card_WaitVerticalSync;
    }
    
    /* Drawing functions */
    
    if (HASFLAG(bi->Flags, BIF_NOBLITTER))
    {
        bi->BlitPattern            = bi->BlitPatternDefault;
        bi->BlitPlanar2Chunky      = bi->BlitPlanar2ChunkyDefault;
        bi->BlitPlanar2Direct      = bi->BlitPlanar2DirectDefault;
        bi->BlitRect               = bi->BlitRectDefault;
        bi->BlitRectNoMaskComplete = bi->BlitRectNoMaskCompleteDefault;
        bi->BlitTemplate           = bi->BlitTemplateDefault;
        bi->DrawLine               = bi->DrawLineDefault; 
        bi->FillRect               = bi->FillRectDefault;
        bi->InvertRect             = bi->InvertRectDefault; 
        bi->ScrollPlanar           = bi->ScrollPlanarDefault;
        bi->UpdatePlanar           = bi->UpdatePlanarDefault;
        bi->WriteYUVRect           = bi->WriteYUVRectDefault;
    }
    
    /* Sprite functions */
    
    if (HASFLAG(bi->Flags, BIF_HARDWARESPRITE))
    {
        bi->SetSprite              = Card_SetSprite;
        bi->SetSpriteColor         = Card_SetSpriteColor;
        bi->SetSpriteImage         = Card_SetSpriteImage;
        bi->SetSpritePosition      = Card_SetSpritePosition;
    }
    
    /* Special-features functions */
    
    if (HASFLAG(bi->Flags, BIF_VIDEOCAPTURE) ||
        HASFLAG(bi->Flags, BIF_VIDEOWINDOW ) ||
        HASFLAG(bi->Flags, BIF_FLICKERFIXER))
    {
        bi->CreateFeature          = Card_CreateFeature;
        bi->DeleteFeature          = Card_DeleteFeature;
        bi->GetFeatureAttrs        = Card_GetFeatureAttrs;
        bi->SetFeatureAttrs        = Card_SetFeatureAttrs;
    }
    
    return(TRUE);
}

/*****************************************************************************/
