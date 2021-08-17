	IFND	SAGA_CUSTOM_I
SAGA_CUSTOM_I	SET	1

CALLSAGA MACRO
	jsr _SAGALIB_\1
	ENDM

SAGA_DMACONR        EQU $DFF002                  ; (R  ) (16-bit)
SAGA_DMACON         EQU $DFF096                  ; (  W) (16-bit)
SAGA_INTENAR        EQU $DFF01C                  ; (R  ) (16-bit)
SAGA_INTENA         EQU $DFF09A                  ; (  W) (16-bit)

SAGA_DMACON2R       EQU $DFF202                  ; (R  ) (16-bit)
SAGA_DMACON2        EQU $DFF296                  ; (  W) (16-bit)
SAGA_INTENA2R       EQU $DFF21C                  ; (R  ) (16-bit)
SAGA_INTENA2        EQU $DFF29A                  ; (  W) (16-bit)

SAGA_MOUSEX         EQU $DFF1D0                  ; (  W) (16-bit)
SAGA_MOUSEY         EQU $DFF1D2                  ; (  W) (16-bit)
SAGA_GFXADDR        EQU $DFF1EC                  ; (R W) (32-bit)
SAGA_GFXMODE        EQU $DFF1F4                  ; (R W) (16-bit)
SAGA_GFXMODULO      EQU $DFF1E6                  ; (  W) (16-bit)

SAGA_JOYP1          EQU $DFF220                  ; (R  ) (16-bit)
SAGA_JOYP2          EQU $DFF222                  ; (R  ) (16-bit)
SAGA_JOYP3          EQU $DFF224                  ; (R  ) (16-bit)
SAGA_JOYP4          EQU $DFF226                  ; (R  ) (16-bit)

SAGA_AUD0LC         EQU $DFF400                  ; (  W) (32-bit)
SAGA_AUD0LEN        EQU $DFF404                  ; (  W) (32-bit)
SAGA_AUD0VOL        EQU $DFF408                  ; (  W) (16-bit)
SAGA_AUD0CTL        EQU $DFF40A                  ; (  W) (16-bit)
SAGA_AUD0PER        EQU $DFF40C                  ; (  W) (16-bit)

SAGA_JOYB_PLUGGED   EQU (0)                      ; PLUG
SAGA_JOYB_A         EQU (1)                      ; FIRE1
SAGA_JOYB_B         EQU (2)                      ; FIRE2
SAGA_JOYB_X         EQU (3)                      ; FIRE3
SAGA_JOYB_Y         EQU (4)                      ; FIRE4
SAGA_JOYB_LB        EQU (5)                      ; FIRE5
SAGA_JOYB_RB        EQU (6)                      ; FIRE6
SAGA_JOYB_LT        EQU (7)                      ; FIRE7
SAGA_JOYB_RT        EQU (8)                      ; FIRE8
SAGA_JOYB_BACK      EQU (9)                      ; BACK
SAGA_JOYB_START     EQU (10)                     ; START
SAGA_JOYB_FREE      EQU (11)                     ; RESERVED
SAGA_JOYB_UP        EQU (12)                     ; UP
SAGA_JOYB_DOWN      EQU (13)                     ; DOWN
SAGA_JOYB_LEFT      EQU (14)                     ; LEFT
SAGA_JOYB_RIGHT     EQU (15)                     ; RIGHT

SAGA_JOYF_PLUGGED   EQU (1<<SAGA_JOYB_PLUGGED)   ; PLUG
SAGA_JOYF_A         EQU (1<<SAGA_JOYB_A)         ; FIRE1
SAGA_JOYF_B         EQU (1<<SAGA_JOYB_B)         ; FIRE2
SAGA_JOYF_X         EQU (1<<SAGA_JOYB_X)         ; FIRE3
SAGA_JOYF_Y         EQU (1<<SAGA_JOYB_Y)         ; FIRE4
SAGA_JOYF_LB        EQU (1<<SAGA_JOYB_LB)        ; FIRE5
SAGA_JOYF_RB        EQU (1<<SAGA_JOYB_RB)        ; FIRE6
SAGA_JOYF_LT        EQU (1<<SAGA_JOYB_LT)        ; FIRE7
SAGA_JOYF_RT        EQU (1<<SAGA_JOYB_RT)        ; FIRE8
SAGA_JOYF_BACK      EQU (1<<SAGA_JOYB_BACK)      ; BACK
SAGA_JOYF_START     EQU (1<<SAGA_JOYB_START)     ; START
SAGA_JOYF_FREE      EQU (1<<SAGA_JOYB_FREE)      ; RESERVED
SAGA_JOYF_UP        EQU (1<<SAGA_JOYB_UP)        ; UP
SAGA_JOYF_DOWN      EQU (1<<SAGA_JOYB_DOWN)      ; DOWN
SAGA_JOYF_LEFT      EQU (1<<SAGA_JOYB_LEFT)      ; LEFT
SAGA_JOYF_RIGHT     EQU (1<<SAGA_JOYB_RIGHT)     ; RIGHT

	ENDC	; SAGA_CUSTOM_I
