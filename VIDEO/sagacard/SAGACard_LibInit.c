/******************************************************************************
**
** SAGACard_LibInit.c
** Picasso96/SAGA.card
** 
******************************************************************************/

#include <dos/dostags.h>
#include <exec/lists.h>
#include <exec/initializers.h>
#include <exec/execbase.h>
#include <utility/tagitem.h>

#include <proto/dos.h>
#include <proto/exec.h>
#include <proto/utility.h>

/*****************************************************************************/

#ifndef boardinfo_H
#include "boardinfo.h"
#endif

#ifndef SAGACARD_H
#include "SAGACard.h"
#endif

#ifndef SAGACARD_LIBINITCARD_H
#include "SAGACard_LibInit.h"
#endif

#ifndef SAGACARD_FINDCARD_H
#include "SAGACard_FindCard.h"
#endif

#ifndef SAGACARD_INITCARD_H
#include "SAGACard_InitCard.h"
#endif

#ifndef SAGACARD_VERSION_H
#include "SAGACard_Version.h"
#endif

/*****************************************************************************/

struct SAGABase
{
    struct Library         LibNode;
    struct ExecBase       *ExecBase;
    BPTR                   LibSegment;
    struct SignalSemaphore LockSemaphore;
};

#define BASENAME SAGABase
#define BASETYPE struct BASENAME 

/*****************************************************************************/

#define xstr(a) str(a)
#define str(a) #a

/*****************************************************************************/

static const unsigned short hdr[] = { 0x2456, 0x4552, 0x3a20 }; /* "$VER: " */

const char *LibVersionString = xstr(LIBID) " " xstr(LIBVERSION) "." xstr(LIBREVISION) xstr(LIBEXTRA) " (" xstr(LIBDATE) ")\r\n";

const char LibName[] = xstr(LIBID);

/*****************************************************************************/

extern ASM LONG LibNull(void);

extern struct ExecBase *SysBase;

BASETYPE *BASENAME = NULL;

/*****************************************************************************/

const APTR LibFunctions[] =
{
    (APTR) LibOpen,
    (APTR) LibClose,
    (APTR) LibExpunge,
    (APTR) LibNull,
    (APTR) &FindCard,
    (APTR) &InitCard,
    (APTR) -1
};

#define WORDINIT(_a_) UWORD _a_ ##W1; UWORD _a_ ##W2; UWORD _a_ ##ARG;
#define LONGINIT(_a_) UBYTE _a_ ##A1; UBYTE _a_ ##A2; ULONG _a_ ##ARG;

struct LibInitData
{
    WORDINIT(w1) 
    LONGINIT(l1)
    WORDINIT(w2) 
    WORDINIT(w3) 
    WORDINIT(w4) 
    LONGINIT(l2)
    ULONG end_initlist;
} LibInitializers =
{
    INITBYTE(OFFSET(Node, ln_Type), NT_LIBRARY),
    0x80, (UBYTE)((LONG)OFFSET(Node, ln_Name)), (ULONG)&LibName[0],
    INITBYTE(OFFSET(Library, lib_Flags), LIBF_SUMUSED|LIBF_CHANGED),
    INITWORD(OFFSET(Library, lib_Version), LIBVERSION),
    INITWORD(OFFSET(Library, lib_Revision), LIBREVISION),
    0x80, (UBYTE)((LONG)OFFSET(Library, lib_IdString)), (ULONG)&LibVersionString,
    (ULONG)0
};

const APTR LibInitTab[] = {
    (APTR) sizeof(BASETYPE),
    (APTR) &LibFunctions,
    (APTR) &LibInitializers,
    (APTR) LibInit
};

/*****************************************************************************/

ASM SAVEDS struct Library * LibInit(
    __REGD0(struct Library *LibBase),
    __REGA0(BPTR Segment),
    __REGA6(struct ExecBase *ExecBase))
{
    SysBase = ExecBase;
    BASENAME = (BASETYPE *)LibBase;
    
    if ((ExecBase->LibNode.lib_Version < 37) || !(ExecBase->AttnFlags & AFF_68020))
    {
    	return(NULL);
    }
    
    BASENAME->LibSegment = Segment;
    BASENAME->ExecBase = ExecBase;
    BASENAME->LibNode.lib_IdString = (char*)LibVersionString;
    
    InitSemaphore(&BASENAME->LockSemaphore);
    
    return(LibBase);
}

/*****************************************************************************/

ASM SAVEDS struct Library * LibOpen(
    __REGA6(struct Library *LibBase))
{
    ObtainSemaphore(&BASENAME->LockSemaphore);
    
    BASENAME->LibNode.lib_Flags &= ~LIBF_DELEXP;
    BASENAME->LibNode.lib_OpenCnt++;
    
    ReleaseSemaphore(&BASENAME->LockSemaphore);
    
    return(LibBase);
}

/*****************************************************************************/

ASM SAVEDS BPTR LibExpunge(
    __REGA6(struct Library *LibBase))
{
    if ((BASENAME->LibNode.lib_OpenCnt == 0) && (BASENAME->LibSegment != 0))
    {
        BPTR TempSegment = BASENAME->LibSegment;
        Remove((struct Node *)BASENAME);
        FreeMem((APTR)((ULONG)(BASENAME) - (ULONG)(BASENAME->LibNode.lib_NegSize)),
        BASENAME->LibNode.lib_NegSize + BASENAME->LibNode.lib_PosSize);
        BASENAME = NULL;
        
        return(TempSegment);
    }
    else
    {
        BASENAME->LibNode.lib_Flags |= LIBF_DELEXP;
    }
    
    return(0);
}

/*****************************************************************************/

ASM SAVEDS BPTR LibClose(
    __REGA6(struct Library *LibBase))
{
    ObtainSemaphore(&BASENAME->LockSemaphore);
    
    if (BASENAME->LibNode.lib_OpenCnt > 0)
    {
        BASENAME->LibNode.lib_OpenCnt--;
    }
    
    ReleaseSemaphore(&BASENAME->LockSemaphore);
    
    if ((BASENAME->LibNode.lib_OpenCnt == 0) && (BASENAME->LibNode.lib_Flags & LIBF_DELEXP))
    {
        return(LibExpunge(LibBase));
    }
    
    return(0);
}

/*****************************************************************************/
