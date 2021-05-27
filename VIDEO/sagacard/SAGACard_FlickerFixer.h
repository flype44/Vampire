/******************************************************************************
**
** SAGACard_FlickerFixer.h
** Picasso96/SAGA.card
** 
******************************************************************************/

#ifndef SAGACARD_FLICKERFIXER_H
#define SAGACARD_FLICKERFIXER_H

/*****************************************************************************/

#ifndef boardinfo_H
#include "boardinfo.h"
#endif

/*****************************************************************************/

struct FlickerFixer
{
    ULONG                Flags;
    ULONG                ChangeFlags;
    struct BitMap       *BitMap;
    BOOL                 IsPAL;
    BOOL                 IsInterlace;
    struct ModeInfo      ModeInfo;
    RGBFTYPE             Format;
};

/*****************************************************************************/

#define FFB_ACTIVE       0
#define FFB_ONBOARD      1

#define FFF_ACTIVE       (1<<FFB_ACTIVE)
#define FFF_ONBOARD      (1<<FFB_ONBOARD)

#define FFCB_ACTIVE      0
#define FFCB_ONBOARD     1

#define FFCF_ACTIVE      (1<<FFCB_ACTIVE)
#define FFCF_ONBOARD     (1<<FFCB_ONBOARD)

/*****************************************************************************/

ULONG Card_SetFlickerFixerAttrs(
    struct BoardInfo     *bi,
    struct FlickerFixer  *ff,
    struct TagItem       *tags);

ULONG Card_GetFlickerFixerAttrs(
    struct BoardInfo     *bi, 
    struct FlickerFixer  *ff,
    struct TagItem       *tags);

ULONG Card_CreateFlickerFixer(
    struct BoardInfo     *bi,
    struct FlickerFixer  *ff, 
    struct TagItem       *tags);

void Card_DeleteFlickerFixer(
    struct BoardInfo     *bi,
    struct FlickerFixer  *ff);

/*****************************************************************************/
