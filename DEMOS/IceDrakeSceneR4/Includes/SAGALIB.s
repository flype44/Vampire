*********************************************************************
* Library: SAGALIB routines
* Short:   Some SAGA routines (Beware: Only 32-bits A8R8G8B8)
* Date:    2021-08-14
* Author:  flype
*********************************************************************

	XDEF     _SAGALIB_Is68080
	XDEF     _SAGALIB_BlitSprite
	XDEF     _SAGALIB_BlitSpriteXY
	XDEF     _SAGALIB_BlitTransparentSprite
	XDEF     _SAGALIB_BlitTransparentSpriteXY
	XDEF     _SAGALIB_BlitRectangle
	XDEF     _SAGALIB_ClearPixels
	XDEF     _SAGALIB_ConvertPixels
	XDEF     _SAGALIB_PlaySound
	XDEF     _SAGALIB_WaitVBL

*********************************************************************
* _SAGALIB_START
*********************************************************************

	CNOP 0,4

_SAGALIB_START:
	JMP      _SAGALIB_END              ; Jump at bottom

*********************************************************************
* _SAGALIB_Is68080(void)
*********************************************************************

	CNOP 0,4

_SAGALIB_Is68080:
	move.l   a0,-(sp)                  ; Push
	move.l   4.w,a0                    ; AmigaOS ExecBase
	btst     #10,296(a0)               ; ExecBase->AttnFlags[BIT10]
	move.l   (sp)+,a0                  ; Pop
	rts                                ; Return

*********************************************************************
* _SAGALIB_BlitRectangle(
* Pixels, Left, Top, Width, Height, PixelPerRow, Color)
* A0      D0    D1   D2     D3      D4           D5
*********************************************************************

	CNOP 0,4

_SAGALIB_BlitRectangle:
	movem.l  d0-d3/a0,-(sp)            ; Push
	lsl.l    #2,d0                     ; Left
	add.l    d0,a0                     ; Left
	mulu.l   d4,d1                     ; Top
	lsl.l    #2,d1                     ; Top
	add.l    d1,a0                     ; Top
	move.l   d4,d0                     ; Modulo
	sub.l    d2,d0                     ; Modulo
	lsl.l    #2,d0                     ; Modulo
	move.l   d2,d1                     ; Width
	lsr.l    #1,d1                     ; Width
.y	move.l   d1,d2                     ; Width
.x	move.l   d5,(a0)+                  ; PutPixel
	move.l   d5,(a0)+                  ; PutPixel
	subq.l   #1,d2                     ; Next X
	bne.s    .x                        ; Continue
	add.l    d0,a0                     ; Modulo
	subq.l   #1,d3                     ; Next Y
	bne.s    .y                        ; Continue
	movem.l  (sp)+,d0-d3/a0            ; Pop
	rts                                ; Return

*********************************************************************
* _SAGALIB_BlitSprite(
* Src, SrcW, SrcH, Dst, DstX, DstY, PixelPerRow)
* A0   D0    D1    A1   D2    D3    D4
*********************************************************************

	CNOP 0,4

_SAGALIB_BlitSprite:
	movem.l  d0-d4/a0-a1,-(sp)         ; Push
	lsl.l    #2,d2                     ; Left
	add.l    d2,a1                     ; Left
	mulu.l   d4,d3                     ; Top
	lsl.l    #2,d3                     ; Top
	add.l    d3,a1                     ; Top
	move.l   d4,d3                     ; Modulo
	sub.l    d0,d3                     ; Modulo
	lsl.l    #2,d3                     ; Modulo
	move.l   d0,d4                     ; Width
	lsr.l    #1,d4                     ; Width
.y	move.l   d4,d0                     ; Width
.x	move.l   (a0)+,(a1)+               ; Copy 1 pixel
	move.l   (a0)+,(a1)+               ; Copy 1 pixel
	subq.l   #1,d0                     ; Next X
	bne.s    .x                        ; Continue
	add.l    d3,a1                     ; Modulo
	subq.l   #1,d1                     ; Next Y
	bne.s    .y                        ; Continue
	movem.l  (sp)+,d0-d4/a0-a1         ; Pop
	rts                                ; Return

*********************************************************************
* _SAGALIB_BlitSpriteXY(
* Src, SrcW, SrcH, Dst, DstX, DstY, PixelPerRow)
* A0   D0    D1    A1   D2    D3    D4
*********************************************************************

	CNOP 0,4

