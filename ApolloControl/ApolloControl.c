/**********************************************************
 * 
 * Project: ApolloControl
 * Version: 1.28
 * Author:  Philippe CARPENTIER (flype)
 * 
 * Copyright (C) 2016-2023 Apollo-Team
 * 
 **********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dos/dos.h>
#include <exec/exec.h>
#include <exec/types.h>
#include <exec/memory.h>
#include <exec/execbase.h>
#include <exec/resident.h>
#include <intuition/screens.h>
#include <graphics/gfxbase.h>
#include <hardware/custom.h>
#include <libraries/expansion.h>
#include <libraries/configvars.h>
#include <cybergraphics/cybergraphics.h>

#include <proto/dos.h>
#include <proto/exec.h>
#include <proto/expansion.h>
#include <proto/cybergraphics.h>

#include "ApolloControl.h"

/**********************************************************
 * DEFINES
 **********************************************************/

#ifndef AFB_68060
#define AFB_68060 7
#define AFF_68060 (1 << AFB_68060)
#endif

#ifndef AFB_68080
#define AFB_68080 10
#define AFF_68080 (1 << AFB_68080)
#endif

/**********************************************************
 * DEFINES
 **********************************************************/

typedef enum {
    BOARD_FULLNAME,  // 0
    BOARD_SHORTNAME, // 1
    BOARD_DESIGNER,  // 2
    BOARD_MAXFIELDS  // 3
} BOARD_FIELD;

typedef enum {
    MODULE_DEVICE,   // 0
    MODULE_LIBRARY,  // 1
    MODULE_RESOURCE, // 2
    MODULE_RESIDENT, // 3
    MODULE_MSGPORT   // 4
} MODULE_TYPE;

typedef struct {
    ULONG  type;
    STRPTR name;
    APTR   addr;
    ULONG  ver;
    ULONG  rev;
} Module;

Module Modules[] = {
    { MODULE_LIBRARY,  "68040.library"           , 0, 0, 0 },
    { MODULE_RESIDENT, "680x0.library"           , 0, 0, 0 },
    { MODULE_LIBRARY,  "vampiregfx.card"         , 0, 0, 0 },
    { MODULE_RESIDENT, "VampireBoot"             , 0, 0, 0 },
    { MODULE_RESIDENT, "VampireFastMem"          , 0, 0, 0 },
    { MODULE_RESIDENT, "VampireSupport"          , 0, 0, 0 },
    { MODULE_RESOURCE, "vampire.resource"        , 0, 0, 0 },
    { MODULE_RESOURCE, "vampiresupport.resource" , 0, 0, 0 },
    { MODULE_RESOURCE, "processor.resource"      , 0, 0, 0 },
    { MODULE_DEVICE,   "sagasd.device"           , 0, 0, 0 },
};

#define TEMPLATE "\
HELP/S,\
AF=ATTNFLAGS/S,\
AK=AKIKO/S,\
BO=BOARD/S,\
BI=BOARDID/S,\
BN=BOARDNAME/S,\
SN=BOARDSERIAL/S,\
CO=CORE/S,\
CR=COREREV/S,\
CP=CPU/S,\
HZ=CPUHERTZ/S,\
DE=DETECT/S,\
DM=DISPLAYMODE/N,\
DP=DPMS/N,\
FP=FPU/N,\
JP=JOYPORT/S,\
ID=IDESPEED/N,\
MR=MAPROM,\
ML=MEMLIST/S,\
MO=MODULES/S,\
CD=CONFIGDEV/S,\
SC=SCANLINES/N,\
SD=SDCLOCKDIV/N,\
SE=SETENV/S,\
SH=SHIELD/N,\
SS=SUPERSCALAR/N,\
TU=TURTLE/N,\
ZO=ZOOM/N,\
ZV=ZOOMSHIFTV/N,\
ZH=ZOOMSHIFTH/N, \
VB=VBRMOVE/N"

typedef enum {
    OPT_HELP,
    OPT_ATTNFLAGS,
    OPT_AKIKO,
    OPT_BOARD,
    OPT_BOARDID,
    OPT_BOARDNAME,
    OPT_BOARDSERIAL,
    OPT_COREREVSTRING,
    OPT_COREREVNUMBER,
    OPT_CPU,
    OPT_CPUHERTZ,
    OPT_DETECT,
    OPT_DISPLAYMODE,
    OPT_DPMS,
    OPT_FPU,
    OPT_JOYPORT,
    OPT_IDESPEED,
    OPT_MAPROM,
    OPT_MEMLIST,
    OPT_MODULES,
    OPT_CONFIGDEV,
    OPT_SCANLINES,
    OPT_SDCLOCKDIV,
    OPT_SETENV,
    OPT_SHIELD,
    OPT_SUPERSCALAR,
    OPT_TURTLE,
    OPT_ZOOM,
    OPT_ZOOMSHIFT_V,
    OPT_ZOOMSHIFT_H,
    OPT_VBRMOVE,
    OPT_COUNT
} OPT_ARGS;

#define MAXBOARDID  (10)
#define MAXMODULES  (10)
#define MAXFILESIZE (13)

#define COLOR0 "\033[30m" /* Color Grey  */
#define COLOR1 "\033[31m" /* Color Black */
#define COLOR2 "\033[32m" /* Color White */
#define COLOR3 "\033[33m" /* Color Blue  */
#define COLOR4 "\033[34m" /* Color #4    */
#define COLOR5 "\033[35m" /* Color #5    */
#define COLOR6 "\033[36m" /* Color #6    */
#define COLOR7 "\033[37m" /* Color #7    */

/**********************************************************
 * GLOBALS
 **********************************************************/

UBYTE * AudioName[2]  = {
    "Paula (8364 rev0)",  // Paula identifier 0x00 
    "Arne (8364 rev1)" // Paula identifier 0x01
};

UBYTE * VideoName[3]  = {
    "OCS Denise (8362)",
    "ECS Denise (8373)",
    "AGA Lisa (4203)"
};

UBYTE * AttnName[16] = { 
    "010",    //  0
    "020",    //  1
    "030",    //  2
    "040",    //  3
    "881",    //  4
    "882",    //  5
    "FPU40",  //  6
    "060",    //  7
    "BIT8",   //  8 (Unused)
    "BIT9",   //  9 (Unused)
    "080",    // 10
    "BIT11",  // 11 (Unused)
    "BIT12",  // 12 (Unused)
    "ADDR32", // 13 (CPU is 32bit)
    "BIT14",  // 14 (MMU presence)
    "PRIVATE" // 15 (FPU presence)
};

