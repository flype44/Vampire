*********************************************************************
* 
* File     : IceDrakeScene.s
* Version  : 1.1 (15.8.2021)
* Author   : APOLLO-Team, flype
* Target   : V4 Core R7+
* 
* Compiler : VASM 1.8l (2021-07-20)
* http://sun.hasenbraten.de/vasm/bin/rel/vasmm68k_mot_os3.lha
* 
*********************************************************************

	INCDIR  'Includes/'
	INCLUDE 'SAGALIB.i'
	INCLUDE 'SAGALIB.s'

*********************************************************************
* 
* CONSTANTS
* 
*********************************************************************

;SOUND EQU 1

SCREENWIDTH      EQU (640)                 ; Width
SCREENHEIGHT     EQU (480)                 ; Height
SCREENROWSIZE    EQU (SCREENWIDTH*4)       ; Bytes Per Row
SCREENSIZE       EQU (SCREENROWSIZE*SCREENHEIGHT) ; ByteCount

*********************************************************************
* 
* Entry Point
* 
*********************************************************************

MAIN:
	CALLSAGA Is68080                   ; Check CPU
	beq.s    .exit                     ; AC68080 required
	
	bsr      BatInit                   ; Init
	bsr      BirdInit                  ; Init
	bsr      BannerInit                ; Init
	bsr      LogoInit                  ; Init
	bsr      JoyHelpInit               ; Init
	bsr      MessageInit               ; Init
	bsr      BackgroundInit            ; Init
	bsr      ScreenOpen                ; Init
	
	IFD SOUND
	lea.l    Sound1s,a0                ; Sound Data
	add.l    #16,a0                    ; Sound Header Size
	lea.l    Sound1e,a1                ; Sound Data End
	CALLSAGA PlaySound                 ; Play immediately
	ENDC
.loop
	bsr      BirdUpdate                ; Update
        bsr      MessageUpdate             ; Update
	bsr      BackgroundUpdate          ; Update
	bsr      JoystickUpdate            ; Update
	bsr      JoyHelpUpdate             ; Update
	bsr      MouseUpdate               ; Update
	
	CALLSAGA WaitVBL                   ; Sync
        
	bsr      BackgroundDraw            ; Blit
	bsr      BirdDraw                  ; Blit
	bsr      MessageDraw               ; Blit
	bsr      BannerDraw                ; Blit
	bsr      BatDraw                   ; Blit
	bsr      JoyHelpDraw               ; Blit
	
	bsr      ScreenSwap                ; Swap buffers
	
	tst.l    ExitApp                   ; Exit program ?
	beq.s	 .loop                     ; Continue
.exit
	bsr      ScreenClose               ; Restore
	moveq.l  #0,d0                     ; Return code
	rts                                ; Exit program

*********************************************************************
* 
* Screen routines
* 
*********************************************************************

ScreenOpen:
	move.w   $DFF002,d0                ; DMACONR
	or.w     d0,DMACON                 ; DMACON
	move.w   #$7FFF,$DFF096            ; DMACON
	move.w   $DFF01C,d0                ; INTENAR
	or.w     d0,INTENA                 ; INTENA
	move.w   #$7FFF,$DFF09A            ; INTENA
	move.l   $DFF1EC,GFXADDR           ; GFXADDR	
	move.w   $DFF1F4,GFXMODE           ; GFXMODE
	
	lea.l    Buffers,a0                ; Prepare buffers
	move.l   a0,d0                     ; 
	addi.l   #15,d0                    ; 
	andi.l   #~15,d0                   ; 
	addi.l   #SCREENSIZE,d0            ; Aligned FBAddr1
	move.l   d0,Screen1                ; 
	addi.l   #SCREENSIZE,d0            ; Aligned FBAddr2
	move.l   d0,Screen2                ; 
	addi.l   #SCREENSIZE,d0            ; Aligned FBAddr3
	move.l   d0,Screen3                ; 
	
	move.l   Screen1,a0                ; Pixels
	move.l   #SCREENWIDTH,d0           ; Width
	mulu.l   #SCREENHEIGHT,d0          ; Height
	mulu.l   #3,d0                     ; Count (Triple-Buffer)
	move.l   #$FF00FF00,d1             ; Color
	CALLSAGA ClearPixels               ; Clear
	
	move.w   #$0000,$DFF1D0            ; MOUSEX
	move.w   #$0000,$DFF1D2            ; MOUSEY
	move.w	 #$0000,$DFF1E6            ; GFXMODULO
	move.w	 #$0505,$DFF1F4            ; GFXMODE (640x480, A8R8G8B8)
	move.l	 Screen1,$DFF1EC           ; GFXADDR
	rts                                ; Return

