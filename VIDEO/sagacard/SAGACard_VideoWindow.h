/******************************************************************************
**
** SAGACard_VideoWindow.h
** Picasso96/SAGA.card
** 
******************************************************************************/

#ifndef SAGACARD_VIDEOWINDOW_H
#define SAGACARD_VIDEOWINDOW_H

/*****************************************************************************/

#ifndef boardinfo_H
#include "boardinfo.h"
#endif

/*****************************************************************************/

struct VideoWindow
{
    UWORD                Left;
    UWORD                Top;
    UWORD                Width;
    UWORD                Height;
    RGBFTYPE             Format;
    ULONG                Flags;
    ULONG                ChangeFlags;
    struct BitMap       *BitMap;
    struct ModeInfo     *ModeInfo;
    RGBFTYPE             ModeFormat;
    ULONG                Color;
    ULONG                Pen;
    UWORD                PanLeft;
    UWORD                PanTop;
    BOOL                 Occlusion;
    BOOL                 Interlace;
    BOOL                 DoubleBuffer;
    UWORD                SourceWidth;
    UWORD                SourceHeight;
    ULONG                CaptureWidth;
    ULONG                CaptureHeight;
    ULONG                Brightness;
    APTR                 P4Context;
    ULONG                MinZoom;
    ULONG                Buffer1Address;
    ULONG                Buffer2Address;
    ULONG                BufferPitch;
    ULONG                Pixels;
    ULONG                Lines;
};

/*****************************************************************************/

#define	VWB_ACTIVE       0
#define	VWB_ONBOARD      1
#define	VWB_RUNNING      2

#define	VWF_ACTIVE       (1<<VWB_ACTIVE)
#define	VWF_ONBOARD      (1<<VWB_ONBOARD)

#define	VWCB_LEFT        0
#define	VWCB_TOP         1
#define	VWCB_WIDTH       2
#define	VWCB_HEIGHT      3
#define	VWCB_SRCWIDTH    4
#define	VWCB_SRCHEIGHT   5
#define	VWCB_FORMAT      6
#define	VWCB_COLOR       7
#define	VWCB_OCCLUSION   8
#define	VWCB_INTERLACE   9
#define	VWCB_PANNING     10
#define	VWCB_BRIGHTNESS  11
#define	VWCB_RESTORE     15

#define	VWCF_LEFT        (1<<VWCB_LEFT)
#define	VWCF_TOP         (1<<VWCB_TOP)
#define	VWCF_WIDTH       (1<<VWCB_WIDTH)
#define	VWCF_HEIGHT      (1<<VWCB_HEIGHT)
#define	VWCF_SRCWIDTH    (1<<VWCB_SRCWIDTH)
#define	VWCF_SRCHEIGHT   (1<<VWCB_SRCHEIGHT)
#define	VWCF_FORMAT      (1<<VWCB_FORMAT)
#define	VWCF_COLOR       (1<<VWCB_COLOR)
#define	VWCF_OCCLUSION   (1<<VWCB_OCCLUSION)
#define	VWCF_INTERLACE   (1<<VWCB_INTERLACE)
#define	VWCF_PANNING     (1<<VWCB_PANNING)
#define	VWCF_BRIGHTNESS  (1<<VWCB_BRIGHTNESS)
#define	VWCF_RESTORE     (1<<VWCB_RESTORE)

/*****************************************************************************/

ULONG Card_SetVideoWindowAttrs(
    struct BoardInfo     *bi,
    struct VideoWindow   *vw,
    struct TagItem       *Tags);

ULONG Card_GetVideoWindowAttrs(
    struct BoardInfo     *bi,
    struct VideoWindow   *vw,
    struct TagItem       *Tags);

ULONG Card_CreateVideoWindow(
    struct BoardInfo     *bi,
    struct VideoWindow   *vw,
    struct TagItem       *Tags);

void Card_DeleteVideoWindow(
    struct BoardInfo     *bi,
    struct VideoWindow   *vw);

/*****************************************************************************/

#endif
