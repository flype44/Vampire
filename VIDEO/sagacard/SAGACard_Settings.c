/******************************************************************************
**
** SAGACARD_Settings.c
** Picasso96/SAGA.card
** 
******************************************************************************/

#include <exec/exec.h>
#include <proto/exec.h>
#include <proto/utility.h>

/*****************************************************************************/

#ifndef boardinfo_H
#include "boardinfo.h"
#endif

#ifndef settings_H
#include "settings.h"
#endif

#ifndef SAGACARD_H
#include "SAGACard.h"
#endif

#ifndef SAGACARD_SETTINGS_H
#include "SAGACard_Settings.h"
#endif

/*****************************************************************************/

BOOL AddResolution(
    struct BoardInfo *bi,
    ULONG             DisplayID,
    ULONG             Width,
    ULONG             Height)
{
    struct ModeInfo      *mi;
    struct LibResolution *res;
    struct Library       *UtilityBase;
    
    ULONG MemSize = sizeof(*res) + (sizeof(struct ModeInfo) + 32) * (MAXMODES - 1);
    
    if (res = AllocMem(MemSize, MEMF_ANY | MEMF_CLEAR))
    {
        if (mi = (struct ModeInfo *)&res[1])
        {
            ULONG i;
            
            res->BoardInfo    = bi;
            res->DisplayID    = (SAGA_DISPLAYIDBASE + (DisplayID << 16));
            res->Flags        = P96F_PUBLIC;
            res->Width        = Width;
            res->Height       = Height;
            res->P96ID[0]     = 'P';
            res->P96ID[1]     = '9';
            res->P96ID[2]     = '6';
            res->P96ID[3]     = '-';
            res->P96ID[4]     = '0';
            res->P96ID[5]     = ':';
            res->Modes[0]     = NULL;
            res->Node.ln_Name = res->P96ID;
            
            /*
            sprintf(res->Name, "SAGA: %ldx%ld", Width, Height);
            */
            
            if (UtilityBase = bi->UtilBase) // V47!
            {
                SNPrintf(res->Name, MAXRESOLUTIONNAMELENGTH, 
                    "SAGA: %ldx%ld", Width, Height);
            }
            
            for (i = CHUNKY; i <= TRUEALPHA; i++)
            {
                static UBYTE Depth[MAXMODES] = { 4, 8, 16, 24, 32 };
                
                mi[i].OpenCount      = 0;
                mi[i].Active         = TRUE;
                mi[i].Width          = Width;
                mi[i].Height         = Height;
                mi[i].Depth          = Depth[i];
                mi[i].Flags          = GMF_HPOLARITY | GMF_VPOLARITY;
                mi[i].HorTotal       = Width;
                mi[i].HorBlankSize   = 0;
                mi[i].HorSyncStart   = Width;
                mi[i].HorSyncSize    = Width;
                mi[i].HorSyncSkew    = 0;
                mi[i].HorEnableSkew  = 0;
                mi[i].VerTotal       = Height;
                mi[i].VerBlankSize   = 0;
                mi[i].VerSyncStart   = Height;
                mi[i].VerSyncSize    = Height;
                mi[i].pll1.Clock     = 1;
                mi[i].pll1.Numerator = 1;
                mi[i].PixelClock     = SAGA_PIXEL_CLOCK;
                
                res->Modes[i] = &mi[i];
            }
            
            AddTail((struct List *)&bi->ResolutionsList, 
                (struct Node *)res);
            
            return(TRUE);
        }
    }
    
    return(FALSE);
}

/*****************************************************************************/

void AddResolutionList(struct BoardInfo *bi)
{
    AddResolution(bi, 0x01,  320, 200);
    AddResolution(bi, 0x02,  320, 240);
    AddResolution(bi, 0x03,  320, 256);
    AddResolution(bi, 0x04,  640, 400);
    AddResolution(bi, 0x05,  640, 480);
    AddResolution(bi, 0x06,  640, 512);
    AddResolution(bi, 0x07,  960, 540);
    AddResolution(bi, 0x08,  480, 270);
    AddResolution(bi, 0x09,  304, 224);
    AddResolution(bi, 0x0A, 1280, 720);
    AddResolution(bi, 0x0B,  640, 360);
    AddResolution(bi, 0x0C,  800, 600);
    AddResolution(bi, 0x0D, 1024, 768);
    AddResolution(bi, 0x0E,  720, 576);
    AddResolution(bi, 0x0F,  848, 480);
}

/*****************************************************************************/
