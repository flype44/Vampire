/******************************************************************************
**
** SAGACard_CardMem.c
** Picasso96/SAGA.card
** 
******************************************************************************/

#include <exec/exec.h>
#include <proto/alib.h>
#include <proto/exec.h>

/*****************************************************************************/

#ifndef boardinfo_H
#include "boardinfo.h"
#endif

#ifndef SAGACARD_H
#include "SAGACard.h"
#endif

/*****************************************************************************/

#define	D(x) x

/*****************************************************************************/

#define FOREACH(TYPE, LIST, NODE) \
    for (NODE = (TYPE)((struct List *)(LIST))->lh_Head; \
        ((struct Node *)NODE)->ln_Succ; \
        NODE = (TYPE)((struct Node *)(NODE))->ln_Succ)

/*****************************************************************************/

struct CardMemChunk
{
    struct MinNode    cmc_Node;        /* MinNode */
    struct BoardInfo *cmc_BoardInfo;   /* Address of the BoardInfo (Safety-check) */
    APTR              cmc_MemBase;     /* Address of the allocated memory */
    APTR              cmc_MemAddr;     /* Address of the aligned memory */
    ULONG             cmc_MemSize;     /* Size in bytes */
};

/*****************************************************************************/

APTR SAVEDS ASM Card_AllocCardMem(
    __REGA0(struct BoardInfo *bi),
    __REGD0(ULONG             size),
    __REGD1(BOOL              force),
    __REGD2(BOOL              system))
{
    APTR  MemBase;
    APTR  MemAddr  = NULL;
    ULONG MemFlags = (MEMF_FAST | MEMF_LOCAL | MEMF_PUBLIC);
    
    D(KPrintf("Card_AllocCardMem(size=%ld)\n", size));
    
    /* Allocate the memory + safety margin */
    
    size += (32 + (1280 * 4));
    
    if (MemBase = (APTR)AllocMem(size, MemFlags))
    {
        struct CardMemChunk *chunk;
        
        /* Align the allocated memory */
        
        MemAddr = (APTR)(((ULONG)MemBase + 31) & ~31);
        
        /* Create a new CardMem-Chunk */
        
        if (chunk = AllocVec(sizeof(struct CardMemChunk), MemFlags | MEMF_CLEAR))
        {
            chunk->cmc_BoardInfo = bi;
            chunk->cmc_MemBase   = MemBase;
            chunk->cmc_MemAddr   = MemAddr;
            chunk->cmc_MemSize   = size;
            
            /* If CardMem-List is NULL, create it */
            
            if (!bi->CardData[CARDDATA_MEMLIST])
            {
                struct MinList *list;
                
                if (list = AllocVec(sizeof(struct MinList), MemFlags | MEMF_CLEAR))
                {
                    NewList((struct List *)list); // in amiga_lib
                    bi->CardData[CARDDATA_MEMLIST] = (ULONG)list;
                }
            }
            
            /* Add the CardMem-Chunk to the CardMem-List */
            
            if (bi->CardData[CARDDATA_MEMLIST])
            {
                D(KPrintf("Allocated: 0x%08lx.\n", MemAddr));
                AddTail((struct List *)bi->CardData[CARDDATA_MEMLIST], 
                        (struct Node *)chunk);
            }
        }
    }
    
    return(MemAddr);
}

/*****************************************************************************/

BOOL SAVEDS ASM Card_FreeCardMem(
    __REGA0(struct BoardInfo *bi),
    __REGA1(APTR              mem))
{  
    BOOL result = FALSE;
    
    struct MinList *list;
    
    D(KPrintf("Card_FreeCardMem(mem=0x%08lx)\n", mem));
    
    /* Search the CardMem-Chunk in the CardMem-List */
    
    if (mem && (list = (struct MinList *)bi->CardData[CARDDATA_MEMLIST]))
    {
        struct CardMemChunk *ck;
        struct CardMemChunk *chunk = NULL;
        
        FOREACH(struct CardMemChunk *, list, ck)
        {
            if ((ck->cmc_MemAddr == mem) && (ck->cmc_BoardInfo == bi))
            {
                /* CardMem-Chunk found */
                
                chunk = ck;
                break;
            }
        }
        
        if (chunk)
        {
            /* Release memory */
            
            D(KPrintf("Released.\n"));
            FreeMem(chunk->cmc_MemBase, ck->cmc_MemSize);
            Remove((struct Node *)chunk);
            FreeVec(chunk);
            result = TRUE;
        }
    }
    
    return(result);
}

/*****************************************************************************/
