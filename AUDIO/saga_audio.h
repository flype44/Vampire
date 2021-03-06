#ifndef	VAMPIRE_SAGA_AUDIO_H
#define	VAMPIRE_SAGA_AUDIO_H
/*
**	$VER: saga/audio.h 0.1 (15.11.2020)
**	
**	Vampire SAGA AUDIO hardware
**	
**	(C) Copyright 2015-2020 APOLLO-Team
**	    All Right Reserved
*/

#include <hardware/custom.h>
#include <hardware/dmabits.h>
#include <hardware/intbits.h>

#define CUSTOM_REGBASE       (0xDFF000)
#define PAULA_CLOCK_PAL      (3546895)
#define PAULA_CLOCK_NTSC     (3579545)
#define MAX_CHANNELS         (8)

/* new defines for POTINP */

#define GETPAULAID           (((*(volatile UWORD*)CUSTOM_REGBASE + 0x016) & 0xFE) >> 1)

/* new defines for ADKCON2 */

#define ADKCONR1             (CUSTOM_REGBASE + 0x010)
#define ADKCONR2             (CUSTOM_REGBASE + 0x010 + 0x200)
#define ADKCON1              (CUSTOM_REGBASE + 0x09E)
#define ADKCON2              (CUSTOM_REGBASE + 0x09E + 0x200)

/* new defines for DMACON2 */

#define DMACONR1             (CUSTOM_REGBASE + 0x002)
#define DMACONR2             (CUSTOM_REGBASE + 0x002 + 0x200)
#define DMACON1              (CUSTOM_REGBASE + 0x096)
#define DMACON2              (CUSTOM_REGBASE + 0x096 + 0x200)

/* new defines for INTENA2 */

#define INTENAR1             (CUSTOM_REGBASE + 0x01C)
#define INTENAR2             (CUSTOM_REGBASE + 0x01C + 0x200)
#define INTENA1              (CUSTOM_REGBASE + 0x09A)
#define INTENA2              (CUSTOM_REGBASE + 0x09A + 0x200)

/* new defines for INTREQ2 */

#define INTREQR1             (CUSTOM_REGBASE + 0x01E)
#define INTREQR2             (CUSTOM_REGBASE + 0x01E + 0x200)
#define INTREQ1              (CUSTOM_REGBASE + 0x09C)
#define INTREQ2              (CUSTOM_REGBASE + 0x09C + 0x200)

/* new defines for SAUDx */

struct SAudChannel {
    ULONG *sac_ptr;    /* location */
    ULONG  sac_len;    /* length   */
    UWORD  sac_vol;    /* volume   */
    UWORD  sac_ctl;    /* control  */
    UWORD  sac_per;    /* period   */
    UWORD  sac_pad;    /* unused   */
} saud[ MAX_CHANNELS ];

#define SAUD_REGBASE(CH)     (CUSTOM_REGBASE + 0x400 + (((CH) & 0x0F) << 4))
#define SAUD_REG(CH, IDX)    (SAUD_REGBASE(CH) + (IDX))

#define SAUD_PTR(CH)         SAUD_REG(CH, 0x00) /* location */
#define SAUD_LEN(CH)         SAUD_REG(CH, 0x04) /* length   */
#define SAUD_VOL(CH)         SAUD_REG(CH, 0x08) /* volume   */
#define SAUD_CTL(CH)         SAUD_REG(CH, 0x0A) /* control  */
#define SAUD_PER(CH)         SAUD_REG(CH, 0x0C) /* period   */

/* new defines for SAUD0 */

#define SAUD0PTR             SAUD_PTR(0) /* channel 0 */
#define SAUD0LEN             SAUD_LEN(0)
#define SAUD0VOL             SAUD_VOL(0)
#define SAUD0CTL             SAUD_CTL(0)
#define SAUD0PER             SAUD_PER(0)

/* new defines for SAUD1 */

#define SAUD1PTR             SAUD_PTR(1) /* channel 1 */
#define SAUD1LEN             SAUD_LEN(1)
#define SAUD1VOL             SAUD_VOL(1)
#define SAUD1CTL             SAUD_CTL(1)
#define SAUD1PER             SAUD_PER(1)

/* new defines for SAUD2 */

#define SAUD2PTR             SAUD_PTR(2) /* channel 2 */
#define SAUD2LEN             SAUD_LEN(2)
#define SAUD2VOL             SAUD_VOL(2)
#define SAUD2CTL             SAUD_CTL(2)
#define SAUD2PER             SAUD_PER(2)

/* new defines for SAUD3 */

#define SAUD3PTR             SAUD_PTR(3) /* channel 3 */
#define SAUD3LEN             SAUD_LEN(3)
#define SAUD3VOL             SAUD_VOL(3)
#define SAUD3CTL             SAUD_CTL(3)
#define SAUD3PER             SAUD_PER(3)

/* new defines for SAUD4 */

#define SAUD4PTR             SAUD_PTR(4) /* channel 4 */
#define SAUD4LEN             SAUD_LEN(4)
#define SAUD4VOL             SAUD_VOL(4)
#define SAUD4CTL             SAUD_CTL(4)
#define SAUD4PER             SAUD_PER(4)

/* new defines for SAUD5 */

#define SAUD5PTR             SAUD_PTR(5) /* channel 5 */
#define SAUD5LEN             SAUD_LEN(5)
#define SAUD5VOL             SAUD_VOL(5)
#define SAUD5CTL             SAUD_CTL(5)
#define SAUD5PER             SAUD_PER(5)

/* new defines for SAUD6 */

