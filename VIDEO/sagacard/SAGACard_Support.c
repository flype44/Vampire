/******************************************************************************
**
** SAGACard_Support.c
** Picasso96/SAGA.card
** 
******************************************************************************/

#include <exec/exec.h>
#include <proto/exec.h>

/*****************************************************************************/

#ifndef boardinfo_H
#include "boardinfo.h"
#endif

#ifndef SAGACARD_H
#include "SAGACard.h"
#endif

#ifndef SAGACARD_SPRITE_H
#include "SAGACard_Sprite.h"
#endif

#ifndef SAGACARD_SUPPORT_H
#include "SAGACard_Support.h"
#endif

/*****************************************************************************/

#define FOREACH(TYPE, LIST, NODE) \
    for (NODE = (TYPE)((struct List *)(LIST))->lh_Head; \
        ((struct Node *)NODE)->ln_Succ; \
        NODE = (TYPE)((struct Node *)(NODE))->ln_Succ)

/*****************************************************************************/

const UWORD PixelWidth[21] = {
    1,   // RGBFB_NONE
    1,   // RGBFB_CLUT
    3,   // RGBFB_R8G8B8
    3,   // RGBFB_B8G8R8
    2,   // RGBFB_R5G6B5PC
    2,   // RGBFB_R5G5B5PC
    4,   // RGBFB_A8R8G8B8
    4,   // RGBFB_A8B8G8R8
    4,   // RGBFB_R8G8B8A8
    4,   // RGBFB_B8G8R8A8
    2,   // RGBFB_R5G6B5
    2,   // RGBFB_R5G5B5
    2,   // RGBFB_B5G6R5PC
    2,   // RGBFB_B5G5R5PC
    2,   // RGBFB_YUV422CGX
    1,   // RGBFB_YUV411
    1,   // RGBFB_YUV411PC
    2,   // RGBFB_YUV422
    2,   // RGBFB_YUV422PC
    2,   // RGBFB_YUV422PA
    2    // RGBFB_YUV422PAPC
};

const UWORD PixelMode[21] = {
    SAGA_VIDEO_FORMAT_OFF,     // RGBFB_NONE
    SAGA_VIDEO_FORMAT_CLUT8,   // CLUT
    SAGA_VIDEO_FORMAT_RGB24,   // RGBFB_R8G8B8
    SAGA_VIDEO_FORMAT_RGB24,   // RGBFB_B8G8R8
    SAGA_VIDEO_FORMAT_RGB16,   // RGBFB_R5G6B5PC
    SAGA_VIDEO_FORMAT_RGB15,   // RGBFB_R5G5B5PC
    SAGA_VIDEO_FORMAT_RGB32,   // RGBFB_A8R8G8B8
    SAGA_VIDEO_FORMAT_RGB32,   // RGBFB_A8B8G8R8
    SAGA_VIDEO_FORMAT_RGB32,   // RGBFB_R8G8B8A8
    SAGA_VIDEO_FORMAT_RGB32,   // RGBFB_B8G8R8A8
    SAGA_VIDEO_FORMAT_RGB16,   // RGBFB_R5G6B5 
    SAGA_VIDEO_FORMAT_RGB15,   // RGBFB_R5G5B5 
    SAGA_VIDEO_FORMAT_RGB16,   // RGBFB_B5G6R5PC
    SAGA_VIDEO_FORMAT_RGB15,   // RGBFB_B5G5R5PC
    SAGA_VIDEO_FORMAT_YUV422,  // RGBFB_YUV422CGX
    SAGA_VIDEO_FORMAT_YUV422,  // RGBFB_YUV411
    SAGA_VIDEO_FORMAT_YUV422,  // RGBFB_YUV411PC
    SAGA_VIDEO_FORMAT_YUV422,  // RGBFB_YUV422
    SAGA_VIDEO_FORMAT_YUV422,  // RGBFB_YUV422PC
    SAGA_VIDEO_FORMAT_YUV422,  // RGBFB_YUV422PA
    SAGA_VIDEO_FORMAT_YUV422,  // RGBFB_YUV422PAPC
};

/*****************************************************************************/

UWORD SAVEDS ASM Card_CalculateBytesPerRow(
    __REGA0(struct BoardInfo *bi),
    __REGD0(UWORD             width),
    __REGD7(RGBFTYPE          rgbFormat))
{
    UWORD bytesPerRow = (UWORD)width;
    
    if (rgbFormat < RGBFB_MaxFormats)
    {
        bytesPerRow *= PixelWidth[rgbFormat];
    }
    
    return(bytesPerRow);
}