UBYTE * BoardName[MAXBOARDID][3] = {
   { "Unknown",                "N/A",     "Unknown" },  // 0x00
   { "Vampire V2 V600",        "V2-600",  "Majsta"  },  // 0x01
   { "Vampire V2 V500",        "V2-500",  "Majsta"  },  // 0x02
   { "Apollo V4 FireBird",     "V4-500",  "ceaich"  },  // 0x03
   { "Apollo V4 IceDrake",     "V4-1200", "ceaich"  },  // 0x04
   { "Apollo V4 StandAlone",   "V4-SA",   "ceaich"  },  // 0x05
   { "Vampire V2 V1200",       "V2-1200", "Majsta"  },  // 0x06
   { "Apollo V4 MantiCore",    "V4-600",  "ceaich"  },  // 0x07
   { "Unknown",                "N/A",     "Unknown" }   // 0x08
};

BOOL  BOARDID_V2  = FALSE;

char TextAttnFlags[256];

struct Library       * CyberGfxBase;
struct ExpansionBase * ExpansionBase;

extern struct ExecBase   * SysBase;
extern struct DosLibrary * DOSBase;
extern struct GfxBase    * GfxBase;

static STRPTR verstring = APP_VSTRING;

/**********************************************************
 ** 
 ** Help()
 ** 
 **********************************************************/

ULONG Help(void)
{
    printf(verstring+6);
    
    printf(
    "\n\n"
    /*COLOR2*/ "HELP/S            " /*COLOR7*/ "This help\n"
    /*COLOR2*/ "DE=DETECT/S       " /*COLOR7*/ "Return $RC=0 if AC68080 is detected\n"
    /*COLOR2*/ "BO=BOARD/S        " /*COLOR7*/ "Output Board Information\n"
    /*COLOR2*/ "BI=BOARDID/S      " /*COLOR7*/ "Output Board Identifier\n"
    /*COLOR2*/ "BN=BOARDNAME/S    " /*COLOR7*/ "Output Board Name\n"
    /*COLOR2*/ "SN=BOARDSERIAL/S  " /*COLOR7*/ "Output Board Serial Number\n"
    /*COLOR2*/ "CO=CORE/S         " /*COLOR7*/ "Output Core Revision String\n"
    /*COLOR2*/ "CR=COREREV/S      " /*COLOR7*/ "Output Core Revision Number\n"
    /*COLOR2*/ "CP=CPU/S          " /*COLOR7*/ "Output CPU Information\n"
    /*COLOR2*/ "HZ=HERTZ/S        " /*COLOR7*/ "Output CPU Frequency (Hertz)\n"
    /*COLOR2*/ "ML=MEMLIST/S      " /*COLOR7*/ "Output Memory list\n"
    /*COLOR2*/ "MO=MODULES/S      " /*COLOR7*/ "Output Modules list\n"
    /*COLOR2*/ "CD=CONFIGDEV/S    " /*COLOR7*/ "Output AutoConfig Devices list\n"
    /*COLOR2*/ "SE=SETENV/S       " /*COLOR7*/ "Create Environment Variables\n"
    /*COLOR2*/ "AK=AKIKO/S        " /*COLOR7*/ "Initialize the Akiko C2P routine\n"
    /*COLOR2*/ "AF=ATTNFLAGS/S    " /*COLOR7*/ "Change the AttnFlags (Force 080's)\n"
    /*COLOR2*/ "DM=DISPLAYMODE/N  " /*COLOR7*/ "Change the Display mode. 0=NTSC, 1=PAL\n"
    /*COLOR2*/ "DP=DPMS/N         " /*COLOR7*/ "Change the DPMS mode. 0=Off, 1=On\n"
    /*COLOR2*/ "FP=FPU/N          " /*COLOR7*/ "Change the FPU mode. 0=Off, 1=On\n"
    /*COLOR2*/ "ID=IDESPEED/N     " /*COLOR7*/ "Change the IDE speed. 0=Slow, 1=Fast, 2=Faster, 3=Fastest\n"
    /*COLOR2*/ "SC=SCANLINES/N    " /*COLOR7*/ "Change the Scanlines mode. 0=Off, 1=On, for V4(+)\n"
    /*COLOR2*/ "SD=SDCLOCKDIV/N   " /*COLOR7*/ "Change the SDPort Clock Divider. 0=Fastest, 255=Slowest\n"
    /*COLOR2*/ "SH=SHIELD/N       " /*COLOR7*/ "Change the Shield mode. 0=Off, 1=On\n"
    /*COLOR2*/ "SS=SUPERSCALAR/N  " /*COLOR7*/ "Change the SuperScalar mode. 0=Off, 1=On\n"
    /*COLOR2*/ "TU=TURTLE/N       " /*COLOR7*/ "Change the Turtle mode. 0=Off, 1=A500, 2=A1200\n"
    /*COLOR2*/ "ZO=ZOOM/N         " /*COLOR7*/ "Change the Zoom mode. 0=Off, 1=320x200, 2=320x240, 3=320x256, for V4(+)\n"
    /*COLOR2*/ "ZV=ZOOMSHIFTV/N   " /*COLOR7*/ "Change the Zoom Vertical SHIFT value. 0 to 126, for V4(+)\n"
    /*COLOR2*/ "ZH=ZOOMSHIFTH/N   " /*COLOR7*/ "Change the Zoom Horizontal SHIFT value. 0 to 62, for V4(+)\n"
    /*COLOR2*/ "VB=VBRMOVE/N      " /*COLOR7*/ "Change the VBR location. 0=ChipRAM, 1=FastRAM\n"
    /*COLOR2*/ "JP=JOYPORT/S      " /*COLOR7*/ "Patch the LowLevel.library to use the GAMEPADs, for V4(+)\n"
    /*COLOR2*/ "MR=MAPROM         " /*COLOR7*/ "Map a ROM file\n");
    
    return(RETURN_OK);
}

/**********************************************************
 ** 
 ** Print size, in bytes, in a human readable format.
 ** 
 **********************************************************/

UBYTE * PrintSize(ULONG size, UBYTE * buf)
{
    ULONG i = 0;
    ULONG r = 0;
    STRPTR units[] = { "B", "KB", "MB", "GB", "TB" };
    
    while(size > 1024)
    {
        r = size % 1024;
        size /= 1024;
        i++;
    }
    
    if(r > 1000)
    {
        size++;
        r = 0;
    }
    
    r /= 100;
    
    sprintf(buf, "%3lu.%lu %s", size, r, units[i]);
    
    return(buf);
}

/**********************************************************
 ** 
 ** GetAttnFlags()
 ** 
 **********************************************************/

ULONG GetAttnFlags(void)
{
    int i, j = 0;
    char buf[128];
    
    memset(buf, 0, 128);
    
    for(i = 0; i < 16; i++)
    {
        if(SysBase->AttnFlags & (1 << i))
        {
            if(j > 0)
            {
                strcat(buf, ",");
            }
            
            strcat(buf, AttnName[i]);
            j++;
        }
    }
    
    printf("ATTN: 0x%04x (%s)\n", SysBase->AttnFlags, buf);
    
    return(1);
}