_SAGALIB_BlitSpriteXY:
	movem.l  d0-d4/a0-a1,-(sp)         ; Push
	lsl.l    #2,d2                     ; Left
	add.l    d2,a1                     ; Left
	mulu.l   d4,d3                     ; Top
	lsl.l    #2,d3                     ; Top
	add.l    d3,a1                     ; Top
	move.l   d4,d3                     ; Modulo
	sub.l    d0,d3                     ; Modulo
	lsl.l    #2,d3                     ; Modulo
	move.l   d0,d4                     ; Width
	lsr.l    #1,d4                     ; Width
.y	move.l   d4,d0                     ; Width
.x	move.l   (a0)+,(a1)+               ; Copy 1 pixel
	move.l   (a0)+,(a1)+               ; Copy 1 pixel
	subq.l   #1,d0                     ; Next X
	bne.s    .x                        ; Continue
	add.l    d3,a1                     ; Modulo
	subq.l   #1,d1                     ; Next Y
	bne.s    .y                        ; Continue
	movem.l  (sp)+,d0-d4/a0-a1         ; Pop
	rts                                ; Return

*********************************************************************
* _SAGALIB_BlitTransparentSprite(
* Src, SrcW, SrcH, Dst, DstX, DstY, PixelPerRow)
* A0   D0    D1    A1   D2    D3    D4
*********************************************************************

	CNOP 0,4

_SAGALIB_BlitTransparentSprite:
	movem.l  d0-d4/a0-a1,-(sp)         ; Push
	lsl.l    #2,d2                     ; Left
	add.l    d2,a1                     ; Left
	mulu.l   d4,d3                     ; Top
	lsl.l    #2,d3                     ; Top
	add.l    d3,a1                     ; Top
	move.l   d4,d3                     ; Modulo
	sub.l    d0,d3                     ; Modulo
	lsl.l    #2,d3                     ; Modulo
	move.l   d0,d4                     ; Width
	lsr.l    #1,d4                     ; Width
.y	move.l   d4,d0                     ; Width
.x	load     (a0)+,d2                  ; Load 2 pixels (8 bytes)
	storem3  d2,d4,(a1)+               ; Write 2 pixels (8 bytes)
	subq.l   #1,d0                     ; Next X
	bne.s    .x                        ; Continue
	add.l    d3,a1                     ; Modulo
	subq.l   #1,d1                     ; Next Y
	bne.s    .y                        ; Continue
	movem.l  (sp)+,d0-d4/a0-a1         ; Pop
	rts                                ; Return

*********************************************************************
* _SAGALIB_BlitTransparentSpriteXY(
* Src, SrcX, SrcY, SrcW, SrcH, Dst, DstX, DstY, PixelPerRow)
* A0   D0    D1    D2    D3    A1   D4    D5    D6
*********************************************************************

	CNOP 0,4

_SAGALIB_BlitTransparentSpriteXY:
	movem.l  d0-d7/a0-a1,-(sp)         ; Push
	lsl.l    #2,d0                     ; SrcX
	adda.l   d0,a0                     ; SrcX
	mulu.l   d2,d1                     ; SrcY
	lsl.l    #2,d1                     ; SrcY
	adda.l   d1,a0                     ; SrcY
	lsl.l    #2,d4                     ; DstX
	adda.l   d4,a1                     ; DstX
	mulu.l   d6,d5                     ; DstY
	lsl.l    #2,d5                     ; DstY
	adda.l   d5,a1                     ; DstY
	move.l   d2,d5                     ; Modulo
	sub.l    d6,d5                     ; Modulo
	lsl.l    #2,d5                     ; Modulo
	lsr.l    #1,d6                     ; Width
.y	move.l   d6,d2                     ; Width
.x	load     (a0)+,d1                  ; Load 2 pixels (8 bytes)
	storem3  d1,d4,(a1)+               ; Write 2 pixels (8 bytes)
	subq.l   #1,d2                     ; Next X
	bne.s    .x                        ; Continue
	adda.l   d5,a0                     ; Modulo
	subq.l   #1,d3                     ; Next Y
	bne.s    .y                        ; Continue
	movem.l  (sp)+,d0-d7/a0-a1         ; Pop
	rts                                ; Return