;--------------------------------------------------------------------

ScreenClose:
	move.w   #$7FFF,$DFF096            ; DMACON
	move.w   DMACON,$DFF096            ; DMACON
	move.w   INTENA,$DFF09A            ; INTENA
	move.l   GFXADDR,$DFF1EC           ; GFXADDR
	move.w   GFXMODE,$DFF1F4           ; GFXMODE
	rts                                ; Return

;--------------------------------------------------------------------

ScreenSwap:
	move.l   Screen1,a0                ; Current buffer
	move.l   a0,$DFF1EC                ; Apply to hardware
	move.l   Screen2,Screen1           ; [0] = [1] \
	move.l   Screen3,Screen2           ; [1] = [2]   Swap buffers
	move.l   a0,Screen3                ; [2] = [0] /
	rts

*********************************************************************
* 
* Bat routines
* 
*********************************************************************

BATW             EQU (120)                 ; Width
BATH             EQU (85)                  ; Height
BATX             EQU (SCREENWIDTH-BATW-4)  ; Left
BATY             EQU (53)                  ; Top

;--------------------------------------------------------------------

BatInit:
	lea      Sprite3,a0                ; Pixels
	move.l   #BATW,d0                  ; Width
	mulu.l   #BATH,d0                  ; Height
	CALLSAGA ConvertPixels             ; Convert
	rts                                ; Return

;--------------------------------------------------------------------

BatDraw:
	movem.l  d0-d4/a0-a1,-(sp)         ; Push
	tst.l    ShowAll                   ; Show ?
	beq.s    .l1                       ; Skip
	lea.l    Sprite3,a0                ; Src
	move.l   Screen1,a1                ; Dst
	move.l   #BATW,d0                  ; SrcW
	move.l   #BATH,d1                  ; SrcH
	move.l   #BATX,d2                  ; DstX
	move.l   #BATY,d3                  ; DstY
	move.l   #SCREENWIDTH,d4           ; PixelPerRow
	CALLSAGA BlitTransparentSprite     ; Blit
.l1	movem.l  (sp)+,d0-d4/a0-a1         ; Pop
	rts                                ; Return

*********************************************************************
* 
* Banner routines
* 
*********************************************************************

BANNER1W         EQU (80)                  ; Width
BANNER1H         EQU (480)                 ; Height
BANNER1X         EQU (0)                   ; Left
BANNER1Y         EQU (0)                   ; Top
BANNER2W         EQU (320)                 ; Width
BANNER2H         EQU (113)                 ; Height
BANNER2X         EQU (SCREENWIDTH-BANNER2W); Left
BANNER2Y         EQU (SCREENHEIGHT-BANNER2H); Top

BannerInit:
	lea      Sprite1,a0                ; Pixels
	move.l   #BANNER1W,d0              ; Width
	mulu.l   #BANNER1H,d0              ; Height
	CALLSAGA ConvertPixels             ; Convert
.l1	lea      Sprite2,a0                ; Pixels
	move.l   #BANNER2W,d0              ; Width
	mulu.l   #BANNER2H,d0              ; Height
	CALLSAGA ConvertPixels             ; Convert
	rts                                ; Return

;--------------------------------------------------------------------

BannerDraw:
	movem.l  d0-d5/a0-a1,-(sp)         ; Push
	tst.l    ShowAll                   ; Show ?
	beq.s    .l4                       ; Skip
.l1	move.l   Screen1,a0                ; Pixels
	move.l   #4,d0                     ; Left
	move.l   #0,d1                     ; Top
	move.l   #26,d2                    ; Width
	move.l   #SCREENHEIGHT,d3          ; Height
	move.l   #SCREENWIDTH,d4           ; PixelsPerRow
	move.l   #$00EF5F00,d5             ; Color
	CALLSAGA BlitRectangle             ; Blit
.l2	lea.l    Sprite1,a0                ; Src
	move.l   Screen1,a1                ; Dst
	move.l   #BANNER1W,d0              ; SrcW
	move.l   #BANNER1H,d1              ; SrcH
	moveq.l  #BANNER1X,d2              ; DstX
	moveq.l  #BANNER1Y,d3              ; DstY
	move.l   #SCREENWIDTH,d4           ; PixelPerRow
	CALLSAGA BlitTransparentSprite     ; Blit
