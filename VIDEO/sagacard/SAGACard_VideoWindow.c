/******************************************************************************
**
** SAGACard_VideoWindow.c
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

#ifndef SAGACARD_VIDEOWINDOW_H
#include "SAGACard_VideoWindow.h"
#endif

/*****************************************************************************/

#define	D(x) x

/*****************************************************************************/

ULONG Card_CreateVideoWindow(
    struct BoardInfo   *bi, 
    struct VideoWindow *vw, 
    struct TagItem     *tags)
{
    ULONG result = PIPErr_NotAvailable;
    
    D(KPrintf("Card_CreateVideoWindow()\n"));
    
    if (bi->Flags & BIF_VIDEOWINDOW)
    {
        struct TagItem *tag;
        struct TagItem *tstate = tags;
        
        /*
        ULONG Alignment = 0;
        ULONG ModeMemorySize = 0;
        */
        
        while (tag = NextTagItem(&tstate))
        {
            ULONG value = tag->ti_Data;
            
            switch (tag->ti_Tag)
            {
            case FA_SourceWidth:
                vw->SourceWidth = value;
                break;
            case FA_SourceHeight:
                vw->SourceHeight = value;
                break;
            case FA_Format:
                vw->Format = value;
                break;
            case FA_Interlace:
                vw->Interlace = value;
                break;
            case FA_DoubleBuffer:
                vw->DoubleBuffer = value;
                break;
            case FA_ModeInfo:
                vw->ModeInfo = (struct ModeInfo *)value;
                break;
            case FA_ModeFormat:
                vw->ModeFormat = value;
                break;
            case FA_ModeMemorySize:
              //ModeMemorySize = value;
                break;
            case FA_Alignment:
              //Alignment = value;
                break;
            }
        }
        
        if (vw->SourceWidth > 4088)
            return(PIPErr_SourceWidthTooBig);
        
        if (vw->ModeInfo->Depth < 8)
            return(PIPErr_WrongScreenMode);
        
        if (!((1<<vw->Format) & bi->GetCompatibleFormats(bi, vw->ModeFormat)))
            return(PIPErr_WrongScreenMode);
        
        if (!((1<<vw->Format) & (RGBFF_CLUT|RGBFF_R5G6B5|RGBFF_R5G5B5)))
            return(PIPErr_UnsupportedSourceFormat);
        
        if (vw->BitMap = Card_AllocBitMapTags(bi, 
            vw->SourceWidth + 16, 
            vw->SourceHeight * (vw->DoubleBuffer ? 2 : 1) + 4,
            ABMA_RGBFormat,           vw->Format,
            ABMA_Clear,               FALSE,
            ABMA_Displayable,         TRUE,
            ABMA_Visible,             TRUE,
            ABMA_ConstantBytesPerRow, GetTagData(FA_ConstantBytesPerRow, FALSE, tags),
            ABMA_NoMemory,            GetTagData(FA_NoMemory,            FALSE, tags),
            ABMA_RenderFunc,          GetTagData(FA_RenderFunc,          NULL,  tags),
            ABMA_SaveFunc,            GetTagData(FA_SaveFunc,            NULL,  tags),
            ABMA_UserData,            GetTagData(FA_UserData,            NULL,  tags),
            ABMA_Alignment,           GetTagData(FA_Alignment,           32,    tags),
            TAG_DONE))
        {
            D(KPrintf("Card_AllocBitMapTags() : OK\n"));
            
            Card_SetVideoWindowAttrs(bi, vw, tags);
            
            result = PIPErr_OK;
        }
        else
        {
            D(KPrintf("Card_AllocBitMapTags() : ERROR\n"));
            
            result = PIPErr_OutOfVideoMemory;
        }
    }
    
    return(result);
}

/*****************************************************************************/

void Card_DeleteVideoWindow(
    struct BoardInfo   *bi,
    struct VideoWindow *vw)
{
    D(KPrintf("Card_DeleteVideoWindow()\n"));
    
    Card_FreeBitMapTags(bi, vw->BitMap, TAG_DONE);
}

/*****************************************************************************/

