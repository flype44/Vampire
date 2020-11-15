# Vampire SAGA ETHERNET chipset

> (C) Copyright 2016-2020 APOLLO-Team

This article describes all the `SAGA` ethernet features, and is still in **Work In Progress** status.

![Vampire Logo](../ASSETS/V_LOGO.png)

# License

This documentation and associated files are licensed under the [Mozilla Public License 2.0](../LICENSE)

Permissions of this weak copyleft license are conditioned on making available source code of licensed files and modifications of those files under the same license (or in certain cases, one of the GNU licenses). Copyright and license notices must be preserved. Contributors provide an express grant of patent rights. However, a larger work using the licensed work may be distributed under different terms and without source code for files added in the larger work.

# Documentation

Written and maintained by `flype`, with the great help from the `APOLLO-Team` members. It is intended to the developers who plans to use the `Vampire` ethernet chip for their programs or games, and to help the community to write drivers. All the specifications mentionned in this documentation always refers to the latest version. Take care to use latest `Vampire` core version in your programs (see below).

# Core releases

Latest `Vampire` cores are officially distributed from [here](https://www.apollo-accelerators.com/wiki/doku.php/start#core_and_software_updates). 

Additionally, Beta cores are regularly distributed in the official `ApolloTeam` Discord Channel [here](https://discord.gg/bM684VW).

# Introduction

The `Vampire` standalone (V4) embeds a hardware ETHERNET phy and a connector.

The `SAGA` chipset provides a set of registers to used in a driver (`SANA2` based on AmigaOS).

# Ethernet registers

NAME       | ADDR   | R/W | FUNCTION
---------- | ------ | --- | --------
MAC1       | DE0020 |  ?  | DMA_ON<<31 | MAC(0:15)                  
MAC2       | DE0024 |  ?  | MAC(16:47)                              
MULTICAST1 | DE0028 |  ?  | high order bits multicast hash          
MULTICAST2 | DE002C |  ?  | low  order bits multicast hash          
RX_START   | DE0030 |  ?  | DMA buffer start (aligned to 2048 bytes)
RX_STOP    | DE0034 |  ?  | DMA buffer end (aligned to 2048 bytes)  
RX_SWPOS   | DE0038 |  ?  | RX software position (barrier)          
RX_HWPOS   | DE003C |  ?  | hardware RX write position              
TXWORD     | DE0040 |  ?  | write port for TX FIFO (32 Bits)        
TXQUEUELEN | DE0044 |  ?  | number of words in TX FIFO              

```
#define ETHERNET_REGBASE     (0xDE0000)

#define V4R_MAC1             (ETHERNET_REGBASE + 0x020)    /* DMA_ON<<31 | MAC(0:15)                   */
#define V4R_MAC2             (ETHERNET_REGBASE + 0x024)    /* MAC(16:47)                               */
#define V4R_MULTICAST1       (ETHERNET_REGBASE + 0x028)    /* high order bits multicast hash           */
#define V4R_MULTICAST2       (ETHERNET_REGBASE + 0x02C)    /* low  order bits multicast hash           */
#define V4R_RX_START         (ETHERNET_REGBASE + 0x030)    /* DMA buffer start (aligned to 2048 bytes) */
#define V4R_RX_STOP          (ETHERNET_REGBASE + 0x034)    /* DMA buffer end (aligned to 2048 bytes)   */
#define V4R_RX_SWPOS         (ETHERNET_REGBASE + 0x038)    /* RX software position (barrier)           */
#define V4R_RX_HWPOS         (ETHERNET_REGBASE + 0x03C)    /* hardware RX write position               */
#define V4R_TXWORD           (ETHERNET_REGBASE + 0x040)    /* write port for TX FIFO (32 Bits)         */
#define V4R_TXQUEUELEN       (ETHERNET_REGBASE + 0x044)    /* number of words in TX FIFO               */
```

# Interrupt registers

NAME       | ADDR   | R/W | FUNCTION
---------- | ------ | --- | --------
DMACONR2   | DFF202 |  R  | DMA control
DMACON2    | DFF296 |  W  | DMA control
INTENAR2   | DFF21C |  R  | Interrupt enable bits
INTENA2    | DFF29A |  W  | Interrupt enable bits
INTREQR2   | DFF21E |  R  | Interrupt request bits
INTREQ2    | DFF29C |  W  | Interrupt request bits

```
#define CUSTOM_REGBASE       (0xDFF000)

#define INTENAR2             (CUSTOM_REGBASE + 0x01C + 0x200)
#define INTENA2	             (CUSTOM_REGBASE + 0x09A + 0x200)
#define INTREQR2             (CUSTOM_REGBASE + 0x01E + 0x200)
#define INTREQ2	             (CUSTOM_REGBASE + 0x09C + 0x200)

#define INT2B_SETCLR         (15)
#define INT2B_ETHERNET       (13)
#define INT2F_SETCLR         (1L<<INT2B_SETCLR)
#define INT2F_ETHERNET       (1L<<INT2B_ETHERNET)
```

# Related ressources files

* The [SAGADriver](https://www.apollo-accelerators.com/wiki/doku.php/saga:updates) package include a `SANA2` ethernet driver.

* Refers to [saga_ethernet.h](saga_ethernet.h) for useful C-lang macros and constants to include in your programs.

# Frequently Asked Question

> Any WiFi chip on the V4 ?

Not on board. However, the V4 provides a set of IOs pins that be used for such a device.

> What internet speed can be reached on the V4 ?

TODO
