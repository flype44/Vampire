# Vampire SAGA AUDIO chipset

AUDIO specifications

# Channels

Register Set | Description
------------ | -------------
DFF40_       | Channel Number 0
DFF41_       | Channel Number 1
DFF42_       | Channel Number 2
DFF43_       | Channel Number 3
DFF44_       | Channel Number 4
DFF45_       | Channel Number 5
DFF46_       | Channel Number 6
DFF47_       | Channel Number 7

# Features per channel

Register | Name         | Size | RW  | Description
-------- | ------------ | ---- | --- | -----------
DFF4_0   | AUDxPTH      | WORD | (W) | PTR HIGH
DFF4_2   | AUDxPTL      | WORD | (W) | PTR LOW
DFF4_4   | AUDxLENH     | WORD | (W) | LEN HIGH
DFF4_6   | AUDxLENL     | WORD | (W) | LEN LOW
DFF4_8   | AUDxVOLUME   | WORD | (W) | VOL 8.8
DFF4_A   | AUDxMODE     | WORD | (W) | MODE (Bit0=16bit, Bit1=OneShot)
DFF4_C   | AUDxPERIOD   | WORD | (W) | PERIOD
DFF4_E   | AUDxRESERVED | WORD | (W) | RESERVED

# Control registers

Register  | Name     | Size | RW  | Description
--------- | -------- | ---- | --- | -----------
DFF016    | POTINP1  | WORD | (R) | Read Paula chip ID (0=Paula, 1=Pamela)
DFF002    | DMACONR1 | WORD | (R) | Control AUD DMA  (Bit00 to Bit03) AUD0..3
DFF202    | DMACONR2 | WORD | (R) | Control AUD DMA  (Bit00 to Bit11) AUD4..7
DFF096    | DMACON1  | WORD | (W) | Control AUD DMA  (Bit00 to Bit03) AUD0..3
DFF296    | DMACON2  | WORD | (W) | Control AUD DMA  (Bit00 to Bit11) AUD4..7
DFF01C    | INTENAR1 | WORD | (R) | Request INT BITS (Bit07 to Bit10) AUD0..3
DFF21C    | INTENAR2 | WORD | (R) | Request INT BITS (Bit00 to Bit11) AUD4..7
DFF09A    | INTENA1  | WORD | (W) | Request INT BITS (Bit07 to Bit10) AUD0..3
DFF29A    | INTENA2  | WORD | (W) | Request INT BITS (Bit00 to Bit11) AUD4..7
DFF01E    | INTREQR1 | WORD | (R) | Request INT BITS (Bit07 to Bit10) AUD0..3
DFF21E    | INTREQR2 | WORD | (R) | Request INT BITS (Bit00 to Bit11) AUD4..7
DFF09C    | INTREQ1  | WORD | (W) | Request INT BITS (Bit07 to Bit10) AUD0..3
DFF29C    | INTREQ2  | WORD | (W) | Request INT BITS (Bit00 to Bit11) AUD4..7
