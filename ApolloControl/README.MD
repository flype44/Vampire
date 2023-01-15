# ApolloControl

(C) Copyright 2016-2023 APOLLO-Team

The purpose of `ApolloControl` is to bring some valuable information and control over the `Vampire` boards.


# Releases

VControl 1.27 (January 15, 2023)

Latest official binary releases : [Download](https://www.apollo-accelerators.com/wiki/doku.php/saga:updates)


# Features

Latest official documentation : [Open](DOCUMENTATION.md#documentation)


Command | Description
---------------- | -------------
DE=DETECT/S      | Return $RC=0 if AC68080 is detected
BO=BOARD/S       | Output Board Information
BI=BOARDID/S     | Output Board Identifier
BN=BOARDNAME/S   | Output Board Name
SN=BOARDSERIAL/S | Output Board Serial Number
CO=CORE/S        | Output Core Revision String
CR=COREREV/S     | Output Core Revision Number
CP=CPU/S         | Output CPU Information
HZ=HERTZ/S       | Output CPU Frequency (Hertz)
ML=MEMLIST/S     | Output Memory list
MO=MODULES/S     | Output Modules list
CD=CONFIGDEV/S   | Output AutoConfig Devices list
SE=SETENV/S      | Create Environment Variables
AK=AKIKO/S       | Initialize the Akiko C2P routine
AF=ATTNFLAGS/S   | Change the AttnFlags (Force 080's)
DP=DPMS/N        | Change the DPMS mode. 0=Off, 1=On
FP=FPU/N         | Change the FPU mode. 0=Off, 1=On
ID=IDESPEED/N    | Change the IDE speed. 0=Slow, 1=Fast, 2=Faster, 3=Fastest
SC=SCANLINES/N   | Change the Scanlines mode. 0=Off, 1=On, for V4(+)
SD=SDCLOCKDIV/N  | Change the SDPort Clock Divider. 0=Fastest, 255=Slowest
SH=SHIELD/N      | Change the Shield mode. 0=Off, 1=On
SS=SUPERSCALAR/N | Change the SuperScalar mode. 0=Off, 1=On
TU=TURTLE/N      | Change the Turtle mode. 0=Off, 1=On
ZO=ZOOM/N        | Change the Zoom PAL mode. 0=720px, 1=640px, for V4(+)
VB=VBRMOVE/N     | Change the VBR location. 0=ChipRAM, 1=FastRAM
JP=JOYPORT/S     | Patch the LowLevel.library to use the GAMEPADs, for V4(+)
MR=MAPROM        | Map a ROM file


# Build instructions

To compile `VControl` you need :

* `SAS/C` 6.59 for CBM AmigaOS3 M68K

  Install and ensure all assets are properly accessible.

* `VASM` 1.8h for CBM AmigaOS3 M68K [Download](http://sun.hasenbraten.de/vasm/bin/rel/vasmm68k_mot_os3.lha)

  Install and ensure `vasmm68k_mot` is properly accessible (in `C:` for example).

* `BoardsLib Developer Kit` [Download](http://aminet.net/util/libs/BoardsLib.lha)

  Copy `BoardsLib/Developer/include/c/` folders to `SC:include/`

* `CyberGraph Developer Kit` [Download](http://aminet.net/dev/misc/CGraphX-DevKit.lha)

  Copy `CGraphX/C/Include/` folders to `SC:include/`

* Use the provided `smakefile`.

  `CD` to the project and type `smake`.

<img src="BuildInstructions.png" />


# Screenshots

<img src="Screenshot01.png" />

<img src="Screenshot02.png" />

<img src="Screenshot03.png" />