/**********************************************************
 ** 
 ** GetBoardFreqHW()
 ** 
 **********************************************************/

ULONG GetBoardFreqHW(void)
{
    ULONG result = 0;
    
    if(v_cpu_is080())
    {
        result = ((*(volatile UWORD*)VREG_BOARD) & 0xff);
    }
    
    return(result);
}

/**********************************************************
 ** 
 ** GetBoardFreqSW()
 ** 
 **********************************************************/

ULONG GetBoardFreqSW(void)
{
    ULONG result = RETURN_WARN;

    if(v_cpu_is080())
    {
        ULONG n = v_cpu_multiplier();
        
        printf("AC68080 @ %u MHz (x%u)\n",
            ( n * SysBase->ex_EClockFrequency ) / 100000, 
            ( n )
        );
        
        result = RETURN_OK;
    }
    
    return(result);
}

/**********************************************************
 ** 
 ** GetBoardID()
 ** 
 **********************************************************/

ULONG GetBoardID(void)
{
    ULONG result = 0;
    
    if(v_cpu_is080())
    {
        result = ((*(volatile UWORD*)VREG_BOARD) >> 8);
    }
    
    return(result);
}

/**********************************************************
 ** 
 ** IsV2()
 ** 
 **********************************************************/

ULONG IsV2(void)
{
    ULONG id = GetBoardID();
    
    return (ULONG)(
        id == VREG_BOARD_V500 || 
        id == VREG_BOARD_V600 || 
        id == VREG_BOARD_V1200);
}

/**********************************************************
 ** 
 ** GetBoardName()
 ** 
 **********************************************************/

ULONG GetBoardName(void)
{
    ULONG result = RETURN_WARN;
    
    if(v_cpu_is080())
    {
        ULONG n = ((*(volatile UWORD*)VREG_BOARD) >> 8);
        
        if(n < MAXBOARDID)
        {
            printf("%s\n", BoardName[n][BOARD_SHORTNAME]);
            result = RETURN_OK;
        }
    }
    
    return(result);
}

/**********************************************************
 ** 
 ** GetBoardSerial()
 ** 
 **********************************************************/

ULONG GetBoardSerialNumber(void)
{
    ULONG result = RETURN_WARN;
    
    if(v_cpu_is080())
    {
        STRPTR s = "XXXXXXXXXXXXXXXX-XX";
        
        if(v_read_serialnumber(s))
        {
            printf("%s\n", s);
            result = RETURN_OK;
        }
    }
    
    return(result);
}

/**********************************************************
 ** 
 ** GetBoard()
 ** 
 **********************************************************/

ULONG GetBoard(void)
{
    ULONG result = RETURN_WARN;
    
    UBYTE * unknown = "Unknown";
    
    UBYTE totalChipStr[MAXFILESIZE];
    UBYTE totalFastStr[MAXFILESIZE];
    UBYTE totalSlowStr[MAXFILESIZE];
    
    ULONG audioChip = v_chipset_audio_rev();
    ULONG videoChip = v_chipset_video_rev();

    ULONG totalChip = AvailMem(MEMF_TOTAL | MEMF_CHIP);
    ULONG totalFast = AvailMem(MEMF_TOTAL | MEMF_FAST);
    ULONG totalSlow = AvailMem(MEMF_TOTAL | MEMF_FAST | MEMF_24BITDMA);
    
    printf("Board information:\n\n");
    
    if(v_cpu_is080())
    {
        ULONG boardId = GetBoardID();
        
        if(boardId < MAXBOARDID)
        {
            STRPTR serial = "XXXXXXXXXXXXXXXX-XX";
            
            if(v_read_serialnumber(serial))
            {
                printf("Product-ID   : %ld\n", boardId);
                printf("Product-Name : %s\n",  BoardName[boardId][BOARD_FULLNAME]);
                printf("Serial-Number: %s\n",  serial);
                printf("Designer     : %s\n",  BoardName[boardId][BOARD_DESIGNER]);
                printf("Manufacturer : Apollo-Team (C)\n");
                printf("\n");
            }
        }
        
        result = RETURN_OK;
    }
    
    printf("EClock Freq. : %lu.%05lu Hz (%s)\n", 
        ( SysBase->ex_EClockFrequency /  100000 ),
        ( SysBase->ex_EClockFrequency %  100000 ),
        ( SysBase->ex_EClockFrequency == 709379 ) ? "PAL" : "NTSC");
    
    printf("VBlank Freq. : %lu Hz\n", SysBase->VBlankFrequency);
    printf("Power  Freq. : %lu Hz\n", SysBase->PowerSupplyFrequency);
    printf("\n");
    
    printf("Video Chip   : %s\n", (videoChip <= 2) ? VideoName[videoChip] : unknown);
    printf("Audio Chip   : %s\n", (audioChip <= 1) ? AudioName[audioChip] : unknown);
    
    printf("Akiko Chip   : %s (0x%04x)\n", 
        ((*(volatile UWORD*)0xB80002) == 0xCAFE) ? "Detected" : "Not detected",
        (*(volatile UWORD*)0xB80002));
    
    printf("Akiko C2P    : %s (0x%08lx)\n", 
        GfxBase->ChunkyToPlanarPtr ? "Initialized" : "Uninitialized",
        GfxBase->ChunkyToPlanarPtr);
    
    printf("\n");
    
    printf("Chip Memory  : %s\n", PrintSize(totalChip, totalChipStr));
    printf("Fast Memory  : %s\n", PrintSize(totalFast, totalFastStr));
    printf("Slow Memory  : %s\n", PrintSize(totalSlow, totalSlowStr));
    printf("\n");
    
    return(result);
}

/**********************************************************
 ** 
 ** GetCoreRevisionNumber()
 ** 
 **********************************************************/

ULONG GetCoreRevisionNumber(void)
{
    ULONG result = RETURN_WARN;
    
    if(v_cpu_is080())
    {
        UBYTE buf[256];
        
        if(v_read_revisionstring(buf) && buf[0] == 'V')
        {
            STRPTR s = strstr(buf, "rev ");
            
            if(s)
            {
                printf("%ld\n", atoi(s + 4));
                result = RETURN_OK;
            }
        }
    }
    
    return(result);
}

/**********************************************************
 ** 
 ** GetCoreRevisionString()
 ** 
 **********************************************************/

ULONG GetCoreRevisionString(void)
{
    ULONG result = RETURN_WARN;
    
    if(v_cpu_is080())
    {
        UBYTE buf[256];
        
        if(v_read_revisionstring(buf) && buf[0] == 'V')
        {
            printf(buf);
            result = RETURN_OK;
        }
    }
    
    return(result);
}

/**********************************************************
 ** 
 ** GetCPU_CPU()
 ** 
 **********************************************************/

