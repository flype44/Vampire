#ifndef	VAMPIRE_SAGA_AUDIO_H
#define	VAMPIRE_SAGA_AUDIO_H
/*
**	$VER: saga/audio.h 0.1 (11.11.2020)
**	
**	Vampire SAGA AUDIO hardware
**	
**	(C) Copyright 2015-2020 APOLLO-Team
**	    All Right Reserved
*/

/*
#include <hardware/custom.h>
#include <hardware/dmabits.h>
#include <hardware/intbits.h>
*/

#define CUSTOM_REGBASE       (0xDFF000)
#define PAULA_CLK            (3546895)
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

struct SAudChannel
{
    ULONG *ac_loc;    /* location */
    ULONG  ac_len;    /* length   */
    UWORD  ac_vol;    /* volume   */
    UWORD  ac_ctl;    /* control  */
    UWORD  ac_per;    /* period   */
    UWORD  ac_pad;    /* unused   */
} aud[MAX_CHANNELS];

#define SAUD_REGBASE(CH)     (CUSTOM_REGBASE + 0x400 + (((CH) & 0x0F) << 4))
#define SAUD_REG(CH, IDX)    (SAUD_REGBASE(CH) + (IDX))

#define SAUD_LOC(CH)         SAUD_REG(CH, 0x00) /* location */
#define SAUD_LEN(CH)         SAUD_REG(CH, 0x04) /* length   */
#define SAUD_VOL(CH)         SAUD_REG(CH, 0x08) /* volume   */
#define SAUD_CTL(CH)         SAUD_REG(CH, 0x0A) /* control  */
#define SAUD_PER(CH)         SAUD_REG(CH, 0x0C) /* period   */

/* new defines for SAUD0 */

#define SAUD0LOC             SAUD_LOC(0) /* channel 0 */
#define SAUD0LEN             SAUD_LEN(0)
#define SAUD0VOL             SAUD_VOL(0)
#define SAUD0CTL             SAUD_CTL(0)
#define SAUD0PER             SAUD_PER(0)

/* new defines for SAUD1 */

#define SAUD1LOC             SAUD_LOC(1) /* channel 1 */
#define SAUD1LEN             SAUD_LEN(1)
#define SAUD1VOL             SAUD_VOL(1)
#define SAUD1CTL             SAUD_CTL(1)
#define SAUD1PER             SAUD_PER(1)

/* new defines for SAUD2 */

#define SAUD2LOC             SAUD_LOC(2) /* channel 2 */
#define SAUD2LEN             SAUD_LEN(2)
#define SAUD2VOL             SAUD_VOL(2)
#define SAUD2CTL             SAUD_CTL(2)
#define SAUD2PER             SAUD_PER(2)

/* new defines for SAUD3 */

#define SAUD3LOC             SAUD_LOC(3) /* channel 3 */
#define SAUD3LEN             SAUD_LEN(3)
#define SAUD3VOL             SAUD_VOL(3)
#define SAUD3CTL             SAUD_CTL(3)
#define SAUD3PER             SAUD_PER(3)

/* new defines for SAUD4 */

#define SAUD4LOC             SAUD_LOC(4) /* channel 4 */
#define SAUD4LEN             SAUD_LEN(4)
#define SAUD4VOL             SAUD_VOL(4)
#define SAUD4CTL             SAUD_CTL(4)
#define SAUD4PER             SAUD_PER(4)

/* new defines for SAUD5 */

#define SAUD5LOC             SAUD_LOC(5) /* channel 5 */
#define SAUD5LEN             SAUD_LEN(5)
#define SAUD5VOL             SAUD_VOL(5)
#define SAUD5CTL             SAUD_CTL(5)
#define SAUD5PER             SAUD_PER(5)

/* new defines for SAUD6 */

#define SAUD6LOC             SAUD_LOC(6) /* channel 6 */
#define SAUD6LEN             SAUD_LEN(6)
#define SAUD6VOL             SAUD_VOL(6)
#define SAUD6CTL             SAUD_CTL(6)
#define SAUD6PER             SAUD_PER(6)

/* new defines for SAUD7 */

#define SAUD7LOC             SAUD_LOC(7) /* channel 7 */
#define SAUD7LEN             SAUD_LEN(7)
#define SAUD7VOL             SAUD_VOL(7)
#define SAUD7CTL             SAUD_CTL(7)
#define SAUD7PER             SAUD_PER(7)

/* new defines for SAUDxCTL */

#define SAUDCTLB_STEREO      (2)
#define SAUDCTLB_ONESHOT     (1)
#define SAUDCTLB_16BITS      (0)

#define SAUDCTLF_STEREO      (1L<<SAUDCTLB_STEREO)
#define SAUDCTLF_ONESHOT     (1L<<SAUDCTLB_ONESHOT)
#define SAUDCTLF_16BITS      (1L<<SAUDCTLB_16BITS)

/* new defines for DMACON2 */

#define DMA2B_SETCLR         (15)
#define DMA2B_AUD7           (3)
#define DMA2B_AUD6           (2)
#define DMA2B_AUD5           (1)
#define DMA2B_AUD4           (0)

#define DMA2F_AUDIO          (0x0FFF)
#define DMA2F_SETCLR         (1L<<DMA2B_SETCLR)
#define DMA2F_AUD7           (1L<<DMA2B_AUD7)
#define DMA2F_AUD6           (1L<<DMA2B_AUD6)
#define DMA2F_AUD5           (1L<<DMA2B_AUD5)
#define DMA2F_AUD4           (1L<<DMA2B_AUD4)

/* new defines for INTENA2 and INTREQ2 */

#define INT2B_SETCLR         (15)
#define INT2B_AUD7           (3)
#define INT2B_AUD6           (2)
#define INT2B_AUD5           (1)
#define INT2B_AUD4           (0)

#define INT2F_AUDIO          (0x0FFF)
#define INT2F_SETCLR         (1L<<INT2B_SETCLR)
#define INT2F_AUD7           (1L<<INT2B_AUD7)
#define INT2F_AUD6           (1L<<INT2B_AUD6)
#define INT2F_AUD5           (1L<<INT2B_AUD5)
#define INT2F_AUD4           (1L<<INT2B_AUD4)

#endif	/* VAMPIRE_SAGA_AUDIO_H */
