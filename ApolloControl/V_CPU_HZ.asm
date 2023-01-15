;------------------------------------------------------------------------------
; File:    CPUMHz
; Author:  Philippe CARPENTIER
; Version: 1.0 (2021-10-02)
; Compile:
; c:vasmm68k_mot_os3 -m68080 -Fhunkexe -IWork:Programming/DevPac/Include V_CPU_HZ.asm
;------------------------------------------------------------------------------

	INCLUDE 'exec/funcdef.i'
	INCLUDE 'exec/exec.i'
	INCLUDE 'exec/exec_lib.i'
	INCLUDE 'dos/dos.i'
	INCLUDE 'dos/dos_lib.i'
	INCLUDE 'devices/timer.i'

;------------------------------------------------------------------------------

    RSRESET
fmt_Model    rs.l 1
fmt_Hertz    rs.l 1
fmt_Type     rs.l 1
fmt_SIZE     rs.l 0

    RSRESET
sid_MsgPort  rs.l 1
sid_Running  rs.l 1
sid_SIZE     rs.l 0

;------------------------------------------------------------------------------

MAIN:
	lea.l    DOSName(pc),a1                 ; Library name
	moveq.l  #0,d0                          ; Library version
	CALLEXEC OpenLibrary                    ; OpenLibrary()
	tst.l    d0                             ; Null ?
	beq.s    .l0                            ; Skip
	move.l   d0,_DOSBase                    ; DOSBase
	
	lea.l    ResultArgs(pc),a0              ; ResultArgs
	move.l   #0,fmt_Type(a0)                ; ResultArgs->Type = CPU
	bsr      GetCPUModel                    ; Obtain CPU model
	bsr      GetFrequency                   ; Obtain CPU frequency
	lea.l    ResultString1(pc),a0           ; fmt
	move.l   a0,d1                          ; fmt
	lea.l    ResultArgs(pc),a0              ; argv
	move.l   a0,d2                          ; argv
	CALLDOS  VPrintf                        ; VPrintf()
	
	lea.l    ResultArgs(pc),a0              ; ResultArgs
	move.l   #1,fmt_Type(a0)                ; ResultArgs->Type = FPU
	bsr      GetFPUModel                    ; Obtain FPU model
	lea.l    ResultArgs(pc),a0              ; argv
	tst.l    fmt_Model(a0)                  ; Has FPU ?
	beq.l    .l1                            ; Skip
	bsr      GetFrequency                   ; Obtain FPU frequency
	lea.l    ResultString2(pc),a0           ; fmt
	move.l   a0,d1                          ; fmt
	lea.l    ResultArgs(pc),a0              ; argv
	move.l   a0,d2                          ; argv
	CALLDOS  VPrintf                        ; VPrintf()
.l1
	move.l   _DOSBase(pc),a1                ; DOSBase
	CALLEXEC CloseLibrary                   ; CloseLibrary()
.l0
	moveq.l  #0,d0                          ; Return code
	rts                                     ; Exit

;------------------------------------------------------------------------------

	CNOP 0,4

GetCPUModel:
	movem.l  d2-d7/a2-a6,-(sp)              ; Push
	move.l   $4.w,a0                        ; ExecBase
	move.w   AttnFlags(a0),d0               ; ExecBase->AttnFlags
	lea.l    CPUModel(pc),a0                ; Array
.l8	btst     #10,d0                         ; AFB_FPGA
	beq.s    .l6                            ; Skip
	adda.l   #(6*8),a0                      ; Array[6]
	bra.s    .l0                            ; Exit
.l6	btst     #7,d0                          ; AFB_68060
	beq.s    .l4                            ; Skip
	adda.l   #(5*8),a0                      ; Array[5]
	bra.s    .l0                            ; Exit
.l4	btst     #3,d0                          ; AFB_68040
	beq.s    .l3                            ; Skip
	adda.l   #(4*8),a0                      ; Array[4]
	bra.s    .l0                            ; Exit
.l3	btst     #2,d0                          ; AFB_68030
	beq.s    .l2                            ; Skip
	adda.l   #(3*8),a0                      ; Array[3]
	bra.s    .l0                            ; Exit
.l2	btst     #1,d0                          ; AFB_68020
	beq.s    .l1                            ; Skip
	adda.l   #(2*8),a0                      ; Array[2]
	bra.s    .l0                            ; Exit
