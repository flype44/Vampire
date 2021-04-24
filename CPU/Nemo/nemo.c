/*****************************************************************
 * 
 * Project  : Nemo
 * Version  : 0.3
 * Author   : flype
 * Short    : M68K/AMMX Mnemonic encoder
 * Usage    : nemo <any valid assembly instruction>
 * 
 *****************************************************************
 * 
 * Examples :
 * 
 * nemo move.l (a1,d1.L*4),(a2,d2.W*2);
 * 
 *      DC.W $25b1 ; 0010 0101 1011 0001
 *      DC.W $1c00 ; 0001 1100 0000 0000
 *      DC.W $2200 ; 0010 0010 0000 0000
 * 
 * nemo paddusb d1,e0,e1;
 * 
 *      DC.W $fe01 ; 1111 1110 0000 0001
 *      DC.W $8914 ; 1000 1001 0001 0100
 * 
 * nemo paddusw d1,e0,e1;
 * 
 *      DC.W $fe01 ; 1111 1110 0000 0001
 *      DC.W $8915 ; 1000 1001 0001 0101
 * 
 *****************************************************************/

#include <exec/types.h>
#include <exec/memory.h>
#include <exec/execbase.h>
#include <dos/dos.h>
#include <stdio.h>
#include <string.h>

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
#define FILE_BIN  "T:NEMO.BIN"
#define FILE_OUT  "T:NEMO.OUT"
#define FILE_CMD  "vasmm68k_mot_os3 -no-opt -Fbin -m68080 %s -o %s -quiet >%s"

#define OPT_MNEMONIC    0
#define OPT_COUNT       1

const UBYTE ver[] = APP_VSTRING;

/*****************************************************************
 * 
 * EXTERNS
 * 
 *****************************************************************/

extern struct ExecBase *SysBase;
extern struct DOSBase  *DOSBase;
extern struct GfxBase  *GfxBase;

/*****************************************************************
 * 
 * VOID PrintBin( UWORD n )
 * 
 *****************************************************************/

const BYTE *bits[16] = {
	"0000", "0001", "0010", "0011",
	"0100", "0101", "0110", "0111",
	"1000", "1001", "1010", "1011",
	"1100", "1101", "1110", "1111"
};

VOID PrintBin( UWORD n )
{
	printf( "%s %s %s %s\n",
		bits[ n >> 12 & 0xf ],
		bits[ n >>  8 & 0xf ],
		bits[ n >>  4 & 0xf ],
		bits[ n >>  0 & 0xf ]
	);
}

/*****************************************************************
 * 
 * VOID PrintHex( UWORD n )
 * 
 *****************************************************************/

VOID PrintHex( UWORD n )
{
	printf( "\tDC.W $%04x ; ", n);
}

/*****************************************************************
 *
 * ULONG main(ULONG argc, STRPTR argv[])
 *
 *****************************************************************/

ULONG main(ULONG argc, STRPTR argv[])
{
	ULONG rc = RETURN_WARN;
	
	struct RDArgs *rdargs;   
	struct FileInfoBlock fib;
	LONG   opts[ OPT_COUNT ];  
	BYTE   commandString[ 255 ];
	USHORT buffer[ 32 ];
	
	memset((STRPTR)opts, 0, sizeof(opts));
	
	if ( rdargs = (struct RDArgs *)ReadArgs(TEMPLATE, opts, NULL) )
	{
		BPTR file;
		
		if( file = Open( FILE_ASM, MODE_NEWFILE ) )
		{
			// 'Mnemonic' to 'ASM file'
			
			FPuts( file, "MAIN:\n\t" );
			FPuts( file, (STRPTR)opts[ OPT_MNEMONIC ] );
			Close( file );
			
			// Prepare command line string
			
			sprintf( commandString, 
				FILE_CMD,
				FILE_ASM,
				FILE_BIN,
				FILE_OUT
			);
			
			// Delete old generated file
			
			DeleteFile( FILE_BIN );
			
			// Call VASM
			
			if( Execute( commandString, NULL, NULL ) )
			{
				// Open new generated file
				
				if( file = Open( FILE_BIN, MODE_OLDFILE ) )
				{
					if( ExamineFH( file, &fib ) )
					{
						ULONG i;
						
						printf( "\n\t; %s\n", opts[ OPT_MNEMONIC ] );
						
						for( i = 0; i < fib.fib_Size; i += 2 )
						{
							if( Read( file, buffer, 2 ) )
							{
								PrintHex( buffer[ 0 ] );
								PrintBin( buffer[ 0 ] );
							}
						}
						
						printf( "\n" );
						
						rc = RETURN_OK;
					}
					
					Close( file );
				}
				else
				{
					printf( "Syntax error.\n" );
				}
			}
			else
			{
				printf( "Required: vasmm68k_mot_os3\n" );
			}
		}
		else
		{
			PrintFault( IoErr(), "Nemo" );
		}
		
		FreeArgs(rdargs);
	}
	else
	{
		PrintFault( IoErr(), "Nemo" );
	}
	
	return( rc );
}


/*****************************************************************
 * 
 * END
 * 
 *****************************************************************/
