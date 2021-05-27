/******************************************************************************
**
** SAGACard_MemoryWindow.h
** Picasso96/SAGA.card
** 
******************************************************************************/

#ifndef SAGACARD_MEMORYWINDOW_H
#define SAGACARD_MEMORYWINDOW_H

/*****************************************************************************/

#ifndef boardinfo_H
#include "boardinfo.h"
#endif

/*****************************************************************************/

struct MemoryWindow
{
    UWORD                Left;
    UWORD                Top;
    UWORD                Width;
    UWORD                Height;
    RGBFTYPE             Format;
    UWORD                PanLeft;
    UWORD                PanTop;
    ULONG                Flags;
    ULONG                ChangeFlags;
    struct BitMap       *BitMap;
    struct ModeInfo     *ModeInfo;
    RGBFTYPE             ModeFormat;
    ULONG                Color;
    ULONG                Pen;
    BOOL                 Occlusion;
    BOOL                 Interlace;
    UWORD                SourceWidth;
    UWORD                SourceHeight;
    UWORD                ClipLeft;
    UWORD                ClipTop;
    UWORD                ClipWidth;
    UWORD                ClipHeight;
    ULONG                Brightness;
    struct CLUTEntry     CLUT[256];
    ULONG                MinZoom;
    ULONG                MinHeight;
};

/*****************************************************************************/

#define	MWB_ACTIVE       0
#define	MWB_ONBOARD      1
#define	MWB_RUNNING      2

#define	MWF_ACTIVE       (1<<MWB_ACTIVE)
#define	MWF_ONBOARD      (1<<MWB_ONBOARD)

#define	MWCB_POS         0
#define	MWCB_SIZE        1
#define	MWCB_SRCSIZE     4
#define	MWCB_FORMAT      6
#define	MWCB_COLOR       7
#define	MWCB_OCCLUSION   8
#define	MWCB_INTERLACE   9
#define	MWCB_PANNING     10
#define	MWCB_BRIGHTNESS  11
#define	MWCB_COLORS      12
#define	MWCB_CLIPPOS     13
#define	MWCB_CLIPSIZE    14
#define	MWCB_RESTORE     15

#define	MWCF_POS         (1<<MWCB_POS)
#define	MWCF_SIZE        (1<<MWCB_SIZE)
#define	MWCF_SRCSIZE     (1<<MWCB_SRCSIZE)
#define	MWCF_FORMAT      (1<<MWCB_FORMAT)
#define	MWCF_COLOR       (1<<MWCB_COLOR)
#define	MWCF_OCCLUSION   (1<<MWCB_OCCLUSION)
#define	MWCF_INTERLACE   (1<<MWCB_INTERLACE)
#define	MWCF_PANNING     (1<<MWCB_PANNING)
#define	MWCF_BRIGHTNESS  (1<<MWCB_BRIGHTNESS)
#define	MWCF_COLORS      (1<<MWCB_COLORS)
#define	MWCF_CLIPPOS     (1<<MWCB_CLIPPOS)
#define	MWCF_CLIPSIZE    (1<<MWCB_CLIPSIZE)
#define	MWCF_RESTORE     (1<<MWCB_RESTORE)

/*****************************************************************************/

ULONG Card_SetMemoryWindowAttrs(
    struct BoardInfo     *bi,
    struct MemoryWindow  *mw,
    struct TagItem       *Tags);

ULONG Card_GetMemoryWindowAttrs(
    struct BoardInfo     *bi,
    struct MemoryWindow  *mw,
    struct TagItem       *Tags);

ULONG Card_CreateMemoryWindow(
    struct BoardInfo     *bi,
    struct MemoryWindow  *mw,
    struct TagItem       *Tags);

void Card_DeleteMemoryWindow(
    struct BoardInfo     *bi,
    struct MemoryWindow  *mw);

/*****************************************************************************/

#endif
