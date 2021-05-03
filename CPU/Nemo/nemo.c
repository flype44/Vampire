/*****************************************************************
 * 
 * Project: Nemo
 * Author:  flype
 * Short:   Motorola 680x0/AMMX Mnemonic encoder
 * Usage:   nemo <any valid assembly instruction>
 * 
 *****************************************************************
 * 
 * Description:
 * 
 * This program use the VASM assembler by Frank Wille to get a 
 * quick view of the byte encoding of any supported Motorola 68000 
 * processor instruction, by typing an assembly instruction in CLI.
 * In short words, the program takes the user input, create a 
 * temporary assembly file, assemble it with VASM using the -Fbin 
 * option, then read back the encoded bytes, and output reusable 
 * DC.w hexadecimal/binary assembly code.
 * 
 * Download VASM for Motorola 68K AmigaOS3, and place it in any
 * accessible path, for example in the C: folder.
 * http://sun.hasenbraten.de/vasm/
 * http://sun.hasenbraten.de/vasm/bin/rel/vasmm68k_mot_os3.lha
 * 
 *****************************************************************
 * 
 * Arguments:
 * 
 * T=TEMPLATE/N:   Color scheme template. 0 (default) to 2.
 * 
 * M=MNEMONIC/A/F: Assembly mnemonic to encode (mandatory).
 * 
 *****************************************************************
 * 
 * Examples:
 * 
 * > nemo nop
 * 
 *      DC.W $4E71 ; 0100 1110 0111 0001
 * 
 * > nemo move.l (a1,d1.L*4),(a2,d2.W*2);
 * 
 *      DC.W $25B1 ; 0010 0101 1011 0001
 *      DC.W $1C00 ; 0001 1100 0000 0000
 *      DC.W $2200 ; 0010 0010 0000 0000
 * 
 * > nemo fmove.x (a0)+,fp1;
 * 
 *      DC.W $F218 ; 1111 0010 0001 1000
 *      DC.W $4880 ; 0100 1000 1000 0000
 * 
 * > nemo fadd.x #123.456,fp0;
 * 
 *      DC.W $F23C ; 1111 0010 0011 1100
 *      DC.W $4822 ; 0100 1000 0010 0010
 *      DC.W $4005 ; 0100 0000 0000 0101
 *      DC.W $0000 ; 0000 0000 0000 0000
 *      DC.W $F6E9 ; 1111 0110 1110 1001
 *      DC.W $78D4 ; 0111 1000 1101 0100
 *      DC.W $FDF0 ; 1111 1101 1111 0000
 *      DC.W $0000 ; 0000 0000 0000 0000
 * 
 * > nemo paddusb d1,e0,e1;
 * 
 *      DC.W $FE01 ; 1111 1110 0000 0001
 *      DC.W $8914 ; 1000 1001 0001 0100
 * 
 * > nemo paddusw d1,e0,e1;
 * 
 *      DC.W $FE01 ; 1111 1110 0000 0001
 *      DC.W $8915 ; 1000 1001 0001 0101
 * 
 *****************************************************************/

#include <exec/types.h>
#include <exec/memory.h>
#include <exec/execbase.h>
#include <proto/exec.h>

#include <dos/dos.h>
#include <proto/dos.h>

#include <libraries/disassembler.h>
#include <proto/disassembler.h>

#include "nemo.h"

/*****************************************************************
 * 
 * DEFINES
 * 
 *****************************************************************/

#define COLOR0    "\033[30m" /* Color Grey  */
#define COLOR1    "\033[31m" /* Color Black */
#define COLOR2    "\033[32m" /* Color White */
#define COLOR3    "\033[33m" /* Color Blue  */
#define COLOR4    "\033[34m" /* Color #4    */
#define COLOR5    "\033[35m" /* Color #5    */
#define COLOR6    "\033[36m" /* Color #6    */
#define COLOR7    "\033[37m" /* Color #7    */

#define FILE_ASM  "T:NEMO.ASM"
#define FILE_OUT  "T:NEMO.OUT"
#define FILE_EXE  "vasmm68k_mot_os3"
#define FILE_CMD  "%s -Fbin -no-opt -m68080 -m68882 %s -o %s -quiet"

#define TEMPLATE  "M=MNEMONIC/A/F,T=TEMPLATE/N"

/*****************************************************************
 * 
 * ENUMS
 * 
 *****************************************************************/

enum {
	OPT_MNEMONIC,
	OPT_TEMPLATE,
	OPT_COUNT
};

enum {
	FIELD_OPEN,
	FIELD_HEX,
	FIELD_BIN,
	FIELD_CLOSE,
	FIELD_STAT,
	FIELD_COUNT
};

enum {
	TEMPLATE_0,
	TEMPLATE_1,
	TEMPLATE_2,
	TEMPLATE_COUNT
};

/*****************************************************************
 * 
 * TEMPLATES
 * 
 *****************************************************************/

const STRPTR templates[TEMPLATE_COUNT][FIELD_COUNT] =
{
	///////////////////////////////////
	//
	// TEMPLATE #1
	//
	///////////////////////////////////
	
	{
		"\nOPCODE MACRO\n\t* %s\n",
		"\tDC.W $%04x",
		" ; %s %s %s %s\n",
		"\tENDM\n",
		"\nOpcode-length: %ld bytes.\n",
	},
	
	///////////////////////////////////
	//
	// TEMPLATE #2
	//
	///////////////////////////////////
	
	{
		"\n" COLOR2 "OPCODE " COLOR3 "MACRO\n" COLOR0 "\t* %s\n",
		"\t" COLOR2 "DC" COLOR2 ".W " COLOR2 "$%04x",
		COLOR0 " ; %s %s %s %s\n",
		COLOR3 "\tENDM\n",
		"\n" COLOR2 "Opcode-length: " COLOR3 "%ld bytes.\n",
	},
	
	///////////////////////////////////
	//
	// TEMPLATE #3
	//
	///////////////////////////////////
	
	{
		"\n" COLOR2 "OPCODE " COLOR7 "MACRO\n" COLOR3 "\t* %s\n",
		"\t" COLOR2 "DC" COLOR2 ".W " COLOR6 "$%04x",
		COLOR0 " ; %s %s %s %s\n",
		COLOR7 "\tENDM\n",
		"\n" COLOR2 "Opcode-length: " COLOR3 "%ld bytes.\n",
	}
};