*********************************************************************
* _SAGALIB_BlitTransparentSpriteXY2(
* Src, SrcX, SrcY, SrcW, SrcH, SrcRowSize, Dst, DstX, DstY, DstRowSize)
* A0   D0    D1    D2    D3    D4          A1   D5    D6    D7
*********************************************************************

_SAGALIB_BlitTransparentSpriteXY2:
	movem.l  d0-d7/a0-a1,-(sp)         ; Push
	lsl.l    #2,d0                     ; SrcX
	adda.l   d0,a0                     ; SrcX
	mulu.l   d4,d1                     ; SrcY
	adda.l   d1,a0                     ; SrcY
	lsl.l    #2,d5                     ; DstX
	adda.l   d5,a1                     ; DstX
	mulu.l   d7,d6                     ; DstY
	adda.l   d6,a1                     ; DstY
	lsl.l    #2,d2                     ; SrcModulo
	sub.l    d2,d4                     ; SrcModulo
	sub.l    d2,d7                     ; DstModulo
	move.l   d2,d5                     ; Width
	lsr.l    #3,d5                     ; Width
.y	move.l   d5,d2                     ; Width     = (106*4)/8
.x	load     (a0)+,d1                  ; Load 2 pixels (8 bytes)
	storem3  d1,d4,(a1)+               ; Write 2 pixels (8 bytes)
	subq.l   #1,d2                     ; Next X
	bne.s    .x                        ; Continue
	adda.l   d4,a0                     ; SrcModulo = (320-106)*4
	adda.l   d7,a1                     ; DstModulo = (640-106)*4
	subq.l   #1,d3                     ; Next Y
	bne.s    .y                        ; Continue
	movem.l  (sp)+,d0-d7/a0-a1         ; Pop
	rts                                ; Return

*********************************************************************
* _SAGALIB_ClearPixels(
* Pixels, PixelCount, Color)
* A0      D0          D1
*********************************************************************

	CNOP 0,4

_SAGALIB_ClearPixels:
	movem.l  d0/a0,-(sp)               ; Push
	lsr.l    #1,d0                     ; PixelCount * 4 / 8
.l1	move.l   d1,(a0)+                  ; Write pixel
	move.l   d1,(a0)+                  ; Write pixel
	subq.l   #1,d0                     ; Next iteration
	bne.s    .l1                       ; Continue
	movem.l  (sp)+,d0/a0               ; Pop
	rts                                ; Return

*********************************************************************
* _SAGALIB_ConvertPixels(
* Pixels, PixelCount)
* A0      D0
*********************************************************************

	CNOP 0,4

_SAGALIB_ConvertPixels:
	movem.l  d0/d1/a0,-(sp)            ; Push
.l1	move.l   (a0),d1                   ; Read pixel
	ror.l    #8,d1                     ; RRGGBBAA -> AARRGGBB
	move.l   d1,(a0)+                  ; Write pixel
	subq.l   #1,d0                     ; Next pixel
	bne.s    .l1                       ; Continue
	movem.l  (sp)+,d0/d1/a0            ; Pop
	rts                                ; Return

*********************************************************************
* _SAGALIB_PlaySound(
* WaveData, WaveDataEnd)
* A0        A1
*********************************************************************

	CNOP 0,4

_SAGALIB_PlaySound:
	movem.l  d0/a0-a1,-(sp)            ; Push
	move.l   a1,d0                     ; AUD Length
	sub.l    a0,d0                     ; AUD Length
	divu.l   #8,d0                     ; AUD Length (16-bits, Stereo)
	move.l   a0,$DFF400                ; AUD Data
	move.l   d0,$DFF404                ; AUD Length
	move.w   #$FFFF,$DFF408            ; AUD Volume
	move.w   #80,$DFF40C               ; AUD Period
	move.w   #$0004,$DFF40A            ; AUD Control
	move.w   #$8201,$DFF096            ; AUD DMA
	movem.l  (sp)+,d0/a0-a1            ; Pop
	rts                                ; Return

*********************************************************************
* _SAGALIB_WaitVBL(void)
*********************************************************************

	CNOP 0,4

_SAGALIB_WaitVBL:
	btst.b   #5,$DFF01F                ; Check VBL interrupt
	beq.s    _SAGALIB_WaitVBL          ; Continue if False
	move.w   #(1<<5),$DFF09C           ; Clear VBL interrupt
	rts                                ; Return

*********************************************************************
* End of file
*********************************************************************

	CNOP 0,4

_SAGALIB_END:
