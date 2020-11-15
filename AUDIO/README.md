# Vampire SAGA AUDIO chipset

> (C) Copyright 2016-2020 APOLLO-Team

![Vampire Logo](V_LOGO.png)


The `SAGA` chipset embeds the legacy `PAULA` 4-channels audio chip.

The chipset extends `PAULA` to a 8-channels audio chip, including new features.

This is only available on the Vampire _standalone_ cards, in opposite to the Vampire _accelerators_ cards.

Refers to ![saga_audio.h] for useful macros and constants to include in your programs.

Refers to ![PlaySnd.c] for a very simple C-lang example.


# PAULA identifier register


* Use this register to detect the `PAULA` version.

* POTINP Bit01 to Bit07 contains the Chip ID code.

* If non-zero then the `SAGA` extended `PAULA` audio chip is available.


NAME      | ADDR | R/W | FUNCTION
--------- | ---- | --- | --------
POTINP    | 016  |  R  | Paula chip ID (0=Paula, 1=Extended)


```
UWORD GetPaulaID()
{
	/* Read Paula Chip ID (0=Paula, 1=Extended) */
	return( ( ( *( volatile UWORD* ) 0xDFF016 ) & 0xFE ) >> 1 );
}
```



# PAULA legacy registers (AUD0 to AUD3)


As reminder, below are the legacy `PAULA` specifications.


**AUDIO legacy channels**


* `PAULA` offers 4 audio channels.

* All channels handles 8-bits BigEndian PCM wavedata.


NAME      | ADDR | R/W | FUNCTION
--------- | ---- | --- | --------
AUD0      | 0A?  |  W  | Channel Number 0
AUD1      | 0B?  |  W  | Channel Number 1
AUD2      | 0C?  |  W  | Channel Number 2
AUD3      | 0D?  |  W  | Channel Number 3


**AUDIO legacy functions per channel**


* Location of waveform data, must be in `Chip RAM`.

* Length of waveform, in words (min=0, max=0xFFFF) (eg. 128KB).

* Volume (min=0, max=64).

* Period (min=0, max=0xFFFF). Example: 3546895 / 22050.


NAME      | ADDR | R/W | FUNCTION
--------- | ---- | --- | --------
AUD?LCH   | 0?0  |  W  | Audio channel ? location (high 3 bits, 5 if ECS)
AUD?LCL   | 0?2  |  W  | Audio channel ? location (low 15 bits)
AUD?LEN   | 0?4  |  W  | Audio channel ? length
AUD?PER   | 0?6  |  W  | Audio channel ? period
AUD?VOL   | 0?8  |  W  | Audio channel ? volume
AUD?DAT   | 0?A  |  W  | Audio channel ? data
AUD?      | 0?C  |     | Audio channel ? reserved
AUD?      | 0?E  |     | Audio channel ? reserved


```
/* harware/custom.h */

struct AudChannel {
    UWORD *ac_ptr;    /* pointer to start of waveform data */
    UWORD  ac_len;    /* length of waveform in words */
    UWORD  ac_per;    /* sample period */
    UWORD  ac_vol;    /* volume */
    UWORD  ac_dat;    /* sample pair */
    UWORD  ac_pad[2]; /* unused */
} aud[4];
```


**AUDIO legacy control registers**


* ADKCON bits

* DMACON bits

* INTENA bits

* INTREQ bits


NAME      | ADDR | R/W | FUNCTION
--------- | ---- | --- | --------
ADKCONR   | 010  | R   | Audio control          (Bit00 to Bit07, for AUD0 to AUD3)
ADKCON    | 09E  | W   | Audio control          (Bit00 to Bit07, for AUD0 to AUD3)
DMACONR   | 002  | R   | DMA control            (Bit00 to Bit03, for AUD0 to AUD3)
DMACON    | 096  | W   | DMA control            (Bit00 to Bit03, for AUD0 to AUD3)
INTENAR   | 01C  | R   | Interrupt enable bits  (Bit07 to Bit10, for AUD0 to AUD3)
INTENA    | 09A  | W   | Interrupt enable bits  (Bit07 to Bit10, for AUD0 to AUD3)
INTREQR   | 01E  | R   | Interrupt request bits (Bit07 to Bit10, for AUD0 to AUD3)
INTREQ    | 09C  | W   | Interrupt request bits (Bit07 to Bit10, for AUD0 to AUD3)


