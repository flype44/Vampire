# Vampire SAGA AUDIO chipset

> (C) Copyright 2016-2020 APOLLO-Team

![Vampire Logo](V_LOGO.png)


AUDIO specifications

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

# PAULA legacy registers

PAULA offers 4 AUDIO channels, all 8-bits.
The wavedata location must be in Chip RAM.

# PAULA control registers (AUD0 to AUD3)

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

NAME      | ADDR | R/W | FUNCTION
--------- | ---- | --- | --------
AUD0      | 0A?  | W   | Channel Number 0
AUD1      | 0B?  | W   | Channel Number 1
AUD2      | 0C?  | W   | Channel Number 2
AUD3      | 0D?  | W   | Channel Number 3

NAME      | ADDR | R/W | FUNCTION
--------- | ---- | --- | --------
AUD?LCH   | 0?0  | W   | Audio channel ? location (high 3 bits, 5 if ECS)
AUD?LCL   | 0?2  | W   | Audio channel ? location (low 15 bits)
AUD?LEN   | 0?4  | W   | Audio channel ? length
AUD?PER   | 0?6  | W   | Audio channel ? period
AUD?VOL   | 0?8  | W   | Audio channel ? volume
AUD?DAT   | 0?A  | W   | Audio channel ? data
          | 0?C  |     | Audio channel ? reserved
          | 0?E  |     | Audio channel ? reserved

NAME      | ADDR | R/W | FUNCTION
--------- | ---- | --- | --------
AUD0LCH   | 0A0  | W   | Audio channel 0 location (high 3 bits, 5 if ECS)
AUD0LCL   | 0A2  | W   | Audio channel 0 location (low 15 bits)
AUD0LEN   | 0A4  | W   | Audio channel 0 length
AUD0PER   | 0A6  | W   | Audio channel 0 period
AUD0VOL   | 0A8  | W   | Audio channel 0 volume
AUD0DAT   | 0AA  | W   | Audio channel 0 data
          | 0AC  |     | Audio channel 0 reserved
          | 0AE  |     | Audio channel 0 reserved
AUD1LCH   | 0B0  | W   | Audio channel 1 location (high 3 bits)
AUD1LCL   | 0B2  | W   | Audio channel 1 location (low 15 bits)
AUD1LEN   | 0B4  | W   | Audio channel 1 length
AUD1PER   | 0B6  | W   | Audio channel 1 period
AUD1VOL   | 0B8  | W   | Audio channel 1 volume
AUD1DAT   | 0BA  | W   | Audio channel 1 data
          | 0BC  |     | Audio channel 1 reserved
          | 0BE  |     | Audio channel 1 reserved
AUD2LCH   | 0C0  | W   | Audio channel 2 location (high 3 bits)
AUD2LCL   | 0C2  | W   | Audio channel 2 location (low 15 bits)
AUD2LEN   | 0C4  | W   | Audio channel 2 length
AUD2PER   | 0C6  | W   | Audio channel 2 period
AUD2VOL   | 0C8  | W   | Audio channel 2 volume
AUD2DAT   | 0CA  | W   | Audio channel 2 data
          | 0CC  |     | Audio channel 2 reserved
          | 0CE  |     | Audio channel 2 reserved
AUD3LCH   | 0D0  | W   | Audio channel 3 location (high 3 bits)
AUD3LCL   | 0D2  | W   | Audio channel 3 location (low 15 bits)
AUD3LEN   | 0D4  | W   | Audio channel 3 length
AUD3PER   | 0D6  | W   | Audio channel 3 period
AUD3VOL   | 0D8  | W   | Audio channel 3 volume
AUD3DAT   | 0DA  | W   | Audio channel 3 data
          | 0DC  |     | Audio channel 3 reserved
          | 0DE  |     | Audio channel 3 reserved


# PAULA extended registers (AUD4 to AUD7)

SAGA offers 8 AUDIO channels, all 8-bits or 16-bits.
The wavedata location can be in Chip RAM or in Fast RAM.

The first 4 channels (AUD0 to AUD3) are accessibles either from the legacy audio register set (from DFF0Ax to DFF0Dx), or from the new audio register set (from DFF40x to DFF43x).
The new additional channels (AUD4 to AUD7) must be accessed from the new audio register set (from DFF44x to DFF47x).

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

NAME      | ADDR | R/W | FUNCTION
--------- | ---- | --- | --------
SAUD?LCH  | 4?0  | W   | Audio channel ? location (high 15 bits)
SAUD?LCL  | 4?2  | W   | Audio channel ? location (low 15 bits)
SAUD?LENH | 4?4  | W   | Audio channel ? length (high 15 bits)
SAUD?LENL | 4?6  | W   | Audio channel ? length (low 15 bits)
SAUD?VOL  | 4?8  | W   | Audio channel ? volume (8.8) (L/R)
SAUD?CTL  | 4?A  | W   | Audio channel ? control (Bit0:16bit, Bit1:OneShot, Bit2:Stereo)
SAUD?PER  | 4?C  | W   | Audio channel ? period (16bits)
          | 4?E  |     | Audio channel ? reserved

