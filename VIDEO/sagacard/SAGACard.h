/******************************************************************************
**
** SAGACard.h
** Picasso96/SAGA.card
** 
******************************************************************************/

#ifndef SAGACARD_H
#define SAGACARD_H

/*****************************************************************************/

#include <exec/types.h>

/*****************************************************************************/

#define SAVEDS __saveds

/*****************************************************************************/

void KPrintf(char *, ...);
void Write16(int reg, UWORD value);
void Write32(int reg, ULONG value);

/*****************************************************************************/

#define HASFLAG(value, flag) (((value) & (flag)) == (flag))

#define	D(x) x

/*
#define	D(x) {if(HASFLAG(bi->CardData[CARDDATA_FLAGS], CARDD_FLG_DEBUG)){x;}}
*/

/*****************************************************************************/

#define SAGA_DISPLAYIDBASE           0x54001000   /* Graphic ModeID */
#define SAGA_MANUFACTURER_ID         0x1398       /* APOLLO-Team    */
#define SAGA_PRODUCT_ID              0x05         /* Vampire V4(+)  */

/*****************************************************************************/

#define CARDDATA_FLAGS               0  /* BoardInfo->CardData[ CARDDATA_ ] */
#define CARDDATA_WIDTH               1
#define CARDDATA_MODEID              2
#define CARDDATA_DACMODE             3
#define CARDDATA_MEMLIST             4
#define CARDDATA_SPRITE_X            5
#define CARDDATA_SPRITE_Y            6
#define CARDDATA_UNUSED7             7
#define CARDDATA_UNUSED8             8
#define CARDDATA_UNUSED9             9
#define CARDDATA_UNUSED10            10
#define CARDDATA_UNUSED11            11
#define CARDDATA_UNUSED12            12
#define CARDDATA_UNUSED13            13
#define CARDDATA_UNUSED14            14
#define CARDDATA_UNUSED15            15  /* MAX */

#define CARDD_FLG_DEBUG              2   /* (1 << 1) */
#define CARDD_FLG_DISP_OFF           4   /* (1 << 2) */
#define CARDD_FLG_SPRITE_OFF         8   /* (1 << 3) */
#define CARDD_FLG_UNUSED16           16  /* (1 << 4) */
#define CARDD_FLG_UNUSED32           32  /* (1 << 5) */

/*****************************************************************************/

#define SAGA_CLUT_ENTRY_VALID        (1UL << 31)

#define SAGA_PIXEL_CLOCK             (28375000)
#define SAGA_VIDEO_SIZE              (4*1024*1024)

#define SAGA_VIDEO_MAXHV             0x4000
#define SAGA_VIDEO_MAXVV             0x4000
#define SAGA_VIDEO_MAXHR             0x8000
#define SAGA_VIDEO_MAXVR             0x8000

#define SAGA_VIDEO_MODULO            0xDFF1E6
#define SAGA_VIDEO_BPLPTR            0xDFF1EC
#define SAGA_VIDEO_FORMAT            0xDFF1F4
#define SAGA_VIDEO_CLUT              0xDFF388

#define SAGA_VIDEO_FORMAT_OFF        0x00
#define SAGA_VIDEO_FORMAT_CLUT8      0x01
#define SAGA_VIDEO_FORMAT_RGB16      0x02
#define SAGA_VIDEO_FORMAT_RGB15      0x03
#define SAGA_VIDEO_FORMAT_RGB24      0x04
#define SAGA_VIDEO_FORMAT_RGB32      0x05
#define SAGA_VIDEO_FORMAT_YUV422     0x06
#define SAGA_VIDEO_FORMAT_RESERVED   0x07
#define SAGA_VIDEO_FORMAT_PLANAR1    0x08
#define SAGA_VIDEO_FORMAT_PLANAR2    0x09
#define SAGA_VIDEO_FORMAT_PLANAR4    0x0A

/*****************************************************************************/

#define SAGA_SPRITE_DELTA_X          16
#define SAGA_SPRITE_DELTA_Y          8

#define SAGA_SPRITE_X                0xDFF1D0
#define SAGA_SPRITE_Y                0xDFF1D2
#define SAGA_SPRITE_CLUT             0xDFF3A2
#define SAGA_SPRITE_BPLPTR           0xDFF800

/*****************************************************************************/

#define SAGA_PIP_DELTA_X             16
#define SAGA_PIP_DELTA_Y             0

#define SAGA_PIP_XSTRT               0xDFF3D0
#define SAGA_PIP_YSTRT               0xDFF3D2
#define SAGA_PIP_XSTOP               0xDFF3D4
#define SAGA_PIP_YSTOP               0xDFF3D6
#define SAGA_PIP_BPLPTR              0xDFF3D8
#define SAGA_PIP_FORMAT              0xDFF3DC
#define SAGA_PIP_MODULO              0xDFF3DE
#define SAGA_PIP_COLORKEY            0xDFF3E0
#define SAGA_PIP_DMAROWLEN           0xDFF3E2
#define SAGA_PIP_CLUT                0xDFF38C

/*****************************************************************************/

#endif