ULONG GetCPU_CPU(void)
{
    ULONG result = RETURN_WARN;
    
    if(v_cpu_is080())
    {
        UWORD n = ((*(volatile UWORD*)VREG_BOARD) & 0xff);
        
        printf("CPU:  AC68080 @ %lu MHz (x%u) (%lup)\n", 
            ( n * SysBase->ex_EClockFrequency ) / 100000,
            n, v_cpu_is2p() + 1);
        
        result = RETURN_OK;
    }
    
    return(result);
}

/**********************************************************
 ** 
 ** GetCPU_CACR()
 ** 
 **********************************************************/

ULONG GetCPU_CACR(void)
{
    ULONG result = RETURN_WARN;
    
    if((SysBase->AttnFlags & (1 << AFB_68020)) > 0)
    {
        ULONG n = v_cpu_cacr();
        
        printf("CACR: 0x%08x (InstCache: %s) (DataCache: %s)\n", 
            ( n ),
            ( n & (1 << 15) ) ? "On" : "Off",
            ( n & (1 << 31) ) ? "On" : "Off"
        );
        
        result = RETURN_OK;
    }
    
    return(result);
}

/**********************************************************
 ** 
 ** GetCPU_TCR()
 ** 
 **********************************************************/

ULONG GetCPU_TCR(void)
{
    ULONG result = RETURN_WARN;
    
    if((SysBase->AttnFlags & (1 << AFB_68080)) > 0)
    {
        ULONG n = v_cpu_tc();
        
        printf("TCR:  0x%08x (R: %s, W: %s, E: %s, N:%s) ", 
            ( n ),
            ( n & (1 << 0) ) ? "On" : "Off",
            ( n & (1 << 1) ) ? "On" : "Off",
            ( n & (1 << 2) ) ? "On" : "Off",
            ( n & (1 << 3) ) ? "On" : "Off"
        );
        
        printf("Shield is %s\n", n ? "enabled" : "disabled");
        
        result = RETURN_OK;
    }
    
    return(result);
}

/**********************************************************
 ** 
 ** GetCPU_PCR()
 ** 
 **********************************************************/

ULONG GetCPU_PCR(void)
{
    ULONG result = RETURN_WARN;
    
    if(    ((SysBase->AttnFlags & (1 << AFB_68060)) > 0) ||
        ((SysBase->AttnFlags & (1 << AFB_68080)) > 0))
    {
        ULONG n = v_cpu_pcr();
        
        printf("PCR:  0x%08x (ID: %04x) (REV: %u) (DFP: %s) (ESS: %s)\n", 
            (  n      ),
            (  n >> 16),
            (  n >>  8) & 0xff,
            (( n >>  1) & 1) ? "On" : "Off",
            (( n >>  0) & 1) ? "On" : "Off"
        );
        
        result = RETURN_OK;
    }
    
    return(result);
}

/**********************************************************
 ** 
 ** GetCPU_VBR()
 ** 
 **********************************************************/

ULONG GetCPU_VBR(void)
{
    ULONG result = RETURN_WARN;
    
    if((SysBase->AttnFlags & (1 << AFB_68010)) > 0)
    {
        ULONG n = v_cpu_vbr();
        
        printf("VBR:  0x%08x (Vector base is located in %s)\n", 
            n, n ? "FAST Ram" : "CHIP Ram");
        
        result = RETURN_OK;
    }
    
    return(result);
}

/**********************************************************
 ** 
 ** GetCPU_FPU()
 ** 
 **********************************************************/

ULONG GetCPU_FPU(void)
{
    printf("FPU:  Is %sworking.\n", v_fpu_isok() ? "" : "not ");
    return(RETURN_OK);
}

/**********************************************************
 ** 
 ** Detect080()
 ** 
 **********************************************************/

ULONG Detect080(void)
{
    ULONG result = RETURN_WARN;
    
    if(v_cpu_is080())
    {
        result = RETURN_OK;
    }
    
    return(result);
}

/**********************************************************
 ** 
 ** GetCPU()
 ** 
 **********************************************************/

ULONG GetCPU(void)
{
    printf("Processor information:\n\n");
    
    GetCPU_CPU();
    GetCPU_FPU();
    GetCPU_PCR();
    GetCPU_VBR();
    GetCPU_TCR();
    GetCPU_CACR();
    GetAttnFlags();
    printf("\n");
    
    return(RETURN_OK);
}

/**********************************************************
 ** 
 ** MoveVBR()
 ** 
 **********************************************************/

ULONG MoveVBR(ULONG mode)
{
    ULONG result = RETURN_WARN;
    
    if((SysBase->AttnFlags & (1 << AFB_68010)) > 0)
    {
        if(mode)
        {
            v_cpu_vbr_on(); 
        }
        else
        {
            v_cpu_vbr_off();
        }
        
        result = RETURN_OK;
    }
    
    return(result);
}

/**********************************************************
 ** 
 ** EnumMemList()
 ** 
 **********************************************************/

ULONG EnumMemList(void)
{
    struct MemHeader * mh;
    UBYTE memsize[MAXFILESIZE];
    
    printf("Memory information:\n\n");
    printf("%-11s%-18s%-4s%-10s%-10s%-6s\n", 
        "Address",
        "Name",
        "Pri",
        "Lower",
        "Upper",
        "Attrs");
    
    for(
        mh = (struct MemHeader *)((struct List *)(&SysBase->MemList))->lh_Head;
        ((struct Node *)mh)->ln_Succ;
        mh = (struct MemHeader *)((struct Node *)mh)->ln_Succ
    )
    {
        PrintSize((ULONG)mh->mh_Upper - (ULONG)mh->mh_Lower, memsize);
        
        printf("$%08lx: %-16s %4ld $%08lx $%08lx $%04lx (%s)\n", 
            mh,
            mh->mh_Node.ln_Name,
            mh->mh_Node.ln_Pri,
            mh->mh_Lower,
            (ULONG)mh->mh_Upper - 1,
            mh->mh_Attributes,
            memsize);
    }
    
    printf("\n");
    
    return(RETURN_OK);
}

/**********************************************************
 ** 
 ** EnumModules()
 ** 
 **********************************************************/

void _findPort(Module *m)
{
    APTR res;
    
    if(res = FindPort(m->name))
    {
        m->addr = res;
        m->ver  = 0L;
        m->rev  = 0L;
    }
}

void _findRes(Module *m)
{
    struct Resident * res;
    
    if(res = FindResident(m->name))
    {
        m->addr = res;
        m->ver  = res->rt_Version;
        m->rev  = 0;
    }
}

void _findLib(Module *m, struct List *list)
{
    struct Node * node;
    
    if(node = FindName(list, m->name))
    {
        struct Library *lib = (struct Library *)node;
        
        m->addr = lib;
        m->ver  = lib->lib_Version;
        m->rev  = lib->lib_Revision;
    }
}