.l3	lea.l    Sprite2,a0                ; Src
	move.l   Screen1,a1                ; Dst
	move.l   #BANNER2W,d0              ; SrcW
	move.l   #BANNER2H,d1              ; SrcH
	move.l   #BANNER2X,d2              ; DstX
	move.l   #BANNER2Y,d3              ; DstY
	move.l   #SCREENWIDTH,d4           ; DstW
	CALLSAGA BlitTransparentSprite     ; Blit
.l4	movem.l  (sp)+,d0-d5/a0-a1         ; Pop
	rts                                ; Return

*********************************************************************
* 
* Bird routines
* 
*********************************************************************

BIRDX            EQU (100)                 ; Left
BIRDY            EQU (65)                  ; Top
BIRDW            EQU (106)                 ; Width
BIRDH            EQU (93)                  ; Height
BIRDWIDTH        EQU (320)                 ; Width
BIRDHEIGHT       EQU (281)                 ; Height
BIRDROWSIZE      EQU (BIRDWIDTH*4)         ; Bytes Per Row
BIRDSPEED        EQU (3)                   ; Speed
BIRDMAXW         EQU (SCREENWIDTH-BIRDW-(BIRDSPEED*2))
BIRDMAXH         EQU (SCREENHEIGHT-BIRDH-(BIRDSPEED*2))

;--------------------------------------------------------------------

BirdInit:
	lea      Sprite4,a0                ; Pixels
	move.l   #BIRDWIDTH,d0             ; Width
	mulu.l   #BIRDHEIGHT,d0            ; Height
	CALLSAGA ConvertPixels             ; Convert
.l1	move.l   #BIRDX,BirdLeft           ; Initial X position
	move.l   #BIRDY,BirdTop            ; Initial Y position
	clr.l    BirdFrameI                ; Initial Frame index
	rts                                ; Return

;--------------------------------------------------------------------

BirdDraw:
	movem.l  d0-d7/a0-a1,-(sp)         ; Push
	lea.l    Sprite4,a0                ; Src
	move.l   Screen1,a1                ; Dst
	move.l   BirdFrameX,d0             ; SrcX
	move.l   BirdFrameY,d1             ; SrcY
	move.l   #BIRDW,d2                 ; SrcW
	move.l   #BIRDH,d3                 ; SrcH
	move.l   #BIRDROWSIZE,d4           ; SrcRowSize
	move.l   BirdLeft,d5               ; DstX
	move.l   BirdTop,d6                ; DstY
	move.l   #SCREENROWSIZE,d7         ; DstRowSize
	CALLSAGA BlitTransparentSpriteXY2  ; Blit
	movem.l  (sp)+,d0-d7/a0-a1         ; Pop
	rts                                ; Return

;--------------------------------------------------------------------

BirdUpdate:
	movem.l  d0/a0,-(sp)               ; Push
	add.l    #1,BirdFrameT             ; Timer + 1
	cmpi.l   #2,BirdFrameT             ; Timer < 3 ?
	blo.s    .l2                       ; Skip
	move.l   #0,BirdFrameT             ; Timer = 0
	lea.l    BirdFrames,a0             ; Table
	move.l   BirdFrameI,d0             ; Index
	cmpi.l   #-1,(a0,d0.L*8)           ; Table end ?
	bne.s    .l1                       ; Skip
	move.l   #0,BirdFrameI             ; Index = 0
	bra.s    .l2                       ; Skip
.l1	move.l    (a0,d0.L*8),BirdFrameX   ; X
	move.l   4(a0,d0.L*8),BirdFrameY   ; Y
	add.l    #1,BirdFrameI             ; Index + 1
.l2	movem.l  (sp)+,d0/a0               ; Pop
	rts                                ; Return

*********************************************************************
* 
* Logo routines
* 
*********************************************************************

LOGOW            EQU (252)                 ; Width
LOGOH            EQU (252)                 ; Height
LOGOX            EQU (SCREENWIDTH-LOGOW)/2 ; Left
LOGOY            EQU (SCREENHEIGHT-LOGOH)/2; Top
LOGOSPEED        EQU (3)                   ; Speed
LOGOMAXW         EQU (SCREENWIDTH-LOGOW-(LOGOSPEED*2))
LOGOMAXH         EQU (SCREENHEIGHT-LOGOH-(LOGOSPEED*2))

;--------------------------------------------------------------------

LogoInit:
	lea.l    Sprite5,a0                ; Pixels
	move.l   #LOGOW,d0                 ; Width
	mulu.l   #LOGOH,d0                 ; Height
	CALLSAGA ConvertPixels             ; Convert
	move.l   #LOGOX,LogoLeft           ; Initial X position
	move.l   #LOGOY,LogoTop            ; Initial Y position
	rts                                ; Return

;--------------------------------------------------------------------

