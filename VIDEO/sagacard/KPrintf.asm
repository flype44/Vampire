;******************************************************************************
;*
;* KPrintF.asm
;* Picasso96/SAGA.card
;* 
;******************************************************************************

    XDEF _KPrintf

_LVORawDoFmt       EQU -$20a
_LVORawPutChar     EQU -$204
_LVORawIOInit      EQU -$1f8
_LVORawMayGetChar  EQU -$1fe

_KPrintf:
    movem.l  d0-d2/a0-a3/a6,-(sp)
    move.l   36(sp),a0
    lea.l    40(sp),a1
    move.l   4.w,a6
    jsr      _LVORawIOInit(a6)
    lea.l    .kprintfproc(pc),a2
    suba.l   a3,a3
    jsr      _LVORawDoFmt(a6)	
    movem.l  (sp)+,d0-d2/a0-a3/a6
    rts
.kprintfproc:	
    movem.l  d0/d1/a0/a1/a6,-(sp)
    move.l   4.w,a6
    jsr      _LVORawPutChar(a6)
    movem.l  (sp)+,d0/d1/a0/a1/a6
    rts