ULONG EnumModules(void)
{
    int i;
    
    printf("Modules information:\n\n");
    
    printf("%-11s%-25s%-8s\n", 
        "Address", 
        "Name", 
        "Version");
    
    for(i = 0; i < MAXMODULES; i++)
    {
        Module m = Modules[i];
        
        switch(m.type)
        {
            case MODULE_DEVICE:
                _findLib(&m, &SysBase->DeviceList);
                break;
                
            case MODULE_LIBRARY:
                _findLib(&m, &SysBase->LibList);
                break;
                
            case MODULE_RESOURCE:
                _findLib(&m, &SysBase->ResourceList);
                break;
                
            case MODULE_RESIDENT:
                _findRes(&m);
                break;
                
            case MODULE_MSGPORT:
                _findPort(&m);
                break;
        }
        
        if(m.addr)
        {
            printf("$%08lx: %-24s %02ld.%02ld\n", 
                m.addr, 
                m.name, 
                m.ver, 
                m.rev);
        }
        else
        {
            printf("$%08lx: %-19s NOT LOADED!\n", 
                0, 
                m.name);
        }
    }
    
    printf("\n");
    
    return(RETURN_OK);
}

/**********************************************************
 ** 
 ** EnumConfigDevs()
 ** 
 **********************************************************/

ULONG EnumConfigDevs(void)
{
    printf("Expansion information:\n\n");
    
    printf("%s    %s %s %s %s %s %s\n", 
        "Address",
        "BoardAddr",
        "BoardSize",
        "Type",
        "Manufacturer",
        "Product",
        "Description");
    
    if(ExpansionBase = (struct ExpansionBase *)OpenLibrary(EXPANSIONNAME, 0L))
    {
        struct ConfigDev* cd = NULL;
        
        while(cd = FindConfigDev(cd, -1, -1))
        {
            UBYTE boardDesc[256];
            UBYTE boardSize[MAXFILESIZE];
            
            PrintSize(cd->cd_BoardSize, boardSize);
            
            if(cd->cd_Rom.er_Manufacturer == 5016 && 
               cd->cd_Rom.er_Product < MAXBOARDID)
            {
                sprintf(boardDesc, "%s @ %s", 
                    BoardName[cd->cd_Rom.er_Product][BOARD_DESIGNER],
                    BoardName[cd->cd_Rom.er_Product][BOARD_FULLNAME]);
            }
            else
            {
                sprintf(boardDesc, "%s @ %s", 
                    BoardName[0][BOARD_DESIGNER],
                    BoardName[0][BOARD_FULLNAME]);
            }
            
            printf("$%08lx: $%08lx %9s 0x%02x 0x%04x       0x%02x    %s\n", 
                cd, 
                cd->cd_BoardAddr, 
                boardSize, 
                cd->cd_Rom.er_Type,
                cd->cd_Rom.er_Manufacturer,
                cd->cd_Rom.er_Product,
                boardDesc);
        }
        
        CloseLibrary((struct Library *)ExpansionBase);
    }
    
    printf("\n");
    
    return(RETURN_OK);
}

/**********************************************************
 ** 
 ** SetSuperScalar()
 ** 
 **********************************************************/

ULONG SetSuperScalar(ULONG mode)
{
    ULONG result = RETURN_WARN;
    
    if(v_cpu_is080())
    {
        ULONG n = mode ? 
            v_cpu_pcr_ess_on() : 
            v_cpu_pcr_ess_off();
        
        printf("SuperScalar: %s\n", 
            (n & (1 << 0)) ? "Enabled" : "Disabled");
        
        result = RETURN_OK;
    }
    
    return(result);
}

/**********************************************************
 ** 
 ** SetTurtle()
 ** 2 methods, the V2 one, and the V4 one.
 ** 
 **********************************************************/

ULONG SetTurtle(ULONG mode)
{
    ULONG result = RETURN_WARN;
    
    if(v_cpu_is080())
    {
        ULONG boardId = GetBoardID();
        
        if(boardId == VREG_BOARD_V500 || boardId == VREG_BOARD_V600 || boardId == VREG_BOARD_V1200)
        {
            // V2: CACR method
            
            ULONG n = mode ? 
                v_cpu_cacr_icache_off() : 
                v_cpu_cacr_icache_on();
            
            printf("Turtle: %s\n", 
                (n & (1 << 15)) ? "Disabled" : "Enabled");
        }
        else
        {
            // V4: PCR method
            
            switch(mode)
            {
            case 0:
                v_cpu_pcr_etu_off();
                printf("Turtle: Disable\n");
                break;
            case 1:
                v_cpu_pcr_etu_a500();
                printf("Turtle: Enabled (A500 mode)\n");
                break;
            case 2:
                v_cpu_pcr_etu_a1200();
                printf("Turtle: Enabled (A1200 mode)\n");
                break;
            default:
                printf("Turtle: Invalid mode (0: Off, 1: A500, 2: A1200)\n");
                break;
            }
        }
        
        result = RETURN_OK;
    }
    
    return(result);
}

/**********************************************************
 ** 
 ** SetFastIDE()
 ** Not all Vampire models have a IDE port.
 ** For now, only the V2-V600 do not have one.
 ** 
 **********************************************************/

ULONG SetFastIDE(ULONG value)
{
    ULONG result = RETURN_WARN;
    
    if(v_cpu_is080())
    {
        ULONG boardId = GetBoardID();
        
        if(    boardId != VREG_BOARD_V600 )
        {
            if(value == 0)
            {
                *((volatile UWORD*)VREG_FASTIDE) = 0x0000;
                printf("FastIDE mode = 0x0000 (Disabled).\n");
                result = RETURN_OK;
            }
            else if(value == 1)
            {
                *((volatile UWORD*)VREG_FASTIDE) = 0x4000;
                printf("FastIDE mode = 0x4000 (Fast).\n");
                result = RETURN_OK;
            }
            else if(value == 2)
            {
                *((volatile UWORD*)VREG_FASTIDE) = 0x8000;
                printf("FastIDE mode = 0x8000 (Faster).\n");
                result = RETURN_OK;
            }
            else if(value == 3)
            {
                *((volatile UWORD*)VREG_FASTIDE) = 0xC000;
                printf("FastIDE mode = 0xC000 (Fastest).\n");
                result = RETURN_OK;
            }
            else
            {
                printf("Invalid value.\n");
            }
        }
    }
    
    return(result);
}

/**********************************************************
 ** 
 ** SD Clock Divider
 ** All Vampire models have a SDCard port
 ** 
 **********************************************************/

ULONG SetSDClockDivider(ULONG value)
{  
    ULONG result = RETURN_WARN;
    
    if(v_cpu_is080())
    {
        if (value >= 0 && value <= 255)
        {
            *((volatile UBYTE*)VREG_SDCLKDIV) = value;
            printf("SDCard Clock Divider = %ld (Fastest=0, 255=Slowest).\n", value);
            result = RETURN_OK;
        }
        else
        {
            printf("Invalid value.\n");
        }
    }
    
    return(result);
}