LogoDraw:
	movem.l  d0-d3/a0-a1,-(sp)         ; Push
	lea.l    Sprite5,a0                ; Src
	move.l   Screen1,a1                ; Dst
	move.l   #LOGOW,d0                 ; SrcW
	move.l   #LOGOH,d1                 ; SrcH
	move.l   LogoLeft,d2               ; DstX
	move.l   LogoTop,d3                ; DstY
	move.l   #SCREENWIDTH,d4           ; PixelPerRow
	CALLSAGA BlitTransparentSprite     ; Blit
	movem.l  (sp)+,d0-d3/a0-a1         ; Pop
	rts                                ; Return

*********************************************************************
* 
* Background routines
* 
*********************************************************************

BACKGROUND1W     EQU (640)
BACKGROUND1H     EQU (304)
BACKGROUND1X     EQU (0)
BACKGROUND1Y     EQU (0)
BACKGROUND2W     EQU (1920)
BACKGROUND2H     EQU (283)
BACKGROUND2X     EQU (0)
BACKGROUND2Y     EQU (SCREENHEIGHT-BACKGROUND2H)
BACKGROUND3W     EQU (1920)
BACKGROUND3H     EQU (347)
BACKGROUND3X     EQU (0)
BACKGROUND3Y     EQU (SCREENHEIGHT-BACKGROUND3H)
BACKGROUND4W     EQU (1920)
BACKGROUND4H     EQU (130)
BACKGROUND4X     EQU (0)
BACKGROUND4Y     EQU (0)
BACKGROUND5W     EQU (1920)
BACKGROUND5H     EQU (226)
BACKGROUND5X     EQU (0)
BACKGROUND5Y     EQU (100)

;--------------------------------------------------------------------

BackgroundInit:
	lea      Parallax1,a0              ; Pixels
	move.l   #BACKGROUND1W,d0          ; Width
	mulu.l   #BACKGROUND1H,d0          ; Height
	CALLSAGA ConvertPixels             ; Convert
.l1	lea      Parallax2,a0              ; Pixels
	move.l   #BACKGROUND2W,d0          ; Width
	mulu.l   #BACKGROUND2H,d0          ; Height
	CALLSAGA ConvertPixels             ; Convert
.l2	lea      Parallax3,a0              ; Pixels
	move.l   #BACKGROUND3W,d0          ; Width
	mulu.l   #BACKGROUND3H,d0          ; Height
	CALLSAGA ConvertPixels             ; Convert
.l3	lea      Parallax4,a0              ; Pixels
	move.l   #BACKGROUND4W,d0          ; Width
	mulu.l   #BACKGROUND4H,d0          ; Height
	CALLSAGA ConvertPixels             ; Convert
.l4	lea      Parallax5,a0              ; Pixels
	move.l   #BACKGROUND5W,d0          ; Width
	mulu.l   #BACKGROUND5H,d0          ; Height
	CALLSAGA ConvertPixels             ; Convert
	rts                                ; Return

;--------------------------------------------------------------------

BackgroundDraw:
	movem.l  d0-d6/a0-a1,-(sp)         ; Push
.l1	lea.l    Parallax1,a0              ; Src
	move.l   Screen1,a1                ; Dst
	move.l   #BACKGROUND1W,d0          ; SrcW
	move.l   #BACKGROUND1H,d1          ; SrcH
	move.l   #BACKGROUND1X,d2          ; DstX
	move.l   #BACKGROUND1Y,d3          ; DstY
	move.l   #SCREENWIDTH,d4           ; DstW
	CALLSAGA BlitSprite                ; Blit
.l2	lea.l    Parallax2,a0              ; Src
	move.l   Screen1,a1                ; Dst
	move.l   BackgroundB,d0            ; SrcX
	lsr.l    #1,d0                     ; SrcX
	move.l   #0,d1                     ; SrcY
	move.l   #BACKGROUND2W,d2          ; SrcW
	move.l   #BACKGROUND2H,d3          ; SrcH
	move.l   #BACKGROUND2X,d4          ; DstX
	move.l   #BACKGROUND2Y,d5          ; DstY
	move.l   #SCREENWIDTH,d6           ; DstW
	CALLSAGA BlitTransparentSpriteXY   ; Blit
.l3	lea.l    Parallax5,a0              ; Src
	move.l   Screen1,a1                ; Dst
	move.l   BackgroundB,d0            ; SrcX
	lsl.l    #1,d0                     ; SrcX
	move.l   #0,d1                     ; SrcY
	move.l   #BACKGROUND5W,d2          ; SrcW
	move.l   #BACKGROUND5H,d3          ; SrcH
	move.l   #BACKGROUND5X,d4          ; DstX
	move.l   #BACKGROUND5Y,d5          ; DstY
	move.l   #SCREENWIDTH,d6           ; DstW
	CALLSAGA BlitTransparentSpriteXY   ; Blit
