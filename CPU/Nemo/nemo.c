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
 * processor, by typing an assembly instruction from CLI shell.
 * In short words, the program takes user input, create a temporary
 * assembly file, assemble it with VASM using -Fbin option, then 
 * read back the encoded bytes, and output hexa/binary reusable
 * assembly code.
 * 
 * Download VASM for Motorola 68K AmigaOS3, and place it in any
 * accessible path, for example in the C: folder.
 * http://sun.hasenbraten.de/vasm/
 * http://sun.hasenbraten.de/vasm/bin/rel/vasmm68k_mot_os3.lha
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
#include <dos/dos.h>
#include <proto/dos.h>
#include <proto/exec.h>
#include "nemo.h"

/*****************************************************************
 * 
 * DEFINES
 * 
 *****************************************************************/

#define TEMPLATE  "MNEMONIC/A/F"

#define FILE_ASM  "T:NEMO.ASM"
#define FILE_OUT  "T:NEMO.OUT"
#define FILE_EXE  "vasmm68k_mot_os3"
#define FILE_CMD  "%s -no-opt -Fbin -m68080 -m68882 %s -o %s -quiet"

#define OPT_MNEMONIC  0
#define OPT_COUNT     1

const UBYTE version[] = APP_VSTRING;

/*****************************************************************
 * 
 * EXTERNS
 * 
 *****************************************************************/

extern struct DOSBase  *DOSBase;
extern struct ExecBase *SysBase;

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
	
	VPrintf("%s %s %s %s\n", argv);
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
	
	VPrintf("\tDC.W $%04x ; ", argv);
}

/*****************************************************************
 * 
 * VSPrintf()
 * 
 *****************************************************************/

VOID VSPrintf(STRPTR str, STRPTR fmt, APTR args)
{
	const UWORD PutChProc[] =
	{ 
		0x16C0, // move.b d0,(a3)+
		0x4E75  // rts
	};
	
	RawDoFmt(fmt, args, (VOID (*)(VOID))&PutChProc, str);
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
	
	opts[0] = NULL;
	
	if (rdargs = ReadArgs(TEMPLATE, opts, NULL))
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
			
			// 'Mnemonic' to 'ASM file'
			
			FPuts(file, "MAIN:\n\t");
			FPuts(file, (STRPTR)opts[OPT_MNEMONIC]);
			Close(file);
			
			// Delete old generated file
			
			DeleteFile(FILE_OUT);
			
			// Call VASM
			
			VSPrintf(commandString, FILE_CMD, commandArgs);
			
			if (Execute(commandString, NULL, NULL))
			{
				// Open new generated file
				
				if (file = Open(FILE_OUT, MODE_OLDFILE))
				{
					struct FileInfoBlock fib;
					
					if (ExamineFH(file, &fib))
					{
						if (fib.fib_Size >= sizeof(UWORD))
						{
							LONG i;
							
							VPrintf("\n\t; %s\n", &opts[OPT_MNEMONIC]);
							
							for (i = 0; i < fib.fib_Size; i += sizeof(UWORD))
							{
								UWORD buffer[32];
								
								if (Read(file, buffer, sizeof(UWORD)))
								{
									PrintHex(*buffer);
									PrintBin(*buffer);
								}
							}
							
							PutStr("\n");
						
							rc = RETURN_OK;
						}
					}
					
					Close(file);
				}
			}
		}
		else
		{
			PrintFault(IoErr(), "Nemo");
		}
		
		FreeArgs(rdargs);
	}
	else
	{
		PrintFault(IoErr(), "Nemo");
	}
	
	return(rc);
}


/*****************************************************************
 * 
 * END
 * 
 *****************************************************************/