/**********************************************************
 ** 
 ** SetFPU()
 ** 
 **********************************************************/

ULONG SetFPU(ULONG mode)
{
    ULONG result = RETURN_WARN;
    
    if(v_cpu_is080())
    {
        if(v_fpu_toggle(mode ? 1 : 2))
        {
            result = RETURN_OK;
        }
    }
    
    return(result);
}

/**********************************************************
 ** 
 ** SetDPMS()
 ** 
 **********************************************************/

ULONG SetDPMS(ULONG mode)
{
    ULONG result = RETURN_WARN;
    
    if(CyberGfxBase = OpenLibrary(CYBERGFXNAME, 0))
    {
        struct TagItem tags[1];
        struct View* view = GfxBase->ActiView;
        struct ViewPort* viewPort = view->ViewPort;

        tags[0].ti_Tag  = SETVC_DPMSLevel;
        tags[0].ti_Data = mode ? DPMS_OFF : DPMS_ON;
        
        CVideoCtrlTagList(viewPort, tags);
        CloseLibrary(CyberGfxBase);
        CyberGfxBase = NULL;
        result = RETURN_OK;
    }
    
    return(result);
}

/**********************************************************
 ** 
 ** SetAkiko()
 ** 
 **********************************************************/

ULONG SetAkiko(void)
{
    ULONG result = RETURN_WARN;
    
    if(GfxBase->LibNode.lib_Version >= 40)
    {
        if(*(volatile UWORD*)0xB80002 == 0xCAFE)
        {
            GfxBase->ChunkyToPlanarPtr = (ULONG*)0xB80038;
            printf("GfxBase->ChunkyToPlanarPtr initialized.\n");
            result = RETURN_OK;
        }
        else
        {
            printf("Akiko Chip not detected !\n");
        }
    }
    else
    {
        printf("Graphics.library V40+ not detected !\n");
    }
    
    return(result);
}

/**********************************************************
 ** 
 ** ForceAttnFlags080()
 ** 
 **********************************************************/

ULONG ForceAttnFlags080(void)
{
    ULONG result = RETURN_OK;
    
    SysBase->AttnFlags |= (
        AFF_68010 |
        AFF_68020 |
        AFF_68030 |
        AFF_68040 |
        AFF_68080 );
    
    printf("AttnFlags: 0x%04x\n", SysBase->AttnFlags);
    
    return(result);
}

/**********************************************************
 ** 
 ** MapROM()
 ** 
 **********************************************************/

#define SIZE256  (1024*(256*1))
#define SIZE512  (1024*(256*2))
#define SIZE1024 (1024*(256*4))

ULONG MapROM(STRPTR filename)
{
    ULONG result = RETURN_WARN;
    
    if(v_cpu_is080())
    {
        BPTR file;
        
        if(file = Open(filename, MODE_OLDFILE))
        {
            struct FileInfoBlock fib;
            
            if(ExamineFH(file, &fib))
            {
                if(fib.fib_Size == SIZE256 || fib.fib_Size == SIZE512 || fib.fib_Size == SIZE1024)
                {
                    APTR buffer;
                    
                    if(buffer = AllocMem(SIZE1024, MEMF_ANY))
                    {
                        if(Read(file, buffer, fib.fib_Size) == fib.fib_Size)
                        {
                            if(fib.fib_Size == SIZE256)
                            {
                                CopyMem(buffer, (APTR)((ULONG)buffer + SIZE256*1), SIZE256);
                                CopyMem(buffer, (APTR)((ULONG)buffer + SIZE256*2), SIZE256);
                                CopyMem(buffer, (APTR)((ULONG)buffer + SIZE256*3), SIZE256);
                            }
                            
                            if(fib.fib_Size == SIZE512)
                            {
                                CopyMem(buffer, (APTR)((ULONG)buffer + SIZE512*1), SIZE512);
                            }
                            
                            Disable();
                            SuperState();
                            v_maprom(buffer);
                            
                        } else printf("Cant read file.\n");
                        
                        FreeMem(buffer, SIZE1024);
                        
                    } else printf("Cant allocate buffer.\n");
                    
                } else printf("File size must be 256KB, 512KB or 1MB.\n");
                
            } else printf("Cant examine file.\n");
            
            Close(file);
            
        } else printf("Cant open file.\n");
        
    } else printf("Wrong hardware.\n");
    
    return(result);
}

/**********************************************************
 ** 
 ** SetEnvVars()
 ** 
 **********************************************************/

ULONG SE_GetBoardId(void) {
    ULONG result = ((*(volatile UWORD*)VREG_BOARD) >> 8);
    return(result);
}

STRPTR SE_GetBoardName(void) {
    ULONG result = ((*(volatile UWORD*)VREG_BOARD) >> 8);
    return(BoardName[result < MAXBOARDID ? result : 0][BOARD_SHORTNAME]);
}

ULONG SE_GetCoreRevision(void) {
    UBYTE buffer[256];
    if(v_read_revisionstring(buffer) && buffer[0] == 'V')
        return((ULONG)atoi(strstr(buffer, "rev ") + 4));
    return(0);
}

ULONG SE_GetCoreMultiplier(void) {
    ULONG result = ((*(volatile UWORD*)VREG_BOARD) & 0xff);
    return(result);
}

ULONG SE_GetCoreFrequency(void) {
    ULONG result = ((*(volatile UWORD *)VREG_BOARD) & 0xff);
    return(result * SysBase->ex_EClockFrequency / 100000);
}

ULONG SetEnvVars(void)
{
    ULONG result = 0;
    
    if(v_cpu_is080())
    {
        UBYTE buf[256];
        
        sprintf(buf, "%ld", SE_GetCoreRevision());
        if(SetVar("VCoreRev", (STRPTR)buf, -1, LV_VAR+GVF_GLOBAL_ONLY))
            result++;
        
        sprintf(buf, "%ld", SE_GetCoreMultiplier());
        if(SetVar("VCoreMult", (STRPTR)buf, -1, LV_VAR+GVF_GLOBAL_ONLY))
            result++;
        
        sprintf(buf, "%ld", SE_GetCoreFrequency());
        if(SetVar("VCoreFreq", (STRPTR)buf, -1, LV_VAR+GVF_GLOBAL_ONLY))
            result++;
        
        sprintf(buf, "%ld", SE_GetBoardId());
        if(SetVar("VBoardID", (STRPTR)buf, -1, LV_VAR+GVF_GLOBAL_ONLY))
            result++;
        
        sprintf(buf, "%s",  SE_GetBoardName());
        if(SetVar("VBoardName", (STRPTR)buf, -1, LV_VAR+GVF_GLOBAL_ONLY))
            result++;
    }
    
    if(result == 5)
    {
        result = RETURN_OK;
    }
    else
    {
        result = RETURN_WARN;
    }
    
    return(result);
}