.l1	btst     #0,d0                          ; AFB_68010
	beq.s    .l0                            ; Skip
	adda.l   #(1*8),a0                      ; Array[1]
.l0	lea.l    ResultArgs(pc),a1              ; ResultArgs
	move.l   a0,fmt_Model(a1)               ; ResultArgs->Model
	movem.l  (sp)+,d2-d7/a2-a6              ; Pop
	rts                                     ; Return

;------------------------------------------------------------------------------

	CNOP 0,4

GetFPUModel:
	movem.l  d2-d7/a2-a6,-(sp)              ; Push
	move.l   $4.w,a0                        ; ExecBase
	move.w   AttnFlags(a0),d0               ; ExecBase->AttnFlags
	lea.l    FPUModel(pc),a0                ; Array
.l6	btst     #6,d0                          ; AFB_FPU040
	beq.s    .l5                            ; Skip
	adda.l   #(2*8),a0                      ; Array[3]
	bra.s    .l0                            ; Exit
.l5	btst     #5,d0                          ; AFB_68882
	beq.s    .l4                            ; Skip
	adda.l   #(1*8),a0                      ; Array[2]
	bra.s    .l0                            ; Exit
.l4	btst     #4,d0                          ; AFB_68881
	beq.s    .l0                            ; Skip
	adda.l   #(0*8),a0                      ; Array[1]
	bra.s    .l0                            ; Exit
.l1	suba.l   a0,a0                          ; No FPU
.l0	lea.l    ResultArgs(pc),a1              ; ResultArgs
	move.l   a0,fmt_Model(a1)               ; ResultArgs->Model
	movem.l  (sp)+,d2-d7/a2-a6              ; Pop
	rts                                     ; Return

;------------------------------------------------------------------------------

	CNOP 0,4

GetCPUFrequency:
	lea.l    SoftIntData(pc),a0             ; SoftIntData
	moveq.l  #0,d0                          ; Counter = 0
.l0	move.l   #10000,d1                      ; Loops
.l1     addq.l   #1,d0                          ; Counter + 1
	dbf      d1,.l1                         ; Continue
	tst.l    sid_Running(a0)                ; SoftIntData->Running ?
	bne.s    .l0                            ; Continue
	rts                                     ; Return

;------------------------------------------------------------------------------

	CNOP 0,4

GetFPUFrequency:
	lea.l    SoftIntData(pc),a0             ; SoftIntData
	moveq.l  #0,d0                          ; Counter = 0
	fmove.l  d0,fp0                         ; Counter = 0
	moveq.l  #1,d0                          ; Increment = 1
	fmove.l  d0,fp1                         ; Increment = 1
.l0	move.l   #10000,d0                      ; Loops
.l1     fadd.x   fp1,fp0                        ; Counter + 1
	dbf      d0,.l1                         ; Continue
	tst.l    sid_Running(a0)                ; SoftIntData->Running ?
	bne.s    .l0                            ; Continue
	fmove.l  fp0,d0                         ; To integer
	rts                                     ; Return

;------------------------------------------------------------------------------

	CNOP 0,4

