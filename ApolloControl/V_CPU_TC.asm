;----------------------------------------------------------

	XDEF    _v_cpu_tc
	XDEF    _v_cpu_tc_on
	XDEF    _v_cpu_tc_off

;----------------------------------------------------------

	include	 exec/funcdef.i
	include	 exec/exec.i
	include	 exec/exec_lib.i
	
;----------------------------------------------------------

	machine  mc68040

;----------------------------------------------------------

	cnop 0,4

_v_cpu_tc:
	movem.l  a5/a6,-(sp)
	move.l   $4.w,a6
	lea.l    .supv(pc),a5
	jsr      _LVOSupervisor(a6)
	movem.l  (sp)+,a5/a6
	rts
.supv:
	movec    tc,d0
	rte

;----------------------------------------------------------
	
	cnop 0,4

_v_cpu_tc_on:
	movem.l  a5/a6,-(sp)
	move.l   $4.w,a6
	lea.l    .supv(pc),a5
	jsr      _LVOSupervisor(a6)
	movem.l  (sp)+,a5/a6
	rts
.supv:
	clr.l    d0
	movec    tc,d0
	bset     #0,d0 ; TCR(0) enables WRITE   protection
	bset     #1,d0 ; TCR(1) enables READ    protection
	bset     #2,d0 ; TCR(2) enables EXECUTE protection
	bset     #3,d0 ; TCR(3) enables NULLPTR protection
	movec    d0,tc
	movec    tc,d0
	rte

;----------------------------------------------------------

	cnop 0,4

_v_cpu_tc_off:
	movem.l  a5/a6,-(sp)
	move.l   $4.w,a6
	lea.l    .supv(pc),a5
	jsr      _LVOSupervisor(a6)
	movem.l  (sp)+,a5/a6
	rts
.supv:
	clr.l    d0
	movec    tc,d0
	bclr     #0,d0 ; TCR(0) disables WRITE   protection
	bclr     #1,d0 ; TCR(1) disables WRITE   protection
	bclr     #2,d0 ; TCR(2) disables EXECUTE protection
	bclr     #3,d0 ; TCR(3) disables NULLPTR protection
	movec    d0,tc
	movec    tc,d0
	rte

;----------------------------------------------------------

	cnop 0,4

