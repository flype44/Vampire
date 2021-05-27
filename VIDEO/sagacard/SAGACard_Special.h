/******************************************************************************
**
** SAGACard_Special.h
** Picasso96/SAGA.card
** 
******************************************************************************/

#ifndef SAGACARD_SPECIAL_H
#define SAGACARD_SPECIAL_H

/*****************************************************************************/

#include <math.h>
#include <exec/memory.h>
#include <proto/exec.h>
#include <proto/utility.h>

/*****************************************************************************/

#ifndef boardinfo_H
#include "boardinfo.h"
#endif

/*****************************************************************************/

#define MAX_ZOOM 16

enum
{
    PIPErr_OK,
    PIPErr_NotAvailable,
    PIPErr_WrongScreenMode,
    PIPErr_NotEnoughBandwidth,
    PIPErr_OutOfVideoMemory,
    PIPErr_CreateContextFailed,
    PIPErr_UnsupportedSourceFormat,
    PIPErr_SourceWidthTooBig
};

/*****************************************************************************/

enum
{
    AFB_Occlude,
    AFB_VInterpolation
};

#define	AFF_Occlude         (1L << AFB_Occlude)
#define	AFF_VInterpolation  (1L << AFB_VInterpolation)

/*****************************************************************************/

APTR SAVEDS ASM Card_CreateFeature(
    __REGA0(struct BoardInfo *bi),
    __REGD0(ULONG             Type),
    __REGA1(struct TagItem   *Tags));

ULONG ASM SAVEDS Card_SetFeatureAttrs(
    __REGA0(struct BoardInfo *bi),
    __REGA1(APTR              FeatureData),
    __REGD0(ULONG             Type),
    __REGA2(struct TagItem   *Tags));

ULONG ASM SAVEDS Card_GetFeatureAttrs(
    __REGA0(struct BoardInfo *bi),
    __REGA1(APTR              FeatureData),
    __REGD0(ULONG             Type),
    __REGA2(struct TagItem   *Tags));

BOOL ASM SAVEDS Card_DeleteFeature(
    __REGA0(struct BoardInfo *bi),
    __REGA1(APTR              FeatureData),
    __REGD0(ULONG             Type));

/*****************************************************************************/

#endif
