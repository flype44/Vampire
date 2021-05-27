/******************************************************************************
**
** SAGACard_LibInit.h
** Picasso96/SAGA.card
** 
******************************************************************************/

#ifndef SAGACARD_LIBINIT_H
#define SAGACARD_LIBINIT_H

/*****************************************************************************/

ASM SAVEDS struct Library * LibInit(
    __REGD0(struct Library *LibBase),
    __REGA0(BPTR Segment),
    __REGA6(struct ExecBase *ExecBase));

ASM SAVEDS struct Library * LibOpen( 
    __REGA6(struct Library *LibBase));

ASM SAVEDS BPTR LibExpunge(
    __REGA6(struct Library *LibBase));

ASM SAVEDS BPTR LibClose(
    __REGA6(struct Library *LibBase));

/*****************************************************************************/

#endif
