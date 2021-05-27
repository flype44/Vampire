/******************************************************************************
**
** SAGACard_MemoryWindow.c
** Picasso96/SAGA.card
** 
******************************************************************************/

#include <intuition/intuition.h>

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

#ifndef SAGACARD_MEMORYWINDOW_H
#include "SAGACard_MemoryWindow.h"
#endif

/*****************************************************************************/

#define	D(x) x

/*****************************************************************************/

UBYTE __VideoBytesPerPixel[21] =
{
    0, 1, 0, 0, 2, 2, 
    0, 0, 0, 0, 2, 2, 
    2, 2, 2, 1, 1, 2, 
    2, 2, 2
};

UBYTE __DisplayBytesPerPixel[21] =
{
    0, 1, 3, 3, 2, 2, 
    4, 4, 4, 4, 2, 2, 
    2, 2, 2, 1, 1, 2, 
    2, 2, 2
};

UBYTE __DisplayKeyable[21] =
{
    0, 1, 0, 0, 1, 1, 
    0, 0, 0, 0, 1, 1, 
    1, 1, 1, 1, 1, 1, 
    1, 1, 1
};

const UWORD PixelFormat[21] = {
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

void SAGA_VIDEO_PIP_OFF(void)
{
    D(KPrintf("XSTRT:  %ld\n", 0));
    D(KPrintf("YSTRT:  %ld\n", 0));
    D(KPrintf("XSTOP:  %ld\n", 0));
    D(KPrintf("YSTOP:  %ld\n", 0));
    
    Write16(SAGA_PIP_FORMAT,   SAGA_VIDEO_FORMAT_OFF);
    Write16(SAGA_PIP_COLORKEY, 0x0000);
    Write16(SAGA_PIP_BPLPTR,   0);
    Write16(SAGA_PIP_MODULO,   0);
    Write16(SAGA_PIP_XSTRT,    0);
    Write16(SAGA_PIP_XSTOP,    0);
    Write16(SAGA_PIP_YSTRT,    0);
    Write16(SAGA_PIP_YSTOP,    0);
}

/*****************************************************************************/

void SAGA_VIDEO_PIP_UPDATE(ULONG fb, struct Rectangle *r)
{
    D(KPrintf("Buffer: 0x%08lx\n", fb));
    
    D(KPrintf("XSTRT:  %ld\n",     r->MinX));
    D(KPrintf("YSTRT:  %ld\n",     r->MinY));
    D(KPrintf("XSTOP:  %ld\n",     r->MaxX));
    D(KPrintf("YSTOP:  %ld\n",     r->MaxY));
    
    Write32(SAGA_PIP_BPLPTR,    fb);
    Write16(SAGA_PIP_MODULO,    0);
    Write16(SAGA_PIP_DMAROWLEN, 0);
    Write16(SAGA_PIP_COLORKEY,  0x0000);
    Write16(SAGA_PIP_XSTRT,     r->MinX + SAGA_PIP_DELTA_X + 1);
    Write16(SAGA_PIP_XSTOP,     r->MaxX + SAGA_PIP_DELTA_X + 1 + 1);
    Write16(SAGA_PIP_YSTRT,     r->MinY + SAGA_PIP_DELTA_Y);
    Write16(SAGA_PIP_YSTOP,     r->MaxY + SAGA_PIP_DELTA_Y);
}

/*****************************************************************************/

void PrintMemoryWindowTags(struct TagItem *tags)
{
    struct TagItem *tag;
    struct TagItem *tstate = tags;
    
    while (tag = NextTagItem(&tstate))
    {
        ULONG value = tag->ti_Data;
        
        if (tag->ti_Tag == FA_Restore)              D(KPrintf("FA_Restore:              %ld\n", value));
        if (tag->ti_Tag == FA_Onboard)              D(KPrintf("FA_Onboard:              %ld\n", value));
        if (tag->ti_Tag == FA_Active)               D(KPrintf("FA_Active:               %ld\n", value));
        if (tag->ti_Tag == FA_Left)                 D(KPrintf("FA_Left:                 %ld\n", value));
        if (tag->ti_Tag == FA_Top)                  D(KPrintf("FA_Top:                  %ld\n", value));
        if (tag->ti_Tag == FA_Width)                D(KPrintf("FA_Width:                %ld\n", value));
        if (tag->ti_Tag == FA_Height)               D(KPrintf("FA_Height:               %ld\n", value));
        if (tag->ti_Tag == FA_Color)                D(KPrintf("FA_Color:            0x%08lx\n", value));
        if (tag->ti_Tag == FA_Occlusion)            D(KPrintf("FA_Occlusion:            %ld\n", value));
        if (tag->ti_Tag == FA_MinWidth)             D(KPrintf("FA_MinWidth:             %ld\n", value));
        if (tag->ti_Tag == FA_MinHeight)            D(KPrintf("FA_MinHeight:            %ld\n", value));
        if (tag->ti_Tag == FA_MaxWidth)             D(KPrintf("FA_MaxWidth:             %ld\n", value));
        if (tag->ti_Tag == FA_MaxHeight)            D(KPrintf("FA_MaxHeight:            %ld\n", value));
        if (tag->ti_Tag == FA_Interlace)            D(KPrintf("FA_Interlace:            %ld\n", value));
        if (tag->ti_Tag == FA_PAL)                  D(KPrintf("FA_PAL:                  %ld\n", value));
        if (tag->ti_Tag == FA_BitMap)               D(KPrintf("FA_BitMap:           0x%08lx\n", value));
        if (tag->ti_Tag == FA_Brightness)           D(KPrintf("FA_Brightness:           %ld\n", value));
        if (tag->ti_Tag == FA_Colors)               D(KPrintf("FA_Colors:           0x%08lx\n", value));
        if (tag->ti_Tag == FA_Colors32)             D(KPrintf("FA_Colors32:         0x%08lx\n", value));
        if (tag->ti_Tag == FA_NoMemory)             D(KPrintf("FA_NoMemory:             %ld\n", value));
        if (tag->ti_Tag == FA_RenderFunc)           D(KPrintf("FA_RenderFunc:           %ld\n", value));
        if (tag->ti_Tag == FA_SaveFunc)             D(KPrintf("FA_SaveFunc:             %ld\n", value));
        if (tag->ti_Tag == FA_UserData)             D(KPrintf("FA_UserData:             %ld\n", value));
        if (tag->ti_Tag == FA_ConstantBytesPerRow)  D(KPrintf("FA_ConstantBytesPerRow:  %ld\n", value));
        if (tag->ti_Tag == FA_DoubleBuffer)         D(KPrintf("FA_DoubleBuffer:         %ld\n", value));
        if (tag->ti_Tag == FA_Pen)                  D(KPrintf("FA_Pen:                  %ld\n", value));
        if (tag->ti_Tag == FA_ClipLeft)             D(KPrintf("FA_ClipLeft:             %ld\n", value));
        if (tag->ti_Tag == FA_ClipTop)              D(KPrintf("FA_ClipTop:              %ld\n", value));
        if (tag->ti_Tag == FA_ClipWidth)            D(KPrintf("FA_ClipWidth:            %ld\n", value));
        if (tag->ti_Tag == FA_ClipHeight)           D(KPrintf("FA_ClipHeight:           %ld\n", value));
        if (tag->ti_Tag == FA_ConstantByteSwapping) D(KPrintf("FA_ConstantByteSwapping: %ld\n", value));
    }
}

/*****************************************************************************/

ULONG Card_CreateMemoryWindow(
    struct BoardInfo    *bi,
    struct MemoryWindow *mw,
    struct TagItem      *tags)
{
    ULONG  result;
    struct TagItem *tag;
    struct TagItem *tstate = tags;
    ULONG  ModeMemorySize = 0;
    ULONG  Alignment = 0;
    
    D(KPrintf("Card_CreateMemoryWindow(mw=0x%08lx)\n", mw));
    
    if (!(bi->Flags & BIF_VIDEOWINDOW))
    {
        return(1);
    }
    
    PrintMemoryWindowTags(tags);
    
    while (tag = NextTagItem(&tstate))
    {
        ULONG value = tag->ti_Data;
        
        switch (tag->ti_Tag)
        {
        case FA_SourceWidth:
            mw->SourceWidth = value;
            break;
        case FA_SourceHeight:
            mw->SourceHeight = value;
            break;
        case FA_Format:
            mw->Format = value;
            break;
        case FA_ModeInfo:
            mw->ModeInfo = (struct ModeInfo *)value;
            break;
        case FA_ModeFormat:
            mw->ModeFormat = value;
            break;
        case FA_ModeMemorySize:
            ModeMemorySize = value;
            break;
        case FA_Alignment:
            Alignment = value;
            break;
        }
    }
    
    D(KPrintf("Alignment: %ld\n", Alignment));
    D(KPrintf("ModeMemorySize: %ld\n", ModeMemorySize));
    
    if (mw->SourceWidth > 4088)
        return(1);
    
    if (( mw->ModeInfo->Depth <  8) || 
        ((mw->ModeInfo->Depth > 16) && 
        (bi->SoftSpriteFlags & (1 << mw->ModeFormat))))
        return(1);
    
    if ((mw->ModeInfo->Depth == 8) && 
        (mw->Format == RGBFB_CLUT))
        return(1);
    
    if (!((1 << mw->Format) & 
        bi->GetCompatibleFormats(bi, mw->ModeFormat)))
        return(1);
    
    if (!((1 << mw->Format) & (
        RGBFF_CLUT   | 
        RGBFF_R5G5B5 | 
        RGBFF_R5G6B5 | 
        RGBFF_YUV422)))
        return(1);
    
    /* Allocate FlickerFixer BitMap */
    
    if (mw->BitMap = (struct BitMap *)Card_AllocBitMapTags(bi, 
        mw->SourceWidth,
        mw->SourceHeight,
        ABMA_RGBFormat,           mw->Format,
        ABMA_Clear,               TRUE,
        ABMA_Displayable,         TRUE,
        ABMA_Visible,             TRUE,
        ABMA_Colors,              GetTagData(FA_Colors,              NULL,  tags),
        ABMA_Colors32,            GetTagData(FA_Colors32,            NULL,  tags),
        ABMA_ConstantBytesPerRow, GetTagData(FA_ConstantBytesPerRow, FALSE, tags),
        ABMA_NoMemory,            GetTagData(FA_NoMemory,            FALSE, tags),
        ABMA_RenderFunc,          GetTagData(FA_RenderFunc,          NULL,  tags),
        ABMA_SaveFunc,            GetTagData(FA_SaveFunc,            NULL,  tags),
        ABMA_UserData,            GetTagData(FA_UserData,            NULL,  tags),
        ABMA_Alignment,           GetTagData(FA_Alignment,           32,    tags),
        TAG_DONE))
    {
        /* Modify hardware */
        
        mw->MinHeight = mw->SourceHeight;
        
        result = Card_SetMemoryWindowAttrs(bi, mw, tags);
    }
    else
    {
        result = 1;
    }
    
    return(result);
}

/*****************************************************************************/

void Card_DeleteMemoryWindow(
    struct BoardInfo    *bi,
    struct MemoryWindow *mw)
{
    D(KPrintf("Card_DeleteMemoryWindow(mw=0x%08lx)\n", mw));
    
    Card_FreeBitMapTags(bi, mw->BitMap, TAG_DONE);
}

/*****************************************************************************/

ULONG Card_SetMemoryWindowAttrs(
    struct BoardInfo    *bi,
    struct MemoryWindow *mw,
    struct TagItem      *tags)
{
    ULONG result = 0;
    struct TagItem *tag;
    struct TagItem *tstate = tags;
    ULONG oldFlags = mw->Flags;
    
    mw->ChangeFlags = 0;
    
    D(KPrintf("Card_SetMemoryWindowAttrs(mw=0x%08lx)\n", mw));
    
    while (tag = NextTagItem(&tstate))
    {
        ULONG value = tag->ti_Data;
        
        switch(tag->ti_Tag)
        {
        case FA_Active:
            if (value)
                mw->Flags |= MWF_ACTIVE;
            else
                mw->Flags &= ~MWF_ACTIVE;
            break;
        case FA_Onboard:
            if (value)
                mw->Flags |= MWF_ONBOARD;
            else
                mw->Flags &= ~MWF_ONBOARD;
            break;
        case FA_Left:
            if (mw->Left != value)
                mw->ChangeFlags |= MWCF_POS;
            mw->Left = value;
            break;
        case FA_Top:
            if (mw->Top != value)
                mw->ChangeFlags |= MWCF_POS;
            mw->Top = value;
            break;
        case FA_Width:
            if (mw->Width != value)
                mw->ChangeFlags |= MWCF_SIZE;
            mw->Width = value;
            break;
        case FA_Height:
            if (mw->Height != value)
                mw->ChangeFlags |= MWCF_SIZE;
            mw->Height = value;
            break;
        case FA_Pen:
            if (mw->Pen != value)
                mw->ChangeFlags |= MWCF_COLOR;
            mw->Pen = value;
            break;
        case FA_Color:
            if (mw->Color != value)
                mw->ChangeFlags |= MWCF_COLOR;
            mw->Color = value;
            break;
        case FA_Occlusion:
            if (mw->Occlusion != value)
                mw->ChangeFlags |= MWCF_OCCLUSION;
            mw->Occlusion = value;
            break;
        case FA_SourceWidth:
            if (mw->SourceWidth != value)
                mw->ChangeFlags |= MWCF_SRCSIZE;
            mw->SourceWidth = value;
            break;
        case FA_SourceHeight:
            if (mw->SourceHeight != value)
                mw->ChangeFlags |= MWCF_SRCSIZE;
            mw->SourceHeight = value;
            break;
        case FA_Format:
            if (mw->Format != value)
                mw->ChangeFlags |= MWCF_FORMAT;
            mw->Format = value;
            break;
        case FA_Brightness:
            if (mw->Brightness != value)
                mw->ChangeFlags |= MWCF_BRIGHTNESS;
            mw->Brightness = value;
            break;
        case FA_Colors:
            if (value)
            {
                struct ColorSpec *cs = (struct ColorSpec *)value;
                
                while (cs->ColorIndex != -1)
                {
                    mw->CLUT[cs->ColorIndex].Red   = (((cs->Red   & 0xf) << 4) | (cs->Red   & 0xf));
                    mw->CLUT[cs->ColorIndex].Green = (((cs->Green & 0xf) << 4) | (cs->Green & 0xf));
                    mw->CLUT[cs->ColorIndex].Blue  = (((cs->Blue  & 0xf) << 4) | (cs->Blue  & 0xf));
                    cs++;
                }
                
                mw->ChangeFlags |= MWCF_COLORS;
            }
            break;
        case FA_Colors32:
            if (value)
            {
                ULONG *ct = (ULONG *)value;
                UWORD count, offset, i;
                do {
                    count  = ((UWORD *)ct)[0];
                    offset = ((UWORD *)ct)[1];
                    ct++;
                    for (i=0; i < count; i++)
                    {
                        mw->CLUT[offset + i].Red   = ((*ct++ & 0xff000000) >> 24);
                        mw->CLUT[offset + i].Green = ((*ct++ & 0xff000000) >> 24);
                        mw->CLUT[offset + i].Blue  = ((*ct++ & 0xff000000) >> 24);
                    }
                }
                while (count > 0);
                mw->ChangeFlags |= MWCF_COLORS;
            }
            break;
        case FA_ClipLeft:
            if (mw->ClipLeft != value)
                mw->ChangeFlags |= MWCF_CLIPPOS;
            mw->ClipLeft = value;
            break;
        case FA_ClipTop:
            if (mw->ClipTop != value)
                mw->ChangeFlags |= MWCF_CLIPPOS;
            mw->ClipTop = value;
            break;
        case FA_ClipWidth:
            if (mw->ClipWidth != value)
                mw->ChangeFlags |= MWCF_CLIPSIZE;
            mw->ClipWidth = value;
            break;
        case FA_ClipHeight:
            if (mw->ClipHeight != value)
                mw->ChangeFlags |= MWCF_CLIPSIZE;
            mw->ClipHeight = value;
            break;
        }
    }
    
    /* maybe pan position of screen changed */
    
    if ((mw->PanLeft != bi->XOffset) || (mw->PanTop != bi->YOffset))
    {
        mw->PanLeft	= bi->XOffset;
        mw->PanTop	= bi->YOffset;
        mw->ChangeFlags |= MWCF_PANNING;
    }
    
    if (mw->ChangeFlags & (MWCF_SRCSIZE | MWCF_FORMAT))
    {
        if ((oldFlags & (MWF_ACTIVE | MWF_ONBOARD)) == (MWF_ACTIVE | MWF_ONBOARD))
        {
            D(KPrintf("Turn memory window off if it is running\n"));
            
            Write16(SAGA_PIP_FORMAT, 0);
        }
        
        Card_FreeBitMapTags(bi, mw->BitMap, TAG_DONE);
        
        mw->BitMap = (struct BitMap *)Card_AllocBitMapTags(bi, 
            mw->SourceWidth, 
            mw->SourceHeight,
            ABMA_RGBFormat,   mw->Format,
            ABMA_Clear,       TRUE,
            ABMA_Displayable, TRUE,
            ABMA_Visible,     TRUE,
            TAG_DONE);
        
        mw->ChangeFlags |= MWCF_RESTORE;
    }
    
    if ((mw->Flags & (MWF_ACTIVE | MWF_ONBOARD)) == (MWF_ACTIVE | MWF_ONBOARD))
    {
        D(KPrintf("MemoryWindow on board and active\n"));
        
        if ((oldFlags & (MWF_ACTIVE | MWF_ONBOARD)) != (MWF_ACTIVE | MWF_ONBOARD))
        {
            mw->ChangeFlags |= MWCF_RESTORE;
        }
        
        if (mw->ChangeFlags & MWCF_RESTORE)
        {
            D(KPrintf("Starting MemoryWindow\n"));
        }
        
        if (mw->ChangeFlags && mw->BitMap)
        {
            ULONG attrFlags = 0;
            
            attrFlags |= ((bi->SoftSpriteFlags & (1 << bi->RGBFormat)) || mw->Occlusion) ? AFF_Occlude : 0;
            
            if ((mw->ChangeFlags & (MWCF_COLORS | MWCF_RESTORE)) && (mw->Format == RGBFB_CLUT) && (mw->ModeFormat != RGBFB_CLUT))
            {
                int i;
                
                for (i = 0; i < 256; i++)
                {
                    bi->CLUT[i].Red   = mw->CLUT[i].Red;
                    bi->CLUT[i].Green = mw->CLUT[i].Green;
                    bi->CLUT[i].Blue  = mw->CLUT[i].Blue;
                }
                
                bi->SetColorArray(bi, 0, 256);
            }
            
            if (mw->ChangeFlags & (MWCF_BRIGHTNESS | MWCF_RESTORE))
            {
                /* No Brightness support */
            }
            
            if (mw->ChangeFlags & (MWCF_OCCLUSION | MWCF_COLOR | MWCF_RESTORE))
            {
                if (attrFlags & AFF_Occlude)
                {
                    if (bi->RGBFormat == RGBFB_CLUT)
                    {
                        D(KPrintf("COLORKEY(CLUT): 0x%08lx\n", (ULONG)mw->Pen));
                        /*
                        Write16(SAGA_PIP_COLORKEY, 
                            mw->Pen | 0x8000);
                        */
                    }
                    else
                    {
                        D(KPrintf("COLORKEY(RGB): 0x%08lx\n", (ULONG)mw->Color));
                        /*
                        Write16(SAGA_PIP_COLORKEY, 
                            (mw->Color >> (((1L << bi->RGBFormat) & (RGBFF_R5G6B5PC | RGBFF_R5G5B5PC)) ? 8 : 0)) | 0x8000);
                        */
                    }
                }
                
                D(KPrintf("FORMAT: 0x%08lx => 0x%08lx\n", 
                    (ULONG)mw->Format, PixelFormat[mw->Format]));
                
                Write16(SAGA_PIP_FORMAT, PixelFormat[mw->Format]);
            }
            
            if (mw->ChangeFlags & (MWCF_OCCLUSION | 
                                   MWCF_POS       | 
                                   MWCF_SIZE      | 
                                   MWCF_CLIPPOS   | 
                                   MWCF_CLIPSIZE  | 
                                   MWCF_PANNING   | 
                                   MWCF_RESTORE))
            {
                struct Rectangle Box;
                struct ModeInfo *mi = mw->ModeInfo;
                
                ULONG SourceWidth   = mw->ClipWidth;
                ULONG SourceHeight  = mw->ClipHeight;
                
                ULONG xMask = ~((1 << (4 - __DisplayBytesPerPixel[mw->ModeFormat])) - 1);
                
                ULONG adr = bi->GetBitMapAttr(bi, mw->BitMap, GBMA_MEMORY);
                
                if (SourceWidth == 0)
                    SourceWidth = mw->SourceWidth;
                
                if (SourceHeight == 0)
                    SourceHeight = mw->SourceHeight;
                
                if (mw->Height < mw->MinHeight)
                    SourceHeight = mw->MinHeight;
                
                Box.MinX = (mw->Left - (bi->XOffset & xMask));
                Box.MaxX = (mw->Left + (mw->Width - 1 - (bi->XOffset & xMask)));
                
                if (Box.MinX < 0)
                    Box.MinX = 0;
                
                if (Box.MaxX >= mi->Width)
                    Box.MaxX = (mi->Width - 1);
                
                Box.MinY = (mw->Top - bi->YOffset);
                Box.MaxY = (Box.MinY + mw->Height - 1);
                
                if (Box.MinY < 0)
                    Box.MinY = 0;
                
                if (Box.MaxY >= mi->Height)
                    Box.MaxY = (mi->Height - 1);
                
                if ((Box.MinX <= Box.MaxX) && 
                    (Box.MinY <= Box.MaxY) && 
                    (!(attrFlags & AFF_Occlude) || __DisplayKeyable[bi->RGBFormat]))
                {
                    D(KPrintf("src: %ldx%ld, dst: %ldx%ld\n", 
                        SourceWidth, SourceHeight, 
                        mw->Width, mw->Height));
                    
                    SAGA_VIDEO_PIP_UPDATE(adr, &Box);
                }
                else
                {
                    Write16(SAGA_PIP_FORMAT, 0);
                }
                
                D(KPrintf("MemoryWindow running\n"));
            }
        }
        else
        {
            D(KPrintf("MemoryWindow did not change\n"));
        }
    }
    else
    {
        D(KPrintf("MemoryWindow off board or inactive\n"));
        
        if((oldFlags & (MWF_ACTIVE | MWF_ONBOARD)) == (MWF_ACTIVE | MWF_ONBOARD))
        {
            D(KPrintf("MemoryWindow was on board and active\n"));
            
            SAGA_VIDEO_PIP_OFF();
            
            D(KPrintf("Stopping MemoryWindow\n"));
        }
    }
    
    D(KPrintf("MemoryWindow set %ld\n", result));
    
    return(result);
}

/*****************************************************************************/

ULONG Card_GetMemoryWindowAttrs(
    struct BoardInfo    *bi,
    struct MemoryWindow *mw,
    struct TagItem      *tags)
{
    ULONG result = 0;
    struct TagItem *tag;
    struct TagItem *tstate = tags;
    
    D(KPrintf("Card_GetMemoryWindowAttrs()\n"));
    
    while (tag = NextTagItem(&tstate))
    {
        ULONG *ptr = ((ULONG *)tag->ti_Data);
        
        switch (tag->ti_Tag)
        {
        case FA_MinWidth:
            *ptr = mw->SourceWidth;
            break;
        case FA_MinHeight:
            *ptr = mw->SourceHeight;
            break;
        case FA_MaxWidth:
            *ptr = mw->SourceWidth;
            break;
        case FA_MaxHeight:
            *ptr = mw->SourceHeight;
            break;
        case FA_Format:
            *ptr = mw->Format;
            break;
        case FA_SourceWidth:
            *ptr = mw->SourceWidth;
            break;
        case FA_SourceHeight:
            *ptr = mw->SourceHeight;
            break;
        case FA_BitMap:
            *ptr = (ULONG)mw->BitMap;
            break;
        case FA_Brightness:
            *ptr = (ULONG)mw->Brightness;
            break;
        case FA_ClipLeft:
            *ptr = mw->ClipLeft;
            break;
        case FA_ClipTop:
            *ptr = mw->ClipTop;
            break;
        case FA_ClipWidth:
            *ptr = mw->ClipWidth;
            break;
        case FA_ClipHeight:
            *ptr = mw->ClipHeight;
            break;
        }
    }
    
    return(result);
}

/*****************************************************************************/