NAME      | ADDR | R/W | FUNCTION
--------- | ---- | --- | --------
SAUD0LCH  | 400  | W   | Audio channel 0 location (high 15 bits)
SAUD0LCL  | 402  | W   | Audio channel 0 location (low 15 bits)
SAUD0LENH | 404  | W   | Audio channel 0 length (high 15 bits)
SAUD0LENL | 406  | W   | Audio channel 0 length (low 15 bits)
SAUD0VOL  | 408  | W   | Audio channel 0 volume (8.8) (L/R)
SAUD0CTL  | 40A  | W   | Audio channel 0 control (Bit0:16bit, Bit1:OneShot, Bit2:Stereo)
SAUD0PER  | 40C  | W   | Audio channel 0 period (16bits)
          | 40E  |     | Audio channel 0 reserved
SAUD1LCH  | 410  | W   | Audio channel 1 location (high 15 bits)
SAUD1LCL  | 412  | W   | Audio channel 1 location (low 15 bits)
SAUD1LENH | 414  | W   | Audio channel 1 length (high 15 bits)
SAUD1LENL | 416  | W   | Audio channel 1 length (low 15 bits)
SAUD1VOL  | 418  | W   | Audio channel 1 volume (8.8) (L/R)
SAUD1CTL  | 41A  | W   | Audio channel 1 control (Bit0:16bit, Bit1:OneShot, Bit2:Stereo)
SAUD1PER  | 41C  | W   | Audio channel 1 period (16bits)
          | 41E  |     | Audio channel 1 reserved
SAUD2LCH  | 420  | W   | Audio channel 2 location (high 15 bits)
SAUD2LCL  | 422  | W   | Audio channel 2 location (low 15 bits)
SAUD2LENH | 424  | W   | Audio channel 2 length (high 15 bits)
SAUD2LENL | 426  | W   | Audio channel 2 length (low 15 bits)
SAUD2VOL  | 428  | W   | Audio channel 2 volume (8.8) (L/R)
SAUD2CTL  | 42A  | W   | Audio channel 2 control (Bit0:16bit, Bit1:OneShot, Bit2:Stereo)
SAUD2PER  | 42C  | W   | Audio channel 2 period (16bits)
          | 42E  |     | Audio channel 2 reserved
SAUD3LCH  | 430  | W   | Audio channel 3 location (high 15 bits)
SAUD3LCL  | 432  | W   | Audio channel 3 location (low 15 bits)
SAUD3LENH | 434  | W   | Audio channel 3 length (high 15 bits)
SAUD3LENL | 436  | W   | Audio channel 3 length (low 15 bits)
SAUD3VOL  | 438  | W   | Audio channel 3 volume (8.8) (L/R)
SAUD3CTL  | 43A  | W   | Audio channel 3 control (Bit0:16bit, Bit1:OneShot, Bit2:Stereo)
SAUD3PER  | 43C  | W   | Audio channel 3 period (16bits)
          | 43E  |     | Audio channel 3 reserved
SAUD4LCH  | 440  | W   | Audio channel 4 location (high 15 bits)
SAUD4LCL  | 442  | W   | Audio channel 4 location (low 15 bits)
SAUD4LENH | 444  | W   | Audio channel 4 length (high 15 bits)
SAUD4LENL | 446  | W   | Audio channel 4 length (low 15 bits)
SAUD4VOL  | 448  | W   | Audio channel 4 volume (8.8) (L/R)
SAUD4CTL  | 44A  | W   | Audio channel 4 control (Bit0:16bit, Bit1:OneShot, Bit2:Stereo)
SAUD4PER  | 44C  | W   | Audio channel 4 period (16bits)
          | 44E  |     | Audio channel 4 reserved
SAUD5LCH  | 450  | W   | Audio channel 5 location (high 15 bits)
SAUD5LCL  | 452  | W   | Audio channel 5 location (low 15 bits)
SAUD5LENH | 454  | W   | Audio channel 5 length (high 15 bits)
SAUD5LENL | 456  | W   | Audio channel 5 length (low 15 bits)
SAUD5VOL  | 458  | W   | Audio channel 5 volume (8.8) (L/R)
SAUD5CTL  | 45A  | W   | Audio channel 5 control (Bit0:16bit, Bit1:OneShot, Bit2:Stereo)
SAUD5PER  | 45C  | W   | Audio channel 5 period (16bits)
          | 45E  |     | Audio channel 5 reserved
SAUD6LCH  | 460  | W   | Audio channel 6 location (high 15 bits)
SAUD6LCL  | 462  | W   | Audio channel 6 location (low 15 bits)
SAUD6LENH | 464  | W   | Audio channel 6 length (high 15 bits)
SAUD6LENL | 466  | W   | Audio channel 6 length (low 15 bits)
SAUD6VOL  | 468  | W   | Audio channel 6 volume (8.8) (L/R)
SAUD6CTL  | 46A  | W   | Audio channel 6 control (Bit0:16bit, Bit1:OneShot, Bit2:Stereo)
SAUD6PER  | 46C  | W   | Audio channel 6 period (16bits)
          | 46E  |     | Audio channel 6 reserved
SAUD7LCH  | 470  | W   | Audio channel 7 location (high 15 bits)
SAUD7LCL  | 472  | W   | Audio channel 7 location (low 15 bits)
SAUD7LENH | 474  | W   | Audio channel 7 length (high 15 bits)
SAUD7LENL | 476  | W   | Audio channel 7 length (low 15 bits)
SAUD7VOL  | 478  | W   | Audio channel 7 volume (8.8) (L/R)
SAUD7CTL  | 47A  | W   | Audio channel 7 control (Bit0:16bit, Bit1:OneShot, Bit2:Stereo)
SAUD7PER  | 47C  | W   | Audio channel 7 period (16bits)
          | 47E  |     | Audio channel 7 reserved