```
/* hardware/dmabits.h */

#define DMAB_SETCLR  (15)
#define DMAB_AUD3    (3)
#define DMAB_AUD2    (2)
#define DMAB_AUD1    (1)
#define DMAB_AUD0    (0)

#define DMAF_SETCLR  (1L<<DMAB_SETCLR)   /* Set/Clear control bit */
#define DMAF_AUD3    (1L<<DMAB_AUD3)     /* Enable DMA for Audio channel 3 */
#define DMAF_AUD2    (1L<<DMAB_AUD2)     /* Enable DMA for Audio channel 2 */
#define DMAF_AUD1    (1L<<DMAB_AUD1)     /* Enable DMA for Audio channel 1 */
#define DMAF_AUD0    (1L<<DMAB_AUD0)     /* Enable DMA for Audio channel 0 */
#define DMAF_AUDIO   (0x000F)            /* Enable DMA for ALL Audio channels */
```


```
/* hardware/intbits.h */

#define INTB_SETCLR  (15)
#define INTB_AUD3    (10)
#define INTB_AUD2    (9)
#define INTB_AUD1    (8)
#define INTB_AUD0    (7)

#define INTF_SETCLR  (1L<<INTB_SETCLR)   /* Set/Clear control bit */
#define INTF_AUD3    (1L<<INTB_AUD3)     /* Audio channel 3 block finished */
#define INTF_AUD2    (1L<<INTB_AUD2)     /* Audio channel 2 block finished */
#define INTF_AUD1    (1L<<INTB_AUD1)     /* Audio channel 1 block finished */
#define INTF_AUD0    (1L<<INTB_AUD0)     /* Audio channel 0 block finished */
```


# PAULA extended registers (AUD4 to AUD7)


Below are the `SAGA` extended `PAULA` specifications.


**AUDIO extended channels**


* `SAGA` offers 8 audio channels, all 8-bits or 16-bits PCM.

* The first 4 channels (AUD0 to AUD3) are accessibles either from the legacy audio register set (from DFF0Ax to DFF0Dx), 

* or from the new audio register set (from DFF40x to DFF43x), which allows to use the new `SAGA` features on them.

* The new additional channels (AUD4 to AUD7) must be accessed from the new audio register set (from DFF44x to DFF47x).


NAME      | ADDR | R/W | FUNCTION
--------- | ---- | --- | --------
SAUD0     | 40?  |  W  | Channel Number 0
SAUD1     | 41?  |  W  | Channel Number 1
SAUD2     | 42?  |  W  | Channel Number 2
SAUD3     | 43?  |  W  | Channel Number 3
SAUD4     | 44?  |  W  | Channel Number 4
SAUD5     | 45?  |  W  | Channel Number 5
SAUD6     | 46?  |  W  | Channel Number 6
SAUD7     | 47?  |  W  | Channel Number 7


```
/* Useful macros */

#define CUSTOM_REGBASE       (0xDFF000)

#define SAUD_REGBASE(CH)     (CUSTOM_REGBASE + 0x400 + (((CH) & 0x0F) << 4))

#define SAUD_REG(CH, IDX)    (SAUD_REGBASE(CH) + (IDX))

#define SAUD_PTR(CH)         SAUD_REG(CH, 0x00) /* location */
#define SAUD_LEN(CH)         SAUD_REG(CH, 0x04) /* length   */
#define SAUD_VOL(CH)         SAUD_REG(CH, 0x08) /* volume   */
#define SAUD_CTL(CH)         SAUD_REG(CH, 0x0A) /* control  */
#define SAUD_PER(CH)         SAUD_REG(CH, 0x0C) /* period   */
```


**AUDIO extended functions per channel**


* Location of waveform data is a 32-bits address, can be in `Chip RAM` or in `Fast RAM`.

* Length of waveform, in words, is also 32-bits (min=0, max=0x00FFFFFF) (eg. 32MB for 8-bits PCM, 64MB for 16-bits PCM).

* Volume is 8.8 for Left and Right (min=0.0, max=128.128).

* Control bits for the 8/16-bits, Continuous/OneShot, and Mono/Stereo modes.

* Period (min=2, max=0xFFFF). Example: 3546895 / 22050.


NAME      | ADDR | R/W | FUNCTION
--------- | ---- | --- | --------
SAUD?PTR  | 4?0  |  W  | Audio channel ? location (32-bits)
SAUD?LEN  | 4?4  |  W  | Audio channel ? length (32-bits)
SAUD?VOL  | 4?8  |  W  | Audio channel ? volume
SAUD?CTL  | 4?A  |  W  | Audio channel ? control bits
SAUD?PER  | 4?C  |  W  | Audio channel ? period
SAUD?     | 4?E  |     | Audio channel ? reserved


```
/* Example using macros */

#include "vampire/saga_audio.h"

void example(void)
{
	for(int channel = 0; channel < MAX_CHANNELS; channel++)
	{
		SAUD_PTR( channel ) = ( ULONG*) ( bufptr );
		SAUD_LEN( channel ) = ( ULONG ) ( buflen >> 1 );
		SAUD_VOL( channel ) = ( UWORD ) ( ( 128 << 8 ) | 128 );
		SAUD_CTL( channel ) = ( UWORD ) ( SAUDCTLF_16BITS | SAUDCTLF_ONESHOT );
		SAUD_PER( channel ) = ( UWORD ) ( PAULA_CLOCK / 22050 );
	}
}
```