.l4	bsr      LogoDraw                  ; Blit
.l5	lea.l    Parallax4,a0              ; Src
	move.l   Screen1,a1                ; Dst
	move.l   BackgroundC,d0            ; SrcX
	lsr.l    #1,d0                     ; SrcX
	move.l   #0,d1                     ; SrcY
	move.l   #BACKGROUND4W,d2          ; SrcW
	move.l   #BACKGROUND4H,d3          ; SrcH
	move.l   #BACKGROUND4X,d4          ; DstX
	move.l   #BACKGROUND4Y,d5          ; DstY
	move.l   #SCREENWIDTH,d6           ; DstW
	CALLSAGA BlitTransparentSpriteXY   ; Blit
.l6	lea.l    Parallax3,a0              ; Src
	move.l   Screen1,a1                ; Dst
	move.l   BackgroundA,d0            ; SrcX
	move.l   #0,d1                     ; SrcY
	move.l   #BACKGROUND3W,d2          ; SrcW
	move.l   #BACKGROUND3H,d3          ; SrcH
	move.l   #BACKGROUND3X,d4          ; DstX
	move.l   #BACKGROUND3Y,d5          ; DstY
	move.l   #SCREENWIDTH,d6           ; DstW
	CALLSAGA BlitTransparentSpriteXY   ; Blit
.l8	movem.l  (sp)+,d0-d6/a0-a1         ; Pop
	rts                                ; Return

;--------------------------------------------------------------------

BackgroundUpdate:
.l1	cmpi.l  #(854<<0),BackgroundA      ; Parallax A
	blo.s   .l2                        ; 
	move.l  #0,BackgroundA             ; 
	bra.s   .l3                        ; 
.l2	add.l   #1,BackgroundA             ; 
.l3	cmpi.l  #(854<<1),BackgroundB      ; Parallax B
	blo.s   .l4                        ; 
	move.l  #0,BackgroundB             ; 
	bra.s   .l5                        ; 
.l4	add.l   #1,BackgroundB             ; 
.l5	cmpi.l  #(854<<1),BackgroundC      ; Parallax C
	blo.s   .l6                        ; 
	move.l  #0,BackgroundC             ; 
	bra.s   .l7                        ; 
.l6	add.l   #1,BackgroundC             ; 
.l7	rts                                ; Return

*********************************************************************
* 
* Message routines
* 
*********************************************************************

MESSAGEX         EQU (0)                   ; Left
MESSAGEY         EQU (31)                  ; Top
MESSAGEW         EQU (3200)                ; Width
MESSAGEH         EQU (40)                  ; Height
MESSAGESPEED     EQU (1)                   ; Speed

;--------------------------------------------------------------------

MessageInit:
	lea      Sprite6,a0                ; Pixels
	move.l   #MESSAGEW,d0              ; Width
	mulu.l   #MESSAGEH,d0              ; Height
	CALLSAGA ConvertPixels             ; Convert
	rts                                ; Return

;--------------------------------------------------------------------

MessageDraw:
	movem.l  d0-d6/a0-a1,-(sp)         ; Push
	lea.l    Sprite6,a0                ; Src
	move.l   Screen1,a1                ; Dst
	move.l   MessageX,d0               ; SrcX
	move.l   #0,d1                     ; SrcY
	move.l   #MESSAGEW,d2              ; SrcW
	move.l   #MESSAGEH,d3              ; SrcH
	move.l   #MESSAGEX,d4              ; DstX
	move.l   #MESSAGEY,d5              ; DstY
	move.l   #SCREENWIDTH,d6           ; PixelPerRow
	CALLSAGA BlitTransparentSpriteXY   ; Blit
	movem.l  (sp)+,d0-d6/a0-a1         ; Pop
	rts                                ; Return

;--------------------------------------------------------------------

MessageUpdate:
	tst.l   MessageD                   ; Check direction
	beq.s   .l2                        ; 
.l1	subi.l  #MESSAGESPEED,MessageX     ; Right to Left
	cmpi.l  #0,MessageX                ; 
	bhi.s   .l3                        ; 
	move.l  #0,MessageD                ; 
	bra.s   .l3                        ; 
.l2	add.l   #MESSAGESPEED,MessageX     ; Left to right
	cmpi.l  #MESSAGEW-SCREENWIDTH,MessageX
	bls.s   .l3                        ; 
	move.l  #1,MessageD                ; 