/**********************************************************
 ** 
 ** PatchLowLevelReadJoyPort(void)
 ** 
 **********************************************************/

ULONG PatchLowLevelReadJoyPort(void)
{
    ULONG result = RETURN_WARN;
    
    if(!IsV2())
    {
        result = v_joyport_init();
    }
    
    return(result);
}

/**********************************************************
 ** 
 ** SetShield(ULONG mode)
 ** 
 **********************************************************/

ULONG SetShield(ULONG mode)
{
    ULONG result = RETURN_WARN;
    
    if(!IsV2())
    {
        ULONG n = mode ? 
            v_cpu_tc_on() : 
            v_cpu_tc_off();
        
        printf("Shield: %s\n", n ? "Enabled" : "Disabled");
        
        result = RETURN_OK;
    }
    
    return(result);
}

/**********************************************************
 ** 
 ** SetZoom(ULONG mode)
 ** 
 **********************************************************/

ULONG SetZoom(ULONG mode)
{
    ULONG result = RETURN_WARN;
    
    if(!IsV2())
    {
        printf("Zoom: ");
        
        switch (mode)
        {
        case 0:
            // NO ZOOM
            *((volatile UWORD*)VREG_SAGACTRL1) = 0x0024;
            printf("%04X (NOZOOM)", mode);
            result = RETURN_OK;
            break;
        case 1:
            // ZOOM1 (NTSC)
            *((volatile UWORD*)VREG_SAGACTRL1) = 0x0024;
            *((volatile UWORD*)VREG_SAGACTRL1) = 0x8004;
            printf("%04X (ZOOM1, NTSC)", mode);
            result = RETURN_OK;
            break;
        case 2:
            // ZOOM2 (NTSC)
            *((volatile UWORD*)VREG_SAGACTRL1) = 0x0024;
            *((volatile UWORD*)VREG_SAGACTRL1) = 0x8020;
            printf("%04X (ZOOM2, NTSC)", mode);
            result = RETURN_OK;
            break;
        case 3:
            // ZOOM3 (PAL)
            *((volatile UWORD*)VREG_SAGACTRL1) = 0x0024;
            *((volatile UWORD*)VREG_SAGACTRL1) = 0x8024;
            printf("%04X (ZOOM3, PAL)", mode);
            result = RETURN_OK;
            break;
        default:
            printf("Invalid mode.");
            break;
        }
    }
    
    return(result);
}

/**********************************************************
 ** 
 ** SetZoomShiftV(ULONG mode)
 ** 
 **********************************************************/

ULONG SetZoomShiftV(ULONG shiftv)
{
    ULONG result = RETURN_WARN;
    UWORD shift = 0; 
    UBYTE shifthor = 0;
    UBYTE shiftver = 0; 
    
    if(!IsV2())
    {
        printf("ZoomShift Vertical Requested: %02X\n", shiftv);
        
        // Read current ZoomShift
        shift = *((volatile UWORD*)VREG_SAGACTRL2);
        printf("ZoomShift Current: %04X\n", shift);        

        // Extract Vertical and Horizontal Zoomshifts
        shifthor = (UBYTE)shift;
        shift = (UWORD)shift >> 8;
        shiftver = (UBYTE)shift;
        printf("ZoomShift Vertical Current: %02X\n", shiftver);
        printf("ZoomShift Horizontal Current: %02X\n", shifthor);
        
        // Test new Vertical value on MAX ($7E)
        if(shiftv>0x7E)
        {
            printf("ZoomShift Vertical %02X is too High (MAX=$7E = 126)\n", shiftver);
            printf("Setting ZoomShift Vertical to Maximum value $7E = 126\n");
            shiftv = 0x7E;
        };

        // Calculate new Zoomshift
        shift = (UBYTE)shiftv;
        shift = (UWORD)shift << 8;
        shift = (UWORD)shift + (UBYTE)shifthor;    
        
        printf("ZoomShift New: %04X\n", shift);  

        // SAGA DISPLAY ROW START
        *((volatile UWORD*)VREG_SAGACTRL2) = (UWORD)shift;
        result = RETURN_OK;
    }
    
    return(result);
}

/**********************************************************
 ** 
 ** SetZoomShiftH(ULONG mode)
 ** 
 **********************************************************/

ULONG SetZoomShiftH(ULONG shifth)
{
    ULONG result = RETURN_WARN;
    UWORD shift = 0; 
    UBYTE shifthor = 0;
    UBYTE shiftver = 0; 
    
    if(!IsV2())
    {
        printf("ZoomShift Horizontal Requested: %02X\n", shifth);
        
        // Read current ZoomShift
        shift = *((volatile UWORD*)VREG_SAGACTRL2);
        printf("ZoomShift Current: %04X\n", shift);        

        // Extract Vertical and Horizontal Zoomshifts
        shifthor = (UBYTE)shift;
        shift = (UWORD)shift >> 8;
        shiftver = (UBYTE)shift;
        printf("ZoomShift Vertical Current: %02X\n", shiftver);
        printf("ZoomShift Horizontal Current: %02X\n", shifthor);
        
        // Test new Horizontal value on MAX ($3E)
        if(shifth>0x3E)
        {
            printf("ZoomShift Horizontal %02X is too High (MAX=$3E = 62)\n", shiftver);
            printf("Setting ZoomShift Horizontal to Maximum value $3E = 62\n");
            shifth = 0x3E;
        };

        // Calculate new Zoomshift
        shift = (UBYTE)shiftver;
        shift = (UWORD)shift << 8;
        shift = (UWORD)shift + (UBYTE)shifth;       
        
        printf("ZoomShift New: %04X\n", shift);  

        // SAGA DISPLAY ROW START
        *((volatile UWORD*)VREG_SAGACTRL2) = (UWORD)shift;
        result = RETURN_OK;
    }
    
    return(result);
}

/**********************************************************
 ** 
 ** SetZoom_OLD(ULONG mode)
 ** 
 **********************************************************/

ULONG SetZoom_OLD(ULONG mode)
{
    ULONG result = RETURN_WARN;
    
    if(!IsV2())
    {
        if (mode == 0)
        {
            // SAGACTRL1 register: CLR Zoom Bit
            *((volatile UWORD*)VREG_SAGACTRL1) = 
                (VREG_SAGACTRL1F_ZOOM);
        }
        else
        {
            // SAGACTRL1 register: SET Zoom Bit
            *((volatile UWORD*)VREG_SAGACTRL1) = 
                (VREG_SAGACTRL1F_ZOOM | VREG_SAGACTRL1F_SETCLR);
        }
        
        printf("Zoom: %s\n", (mode == 0) ? "Disabled" : "Enabled");
        
        result = RETURN_OK;
    }
    
    return(result);
}

