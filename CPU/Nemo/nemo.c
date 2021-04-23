/*****************************************************************
 * 
 * Project: Nemo
 * Version: 0.2
 * Author:  flype
 * Short:   68K/AMMX Mnemonic encoder
 * 
 *****************************************************************/

#include <exec/types.h>
#include <exec/memory.h>
#include <exec/execbase.h>
#include <dos/dos.h>
#include <stdio.h>
#include <string.h>
#include "nemo.h"

/*****************************************************************
 * DEFINES
 *****************************************************************/

#define TEMPLATE  "MNEMONIC/A/F"
#define FILE_ASM  "T:NEMO.ASM"
#define FILE_BIN  "T:NEMO.BIN"
#define FILE_LOG  "T:NEMO.LOG"
#define FILE_CMD  "vasmm68k_mot_os3 -no-opt -Fbin -m68080 %s -o %s -quiet >%s"

#define OPT_MNEMONIC    0
#define OPT_COUNT       1

const unsigned char ver[] = APP_VSTRING;

struct ExecBase *SysBase;

const char *bits[16] = {
	[0x0]="0000", [0x1]="0001", [0x2]="0010", [0x3]="0011",
	[0x4]="0100", [0x5]="0101", [0x6]="0110", [0x7]="0111",
	[0x8]="1000", [0x9]="1001", [0xa]="1010", [0xb]="1011",
	[0xc]="1100", [0xd]="1101", [0xe]="1110", [0xf]="1111"
};

void printbin( unsigned short n )
{
	printf( "%s %s %s %s\n",
		bits[ n >> 12 & 0xf ],
		bits[ n >>  8 & 0xf ],
		bits[ n >>  4 & 0xf ],
		bits[ n >>  0 & 0xf ]
	);
}

/*****************************************************************
 * ENTRY POINT
 *****************************************************************/

int main(int argc, char *argv[])
{
	int rc = FALSE;
	
	struct RDArgs *rdargs;   
	struct FileInfoBlock fib;
	long opts[OPT_COUNT];  
	char commandString[255];
	unsigned short buffer[32];
	
	memset((char *)opts, 0, sizeof(opts));
	
	if ( rdargs = (struct RDArgs *)ReadArgs(TEMPLATE, opts, NULL) )
	{
		BPTR file;
		
		file = Open( FILE_ASM, MODE_NEWFILE );
		
		if( file != 0 )
		{
			// 'Mnemonic' to 'ASM file'
			
			FPuts( file, "MAIN:\n\t" );
			FPuts( file, opts[ OPT_MNEMONIC ] );
			Close( file );
			
			// Call VASM
			
			sprintf( commandString, 
				FILE_CMD,
				FILE_ASM,
				FILE_BIN,
				FILE_LOG
			);
			
			DeleteFile( FILE_BIN );
			
			if( Execute( commandString, NULL, NULL ) )
			{
				file = Open( FILE_BIN, MODE_OLDFILE );
				
				if( file != 0 )
				{
					if( ExamineFH( file, &fib ) )
					{
						int i;
						
						printf( "\n\t; %s\n", opts[ OPT_MNEMONIC ] );
						
						for( i = 0; i < fib.fib_Size; i+=2 )
						{
							if( Read( file, buffer, 2 ) )
							{
								printf( "\tDC.W $%04x ; ", buffer[0]);
								printbin( buffer[0] );
							}
						}
						
						printf( "\n" );
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
			
		rc = TRUE;

		FreeArgs(rdargs);
	}
	else
	{
		PrintFault( IoErr(), "Nemo" );
	}
	
	exit( rc );
}


/*****************************************************************
 * END
 *****************************************************************/