.l3	rts                                ; Return

*********************************************************************
* 
* JoyHelp routines
* 
*********************************************************************

JOYHELPW         EQU (480)                 ; Width
JOYHELPH         EQU (394)                 ; Height
JOYHELPX         EQU (SCREENWIDTH-JOYHELPW)/2 ; Left
JOYHELPY         EQU (SCREENHEIGHT-JOYHELPH)/2; Top

JOYTIMER         EQU (50)                  ; Timer

JOYPLUG0W        EQU (292)                 ; Width
JOYPLUG0H        EQU (43)                  ; Height
JOYPLUG0X        EQU (84)                  ; Left
JOYPLUG0Y        EQU (SCREENHEIGHT-JOYPLUG0H-4); Top

JOYPLUG1W        EQU (214)                 ; Width
JOYPLUG1H        EQU (38)                  ; Height
JOYPLUG1X        EQU (84)                  ; Left
JOYPLUG1Y        EQU (SCREENHEIGHT-JOYPLUG1H-4); Top

;--------------------------------------------------------------------

JoyHelpInit:
	lea.l    Sprite7,a0                ; Pixels
	move.l   #JOYHELPW,d0              ; Width
	mulu.l   #JOYHELPH,d0              ; Height
	CALLSAGA ConvertPixels             ; Convert
.l1	lea.l    Sprite8,a0                ; Pixels
	move.l   #JOYPLUG0W,d0             ; Width
	mulu.l   #JOYPLUG0H,d0             ; Height
	CALLSAGA ConvertPixels             ; Convert
.l2	lea.l    Sprite9,a0                ; Pixels
	move.l   #JOYPLUG1W,d0             ; Width
	mulu.l   #JOYPLUG1H,d0             ; Height
	CALLSAGA ConvertPixels             ; Convert
	rts                                ; Return

;--------------------------------------------------------------------

JoyHelpDraw:
	movem.l  d0-d3/a0-a1,-(sp)         ; Push
.l1	tst.l    ShowJoyHelp               ; Show ?
	beq.s    .l2                       ; Skip
	lea.l    Sprite7,a0                ; Src
	move.l   Screen1,a1                ; Dst
	move.l   #JOYHELPW,d0              ; SrcW
	move.l   #JOYHELPH,d1              ; SrcH
	move.l   #JOYHELPX,d2              ; DstX
	move.l   #JOYHELPY,d3              ; DstY
	move.l   #SCREENWIDTH,d4           ; PixelPerRow
	CALLSAGA BlitTransparentSprite     ; Blit
.l2	tst.l    ShowAll                   ; Show ?
	beq.s    .l4                       ; Skip
	cmp.l    #JOYTIMER,JoyPluggedT     ; Timer
	bhi.s    .l4                       ; Skip
	tst.l    JoyPlugged                ; Plugged ?
	bne.s    .l3                       ; Skip
	lea.l    Sprite8,a0                ; Src
	move.l   Screen1,a1                ; Dst
	move.l   #JOYPLUG0W,d0             ; SrcW
	move.l   #JOYPLUG0H,d1             ; SrcH
	move.l   #JOYPLUG0X,d2             ; DstX
	move.l   #JOYPLUG0Y,d3             ; DstY
	move.l   #SCREENWIDTH,d4           ; PixelPerRow
	CALLSAGA BlitTransparentSprite     ; Blit
	bra.s    .l4                       ; Skip
.l3	lea.l    Sprite9,a0                ; Src
	move.l   Screen1,a1                ; Dst
	move.l   #JOYPLUG1W,d0             ; SrcW
	move.l   #JOYPLUG1H,d1             ; SrcH
	move.l   #JOYPLUG1X,d2             ; DstX
	move.l   #JOYPLUG1Y,d3             ; DstY
	move.l   #SCREENWIDTH,d4           ; PixelPerRow
	CALLSAGA BlitTransparentSprite     ; Blit
.l4	movem.l  (sp)+,d0-d3/a0-a1         ; Pop
	rts                                ; Return

;--------------------------------------------------------------------

JoyHelpUpdate:
	add.l    #1,JoyPluggedT            ; Timer + 1
	cmp.l    #(JOYTIMER*2),JoyPluggedT ; Timer < JOYTIMER*2 ?
	blo.s    .l1                       ; Skip
	move.l   #0,JoyPluggedT            ; Timer = 0
.l1	rts                                ; Return
	
*********************************************************************
* 
* Joystick routines
* 
*********************************************************************