GetFrequency:
	movem.l  d2-d7/a2-a6,-(sp)              ; Push
	lea.l    MySoftInt(pc),a0               ; SoftInt
	lea.l    SoftIntCode(pc),a1             ; SoftIntCode
	move.l   a1,IS_CODE(a0)                 ; SoftIntCode
	lea.l    SoftIntData(pc),a1             ; SoftIntData
	move.l   a1,IS_DATA(a0)                 ; SoftIntData
	move.b   #32,LN_PRI(a0)                 ; Priority
	CALLEXEC CreateMsgPort                  ; CreateMsgPort()
	tst.l    d0                             ; Null ?
	beq.w    .l0                            ; Skip
	
	lea.l    SoftIntData(pc),a0             ; SoftIntData
	lea.l    MySoftInt(pc),a1               ; SoftInt
	move.l   d0,sid_MsgPort(a0)             ; MsgPort
	move.l   d0,a0                          ; MsgPort
	move.b   #NT_MSGPORT,LN_TYPE(a0)        ; MsgPort->Type
	move.b   #PA_SOFTINT,MP_FLAGS(a0)       ; MsgPort->Flags
	move.l   a1,MP_SIGTASK(a0)              ; MsgPort->SigTask
	
	lea.l    MyTimeReq(pc),a0               ; IOTV
	move.b   #NT_REPLYMSG,LN_TYPE(a0)       ; IOTV->Type
	move.w   #IOTV_SIZE,MN_LENGTH(a0)       ; IOTV->Length
	move.l   d0,MN_REPLYPORT(a0)            ; IOTV->ReplyPort
	
	lea.l    TimerName(pc),a0               ; Device name
	moveq.l  #UNIT_MICROHZ,d0               ; Device unit
	lea.l    MyTimeReq(pc),a1               ; Device iorequest
	moveq.l  #0,d1                          ; Device flags
	CALLEXEC OpenDevice                     ; OpenDevice()
	tst.l    d0                             ; Error ?
	bne.s    .l1                            ; Skip
	
	lea.l    MyTimeReq(pc),a1               ; IOTV
	move.w   #TR_ADDREQUEST,IO_COMMAND(a1)  ; IOTV->Command
	move.l   #1,IOTV_TIME+TV_SECS(a1)       ; IOTV->Secs
	move.l   #0,IOTV_TIME+TV_MICRO(a1)      ; IOTV->Micro
	LINKLIB  DEV_BEGINIO,IO_DEVICE(a1)      ; IOTV->BeginIO()
	lea.l    SoftIntData(pc),a0             ; SoftIntData
	move.l   #1,sid_Running(a0)             ; SoftIntData->Running
	
	CALLEXEC Forbid                         ; Forbid()
	lea.l    ResultArgs(pc),a0              ; ResultArgs
	tst.l    fmt_Type(a0)                   ; ResultArgs->Type
	bne.s    .l3                            ; Skip
	bsr      GetCPUFrequency                ; Obtain CPU frequency
	bra.s    .l2                            ; Skip
.l3	bsr      GetFPUFrequency                ; Obtain FPU frequency
.l2	divu.l   #10000,d0                      ; Counter /    10 000
	mulu.l   #10180,d0                      ; Counter *    10 180
	divu.l   #1000000,d0                    ; Counter / 1 000 000
	lea.l    ResultArgs(pc),a0              ; ResultArgs
	move.l   d0,fmt_Hertz(a0)               ; ResultArgs->Hertz
	CALLEXEC Permit                         ; Permit()
	
	lea.l    MyTimeReq(pc),a1               ; Device
	CALLEXEC CloseDevice                    ; CloseDevice(dev)
.l1
	lea.l    SoftIntData(pc),a0             ; SoftIntData
	move.l   sid_MsgPort(a0),a0             ; SoftIntData->MsgPort
	CALLEXEC DeleteMsgPort                  ; DeleteMsgPort()
.l0
	movem.l  (sp)+,d2-d7/a2-a6              ; Pop
	rts                                     ; Return

;------------------------------------------------------------------------------

	CNOP 0,4

SoftIntCode:
	move.l   a4,-(sp)                       ; Push
	move.l   a1,a4                          ; SoftIntData
	move.l   sid_MsgPort(a1),a0             ; MsgPort
	CALLEXEC GetMsg                         ; GetMsg()
	move.l   #0,sid_Running(a4)             ; SoftIntData->Running
	move.l   (sp)+,a4                       ; Pop
	moveq.l  #0,d0                          ; Return code
	rts                                     ; Return

;------------------------------------------------------------------------------

	CNOP 0,4

_DOSBase:
	ds.l 1

MySoftInt:
	ds.b IS_SIZE

MyTimeReq:
	ds.b IOTV_SIZE

SoftIntData:
	ds.b sid_SIZE

ResultArgs:
	ds.b fmt_SIZE

ResultString1:
	dc.b 'CPU: %s @ %ld MHz',10,0

ResultString2:
	dc.b 'FPU: %s @ %ld MHz',10,0

DOSName:
	DOSNAME

TimerName:
	TIMERNAME

;------------------------------------------------------------------------------
	
	CNOP 0,4

CPUModel:
	dc.b "MC68000",0
	dc.b "MC68010",0
	dc.b "MC68020",0
	dc.b "MC68030",0
	dc.b "MC68040",0
	dc.b "MC68060",0
	dc.b "AC68080",0
	dc.b 0

;------------------------------------------------------------------------------

	CNOP 0,4

FPUModel:
	dc.b "MC68881",0
	dc.b "MC68882",0
	dc.b "MC68040",0
	dc.b 0

;------------------------------------------------------------------------------