/*****************************************************************************/

APTR SAVEDS ASM Card_CalculateMemory(
    __REGA0(struct BoardInfo *bi),
    __REGA1(APTR              LogicalMemory),
    __REGD7(RGBFTYPE          rgbFormat))
{
    return(LogicalMemory);
}

/*****************************************************************************/

ULONG SAVEDS ASM Card_GetCompatibleFormats(
    __REGA0(struct BoardInfo *bi),
    __REGD7(RGBFTYPE          rgbFormat))
{
    return(-1);
}

/*****************************************************************************/

ULONG SAVEDS ASM Card_GetPixelClock(
    __REGA0(struct BoardInfo *bi),
    __REGA1(struct ModeInfo  *mi),
    __REGD0(ULONG             clockIndex),
    __REGD7(RGBFTYPE          rgbFormat))
{
    return(SAGA_PIXEL_CLOCK);
}

/*****************************************************************************/

ULONG ASM SAVEDS Card_GetVBeamPos(
    __REGA0(struct BoardInfo *bi))
{
    return(((*(volatile ULONG*)0xDFF004)>>8)&0xFFF);
}

/*****************************************************************************/

BOOL ASM SAVEDS Card_GetVSyncState(
    __REGA0(struct BoardInfo *bi),
    __REGD0(BOOL              expectedState))
{
    return(expectedState);
}

/*****************************************************************************/

void SAVEDS ASM Card_ResetChip(
    __REGA0(struct BoardInfo *bi))
{
    return;
}

/*****************************************************************************/

LONG SAVEDS ASM Card_ResolvePixelClock(
    __REGA0(struct BoardInfo *bi),
    __REGA1(struct ModeInfo  *mi),
    __REGD0(ULONG             pixelClock),
    __REGD7(RGBFTYPE          rgbFormat))
{
    return(1);
}

/*****************************************************************************/

void SAVEDS ASM Card_SetClearMask(
    __REGA0(struct BoardInfo *bi),
    __REGD0(UBYTE             mask))
{
    return;
}

/*****************************************************************************/

void SAVEDS ASM Card_SetClock(
    __REGA0(struct BoardInfo *bi))
{
    return;
}

/*****************************************************************************/

void SAVEDS ASM Card_SetColorArray(
    __REGA0(struct BoardInfo *bi),
    __REGD0(UWORD             startIndex),
    __REGD1(UWORD             count))
{
    struct CLUTEntry *ce;
    
    if ((startIndex < 256) && (bi->RGBFormat == RGBFB_CHUNKY))
    {
        if (startIndex + count > 256)
        {
            count = 256 - startIndex;
        }
        
        if (ce = &bi->CLUT[startIndex])
        {
            int i;
            
            for (i = 0; i < count; i++, ce++)
            {
                Write32(SAGA_VIDEO_CLUT, 
                    ((startIndex + i) << 24) |
                    ((ULONG)ce->Red   << 16) |
                    ((ULONG)ce->Green <<  8) |
                    ((ULONG)ce->Blue));
            }
        }
    }
    
    return;
}

/*****************************************************************************/

void SAVEDS ASM Card_SetDAC(
    __REGA0(struct BoardInfo *bi),
    __REGD7(RGBFTYPE          rgbFormat))
{
    if (rgbFormat < RGBFB_MaxFormats)
    {
        UWORD DACMode = (bi->CardData[CARDDATA_MODEID] | PixelMode[rgbFormat]);
        
        Write16(SAGA_VIDEO_FORMAT, DACMode);
        
        bi->CardData[CARDDATA_DACMODE] = DACMode;
    }

    return;
}

/*****************************************************************************/

BOOL SAVEDS ASM Card_SetDisplay(
    __REGA0(struct BoardInfo *bi),
    __REGD0(BOOL              state))
{
    if (state)
    {
        // Display ON
        
        bi->CardData[CARDDATA_FLAGS] &= ~CARDD_FLG_DISP_OFF;
        Write16(SAGA_VIDEO_FORMAT, bi->CardData[CARDDATA_DACMODE]);
    }
    else
    {
        // Display OFF
        
        bi->CardData[CARDDATA_FLAGS] |= CARDD_FLG_DISP_OFF;
        Write16(SAGA_VIDEO_FORMAT, SAGA_VIDEO_FORMAT_OFF);
    }
    
    return (state);
}

