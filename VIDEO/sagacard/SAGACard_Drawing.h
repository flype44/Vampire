/******************************************************************************
**
** SAGACard_Drawing.h
** Picasso96/SAGA.card
** 
******************************************************************************/

#ifndef SAGACARD_DRAWING_H
#define SAGACARD_DRAWING_H

/*****************************************************************************/

#ifndef boardinfo_H
#include "boardinfo.h"
#endif

/*****************************************************************************/

void ASM SAVEDS Card_BlitPattern(
    __REGA0(struct BoardInfo  *bi),
    __REGA1(struct RenderInfo *ri),
    __REGA2(struct Pattern    *pattern),
    __REGD0(WORD               x),
    __REGD1(WORD               y),
    __REGD2(WORD               w),
    __REGD3(WORD               h),
    __REGD4(UBYTE              mask),
    __REGD7(RGBFTYPE           rgbFormat));

void ASM SAVEDS Card_BlitPlanar2Chunky(
    __REGA0(struct BoardInfo  *bi),
    __REGA1(struct BitMap     *bm),
    __REGA2(struct RenderInfo *ri),
    __REGD0(WORD               srcX),
    __REGD1(WORD               srcY),
    __REGD2(WORD               dstX),
    __REGD3(WORD               dstY),
    __REGD4(WORD               sizeX),
    __REGD5(WORD               sizeY),
    __REGD6(UBYTE              minTerm),
    __REGD7(UBYTE              mask));

void ASM SAVEDS Card_BlitPlanar2Direct(
    __REGA0(struct BoardInfo  *bi),
    __REGA1(struct BitMap     *bm),
    __REGA2(struct RenderInfo *ri),
    __REGA3(struct ColorIndexMapping *cim),
    __REGD0(WORD               srcX),
    __REGD1(WORD               srcY),
    __REGD2(WORD               dstX),
    __REGD3(WORD               dstY),
    __REGD4(WORD               sizeX),
    __REGD5(WORD               sizeY),
    __REGD6(UBYTE              minTerm),
    __REGD7(UBYTE              mask));

void ASM SAVEDS Card_BlitRect(
    __REGA0(struct BoardInfo  *bi),
    __REGA1(struct RenderInfo *ri),
    __REGD0(WORD               srcX),
    __REGD1(WORD               srcY),
    __REGD2(WORD               dstX),
    __REGD3(WORD               dstY),
    __REGD4(WORD               sizeX),
    __REGD5(WORD               sizeY),
    __REGD6(UBYTE              mask),
    __REGD7(RGBFTYPE           rgFormat));

void ASM SAVEDS Card_BlitRectNoMaskComplete(
    __REGA0(struct BoardInfo  *bi),
    __REGA1(struct RenderInfo *sri),
    __REGA2(struct RenderInfo *dri),
    __REGD0(WORD               srcX),
    __REGD1(WORD               srcY),
    __REGD2(WORD               dstX),
    __REGD3(WORD               dstY),
    __REGD4(WORD               sizeX),
    __REGD5(WORD               sizeY),
    __REGD6(UBYTE              opCode),
    __REGD7(RGBFTYPE           rgbFormat));

void ASM SAVEDS Card_BlitTemplate(
    __REGA0(struct BoardInfo  *bi),
    __REGA1(struct RenderInfo *ri),
    __REGA2(struct Template   *tmplt),
    __REGD0(WORD               x),
    __REGD1(WORD               y),
    __REGD2(WORD               w),
    __REGD3(WORD               h),
    __REGD4(UBYTE              mask),
    __REGD7(RGBFTYPE           rgbFormat));

void ASM SAVEDS Card_DrawLine(
    __REGA0(struct BoardInfo  *bi),
    __REGA1(struct RenderInfo *ri),
    __REGA2(struct Line       *line),
    __REGD0(UBYTE              mask),
    __REGD7(RGBFTYPE           rgbFormat));

void ASM SAVEDS Card_FillRect(
    __REGA0(struct BoardInfo  *bi),
    __REGA1(struct RenderInfo *ri), 
    __REGD0(WORD               x),
    __REGD1(WORD               y),
    __REGD2(WORD               w),
    __REGD3(WORD               h),
    __REGD4(ULONG              pen),
    __REGD5(UBYTE              mask),
    __REGD7(RGBFTYPE           rgbFormat));

void ASM SAVEDS Card_InvertRect(
    __REGA0(struct BoardInfo  *bi),
    __REGA1(struct RenderInfo *ri),
    __REGD0(WORD               x),
    __REGD1(WORD               y),
    __REGD2(WORD               w),
    __REGD3(WORD               h),
    __REGD4(UBYTE              mask),
    __REGD7(RGBFTYPE           rgbFormat));

void ASM SAVEDS Card_ScrollPlanar(
    __REGA0(struct BoardInfo  *bi),
    __REGA1(struct RenderInfo *ri),
    __REGD0(UWORD              a),
    __REGD1(UWORD              b),
    __REGD2(UWORD              c),
    __REGD3(UWORD              d),
    __REGD4(UWORD              e),
    __REGD5(UWORD              f),
    __REGD6(UBYTE              g));

void ASM SAVEDS Card_UpdatePlanar(
    __REGA0(struct BoardInfo  *bi),
    __REGA1(struct BitMap     *bm),
    __REGA2(struct RenderInfo *ri),
    __REGD0(SHORT              minX),
    __REGD1(SHORT              minY),
    __REGD2(SHORT              maxX),
    __REGD3(SHORT              maxY),
    __REGD4(UBYTE              mask));

/*****************************************************************************/

#endif

