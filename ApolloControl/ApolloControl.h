#ifndef __APOLLOCONTROL_H__
#define __APOLLOCONTROL_H__

#define APP_VSTRING "$VER: ApolloControl 1.27 (15.01.2023) (C) Apollo-Team"

/*****************************************************************
 * APOLLO HARDWARE
 *****************************************************************/

#define VREG_FASTIDE    0xdd1020 /* FastIDE speed mode         */
#define VREG_SDCLKDIV   0xde000c /* SDCard Clock Divider       */
#define VREG_BEAMCON0   0xdff1dc /* Amiga BEAMCON0 register    */
#define VREG_SAGACTRL1  0xdff3ec /* AGA, Scanlines, Zoom modes */
#define VREG_SAGACTRL2  0xdff3ee /* AGA, Zoom shift            */
#define VREG_BOARD      0xdff3fc /* Board Model and Multiplier */
#define VREG_MAPROM     0xdff3fe /* MapROM memory protection   */

#define VREG_BOARD_Unknown  0x00 /* Unknow                          */
#define VREG_BOARD_V600     0x01 /* Vampire V2 V600(+),   for A600  */
#define VREG_BOARD_V500     0x02 /* Vampire V2 V500(+),   for A500  */
#define VREG_BOARD_V4FB     0x03 /* Apollo V4 FireBird,   for A500  */
#define VREG_BOARD_V4ID     0x04 /* Apollo V4 IceDrake,   for A1200 */
#define VREG_BOARD_V4SA     0x05 /* Apollo V4 Standalone            */
#define VREG_BOARD_V1200    0x06 /* Vampire V2 V1200,     for A1200 */
#define VREG_BOARD_V4MC     0x07 /* Apollo V4 Manticore,  for A600  */
#define VREG_BOARD_Future   0x08 /* Unknow                          */

#define VREG_BEAMCON0B_PAL        (5)
#define VREG_BEAMCON0B_SETCLR     (15)

#define VREG_SAGACTRL1B_SCANLINES (0)
#define VREG_SAGACTRL1B_AGAMODE   (1)
#define VREG_SAGACTRL1B_ZOOM      (2)
#define VREG_SAGACTRL1B_SETCLR    (15)

#define VREG_BEAMCON0F_PAL        (1L<<VREG_BEAMCON0B_PAL)
#define VREG_BEAMCON0F_SETCLR     (1L<<VREG_BEAMCON0B_SETCLR)

#define VREG_SAGACTRL1F_SCANLINES (1L<<VREG_SAGACTRL1B_SCANLINES)
#define VREG_SAGACTRL1F_AGAMODE   (1L<<VREG_SAGACTRL1B_AGAMODE)
#define VREG_SAGACTRL1F_ZOOM      (1L<<VREG_SAGACTRL1B_ZOOM)
#define VREG_SAGACTRL1F_SETCLR    (1L<<VREG_SAGACTRL1B_SETCLR)

/*****************************************************************
 * EXTERNAL FUNCTIONS
 *****************************************************************/

#define ASM __asm __saveds 

ULONG ASM v_chipset_audio_rev(void);
ULONG ASM v_chipset_video_rev(void);

ULONG ASM v_cpu_multiplier(void);

ULONG ASM v_cpu_vbr(void);
ULONG ASM v_cpu_vbr_on(void);
ULONG ASM v_cpu_vbr_off(void);

ULONG ASM v_cpu_pcr(void);
ULONG ASM v_cpu_pcr_dfp_on(void);
ULONG ASM v_cpu_pcr_dfp_off(void);
ULONG ASM v_cpu_pcr_ess_on(void);
ULONG ASM v_cpu_pcr_ess_off(void);

ULONG ASM v_cpu_pcr_etu_off(void);
ULONG ASM v_cpu_pcr_etu_a500(void);
ULONG ASM v_cpu_pcr_etu_a1200(void);

ULONG ASM v_cpu_cacr(void);
ULONG ASM v_cpu_cacr_dcache_on(void);
ULONG ASM v_cpu_cacr_dcache_off(void);
ULONG ASM v_cpu_cacr_icache_on(void);
ULONG ASM v_cpu_cacr_icache_off(void);

ULONG ASM v_cpu_tc(void);
ULONG ASM v_cpu_tc_on(void);
ULONG ASM v_cpu_tc_off(void);

ULONG ASM v_cpu_is2p(void);
ULONG ASM v_cpu_is080(void);

ULONG ASM v_fpu_isok(void);
ULONG ASM v_fpu_toggle(register __d0 ULONG state);

ULONG ASM v_joyport_init(void);

ULONG ASM v_read_flashidentifier(void);
ULONG ASM v_read_serialnumber(register __a0 APTR buffer);
ULONG ASM v_read_revisionstring(register __a0 APTR buffer);

ULONG ASM v_maprom(register __a0 APTR buffer);

#endif // __APOLLOCONTROL_H__