JoystickUpdate:
	movem.l  d0,-(sp)                  ; Push
	move.l   #0,JoyMode                ; FALSE
	move.l   #0,ShowJoyHelp            ; FALSE
	move.l   #0,JoyPlugged             ; FALSE
	move.w   $DFF220,d0                ; Read Joystick #1
.b00	btst     #0,d0                     ; Is it connected ?
	beq.s    .b16                      ; 
	move.l   #1,JoyPlugged             ; 
.b01	btst     #1,d0                     ; Btn 1
	beq.s    .b02                      ; 
	bsr      JoystickOnBtn1            ; 
.b02	btst     #2,d0                     ; Btn 2
	beq.s    .b03                      ; 
	bsr      JoystickOnBtn2            ; 
.b03	btst     #3,d0                     ; Btn 3
	beq.s    .b04                      ; 
	bsr      JoystickOnBtn3            ; 
.b04	btst     #4,d0                     ; Btn 4
	beq.s    .b09                      ; 
	bsr      JoystickOnBtn4            ; 
.b09	btst     #09,d0                    ; Btn 9
	beq.s    .b10                      ; 
	bsr      JoystickOnBtn9            ; 
.b10	btst     #10,d0                    ; Btn 10
	beq.s    .b12                      ; 
	bsr      JoystickOnBtn10           ; 
.b12	btst     #12,d0                    ; Dir Up
	beq.s    .b13                      ; 
	bsr      JoystickOnUp              ; 
.b13	btst     #13,d0                    ; Dir Down
	beq.s    .b14                      ; 
	bsr      JoystickOnDown            ; 
.b14	btst     #14,d0                    ; Dir Left
	beq.s    .b15                      ; 
	bsr      JoystickOnLeft            ; 
.b15	btst     #15,d0                    ; Dir Right
	beq.s    .b16                      ; 
	bsr      JoystickOnRight           ; 
.b16	movem.l  (sp)+,d0                  ; Pop
	rts                                ; Return

;--------------------------------------------------------------------

JoystickOnUp:
	tst.l    JoyMode                   ; Bird or Logo ?
	beq.s    .l2                       ; 
.l1	cmp.l    #LOGOSPEED,LogoTop        ; Logo
	blo.s    .l3                       ; 
	sub.l    #LOGOSPEED,LogoTop        ; 
	bra.s    .l3	                   ; 
.l2	cmp.l    #BIRDSPEED,BirdTop        ; Bird
	blo.s    .l3                       ; 
	sub.l    #BIRDSPEED,BirdTop        ; 
.l3	rts                                ; Return

;--------------------------------------------------------------------

JoystickOnDown:
	tst.l    JoyMode                   ; Bird or Logo ?
	beq.s    .l2                       ; 
.l1	cmp.l    #LOGOMAXH,LogoTop         ; Logo
	bhi.s    .l3                       ; 
	add.l    #LOGOSPEED,LogoTop        ; 
	bra.s    .l3	                   ; 
.l2	cmp.l    #BIRDMAXH,BirdTop         ; Bird
	bhi.s    .l3                       ; 
	add.l    #BIRDSPEED,BirdTop        ; 	
.l3	rts                                ; Return

;--------------------------------------------------------------------

JoystickOnLeft:
	tst.l    JoyMode                   ; Bird or Logo ?
	beq.s    .l2                       ; 
.l1	cmp.l    #LOGOSPEED,LogoLeft       ; Logo
	blo.s    .l3                       ; 
	sub.l    #LOGOSPEED,LogoLeft       ; 
	bra.s    .l3	                   ; 
.l2	cmp.l    #BIRDSPEED,BirdLeft       ; Bird
	blo.s    .l3                       ; 
	sub.l    #BIRDSPEED,BirdLeft       ; 	
.l3	rts                                ; Return

;--------------------------------------------------------------------

JoystickOnRight:
	tst.l    JoyMode                   ; Bird or Logo ?
	beq.s    .l2                       ; 
.l1	cmp.l    #LOGOMAXW,LogoLeft        ; Logo
	bhi.s    .l3                       ; 
	add.l    #LOGOSPEED,LogoLeft       ; 
	bra.s    .l3	                   ; 
.l2	cmp.l    #BIRDMAXW,BirdLeft        ; Bird
	bhi.s    .l3                       ; 
	add.l    #BIRDSPEED,BirdLeft       ; 
.l3	rts                                ; Return

;--------------------------------------------------------------------

JoystickOnBtn1:
	move.l   #1,ShowJoyHelp            ; TRUE
	rts                                ; Return

