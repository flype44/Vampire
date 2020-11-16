# Vampire SAGA AUDIO chipset

> (C) Copyright 2016-2020 APOLLO-Team

This article describes all the `SAGA` audio features, and is still in **Work In Progress** status.

![Vampire Logo](../ASSETS/V_LOGO.png)

# License

This documentation and associated files are licensed under the [Mozilla Public License 2.0](../LICENSE)

Permissions of this weak copyleft license are conditioned on making available source code of licensed files and modifications of those files under the same license (or in certain cases, one of the GNU licenses). Copyright and license notices must be preserved. Contributors provide an express grant of patent rights. However, a larger work using the licensed work may be distributed under different terms and without source code for files added in the larger work.

# Documentation

Written and maintained by `flype`, with the great help from the `APOLLO-Team` members. It is intended for developers who plan to use the `Vampire` audio chip for their programs or games, and to help the community to write drivers. All specifications mentioned in this documentation always refers to the latest version. Be sure to use the latest `Vampire` core version in your programs (see below).

# Core releases

Latest `Vampire` cores are officially distributed from [here](https://www.apollo-accelerators.com/wiki/doku.php/start#core_and_software_updates). 

Additionally, Beta cores are regularly distributed in the official `ApolloTeam` Discord Channel [here](https://discord.gg/bM684VW).

# Introduction

The `SAGA` chipset embeds the legacy `PAULA` 4-channels audio chip.

It extends `PAULA` to a **8-channels** audio chip.

It supports 8-bits and **16-bits** PCM wavedata.

It supports replaying from either Chip RAM or **Fast RAM**.

It supports much longer samples, up to **32MB** (in 8-bits) and **64MB** (in 16-bits).

It also includes some other interesting **new features** (OneShot mode, Stereo mode).

It is only available on the `Vampire` **standalone** cards, contrary to the `Vampire` **accelerators** cards.

# PAULA specifications

As a reminder, below are the legacy `PAULA` specifications.

## AUDIO chip identifier register

### Sysnopis

Use this register to detect the `PAULA` version.

[POTINP](http://amigadev.elowar.com/read/ADCD_2.1/Hardware_Manual_guide/node018B.html) Bit01 to Bit07 contains the Chip ID code.

If non-zero then the `SAGA` extended `PAULA` audio chip is available.

### Registers

NAME      | ADDR | R/W | FUNCTION
--------- | ---- | --- | --------
[POTINP](http://amigadev.elowar.com/read/ADCD_2.1/Hardware_Manual_guide/node018B.html) | 016  |  R  | Paula chip ID (0=Paula, 1=Extended)

### Examples

```c
UWORD GetPaulaID()
{
	/* Read Paula Chip ID (0=Paula, 1=Extended) */
	return( ( ( *( volatile UWORD* ) 0xDFF016 ) & 0xFE ) >> 1 );
}
```

## AUDIO legacy channels

### Sysnopis

`PAULA` offers 4 audio channels.

All channels are 8-bits, BigEndian, PCM wavedata.

### Registers

NAME      | ADDR | R/W | FUNCTION
--------- | ---- | --- | --------
AUD0      | 0A?  |  W  | Channel Number 0
AUD1      | 0B?  |  W  | Channel Number 1
AUD2      | 0C?  |  W  | Channel Number 2
AUD3      | 0D?  |  W  | Channel Number 3

## AUDIO legacy functions per channel

### Sysnopis

Location of waveform data, must be located in `Chip RAM`.

Length of waveform, in words (min=0, max=0xFFFF) (eg. 128KB).

Period (min=0, max=0xFFFF). Example: 3546895 / 22050.

Volume (min=0, max=64).

### Registers

NAME      | ADDR | R/W | FUNCTION
--------- | ---- | --- | --------
[AUDxLCH](http://amigadev.elowar.com/read/ADCD_2.1/Hardware_Manual_guide/node0014.html) | 0?0  |  W  | Audio channel ? location (high 3 bits, 5 if ECS)
[AUDxLCL](http://amigadev.elowar.com/read/ADCD_2.1/Hardware_Manual_guide/node0014.html) | 0?2  |  W  | Audio channel ? location (low 15 bits)
[AUDxLEN](http://amigadev.elowar.com/read/ADCD_2.1/Hardware_Manual_guide/node0015.html) | 0?4  |  W  | Audio channel ? length
[AUDxPER](http://amigadev.elowar.com/read/ADCD_2.1/Hardware_Manual_guide/node0016.html) | 0?6  |  W  | Audio channel ? period
[AUDxVOL](http://amigadev.elowar.com/read/ADCD_2.1/Hardware_Manual_guide/node0017.html) | 0?8  |  W  | Audio channel ? volume
[AUDxDAT](http://amigadev.elowar.com/read/ADCD_2.1/Hardware_Manual_guide/node0013.html) | 0?A  |  W  | Audio channel ? data

### Examples

[custom.h](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node00CD.html)

```c
struct AudChannel {
    UWORD *ac_ptr;    /* pointer to start of waveform data */
    UWORD  ac_len;    /* length of waveform in words */
    UWORD  ac_per;    /* sample period */
    UWORD  ac_vol;    /* volume */
    UWORD  ac_dat;    /* sample pair */
    UWORD  ac_pad[2]; /* unused */
} aud[4];
```

## AUDIO legacy control registers

### Sysnopis

Control modulation.

Start/Stop audio channels.

Be notified when a sound block is finished.

### Registers

NAME      | ADDR | R/W | FUNCTION
--------- | ---- | --- | --------
[ADKCONR](http://amigadev.elowar.com/read/ADCD_2.1/Hardware_Manual_guide/node0012.html) | 010  | R   | Audio control          (Bit00 to Bit07, for AUD0 to AUD3)
[ADKCON](http://amigadev.elowar.com/read/ADCD_2.1/Hardware_Manual_guide/node0012.html)  | 09E  | W   | Audio control          (Bit00 to Bit07, for AUD0 to AUD3)
[DMACONR](http://amigadev.elowar.com/read/ADCD_2.1/Hardware_Manual_guide/node002F.html) | 002  | R   | DMA control            (Bit00 to Bit03, for AUD0 to AUD3)
[DMACON](http://amigadev.elowar.com/read/ADCD_2.1/Hardware_Manual_guide/node002F.html)  | 096  | W   | DMA control            (Bit00 to Bit03, for AUD0 to AUD3)
[INTENAR](http://amigadev.elowar.com/read/ADCD_2.1/Hardware_Manual_guide/node0036.html) | 01C  | R   | Interrupt enable bits  (Bit07 to Bit10, for AUD0 to AUD3)
[INTENA](http://amigadev.elowar.com/read/ADCD_2.1/Hardware_Manual_guide/node0036.html)  | 09A  | W   | Interrupt enable bits  (Bit07 to Bit10, for AUD0 to AUD3)
[INTREQR](http://amigadev.elowar.com/read/ADCD_2.1/Hardware_Manual_guide/node0037.html) | 01E  | R   | Interrupt request bits (Bit07 to Bit10, for AUD0 to AUD3)
[INTREQ](http://amigadev.elowar.com/read/ADCD_2.1/Hardware_Manual_guide/node0037.html)  | 09C  | W   | Interrupt request bits (Bit07 to Bit10, for AUD0 to AUD3)

### Examples

[adkbits.h](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node00C6.html)

```c
#define ADKB_SETCLR  (15)             
#define ADKB_USE3PN  (7)              
#define ADKB_USE2P3  (6)              
#define ADKB_USE1P2  (5)              
#define ADKB_USE0P1  (4)              
#define ADKB_USE3VN  (3)              
#define ADKB_USE2V3  (2)              
#define ADKB_USE1V2  (1)              
#define ADKB_USE0V1  (0)              

#define ADKF_SETCLR  (1L<<ADKB_SETCLR)   /* Standard set/clear bit */
#define ADKF_USE3PN  (1L<<ADKB_USE3PN)   /* Use aud chan 3 to modulate period of ?? */
#define ADKF_USE2P3  (1L<<ADKB_USE2P3)   /* Use aud chan 2 to modulate period of 3  */
#define ADKF_USE1P2  (1L<<ADKB_USE1P2)   /* Use aud chan 1 to modulate period of 2  */
#define ADKF_USE0P1  (1L<<ADKB_USE0P1)   /* Use aud chan 0 to modulate period of 1  */
#define ADKF_USE3VN  (1L<<ADKB_USE3VN)   /* Use aud chan 3 to modulate volume of ?? */
#define ADKF_USE2V3  (1L<<ADKB_USE2V3)   /* Use aud chan 2 to modulate volume of 3  */
#define ADKF_USE1V2  (1L<<ADKB_USE1V2)   /* Use aud chan 1 to modulate volume of 2  */
#define ADKF_USE0V1  (1L<<ADKB_USE0V1)   /* Use aud chan 0 to modulate volume of 1  */
```

### Examples

[dmabits.h](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node00C8.html)

```c
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

### Examples

[intbits.h](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node00CE.html)

```c
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

## AUDIO extended channels

### Sysnopis

The first 4 channels (`AUD0` to `AUD3`) are accessibles either from the legacy audio register set (from `DFF0Ax` to `DFF0Dx`), 

or from the **new audio register set** (from `DFF40x` to `DFF43x`), which allows to use the new `SAGA` features on them.

The new additional channels (`AUD4` to `AUD7`) must be accessed from the **new audio register set** (from `DFF44x` to `DFF47x`).

### Registers

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

### Examples

```c
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

## AUDIO extended functions per channel

### Sysnopis

Location of waveform data is a 32-bits address, can be in `Chip RAM` or in `Fast RAM`.

Length of waveform, in words, is also 32-bits (min=0, max=0x00FFFFFF) (eg. 32MB for 8-bits PCM, 64MB for 16-bits PCM).

Volume is 8.8 for Left and Right (min=0.0, max=128.128).

Control bits for the 8/16-bits, Continuous/OneShot, and Mono/Stereo modes.

Period (min=2, max=0xFFFF). Example: 3546895 / 22050.

### Registers

NAME      | ADDR | R/W | FUNCTION
--------- | ---- | --- | --------
SAUD?PTR  | 4?0  |  W  | Audio channel ? location (32-bits)
SAUD?LEN  | 4?4  |  W  | Audio channel ? length (32-bits)
SAUD?VOL  | 4?8  |  W  | Audio channel ? volume
SAUD?CTL  | 4?A  |  W  | Audio channel ? control bits
SAUD?PER  | 4?C  |  W  | Audio channel ? period
SAUD?     | 4?E  |     | Audio channel ? reserved

### Examples

```c
struct SAudChannel {
    ULONG *sac_ptr;    /* location */
    ULONG  sac_len;    /* length   */
    UWORD  sac_vol;    /* volume   */
    UWORD  sac_ctl;    /* control  */
    UWORD  sac_per;    /* period   */
    UWORD  sac_pad;    /* unused   */
} saud[ MAX_CHANNELS ];
```

### Examples

```c
#define SAUDCTLB_ENDIANNESS  (4) /* 0=BigEndian,  1=LittleEndian [UNIMPLEMENTED] */
#define SAUDCTLB_INTERLEAVED (3) /* 0=Contiguous, 1=Interleaved  [UNIMPLEMENTED] */
#define SAUDCTLB_STEREO      (2) /* 0=Mono,       1=Stereo       [IMPLEMENTED]   */
#define SAUDCTLB_ONESHOT     (1) /* 0=Continuous, 1=OneShot      [IMPLEMENTED]   */
#define SAUDCTLB_16BITS      (0) /* 0=8-bits,     1=16-bits      [IMPLEMENTED]   */

#define SAUDCTLF_ENDIANNESS  (1L<<SAUDCTLB_ENDIANNESS)
#define SAUDCTLF_INTERLEAVED (1L<<SAUDCTLB_INTERLEAVED)
#define SAUDCTLF_STEREO      (1L<<SAUDCTLB_STEREO)
#define SAUDCTLF_ONESHOT     (1L<<SAUDCTLB_ONESHOT)
#define SAUDCTLF_16BITS      (1L<<SAUDCTLB_16BITS)
```

### Examples

```c
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

### Examples

```c
/* Example using a structure */

#include "vampire/saga_audio.h"

void example(void)
{
	struct SAudChannel* saud = (struct SAudChannel*) SAUD0LOC;
	
	for(int channel = 0; channel < MAX_CHANNELS; channel++)
	{
		saud[ channel ].sac_ptr = ( ULONG*) ( bufptr );
		saud[ channel ].sac_len = ( ULONG ) ( buflen >> 1 );
		saud[ channel ].sac_vol = ( UWORD ) ( ( 128 << 8 ) | 128 );
		saud[ channel ].sac_ctl = ( UWORD ) ( SAUDCTLF_16BITS | SAUDCTLF_ONESHOT );
		saud[ channel ].sac_per = ( UWORD ) ( PAULA_CLOCK / 22050 );
	}
}
```

## AUDIO extended control registers

### Sysnopis

New ADKCON2 bits

New DMACON2 bits

New INTENA2 bits

New INTREQ2 bits

### Registers

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

### Examples

```c
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

### Examples

```c
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

### Examples

```c
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

AMIGA interrupts auto-vectors table [More informations](http://amigadev.elowar.com/read/ADCD_2.1/Hardware_Manual_guide/node016F.html)

![Vector Table](VectorTable.png)

# Related ressources files

> Refers to [PlaySnd.c](PlaySnd.c) for a very simple C-lang example.

> Refers to [saga_audio.h](saga_audio.h) for useful C-lang macros and constants to include in your programs.

# Frequently Asked Question

> Is it or will it be available on the `Vampire` (V2) accelerators ?

For now it is only available on the `Vampire` (V4) standalone board.

It is eventually planned for the V2 boards within a `GOLD3` core.

Main difficulties are : Time, FPGA size on the V2, and finish the V4 core first.

There is no date announced, it all depends on the time the `APOLLO-Team` can spend on this project.

> It was announced 16 channels, the documentation mentions 8 channels, why ?

The specifications were reconsidered for 2 main reasons.

First, implementing 16 channels on the current FPGA appears to be more hungry than reasonable.

Second, the `Vampire` CPU is more than powerful enough to use software audio mixing (maybe only 5-10% of the CPU, if done right).

> Are there software that already use the new audio chip ?

There was a **EaglePlayer** plugin, by Bax (year 2018), but it is obsolete since the audio chip redesign.

There is the Amiga **MilkyTracker** port, by Neoman (year 2020), which fully implements the 8-channels, and 16-bits modes.

There are plans for a **AHI driver** (any help would be appreciated).

There are plans for a **audio.device** (any help would be appreciated).

There are plans for a **ptreplay** routine (any help would be appreciated).
