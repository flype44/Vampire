#ifndef __WBCANDY_H
#define __WBCANDY_H

#include <stdlib.h>

/**********************************************************
 *
 * Defines for the application
 * 
 **********************************************************/

#define APP_NAME     "WBCandy"
#define APP_TITLE    "WBCandy 0.1alpha"
#define APP_DESCR    "AmigaOS3 Workbench enhancer for Vampire cards"
#define VERSTRING    "$VER: WBCandy (28.11.2021) Philippe CARPENTIER"

#define APP_TEMPLATE "\
D=DIRECTORY/A,\
C=FRAMECOUNT/N/A,\
MS=MILLISECONDS/N,\
K=COLORKEY/N,\
QUIET/S"

enum {
	OPT_DIRECTORY,  // 0
	OPT_FRAMECOUNT, // 1
	OPT_FRAMEDELAY, // 2
	OPT_COLORKEY,   // 3
	OPT_QUIET,      // 4
	OPT_COUNT       // 5
};

/**********************************************************
 *
 * Defines for the SAGA hardware
 * 
 **********************************************************/

#define READ16(a) (*(volatile UWORD*)a)
#define READ32(a) (*(volatile ULONG*)a)

#define WRITE16(a, b) *(volatile UWORD*)(a) = (b)
#define WRITE32(a, b) *(volatile ULONG*)(a) = (b)

#define SAGA_PIP_DELTAX     16
#define SAGA_PIP_DELTAY     0
#define SAGA_PIP_X0         0xDFF3D0
#define SAGA_PIP_Y0         0xDFF3D2
#define SAGA_PIP_X1         0xDFF3D4
#define SAGA_PIP_Y1         0xDFF3D6
#define SAGA_PIP_BPLPTR     0xDFF3D8
#define SAGA_PIP_PIXFMT     0xDFF3DC
#define SAGA_PIP_MODULO     0xDFF3DE
#define SAGA_PIP_COLORKEY   0xDFF3E0
#define SAGA_PIP_COLOR0     0xDFFC00

#define SAGAF_OFF           0x0000
#define SAGAF_CLUT          0x0001
#define SAGAF_RGB16         0x0002
#define SAGAF_RGB15         0x0003
#define SAGAF_RGB24         0x0004
#define SAGAF_RGB32         0x0005
#define SAGAF_YUV422        0x0006

#endif

/**********************************************************
 *
 * END OF FILE
 * 
 **********************************************************/
