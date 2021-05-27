/******************************************************************************
**
** SAGACard_VideoCapture.h
** Picasso96/SAGA.card
** 
******************************************************************************/

#ifndef SAGACARD_VIDEOCAPTURE_H
#define SAGACARD_VIDEOCAPTURE_H

/*****************************************************************************/

#ifndef boardinfo_H
#include "boardinfo.h"
#endif

/*****************************************************************************/

struct VideoCapture
{
    UWORD                   BytesPerRow;
    UWORD                   Format;
    ULONG                   Flags;
    ULONG                   ChangeFlags;
    struct BitMap          *BitMap;
    APTR                    Memory;
    APTR                    DBufferMemory;
    BOOL                    Interlace;
    UWORD                   SourceWidth;
    UWORD                   SourceHeight;
};

/*****************************************************************************/

#define	VCB_ACTIVE          0
#define	VCB_ONBOARD         1
#define	VCB_RUNNING         2

#define	VCF_ACTIVE          (1<<VCB_ACTIVE)
#define	VCF_ONBOARD         (1<<VCB_ONBOARD)

#define	VCCB_SRCWIDTH       4
#define	VCCB_SRCHEIGHT      5
#define	VCCB_FORMAT         6
#define	VCCB_INTERLACE      9
#define	VCCB_RESTORE        15

#define	VCCF_SRCWIDTH       (1<<VCCB_SRCWIDTH)
#define	VCCF_SRCHEIGHT      (1<<VCCB_SRCHEIGHT)
#define	VCCF_FORMAT         (1<<VCCB_FORMAT)
#define	VCCF_INTERLACE      (1<<VCCB_INTERLACE)
#define	VCCF_RESTORE        (1<<VCCB_RESTORE)

/*****************************************************************************/

ULONG Card_SetVideoCaptureAttrs(
    struct BoardInfo        *bi,
    struct VideoCapture     *vc,
    struct TagItem          *tags);

ULONG Card_GetVideoCaptureAttrs(
    struct BoardInfo        *bi,
    struct VideoCapture     *vc,
    struct TagItem          *tags);

ULONG Card_CreateVideoCapture(
    struct BoardInfo        *bi,
    struct VideoCapture     *vc,
    struct TagItem          *tags);

void Card_DeleteVideoCapture(
    struct BoardInfo        *bi,
    struct VideoCapture     *vc);

/*****************************************************************************/
