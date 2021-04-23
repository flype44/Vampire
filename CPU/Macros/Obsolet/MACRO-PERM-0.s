*******************************************************************************
*
* APOLLO-Core - SILVER 8
* 
* PERM32 - Permute 32bits
* 
* Syntax:
* 
* PERM32 Rn, Sel1, Sel2, Sel3, Sel4
* PERM32 Rx, Ry, Sel1, Sel2, Sel3, Sel4
* 
* Inputs:
* 
* Rx   = Source #1 (D0..A7)
* Ry   = Source #2 (D0..A7) - Can be same as Rx.
* Sel1 = Selector for Byte #0 (0..7)
* Sel2 = Selector for Byte #1 (0..7)
* Sel3 = Selector for Byte #2 (0..7)
* Sel4 = Selector for Byte #3 (0..7)
* 
* Outputs:
* 
* Ry   = Permuted bytes (4 bytes = 32 bits)
* 
* Encoding:
* 
*       PERM32   Rx   Ry  S1  S2  S3  S4
*           12    4    4   3   3   3   3
* 010011001100 0000 0000 000 000 000 000
* 
*******************************************************************************

;------------------------------------------------------------------------------
; PERM32 Rn,Sel1,Sel2,Sel3,Sel4
; PERM32 Rx,Ry,Sel1,Sel2,Sel3,Sel4
;------------------------------------------------------------------------------

PERM32	MACRO
	IF NARG=5
		dc.l ($4CC00000|($10000*(\1))|($1000*(\1))|($200*(\2))|($40*(\3))|($8*(\4))|(\5))
		MEXIT
	ENDC
	IF NARG=6
		dc.l ($4CC00000|($10000*(\1))|($1000*(\2))|($200*(\3))|($40*(\4))|($8*(\5))|(\6))
		MEXIT
	ENDC
	FAIL Incorrect number of argument !
	ENDM

;------------------------------------------------------------------------------
; MAIN PROGRAM
;------------------------------------------------------------------------------

Main:

.test0
	
	; PERM32 Incorrect number of args
	
	PERM32 3,2,1,0         ; PERM32 ?,3,2,1,0
	stop   #1              ; 
	
.test1
	
	; PERM32 Dn,S1,S2,S3,S4
	
	move.l #$11223344,d7   ; d7 = $11223344
	PERM32 7,3,2,1,0       ; PERM32 d7,3,2,1,0
	stop   #1              ; d7 = $44332211
	
.test2
	
	; PERM32 An,S1,S2,S3,S4
	
	move.l #$ABADCAFE,a5   ; a5 = $ABADCAFE
	PERM32 (5+8),3,2,1,0   ; PERM32 a5,3,2,1,0
	stop   #1              ; a5 = $FECAADAB
	
.test3	
	
	; PERM32 Dx,Dy,S1,S2,S3,S4
	
	move.l #$11223344,d3   ; d3 = $11223344
	move.l #$55667788,d6   ; d6 = $55667788	
	PERM32 3,6,7,5,3,1     ; PERM32 d3,d6,7,5,3,1
	stop   #1              ; d6 = $88664422
	
.exit
	
	rts
	
	