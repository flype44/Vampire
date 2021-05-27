/******************************************************************************
**
** SAGACard_Support.h
** Picasso96/SAGA.card
** 
******************************************************************************/

#ifndef SAGACARD_SUPPORT_H
#define SAGACARD_SUPPORT_H

/*****************************************************************************/

#ifndef boardinfo_H
#include "boardinfo.h"
#endif

/*****************************************************************************/

UWORD ASM SAVEDS Card_CalculateBytesPerRow(
    __REGA0(struct BoardInfo  *bi),
    __REGD0(UWORD              width),
    __REGD7(RGBFTYPE           rgbFormat));

APTR ASM SAVEDS Card_CalculateMemory(
    __REGA0(struct BoardInfo  *bi),
    __REGA1(APTR               logicalAddr),
    __REGD7(RGBFTYPE           rgbFormat));

ULONG ASM SAVEDS Card_GetCompatibleFormats(
    __REGA0(struct BoardInfo  *bi),
    __REGD7(RGBFTYPE           rgbFormat));

ULONG ASM SAVEDS Card_GetPixelClock(
    __REGA0(struct BoardInfo  *bi),
    __REGA1(struct ModeInfo   *mi),
    __REGD0(ULONG              clockIndex),
    __REGD7(RGBFTYPE           rgbFormat));

ULONG ASM SAVEDS Card_GetVBeamPos(
    __REGA0(struct BoardInfo  *bi));

BOOL ASM SAVEDS Card_GetVSyncState(
    __REGA0(struct BoardInfo  *bi),
    __REGD0(BOOL               ExpectedState));

void ASM SAVEDS Card_ResetChip(
	__REGA0(struct BoardInfo  *bi));

LONG ASM SAVEDS Card_ResolvePixelClock(
    __REGA0(struct BoardInfo  *bi),
    __REGA1(struct ModeInfo   *mi),
    __REGD0(ULONG              pixelClock),
    __REGD7(RGBFTYPE           rgbFormat));

void ASM SAVEDS Card_SetClearMask(
    __REGA0(struct BoardInfo  *bi),
    __REGD0(UBYTE              mask));

void ASM SAVEDS Card_SetClock(
    __REGA0(struct BoardInfo  *bi));

void ASM SAVEDS Card_SetColorArray(
    __REGA0(struct BoardInfo  *bi),
    __REGD0(UWORD              startIndex),
    __REGD1(UWORD              count));

void ASM SAVEDS Card_SetDAC(
    __REGA0(struct BoardInfo  *bi),
    __REGD7(RGBFTYPE           rgbFormat));

BOOL ASM SAVEDS Card_SetDisplay(
    __REGA0(struct BoardInfo  *bi),
    __REGD0(BOOL               state));

void SAVEDS ASM Card_SetDPMSLevel(
    __REGA0(struct BoardInfo  *bi),
    __REGD0(ULONG              DPMSLevel));

void ASM SAVEDS Card_SetGC(
    __REGA0(struct BoardInfo  *bi),
    __REGA1(struct ModeInfo   *mi),
    __REGD0(BOOL               border));

BOOL ASM SAVEDS Card_SetInterrupt(
    __REGA0(struct BoardInfo  *bi),
    __REGD0(BOOL               enable));

void ASM SAVEDS Card_SetMemoryMode(
    __REGA0(struct BoardInfo  *bi),
    __REGD7(RGBFTYPE           rgbFormat));

void ASM SAVEDS Card_SetPanning(
    __REGA0(struct BoardInfo  *bi),
    __REGA1(UBYTE             *startMemory),
    __REGD0(UWORD              width),
    __REGD1(WORD               xOffset),
    __REGD2(WORD               yOffset),
    __REGD7(RGBFTYPE           rgbFormat));

void ASM SAVEDS Card_SetReadPlane(
    __REGA0(struct BoardInfo  *bi),
    __REGD0(UBYTE              plane));

BOOL ASM SAVEDS Card_SetSwitch(
    __REGA0(struct BoardInfo  *bi),
    __REGD0(BOOL               state));

void ASM SAVEDS Card_SetWriteMask(
    __REGA0(struct BoardInfo  *bi),
    __REGD0(UBYTE              mask));

void ASM SAVEDS Card_WaitBlitter(
    __REGA0(struct BoardInfo  *bi));

void ASM SAVEDS Card_WaitVerticalSync(
    __REGA0(struct BoardInfo  *bi),
    __REGD0(BOOL               reserved));

/*****************************************************************************/

#endif