/*****************************************************************
 * 
 * PutChProc()
 * 
 *****************************************************************/

UWORD PutChProc[] =
{ 
	0x16C0, // move.b d0,(a3)+
	0x4E75  // rts
};

/*****************************************************************
 * 
 * GLOBALS
 * 
 *****************************************************************/

ULONG tmplt = 0;

STRPTR appName = APP_NAME;
STRPTR appVers = APP_VSTRING;

extern struct DOSBase  *DOSBase;
extern struct ExecBase *SysBase;

struct Library *DisassemblerBase;

/*****************************************************************
 * 
 * VSPrintf()
 * 
 *****************************************************************/

VOID VSPrintf(STRPTR str, STRPTR fmt, APTR args)
{
	RawDoFmt(fmt, args, (VOID (*)(VOID))&PutChProc, str);
}

/*****************************************************************
 * 
 * PrintHex()
 * 
 *****************************************************************/

VOID PrintHex(UWORD n)
{
	UWORD argv[1];
	
	argv[0] = n;
	
	VPrintf(templates[tmplt][FIELD_HEX], argv);
}

/*****************************************************************
 * 
 * PrintBin()
 * 
 *****************************************************************/

const STRPTR nibble[16] = 
{
	"0000", "0001", "0010", "0011",
	"0100", "0101", "0110", "0111",
	"1000", "1001", "1010", "1011",
	"1100", "1101", "1110", "1111"
};

VOID PrintBin(UWORD n)
{
	STRPTR argv[4];
	
	argv[0] = nibble[(n >> 12) & 15];
	argv[1] = nibble[(n >>  8) & 15];
	argv[2] = nibble[(n >>  4) & 15];
	argv[3] = nibble[(n >>  0) & 15];
	
	VPrintf(templates[tmplt][FIELD_BIN], argv);
}

/*****************************************************************
 * 
 * PrintInt()
 * 
 *****************************************************************/

VOID PrintInt(STRPTR s, ULONG n)
{
	ULONG argv[1];
	
	argv[0] = n;
	
	VPrintf(s, &argv);
}

/*****************************************************************
 *
 * main()
 *
 *****************************************************************/

ULONG main(ULONG argc, STRPTR argv[])
{
	ULONG rc = RETURN_WARN;
	LONG opts[OPT_COUNT];  
	struct RDArgs *rdargs;   
	
	opts[0] = 0;
	opts[1] = 0;
	
	if (rdargs = ReadArgs(TEMPLATE, opts, NULL))
	{
		tmplt = *(ULONG *)opts[OPT_TEMPLATE];
		
		if (tmplt < TEMPLATE_COUNT)
		{
			BPTR file;
			
			if (file = Open(FILE_ASM, MODE_NEWFILE))
			{
				BYTE commandString[255];
				
				STRPTR commandArgs[] = {
					FILE_EXE,
					FILE_ASM,
					FILE_OUT
				};
			
				FPuts(file, "MAIN:\n\t");
				FPuts(file, (STRPTR)opts[OPT_MNEMONIC]);
				Close(file);
				
				VSPrintf(commandString, FILE_CMD, commandArgs);
				
				if (Execute(commandString, NULL, NULL))
				{
					if (file = Open(FILE_OUT, MODE_OLDFILE))
					{
						UWORD opcode;
						ULONG count = 0;
						UWORD opcodes[32];
						
						VPrintf(templates[tmplt][FIELD_OPEN], 
							&opts[OPT_MNEMONIC]);
						
						while (Read(file, &opcode, sizeof(UWORD)) > 0)
						{
							opcodes[count] = opcode;
							PrintHex(opcode);
							PrintBin(opcode);
							count++;
						}
						
						PutStr(templates[tmplt][FIELD_CLOSE]);
						
						PrintInt(templates[tmplt][FIELD_STAT], count * 2);
						
						/*
						if (DisassemblerBase = OpenLibrary(DISASSEMBLER_NAME, 40))
						{
							struct DisData ds;
							
							ds.ds_PutProc  = (void *)&PutChProc;
							ds.ds_PC       = &opcodes;
							ds.ds_From     = &opcodes;
							ds.ds_UpTo     = (APTR)(((ULONG)&opcodes) + (count * 2));
							ds.ds_UserData = NULL;
							ds.ds_UserBase = NULL;
							ds.ds_Truncate = 0;
							ds.ds_reserved = 0;
							
							Disassemble(&ds);
							
							CloseLibrary(DisassemblerBase);
						}
						*/
						
						rc = RETURN_OK;
						
						Close(file);
						
						DeleteFile(FILE_OUT);
					}
				}
				
				DeleteFile(FILE_ASM);
			}
			else
			{
				PrintFault(IoErr(), appName);
			}
		}
		else
		{
			PutStr("Invalid template! 0=B&W (default), 1=Colored\n");
		}
		
		FreeArgs(rdargs);
	}
	else
	{
		PrintFault(IoErr(), appName);
	}
	
	return(rc);
}


/*****************************************************************
 * 
 * END
 * 
 *****************************************************************/