JoystickOnBtn2:
	move.l   #1,JoyMode                ; TRUE
	rts                                ; Return

JoystickOnBtn3:
	move.l   #0,ShowAll                ; FALSE
	rts                                ; Return

JoystickOnBtn4:
	move.l   #1,ShowAll                ; TRUE
	rts                                ; Return

JoystickOnBtn9:
	move.l   #1,ExitApp                ; TRUE
	rts                                ; Return

JoystickOnBtn10:
	move.l   #1,ExitApp                ; TRUE
	rts                                ; Return

*********************************************************************
* 
* Mouse routines
* 
*********************************************************************

MouseUpdate:
	btst     #6,$BFE001                ; Read CIAAPRA/FIR1
	bne.s    .l1                       ; Skip if not pressed
	move.l   #1,ExitApp                ; ExitApp = TRUE
.l1	rts                                ; Return

*********************************************************************
* 
* Variables
* 
*********************************************************************

	CNOP 0,4

ExitApp      DC.L 0
JoyMode      DC.L 0
ShowAll      DC.L 1
ShowJoyHelp  DC.L 0
JoyPlugged   DC.L 0
JoyPluggedT  DC.L 0

Screen1      DC.L 0
Screen2      DC.L 0
Screen3      DC.L 0

GFXADDR      DC.L 0
GFXMODE      DC.W 0
DMACON       DC.W $8000
INTENA       DC.W $8000

;--------------------------------------------------------------------

	CNOP 0,4

MessageD:    DC.L 0  ; Direction
MessageX:    DC.L 0  ; Position X

;--------------------------------------------------------------------

	CNOP 0,4

BackgroundA: DC.L 0  ; Position X
BackgroundB: DC.L 0  ; Position X
BackgroundC: DC.L 0  ; Position X

;--------------------------------------------------------------------

	CNOP 0,4

LogoLeft:    DC.L 0  ; Position X
LogoTop:     DC.L 0  ; Position Y

;--------------------------------------------------------------------

	CNOP 0,4

BirdLeft:    DC.L 0  ; Position X
BirdTop:     DC.L 0  ; Position Y
BirdFrameX:  DC.L 0  ; Frame XPos
BirdFrameY:  DC.L 0  ; Frame YPos
BirdFrameI:  DC.L 0  ; Frame Index
BirdFrameT:  DC.L 0  ; Frame Timer

BirdFrames:
	DC.L 0,0     ; 0
	DC.L 106,0   ; 1
	DC.L 212,0   ; 3
	DC.L 0,93    ; 4
	DC.L 106,93  ; 5
	DC.L 212,93  ; 6
	DC.L 0,186   ; 7
	DC.L 106,186 ; 8
	DC.L 212,186 ; 9
	DC.L -1
	
*********************************************************************
* 
* Gfx Assets
* 
*********************************************************************

	SECTION S1,DATA_F

	INCDIR 'RAM:Assets/'

;--------------------------------------------------------------------

Sprite1:   INCBIN 'SpriteBanner1.data'
Sprite2:   INCBIN 'SpriteBanner2.data'
Sprite3:   INCBIN 'SpriteBatBlue.data'
Sprite4:   INCBIN 'SpriteBird.data'
Sprite5:   INCBIN 'SpriteLogo.data'
Sprite6:   INCBIN 'SpriteMessage.data'
Sprite7:   INCBIN 'SpriteJoypad.data'
Sprite8:   INCBIN 'SpriteJoypadNotConnected.data'
Sprite9:   INCBIN 'SpriteJoypadConnected.data'

;--------------------------------------------------------------------

Parallax1: INCBIN 'BackgroundSky.data'
Parallax2: INCBIN 'BackgroundRocks1.data'
Parallax3: INCBIN 'BackgroundRocks2.data'
Parallax4: INCBIN 'BackgroundClouds1.data'
Parallax5: INCBIN 'BackgroundClouds2.data'

;--------------------------------------------------------------------

	IFD SOUND
Sound1s: INCBIN 'sound.aiff'
Sound1e:
	ENDC

*********************************************************************
* 
* Triple-Buffer
* 
*********************************************************************

	SECTION S2,BSS

Buffers:
	DS.B 16
	DS.B SCREENSIZE	; Buffer[-1] Safety
	DS.B SCREENSIZE	; Buffer[ a] Screen1
	DS.B SCREENSIZE	; Buffer[ b] Screen2
	DS.B SCREENSIZE	; Buffer[ c] Screen3
	DS.B SCREENSIZE	; Buffer[+1] Safety

*********************************************************************
* 
* End of file
* 
*********************************************************************

	END

; Amiga Rulez!