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
 *      DC.W $25B1 ; 0010 0101 1011 0001
 *      DC.W $1C00 ; 0001 1100 0000 0000
 *      DC.W $2200 ; 0010 0010 0000 0000
 * 
 * nemo paddusb d1,e0,e1;
 * 
 *      DC.W $FE01 ; 1111 1110 0000 0001
 *      DC.W $8914 ; 1000 1001 0001 0100
 * 
 * nemo paddusw d1,e0,e1;
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
#define FILE_BIN  "T:NEMO.BIN"
#define FILE_OUT  "T:NEMO.OUT"
#define FILE_CMD  "vasmm68k_mot_os3 -no-opt -Fbin -m68080 %s -o %s -quiet >%s"

#define OPT_MNEMONIC    0
#define OPT_COUNT       1

const UBYTE version[] = APP_VSTRING;

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

const STRPTR nibble[ 16 ] = 
{
	"0000", "0001", "0010", "0011",
	"0100", "0101", "0110", "0111",
	"1000", "1001", "1010", "1011",
	"1100", "1101", "1110", "1111"
};

VOID PrintBin( UWORD n )
{
	STRPTR argv[ 4 ];
	
	argv[ 0 ] = nibble[ ( n >> 12 ) & 0xf ];
	argv[ 1 ] = nibble[ ( n >>  8 ) & 0xf ];
	argv[ 2 ] = nibble[ ( n >>  4 ) & 0xf ];
	argv[ 3 ] = nibble[ ( n >>  0 ) & 0xf ];
	
	VPrintf("%s %s %s %s\n", argv);
}

/*****************************************************************
 * 
 * VOID PrintHex( UWORD n )
 * 
 *****************************************************************/

VOID PrintHex( UWORD n )
{
	UWORD argv[ 1 ];
	
	argv[ 0 ] = n;
	
	VPrintf("\tDC.W $%04x ; ", argv);
}

/*****************************************************************
 * 
 * VOID VSPrintf( STRPTR str, STRPTR fmt, APTR args )
 * 
 *****************************************************************/

VOID VSPrintf( STRPTR str, STRPTR fmt, APTR args )
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
	STRPTR commandArgs[] = { FILE_ASM, FILE_BIN, FILE_OUT };
	USHORT buffer[ 32 ];
	
	opts[ 0 ] = NULL;
	
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
			
			VSPrintf( commandString, FILE_CMD, commandArgs );
			
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
						
						VPrintf( "\n\t; %s\n", &opts[ OPT_MNEMONIC ] );
						
						for( i = 0; i < fib.fib_Size; i += 2 )
						{
							if( Read( file, buffer, 2 ) )
							{
								PrintHex( buffer[ 0 ] );
								PrintBin( buffer[ 0 ] );
							}
						}
						
						PutStr( "\n" );
						
						rc = RETURN_OK;
					}
					
					Close( file );
				}
				else
				{
					PutStr( "Syntax error.\n" );
				}
			}
			else
			{
				PutStr( "Required: vasmm68k_mot_os3\n" );
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