#define SAUD6PTR             SAUD_PTR(6) /* channel 6 */
#define SAUD6LEN             SAUD_LEN(6)
#define SAUD6VOL             SAUD_VOL(6)
#define SAUD6CTL             SAUD_CTL(6)
#define SAUD6PER             SAUD_PER(6)

/* new defines for SAUD7 */

#define SAUD7PTR             SAUD_PTR(7) /* channel 7 */
#define SAUD7LEN             SAUD_LEN(7)
#define SAUD7VOL             SAUD_VOL(7)
#define SAUD7CTL             SAUD_CTL(7)
#define SAUD7PER             SAUD_PER(7)

/* new defines for SAUDxCTL */

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

/* new defines for DMACON2 */

#define DMA2B_SETCLR         (15) /* set/clear */
#define DMA2B_SLOT14         (14) /* reserved  */
#define DMA2B_SLOT13         (13) /* reserved  */
#define DMA2B_SLOT12         (12) /* reserved  */
#define DMA2B_SLOT11         (11) /* unused    */
#define DMA2B_SLOT10         (10) /* unused    */
#define DMA2B_SLOT09         (9)  /* unused    */
#define DMA2B_SLOT08         (8)  /* unused    */
#define DMA2B_SLOT07         (7)  /* unused    */
#define DMA2B_SLOT06         (6)  /* unused    */
#define DMA2B_SLOT05         (5)  /* unused    */
#define DMA2B_SLOT04         (4)  /* unused    */
#define DMA2B_AUD7           (3)  /* channel 7 */
#define DMA2B_AUD6           (2)  /* channel 6 */
#define DMA2B_AUD5           (1)  /* channel 5 */
#define DMA2B_AUD4           (0)  /* channel 4 */

#define DMA2F_SETCLR         (1L<<DMA2B_SETCLR)
#define DMA2F_SLOT14         (1L<<DMA2B_SLOT14)
#define DMA2F_SLOT13         (1L<<DMA2B_SLOT13)
#define DMA2F_SLOT12         (1L<<DMA2B_SLOT12)
#define DMA2F_SLOT11         (1L<<DMA2B_SLOT11)
#define DMA2F_SLOT10         (1L<<DMA2B_SLOT10)
#define DMA2F_SLOT09         (1L<<DMA2B_SLOT09)
#define DMA2F_SLOT08         (1L<<DMA2B_SLOT08)
#define DMA2F_SLOT07         (1L<<DMA2B_SLOT07)
#define DMA2F_SLOT06         (1L<<DMA2B_SLOT06)
#define DMA2F_SLOT05         (1L<<DMA2B_SLOT05)
#define DMA2F_SLOT04         (1L<<DMA2B_SLOT04)
#define DMA2F_AUD7           (1L<<DMA2B_AUD7)
#define DMA2F_AUD6           (1L<<DMA2B_AUD6)
#define DMA2F_AUD5           (1L<<DMA2B_AUD5)
#define DMA2F_AUD4           (1L<<DMA2B_AUD4)
#define DMA2F_AUDIO          (DMA2F_AUD4|DMA2F_AUD5|DMA2F_AUD6|DMA2F_AUD7) /* all audio channels */

/* new defines for INTENA2 and INTREQ2 */

#define INT2B_SETCLR         (15) /* set/clear */
#define INT2B_SLOT14         (14) /* reserved  */
#define INT2B_ETHERNET       (13) /* ethernet  */
#define INT2B_SLOT12         (12) /* reserved  */
#define INT2B_SLOT11         (11) /* unused    */
#define INT2B_SLOT10         (10) /* unused    */
#define INT2B_SLOT09         (9)  /* unused    */
#define INT2B_SLOT08         (8)  /* unused    */
#define INT2B_SLOT07         (7)  /* unused    */
#define INT2B_SLOT06         (6)  /* unused    */
#define INT2B_SLOT05         (5)  /* unused    */
#define INT2B_SLOT04         (4)  /* unused    */
#define INT2B_AUD7           (3)  /* channel 7 */
#define INT2B_AUD6           (2)  /* channel 6 */
#define INT2B_AUD5           (1)  /* channel 5 */
#define INT2B_AUD4           (0)  /* channel 4 */

#define INT2F_SETCLR         (1L<<INT2B_SETCLR)
#define INT2F_SLOT14         (1L<<INT2B_SLOT14)
#define INT2F_ETHERNET       (1L<<INT2B_ETHERNET)
#define INT2F_SLOT12         (1L<<INT2B_SLOT12)
#define INT2F_SLOT11         (1L<<INT2B_SLOT11)
#define INT2F_SLOT10         (1L<<INT2B_SLOT10)
#define INT2F_SLOT09         (1L<<INT2B_SLOT09)
#define INT2F_SLOT08         (1L<<INT2B_SLOT08)
#define INT2F_SLOT07         (1L<<INT2B_SLOT07)
#define INT2F_SLOT06         (1L<<INT2B_SLOT06)
#define INT2F_SLOT05         (1L<<INT2B_SLOT05)
#define INT2F_SLOT04         (1L<<INT2B_SLOT04)
#define INT2F_AUD7           (1L<<INT2B_AUD7)
#define INT2F_AUD6           (1L<<INT2B_AUD6)
#define INT2F_AUD5           (1L<<INT2B_AUD5)
#define INT2F_AUD4           (1L<<INT2B_AUD4)
#define INT2F_AUDIO          (INT2F_AUD4|INT2F_AUD5|INT2F_AUD6|INT2F_AUD7) /* all audio channels */

#endif	/* VAMPIRE_SAGA_AUDIO_H */