ULONG Card_SetVideoWindowAttrs(
    struct BoardInfo   *bi,
    struct VideoWindow *vw,
    struct TagItem     *tags)
{
    ULONG result = 0;
    
    struct TagItem *tag;
    struct TagItem *tstate = tags;
    
    D(KPrintf("Card_SetVideoWindowAttrs()\n"));
    
    while (tag = NextTagItem(&tstate))
    {
        ULONG value = tag->ti_Data;
        
        switch (tag->ti_Tag)
        {
        case FA_Active:
            if (value)
                vw->Flags |= VWF_ACTIVE;
            else
                vw->Flags &= ~VWF_ACTIVE;
            break;
        case FA_Onboard:
            if (value)
                vw->Flags |= VWF_ONBOARD;
            else
                vw->Flags &= ~VWF_ONBOARD;
            break;
        case FA_Left:
            if (vw->Left != value)
                vw->ChangeFlags |= VWCF_LEFT;
            vw->Left = value;
            break;
        case FA_Top:
            if (vw->Top != value)
                vw->ChangeFlags |= VWCF_TOP;
            vw->Top = value;
            break;
        case FA_Width:
            if (vw->Width != value)
                vw->ChangeFlags |= VWCF_WIDTH;
            vw->Width = value;
            break;
        case FA_Height:
            if (vw->Height != value)
                vw->ChangeFlags |= VWCF_HEIGHT;
            vw->Height = value;
            break;
        case FA_Pen:
            if (vw->Pen != value)
                vw->ChangeFlags |= VWCF_COLOR;
            vw->Pen = value;
            break;
        case FA_Color:
            if (vw->Color != value)
                vw->ChangeFlags |= VWCF_COLOR;
            vw->Color = value;
            break;
        case FA_Occlusion:
            if (vw->Occlusion != value)
                vw->ChangeFlags |= VWCF_OCCLUSION;
            vw->Occlusion = value;
            break;
        case FA_Brightness:
            if (vw->Brightness != value)
                vw->ChangeFlags |= VWCF_BRIGHTNESS;
            vw->Brightness = value;
            break;
        }
        
        if ((vw->PanLeft != bi->XOffset) || (vw->PanTop != bi->YOffset))
        {
            vw->PanLeft = bi->XOffset;
            vw->PanTop = bi->YOffset;
            vw->ChangeFlags |= VWCF_PANNING;
        }
        
        if ((vw->Flags & (VWF_ACTIVE|VWF_ONBOARD)) == (VWF_ACTIVE|VWF_ONBOARD))
        {
            D(KPrintf("VideoWindow on board and active\n"));
        }
        else
        {
            D(KPrintf("VideoWindow off board or inactive\n"));
        }
    }
    
    return(result);
}

/*****************************************************************************/

ULONG Card_GetVideoWindowAttrs(
    struct BoardInfo   *bi,
    struct VideoWindow *vw,
    struct TagItem     *tags)
{
    ULONG result = 1;
    
    struct TagItem *tag;
    struct TagItem *tstate = tags;
    
    D(KPrintf("Card_GetVideoWindowAttrs()\n"));
    
    while (tag = NextTagItem(&tstate))
    {
        ULONG *ptr = ((ULONG *)tag->ti_Data);
        
        switch (tag->ti_Tag)
        {
        case FA_MinWidth:
            *ptr = vw->SourceWidth / 32;
            break;
        case FA_MinHeight:
            *ptr = vw->SourceHeight / 32;
            break;
        case FA_MaxWidth:
            *ptr = vw->SourceWidth * MAX_ZOOM;
            break;
        case FA_MaxHeight:
            *ptr = vw->SourceHeight * MAX_ZOOM;
            break;
        case FA_SourceWidth:
            *ptr = vw->SourceWidth;
            break;
        case FA_SourceHeight:
            *ptr = vw->SourceHeight;
            break;
        case FA_BitMap:
            *ptr = (ULONG)vw->BitMap;
            break;
        case FA_Brightness:
            *ptr = (ULONG)vw->Brightness;
            break;
        }
    }
    
    return(result);
}

/*****************************************************************************/