/*****************************************************************************/

void SAVEDS ASM Card_SetDPMSLevel(
    __REGA0(struct BoardInfo *bi),
    __REGD0(ULONG             DPMSLevel))
{
    UWORD DACMode;

    if (!DPMSLevel)
    {
        DACMode = bi->CardData[CARDDATA_DACMODE];
    }
    else
    {
        DACMode = SAGA_VIDEO_FORMAT_OFF;
    }
    
    Write16(SAGA_VIDEO_FORMAT, DACMode);
    
    return;
}

/*****************************************************************************/

void SAVEDS ASM Card_SetGC(
    __REGA0(struct BoardInfo *bi),
    __REGA1(struct ModeInfo  *mi),
    __REGD0(BOOL              border))
{
    struct LibResolution *res;
    
    bi->ModeInfo = mi;
    bi->Border   = border;
    bi->CardData[CARDDATA_WIDTH] = mi->Width;
    
    FOREACH(struct LibResolution *, &bi->ResolutionsList, res)
    {
        if ((res->Width == mi->Width) && (res->Height == mi->Height))
        {
            ULONG ModeID = ((res->DisplayID & 0x00FF0000) >> 8);
            bi->CardData[CARDDATA_MODEID] = ModeID;
            break;
        }
    }
    
    if (bi->SoftSpriteFlags == 0xffff)
    {
        Card_SetSprite(bi, FALSE, 0);
    }
    
    return;
}

/*****************************************************************************/

void SAVEDS ASM Card_SetPanning(
    __REGA0(struct BoardInfo *bi),
    __REGA1(UBYTE            *mem),
    __REGD0(UWORD             width),
    __REGD1(WORD              xoffset),
    __REGD2(WORD              yoffset),
    __REGD7(RGBFTYPE          rgbFormat))
{
    ULONG ptr;
    UWORD modulo, bpp = 1;

    // VIDEO OFFSETS
    
    bi->XOffset = xoffset;
    bi->YOffset = yoffset;
    
    if (rgbFormat < RGBFB_MaxFormats)
    {
        bpp = PixelWidth[rgbFormat];

        if (bpp == 3)
        {
            // 24-bit alignment.
            xoffset = ((xoffset + 11) & ~11);
        }
    }
    
    // VIDEO ADDRESS
    
    ptr = (ULONG)mem +
          (bpp * xoffset) +
          (bpp * yoffset * width);
    
    if (bpp != 3)
    {
        ptr = ((ptr + 15) & ~15);
    }
    
    Write32(SAGA_VIDEO_BPLPTR, ptr);
    
    // VIDEO MODULO
    
    modulo = (width - bi->CardData[CARDDATA_WIDTH]) * bpp;
    Write16(SAGA_VIDEO_MODULO, modulo);
    
    return;
}

/*****************************************************************************/

BOOL SAVEDS ASM Card_SetSwitch(
    __REGA0(struct BoardInfo *bi),
    __REGD0(BOOL              newState))
{
    if (newState)
    {
        UWORD DACMode = bi->CardData[CARDDATA_DACMODE];
        Write16(SAGA_VIDEO_FORMAT, DACMode);
        newState = 1 - newState;
    }
    
    return(newState);
}

/*****************************************************************************/

BOOL SAVEDS ASM Card_SetInterrupt(
    __REGA0(struct BoardInfo *bi),
    __REGD0(BOOL              state))
{
    return (FALSE);
}

/*****************************************************************************/

void SAVEDS ASM Card_SetMemoryMode(
    __REGA0(struct BoardInfo *bi),
    __REGD7(RGBFTYPE          rgbFormat))
{
    return;
}

/*****************************************************************************/

void SAVEDS ASM Card_SetReadPlane(
    __REGA0(struct BoardInfo *bi),
    __REGD0(UBYTE             plane))
{
    return;
}

/*****************************************************************************/

void SAVEDS ASM Card_SetWriteMask(
    __REGA0(struct BoardInfo *bi),
    __REGD0(UBYTE             mask))
{
    return;
}

/*****************************************************************************/

void SAVEDS ASM Card_WaitBlitter(
    __REGA0(struct BoardInfo *bi))
{
    return;
}

/*****************************************************************************/

void SAVEDS ASM Card_WaitVerticalSync(
    __REGA0(struct BoardInfo *bi),
    __REGD0(BOOL              reserved))
{
    return;
}

/*****************************************************************************/
