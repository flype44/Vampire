
; \1 RegA Bn Bit (0..1)
; \2 RegB Bn Bit (0..1)
; \3 RegD Bn Bit (0..1)
; \4 EA Mode     (0..7)
; \5 EA RegA     (0..7)
; \6 RegB        (0..F)
; \7 RegD        (0..F)

PEOR MACRO
  dc.w ((%1111111<<9)|(\1<<8)|(\2<<7)|(\3<<6)|(\4<<3)|\5)
  dc.w ((\6<<12)|(\7<<8)|%00001010)
  ENDM
	
main:
	move.l #$12345678,d1
	
	dc.w %1111111000000001,%0001000100001010
	
	PEOR 0,0,0,0,1,1,1 ; PEOR d1,d1,d1
	
	rts
	