/**********************************************************
 ** 
 ** SetAGAMode(ULONG mode)
 ** 
 **********************************************************/

ULONG SetAGAMode(ULONG mode)
{
    ULONG result = RETURN_WARN;
    
    if(!IsV2())
    {
        if (mode == 0)
        {
            // SAGACTRL1 register: CLR AGAMode Bit
            *((volatile UWORD*)VREG_SAGACTRL1) = 
                (VREG_SAGACTRL1F_AGAMODE);
        }
        else
        {
            // SAGACTRL1 register: SET AGAMode Bit
            *((volatile UWORD*)VREG_SAGACTRL1) = 
                (VREG_SAGACTRL1F_AGAMODE | VREG_SAGACTRL1F_SETCLR);
        }
        
        printf("AGAMode: %s\n", (mode == 0) ? "Disabled" : "Enabled");
        
        result = RETURN_OK;
    }
    
    return(result);
}

/**********************************************************
 ** 
 ** SetDisplayMode(ULONG mode)
 ** 
 **********************************************************/

ULONG SetDisplayMode(ULONG mode)
{
    ULONG result = RETURN_WARN;
    
    if(!IsV2())
    {
        if (mode == 0)
        {
            // BEAMCON0 register: CLR PAL Bit
            *((volatile UWORD*)VREG_BEAMCON0) = 
                (VREG_BEAMCON0F_PAL);
        }
        else
        {
            // BEAMCON0 register: SET PAL Bit
            *((volatile UWORD*)VREG_BEAMCON0) = 
                (VREG_BEAMCON0F_PAL | VREG_BEAMCON0F_SETCLR);
        }
        
        printf("DisplayMode: %s\n", (mode == 0) ? "NTSC" : "PAL");
        
        result = RETURN_OK;
    }
    
    return(result);
}

/**********************************************************
 ** 
 ** SetScanlines(ULONG mode)
 ** 
 **********************************************************/

ULONG SetScanlines(ULONG mode)
{
    ULONG result = RETURN_WARN;
    
    if(!IsV2())
    {
        if (mode == 0)
        {
            // SAGACTRL1 register: CLR Scanlines Bit
            *((volatile UWORD*)VREG_SAGACTRL1) = 
                (VREG_SAGACTRL1F_SCANLINES);
        }
        else
        {
            // SAGACTRL1 register: SET Scanlines Bit
            *((volatile UWORD*)VREG_SAGACTRL1) = 
                (VREG_SAGACTRL1F_SCANLINES | VREG_SAGACTRL1F_SETCLR);
        }
        
        printf("Scanlines: %s\n", (mode == 0) ? "Disabled" : "Enabled");
        
        result = RETURN_OK;
    }
    
    return(result);
}

/**********************************************************
 ** 
 ** Entry point
 ** 
 **********************************************************/

ULONG main(ULONG argc, char *argv[])
{
    int result = 0;
    LONG opts[OPT_COUNT];
    struct RDArgs *rdargs;
    
    if(DOSBase)
    {
        if(argc < 2)
        {
            result = Help();
        }
        else
        {
            memset((char *)opts, 0, sizeof(opts));

            if(rdargs = (struct RDArgs *)ReadArgs(TEMPLATE, opts, NULL))
            {
                if(opts[OPT_HELP])
                    result = Help();
                
                if(opts[OPT_JOYPORT])
                    result = PatchLowLevelReadJoyPort();
                
                if(opts[OPT_SETENV])
                    result = SetEnvVars();
                
                if(opts[OPT_AKIKO])
                    result = SetAkiko();
                
                if(opts[OPT_ATTNFLAGS])
                    result = ForceAttnFlags080();
                
                if(opts[OPT_BOARD])
                    result = GetBoard();
                
                if(opts[OPT_BOARDID])
                    result = GetBoardID();
                
                if(opts[OPT_BOARDNAME])
                    result = GetBoardName();
                
                if(opts[OPT_BOARDSERIAL])
                    result = GetBoardSerialNumber();
                
                if(opts[OPT_COREREVNUMBER])
                    result = GetCoreRevisionNumber();
                
                if(opts[OPT_COREREVSTRING])
                    result = GetCoreRevisionString();
                
                if(opts[OPT_DETECT])
                    result = Detect080();
                
                if(opts[OPT_DISPLAYMODE])
                    result = SetDisplayMode(*(LONG *)opts[OPT_DISPLAYMODE]);
                
                if(opts[OPT_DPMS])
                    result = SetDPMS(*(LONG *)opts[OPT_DPMS]);
                
                if(opts[OPT_FPU])
                    result = SetFPU(*(LONG *)opts[OPT_FPU]);
                
                if(opts[OPT_IDESPEED])
                    result = SetFastIDE(*(LONG *)opts[OPT_IDESPEED]);
                
                if(opts[OPT_SCANLINES])
                    result = SetScanlines(*(LONG *)opts[OPT_SCANLINES]);
                
                if(opts[OPT_SDCLOCKDIV])
                    result = SetSDClockDivider(*(LONG *)opts[OPT_SDCLOCKDIV]);
                
                if(opts[OPT_SHIELD])
                    result = SetShield(*(LONG *)opts[OPT_SHIELD]);
                
                if(opts[OPT_SUPERSCALAR])
                    result = SetSuperScalar(*(LONG *)opts[OPT_SUPERSCALAR]);
                
                if(opts[OPT_TURTLE])
                    result = SetTurtle(*(LONG *)opts[OPT_TURTLE]);
                
                if(opts[OPT_ZOOM])
                    result = SetZoom(*(LONG *)opts[OPT_ZOOM]);
                
                if(opts[OPT_ZOOMSHIFT_V])
                    result = SetZoomShiftV(*(LONG *)opts[OPT_ZOOMSHIFT_V]);
                
                if(opts[OPT_ZOOMSHIFT_H])
                    result = SetZoomShiftH(*(LONG *)opts[OPT_ZOOMSHIFT_H]);
                
                if(opts[OPT_VBRMOVE])
                    result = MoveVBR(*(LONG *)opts[ OPT_VBRMOVE]);
                
                if(opts[OPT_CPU])
                    result = GetCPU();
                
                if(opts[OPT_CPUHERTZ])
                    result = GetBoardFreqSW();
                
                if(opts[OPT_MEMLIST])
                    result = EnumMemList();
                
                if(opts[OPT_MODULES])
                    result = EnumModules();
                
                if(opts[OPT_CONFIGDEV])
                    result = EnumConfigDevs();
                
                if(opts[OPT_MAPROM])
                    result = MapROM((char *)opts[OPT_MAPROM]);
                
                FreeArgs(rdargs);
            }
            else
            {
                printf("\nInvalid arguments.\n\n");
                result = Help();
            }
        }
    }
    
    return(result);
}

/**********************************************************
 **
 ** END
 **
 **********************************************************/
