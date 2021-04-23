*******************************************************************************
*
* APOLLO-Core - SILVER 8
* 
* MOVEX - Reverse bytes
* 
* Syntax:
* 
* MOVEX Rn
* MOVEX Rx,Ry
* 
* Inputs:
* 
* Rn/Rx = Source
* 
* Outputs:
* 
* Rn/Ry = Reversed bytes (4 bytes = 32 bits)
* 
* Encoding:
* 
*   Header   Rx   Ry   Footer
*        8    4    4        8
* 11101000 0000 0000 00010000
* 
*******************************************************************************

;------------------------------------------------------------------------------
; MOVEX Rn
; MOVEX Rx,Ry
;------------------------------------------------------------------------------

MOVEX	MACRO
	IF NARG=1
		dc.l ($0E800000|($10000*(\1))|($1000*(\1))|($010))
		MEXIT
	ENDC
	IF NARG=2
		dc.l ($0E800000|($10000*(\1))|($1000*(\2))|($010))
		MEXIT
	ENDC
	FAIL Incorrect number of arguments !
	ENDM

;------------------------------------------------------------------------------
; MAIN PROGRAM
;------------------------------------------------------------------------------

Main:

.test0

	; MOVEX Incorrect number of args
	
	MOVEX  1,2,3           ; MOVEX ?
	stop   #1              ; 

.test1

	; MOVEX Dn
	
	move.l #$11223344,d3   ; d3 = $11223344
	MOVEX  3               ; MOVEX d3
	stop   #1              ; d3 = $44332211
	
.test2

	; MOVEX An
	
	move.l #$22334455,a5   ; a5 = $22334455
	MOVEX  5+8             ; MOVEX a5
	stop   #1              ; a5 = $55443322

.test3

	; MOVEX Dx,Dy
	
	move.l #$33445566,d2   ; d2 = $33445566
	move.l #$00000000,d4   ; d4 = $00000000
	MOVEX  2,4             ; MOVEX d2,d4
	stop   #1              ; d4 = $66554433

.test4

	; MOVEX Dn,An
	
	move.l #$44556677,d1   ; d1 = $44556677
	move.l #$00000000,a4   ; a4 = $00000000
	MOVEX  1,4+8           ; MOVEX d1,a4
	stop   #1              ; a4 = $77665544

.end

	rts
	