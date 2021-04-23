
    MACHINE MC68040

;--------------------------------------------------------------------
; APOLLO 68080 Extra Control Registers
;--------------------------------------------------------------------

SPR_CLK    EQU $809   ; Clock Cycle Counter
SPR_MIPS1  EQU $80A   ; MIPS in Pipe 1
SPR_MIPS2  EQU $80B   ; MIPS in Pipe 2
SPR_BPC    EQU $80C   ; Branch Predict Correct
SPR_BPW    EQU $80D   ; Branch Predict Wrong
SPR_DCH    EQU $80E   ; Data Cache Hit
SPR_DCM    EQU $80F   ; Data Cache Miss

;--------------------------------------------------------------------
; MOVC Dir,RegType,RegNum,CtrlReg
;--------------------------------------------------------------------
; Dir:     Direction (0=Rc to Rn, 1=Rn to Rc) 
; RegType: Register Type (0=D, 1=A)
; RegNum:  Register Number (0 to 7)
; CtrlReg: Control Register (0 to $fff)
;--------------------------------------------------------------------
	
MOVC MACRO
    dc.w ($4e7a|(\1))
    dc.w (((\2)<<15)|((\3)<<12)|(\4))
    ENDM

;--------------------------------------------------------------------
; Test
;--------------------------------------------------------------------

    INCDIR   includes
    INCLUDE  exec/exec_lib.i
	
UserCode:	
    MOVE.L   #SuperCode,a5
    CALLEXEC Supervisor
    STOP     #-1
    RTS
    
SuperCode:
    MOVC     0,0,2,SPR_CLK   ; MOVEC Rc,d2
    MOVC     0,1,3,SPR_CLK   ; MOVEC Rc,a3
    ;MOVC    1,0,2,SPR_CLK   ; MOVEC d2,Rc
    ;MOVC    1,1,3,SPR_CLK   ; MOVEC a3,Rc
    RTE
	
;--------------------------------------------------------------------

    END
	