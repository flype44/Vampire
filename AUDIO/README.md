# Vampire SAGA AUDIO chipset

> (C) Copyright 2016-2020 APOLLO-Team

![Vampire Logo](V_LOGO.png)


**AUDIO specifications**

The SAGA chipset embeds the legacy PAULA 4-channels audio chip.

And it extends it to 8-channels, including new features.


# PAULA identifier register

Use this register to detect the PAULA version.
Bit01 to Bit07 contains the Chip ID code.

NAME      | ADDR | R/W | FUNCTION
--------- | ---- | --- | --------
POTINP1   | 016  | R   | Paula chip ID (0=Paula, 1=SAGA)

```
UWORD ChipID = ( ( ( *( volatile UWORD* ) 0xDFF016 ) & 0xFE ) >> 1 );
```

# PAULA legacy registers (AUD0 to AUD3)

* PAULA offers 4 AUDIO channels, all 8-bits.

* The wavedata location must be in Chip RAM.


**AUDIO legacy channels**

NAME      | ADDR | R/W | FUNCTION
--------- | ---- | --- | --------
AUD0      | 0A?  | W   | Channel Number 0
AUD1      | 0B?  | W   | Channel Number 1
AUD2      | 0C?  | W   | Channel Number 2
AUD3      | 0D?  | W   | Channel Number 3

**AUDIO legacy functions per channel**

NAME      | ADDR | R/W | FUNCTION
--------- | ---- | --- | --------
AUD?LCH   | 0?0  | W   | Audio channel ? location (high 3 bits, 5 if ECS)
AUD?LCL   | 0?2  | W   | Audio channel ? location (low 15 bits)
AUD?LEN   | 0?4  | W   | Audio channel ? length
AUD?PER   | 0?6  | W   | Audio channel ? period
AUD?VOL   | 0?8  | W   | Audio channel ? volume
AUD?DAT   | 0?A  | W   | Audio channel ? data
AUD?---   | 0?C  |     | Audio channel ? reserved
AUD?---   | 0?E  |     | Audio channel ? reserved

**AUDIO legacy control registers**

NAME      | ADDR | R/W | FUNCTION
--------- | ---- | --- | --------
DMACONR   | 002  | R   | DMA control            (Bit00 to Bit03)
DMACON    | 096  | W   | DMA control            (Bit00 to Bit03)
ADKCONR   | 010  | R   | Audio control          (Bit00 to Bit07)
ADKCON    | 09E  | W   | Audio control          (Bit00 to Bit07)
INTENAR   | 01C  | R   | Interrupt enable bits  (Bit07 to Bit10)
INTENA    | 09A  | W   | Interrupt enable bits  (Bit07 to Bit10)
INTREQR   | 01E  | R   | Interrupt request bits (Bit07 to Bit10)
INTREQ    | 09C  | W   | Interrupt request bits (Bit07 to Bit10)


# PAULA extended registers (AUD4 to AUD7)

* SAGA offers 8 AUDIO channels, all 8-bits or 16-bits.

* The wavedata location can be in Chip RAM or in Fast RAM.

* The first 4 channels (AUD0 to AUD3) are accessibles either from the legacy audio register set (from DFF0Ax to DFF0Dx), or from the new audio register set (from DFF40x to DFF43x).

* The new additional channels (AUD4 to AUD7) must be accessed from the new audio register set (from DFF44x to DFF47x).


**AUDIO extended channels**

NAME      | ADDR | R/W | FUNCTION
--------- | ---- | --- | --------
SAUD0     | 40?  | W   | Channel Number 0
SAUD1     | 41?  | W   | Channel Number 1
SAUD2     | 42?  | W   | Channel Number 2
SAUD3     | 43?  | W   | Channel Number 3
SAUD4     | 44?  | W   | Channel Number 4
SAUD5     | 45?  | W   | Channel Number 5
SAUD6     | 46?  | W   | Channel Number 6
SAUD7     | 47?  | W   | Channel Number 7

**AUDIO extended functions per channel**

NAME      | ADDR | R/W | FUNCTION
--------- | ---- | --- | --------
SAUD?LCH  | 4?0  | W   | Audio channel ? location (high 15 bits)
SAUD?LCL  | 4?2  | W   | Audio channel ? location (low 15 bits)
SAUD?LENH | 4?4  | W   | Audio channel ? length (high 15 bits)
SAUD?LENL | 4?6  | W   | Audio channel ? length (low 15 bits)
SAUD?VOL  | 4?8  | W   | Audio channel ? volume (8.8) (L/R)
SAUD?CTL  | 4?A  | W   | Audio channel ? control (Bit0:16bit, Bit1:OneShot, Bit2:Stereo)
SAUD?PER  | 4?C  | W   | Audio channel ? period (16bits)
SAUD?---  | 4?E  |     | Audio channel ? reserved

**AUDIO extended control registers**

NAME      | ADDR | R/W | FUNCTION
--------- | ---- | --- | --------
DMACONR2  | 202  | R   | Control AUD DMA  read  (Bit00 to Bit07)
DMACON2   | 296  | W   | Control AUD DMA  write (Bit00 to Bit07)
ADKCONR2  | 010  | R   | Audio control          (Bit00 to Bit07)
ADKCON2   | 09E  | W   | Audio control          (Bit00 to Bit07)
INTENAR2  | 21C  | R   | Request INT BITS read  (Bit00 to Bit07)
INTENA2   | 29A  | W   | Request INT BITS write (Bit00 to Bit07)
INTREQR2  | 21E  | R   | Request INT BITS read  (Bit00 to Bit07)
INTREQ2   | 29C  | W   | Request INT BITS write (Bit00 to Bit07)