```
/* Example using a structure */

#include "vampire/saga_audio.h"

void example(void)
{
	struct SAudChannel* saud = (struct SAudChannel*) SAUD0LOC;
	
	for(int channel = 0; channel < MAX_CHANNELS; channel++)
	{
		saud[ channel ].ac_ptr = ( ULONG*) ( bufptr );
		saud[ channel ].ac_len = ( ULONG ) ( buflen >> 1 );
		saud[ channel ].ac_vol = ( UWORD ) ( ( 128 << 8 ) | 128 );
		saud[ channel ].ac_ctl = ( UWORD ) ( SAUDCTLF_16BITS | SAUDCTLF_ONESHOT );
		saud[ channel ].ac_per = ( UWORD ) ( PAULA_CLOCK / 22050 );
	}
}
```


**AUDIO extended control registers**


* New DMACON2 bits

* New INTENA2 bits

* New INTREQ2 bits


NAME      | ADDR | R/W | FUNCTION
--------- | ---- | --- | --------
ADKCONR2  | 210  |  R  | Audio control (Bit00 to Bit07, for AUD4 to AUD7) _UNUSED_
ADKCON2   | 29E  |  W  | Audio control (Bit00 to Bit07, for AUD4 to AUD7) _UNUSED_
DMACONR2  | 202  |  R  | DMA control (Bit00 to Bit03, for AUD4 to AUD7)
DMACON2   | 296  |  W  | DMA control (Bit00 to Bit03, for AUD4 to AUD7)
INTENAR2  | 21C  |  R  | Interrupt enable bits (Bit00 to Bit03, for AUD4 to AUD7)
INTENA2   | 29A  |  W  | Interrupt enable bits (Bit00 to Bit03, for AUD4 to AUD7)
INTREQR2  | 21E  |  R  | Interrupt request bits (Bit00 to Bit03, for AUD4 to AUD7)
INTREQ2   | 29C  |  W  | Interrupt request bits (Bit00 to Bit03, for AUD4 to AUD7)


```
/* vampire/saga_audio.h */

#define DMA2B_SETCLR         (15)
#define DMA2B_AUD7           (3)
#define DMA2B_AUD6           (2)
#define DMA2B_AUD5           (1)
#define DMA2B_AUD4           (0)

#define DMA2F_SETCLR         (1L<<DMA2B_SETCLR)   /* Set/Clear control bit */
#define DMA2F_AUD7           (1L<<DMA2B_AUD7)     /* Enable DMA for Audio channel 7 */
#define DMA2F_AUD6           (1L<<DMA2B_AUD6)     /* Enable DMA for Audio channel 6 */
#define DMA2F_AUD5           (1L<<DMA2B_AUD5)     /* Enable DMA for Audio channel 5 */
#define DMA2F_AUD4           (1L<<DMA2B_AUD4)     /* Enable DMA for Audio channel 4 */
#define DMA2F_AUDIO          (0x000F)             /* Enable DMA for ALL Audio channels */
```

```
/* vampire/saga_audio.h */

#define INT2B_SETCLR         (15)
#define INT2B_AUD7           (3)
#define INT2B_AUD6           (2)
#define INT2B_AUD5           (1)
#define INT2B_AUD4           (0)

#define INT2F_SETCLR         (1L<<INT2B_SETCLR)   /* Set/Clear control bit */
#define INT2F_AUD7           (1L<<INT2B_AUD7)     /* Audio channel 7 block finished */
#define INT2F_AUD6           (1L<<INT2B_AUD6)     /* Audio channel 6 block finished */
#define INT2F_AUD5           (1L<<INT2B_AUD5)     /* Audio channel 5 block finished */
#define INT2F_AUD4           (1L<<INT2B_AUD4)     /* Audio channel 4 block finished */
```


```
#include "vampire/saga_audio.h"

#define CLR16(a, b) *(volatile UWORD*) (a) = (UWORD) (0x0000 + (b))
#define SET16(a, b) *(volatile UWORD*) (a) = (UWORD) (0x8000 + (b))

void example(void)
{
.	CLR16( DMACON1, DMAF_AUDIO );
	CLR16( DMACON2, DMA2F_AUDIO );
	
	SET16( DMACON1, DMAF_AUDIO );
	SET16( DMACON2, DMA2F_AUDIO );
}
```


# PAULA interrupt vectors (AUD0 to AUD7)


* AMIGA interrupts auto-vectors table.


![Vector Table](VectorTable.png)



