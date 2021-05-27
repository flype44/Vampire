/******************************************************************************
**
** SAGACard_Sprite.c
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

#ifndef SAGACARD_FUNCTIONS_H
#include "SAGACard_Sprite.h"
#endif

/*****************************************************************************/

BOOL ASM SAVEDS Card_SetSprite(
    __REGA0(struct BoardInfo *bi),
    __REGD0(BOOL              activate),
    __REGD7(RGBFTYPE          rgbFormat))
{
    if (activate)
    {
        // Enable: Put position inside the video
        
        Write16(SAGA_SPRITE_X, bi->CardData[CARDDATA_SPRITE_X]);
        Write16(SAGA_SPRITE_Y, bi->CardData[CARDDATA_SPRITE_Y]);
        
        bi->CardData[CARDDATA_FLAGS] &= ~CARDD_FLG_SPRITE_OFF;
    }
    else
    {
        // Disable: Put position outside the video
        
        Write16(SAGA_SPRITE_X, SAGA_VIDEO_MAXHV - 1);
        Write16(SAGA_SPRITE_Y, SAGA_VIDEO_MAXHV - 1);
        
        bi->CardData[CARDDATA_FLAGS] |= CARDD_FLG_SPRITE_OFF;
    }
    
    return(activate);
}

/*****************************************************************************/

void SAVEDS ASM Card_SetSpriteColor(
    __REGA0(struct BoardInfo *bi),
    __REGD0(UBYTE             colorIndex),
    __REGD1(UBYTE             r),
    __REGD2(UBYTE             g),
    __REGD3(UBYTE             b),
    __REGD7(RGBFTYPE          rgbFormat))
{
    if (colorIndex <= 2)
    {
        // Input : 0x00RRGGBB
        // Output: 0x0RGB
        
        Write16(
            SAGA_SPRITE_CLUT + (colorIndex << 1), 
            ((r & 0xf0) << 4) + // RED
            ((g & 0xf0)     ) + // GREEN
            ((b & 0xf0) >> 4)   // BLUE
        );
    }
    
    return;
}

/*****************************************************************************/

void ASM SAVEDS Card_SetSpriteImage(
    __REGA0(struct BoardInfo *bi),
    __REGD7(RGBFTYPE          rgbFormat))
{
    UBYTE i, h = bi->MouseHeight;
    ULONG dptr = SAGA_SPRITE_BPLPTR;
    ULONG *sptr;
    
    sptr = (ULONG*)bi->MouseImage;
    
    sptr++; // Skip header (4 Bytes)
    
    if (h > 16)
    {
        // AmigaOS Mouse Sprite is 16x24
        // SAGA HW Mouse Sprite is 16x16
        h = 16;
    }
    
    for (i = 0; i < h; i++)
    {
        Write32(dptr, *sptr++);
        dptr += 4;
    }
    
    for (; i < 16; i++)
    {
        Write32(dptr, 0);
        dptr += 4;
    }
    
    return;
}

/*****************************************************************************/

void ASM SAVEDS Card_SetSpritePosition(
    __REGA0(struct BoardInfo *bi),
    __REGD0(WORD              x),
    __REGD1(WORD              y),
    __REGD7(RGBFTYPE          rgbFormat))
{
    UBYTE flags = bi->CardData[CARDDATA_FLAGS];
    
    if (flags & CARDD_FLG_SPRITE_OFF)
    {
        return;
    }
    
    x = bi->MouseX - bi->XOffset + SAGA_SPRITE_DELTA_X;
    y = bi->MouseY - bi->YOffset + SAGA_SPRITE_DELTA_Y;
    
    Write16(SAGA_SPRITE_X, x);
    Write16(SAGA_SPRITE_Y, y);
    
    bi->CardData[CARDDATA_SPRITE_X] = (ULONG)x;
    bi->CardData[CARDDATA_SPRITE_Y] = (ULONG)y;
}

/*****************************************************************************/
