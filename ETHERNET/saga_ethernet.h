#ifndef	VAMPIRE_SAGA_ETHERNET_H
#define	VAMPIRE_SAGA_ETHERNET_H
/*
**	$VER: saga/ethernet.h 0.1 (15.11.2020)
**	
**	Vampire SAGA ETHERNET hardware
**	
**	(C) Copyright 2015-2020 APOLLO-Team
**	    All Right Reserved
*/

#define CUSTOM_REGBASE       (0xDFF000)
#define ETHERNET_REGBASE     (0xDE0000)

#define INTENAR2             (CUSTOM_REGBASE + 0x01C + 0x200)
#define INTENA2	             (CUSTOM_REGBASE + 0x09A + 0x200)
#define INTREQR2             (CUSTOM_REGBASE + 0x01E + 0x200)
#define INTREQ2	             (CUSTOM_REGBASE + 0x09C + 0x200)

#define INT2B_SETCLR         (15)
#define INT2B_ETHERNET       (13)
#define INT2F_SETCLR         (1L<<INT2B_SETCLR)
#define INT2F_ETHERNET       (1L<<INT2B_ETHERNET)

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

#endif	/* VAMPIRE_SAGA_ETHERNET_H */
