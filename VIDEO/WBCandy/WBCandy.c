/**********************************************************
 * 
 * Project:  WBCandy for Amiga Vampire
 * Authors:  Philippe CARPENTIER
 * Date:     nov-2021
 * Compiler: SAS/C 6.59
 * 
 **********************************************************
 * 
 * Example:
 * WBCandy d=anime2 c=12 ms=5 k=2116
 * 
 **********************************************************/

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	<dos/dos.h>
#include	<exec/exec.h>
#include	<exec/ports.h>
#include	<graphics/gfxbase.h>
#include	<intuition/intuitionbase.h>
#include	<libraries/commodities.h>
#include	<workbench/startup.h>
#include	<workbench/workbench.h>

#include	<proto/commodities.h>
#include	<proto/dos.h>
#include	<proto/exec.h>
#include	<proto/graphics.h>
#include	<proto/icon.h>
#include	<proto/intuition.h>

#include	"vampire/saga.h"
#include	"vampire/vampire.h"
#include	"proto/vampire.h"

/**********************************************************
 *
 * Defines
 * 
 **********************************************************/

#define APP_NAME     "WBCandy"
#define APP_TITLE    "WBCandy 0.1alpha"
#define APP_DESCR    "An AmigaOS3 Workbench-Candy for Vampire"
#define APP_TEMPLATE "D=DIRECTORY/A,C=FRAMECOUNT/N/A,MS=MILLISECONDS/N,K=COLORKEY/N,QUIET/S"
#define VERSTRING    "$VER: WBCandy (28.11.2021) Philippe CARPENTIER"

enum {
	OPT_DIRECTORY,  // 0
	OPT_FRAMECOUNT, // 1
	OPT_FRAMEDELAY, // 2
	OPT_COLORKEY,   // 3
	OPT_QUIET,      // 4
	OPT_COUNT       // 5
};

/**********************************************************
 *
 * Globals
 * 
 **********************************************************/

extern struct ExecBase   *  SysBase;
extern struct DosLibrary *  DOSBase;
extern struct WBStartup  * _WBenchMsg;

struct Library       * CxBase        = NULL;
struct GfxBase       * GfxBase       = NULL;
struct Library       * IconBase      = NULL;
struct IntuitionBase * IntuitionBase = NULL;
struct Screen        * screen        = NULL;

ULONG FBMem   = NULL;
ULONG FBPtr   = NULL;
BOOL  bRender = TRUE;

ULONG quiet       = FALSE;
ULONG colorKey    = 0x0000;
ULONG frameCount  = 1;
ULONG frameDelay  = 1;
ULONG frameIndex  = 0;
ULONG frameWidth  = 1280; 
ULONG frameHeight = 720;
ULONG frameBuffer = NULL;
UBYTE directory[256];

CxObj *broker;
struct MsgPort *broker_mp;

struct NewBroker newbroker = 
{
    NB_VERSION, // nb_Version
    APP_NAME,   // nb_Name
    APP_TITLE,  // nb_Title
    APP_DESCR,  // nb_Descr
    NBU_NOTIFY, // nb_Unique
    0,          // nb_Flags
    1,          // nb_Pri
    0,          // nb_Port
    0           // nb_ReservedChannel
};

/**********************************************************
 *
 * Init & Cleanup functions
 * 
 **********************************************************/

void App_Init(void) 
{
	if (!(SysBase->AttnFlags & (1 << 10)))
	{
		printf("No AC68080 processor!");
		exit(RETURN_ERROR);
	}
	
	if (!(IntuitionBase = (struct IntuitionBase*)OpenLibrary("intuition.library", 39L)))
	{
		printf("No intuition.library!");
		exit(RETURN_ERROR);
	}
	
	if (!(GfxBase = (struct GfxBase*)OpenLibrary("graphics.library", 0L)))
	{
		printf("No graphics.library!");
		exit(RETURN_ERROR);
	}
	
	if (!(IconBase = (struct Library*)OpenLibrary("icon.library", 0L)))
	{
		printf("No icon.library!");
		exit(RETURN_ERROR);
	}
	
	if (!(CxBase = (struct Library*)OpenLibrary("commodities.library", 0L)))
	{
		printf("No commodities.library!");
		exit(RETURN_ERROR);
	}
}

void App_Cleanup(void)
{
	if (CxBase)
	{
		CloseLibrary((struct Library*)CxBase);
		CxBase = NULL;
	}
	
	if (GfxBase)
	{
		CloseLibrary((struct Library*)GfxBase);
		GfxBase = NULL;
	}
	
	if (IconBase)
	{
		CloseLibrary((struct Library*)IconBase);
		IconBase = NULL;
	}
	
	if (IntuitionBase)
	{
		CloseLibrary((struct Library*)IntuitionBase);
		IntuitionBase = NULL;
	}
}

/**********************************************************
 *
 * Show/Hide PiP display
 * 
 **********************************************************/

void HidePiP(void)
{
	WRITE16(SAGA_PIP_PIXFMT,   0);
	WRITE16(SAGA_PIP_X0,       0);
	WRITE16(SAGA_PIP_Y0,       0);
	WRITE16(SAGA_PIP_X1,       0);
	WRITE16(SAGA_PIP_Y1,       0);
	WRITE16(SAGA_PIP_COLORKEY, 0);
	WRITE32(SAGA_PIP_BPLPTR,   0);
}

void ShowPiP(void)
{
	WRITE16(SAGA_PIP_COLORKEY, ((UWORD)colorKey) + 0x8000);
	WRITE16(SAGA_PIP_PIXFMT,   SAGAF_RGB16);
	WRITE32(SAGA_PIP_BPLPTR,   frameBuffer);
	WRITE16(SAGA_PIP_X0,       SAGA_PIP_DELTAX);
	WRITE16(SAGA_PIP_Y0,       SAGA_PIP_DELTAY);
	WRITE16(SAGA_PIP_X1,       SAGA_PIP_DELTAX + frameWidth);
	WRITE16(SAGA_PIP_Y1,       SAGA_PIP_DELTAY + frameHeight);
}

/**********************************************************
 *
 * Frames functions
 * 
 **********************************************************/

BOOL LoadFrame(UBYTE *filename, UBYTE *frameBuf)
{
	BOOL  result = FALSE;
	FILE  *filePtr;
	UBYTE *fileBuf;
	ULONG i;
	
	// Open BMP file
	
	if (!quiet)
	{
		printf("LoadFrame('%s')\n", filename);
	}
	
	if (filePtr = fopen(filename, "rb"))
	{
		ULONG fileSize = (frameWidth * frameHeight * 2);
		
		if (fileBuf = malloc(fileSize))
		{
			// Skip BMP header (138 bytes)
			
			fseek(filePtr, 138, SEEK_SET);
			
			// Read BMP pixels
			
			fread(fileBuf, fileSize, 1, filePtr);
			
			// Convert pixels to little endian
			
			for (i = 0; i < fileSize; i += 2)
			{
				frameBuf[i+0] = fileBuf[i+1];
				frameBuf[i+1] = fileBuf[i+0];
			}
			
			free(fileBuf);
			result = TRUE;
		}
		
		fclose(filePtr);
	}
	
	return(result);
}

BOOL LoadFrames(void)
{
	ULONG result = 0;
	ULONG bytesPerFrame;
	UBYTE filename[256];
	
	if (frameCount == 0)
		frameCount = 1;
	
	if (frameDelay == 0)
		frameDelay = 1;
	
	// Allocate memory for N frames
	
	bytesPerFrame = (frameWidth * frameHeight * 2);
	
	if (FBMem = (ULONG)malloc((bytesPerFrame * frameCount) + 16))
	{
		// Align memory to 16 bytes
		
		FBPtr = (FBMem + (16 - (FBMem % 16)));
		
		// Load all frames
		
		for (frameIndex = 0; frameIndex < frameCount; frameIndex++)
		{
			sprintf(filename, "%s/%02ld.bmp", 
				directory, frameIndex);
			
			result += LoadFrame(filename, 
				(UBYTE *)(FBPtr + (bytesPerFrame * frameIndex)));
			
			Delay(1);
		}
	}
	
	return((BOOL)((result == frameCount) ? TRUE : FALSE));
}

void FreeFrames(void)
{
	if (FBMem)
	{
		free((void *)FBMem);
		FBMem = NULL;
	}
}

void UpdateFrames(void)
{
	frameIndex++;
	
	if (frameIndex >= frameCount)
		frameIndex = 0;
	
	frameBuffer = (FBPtr + 
		(frameWidth * frameHeight * 2 * frameIndex));
}

/**********************************************************
 *
 * ProcessMsg()
 * 
 **********************************************************/

LONG ProcessMsg(void)
{
	CxMsg *msg;
	ULONG msgid, msgtype;
	LONG sigs = SetSignal(0L, 0L);
	
	while(msg = (CxMsg *)GetMsg(broker_mp))
	{
		msgid = CxMsgID(msg);
		msgtype = CxMsgType(msg);
		ReplyMsg((struct Message *)msg);
		
		switch(msgtype)
		{
		case CXM_COMMAND:
			switch(msgid)
			{
			case CXCMD_DISABLE:
				ActivateCxObj(broker, 0L);
				bRender = FALSE;
				HidePiP();
				break;
			case CXCMD_ENABLE:
				ActivateCxObj(broker, 1L);
				bRender = TRUE;
				break;
			case CXCMD_UNIQUE:
				sigs |= SIGBREAKF_CTRL_C;
				break;
			case CXCMD_KILL:
				sigs |= SIGBREAKF_CTRL_C;
				break;
			}
			break;
		case CXM_IEVENT:
			break;
		}
	}
	
	return(sigs);
}

/**********************************************************
 *
 * CLIStart()
 * 
 **********************************************************/

void CLIStart(void)
{
	LONG opts[OPT_COUNT];
	struct RDArgs * rdargs;
	
	opts[OPT_DIRECTORY ] = 0;
	opts[OPT_FRAMECOUNT] = 0;
	opts[OPT_FRAMEDELAY] = 0;
	opts[OPT_COLORKEY  ] = 0;
	
	if (rdargs = ReadArgs(APP_TEMPLATE, opts, NULL))
	{
		if (opts[OPT_DIRECTORY])
			sprintf(directory, "%s", (STRPTR)opts[OPT_DIRECTORY]);
		
		if (opts[OPT_FRAMECOUNT])
			frameCount = *(ULONG *)opts[OPT_FRAMECOUNT];
		
		if (opts[OPT_FRAMEDELAY])
			frameDelay = *(ULONG *)opts[OPT_FRAMEDELAY];
		
		if (opts[OPT_COLORKEY])
			colorKey = *(ULONG *)opts[OPT_COLORKEY];
		
		if (opts[OPT_QUIET])
			quiet = TRUE;
		
		FreeArgs(rdargs);
	}
	else
	{
		PrintFault(IoErr(), APP_NAME);
	}
}

/**********************************************************
 *
 * WBStart()
 * 
 **********************************************************/

void WBStart(void)
{
	struct DiskObject * diskObject;
	
	struct WBArg *wbArg = _WBenchMsg->sm_ArgList;
	
	if (diskObject = GetDiskObject(wbArg->wa_Name))
	{
		STRPTR *toolTypes;
		
		if (toolTypes = diskObject->do_ToolTypes)
		{
			STRPTR toolType;
			
			if (toolType = FindToolType(toolTypes, "DIRECTORY"))
				sprintf(directory, "%s", toolType);
			
			if (toolType = FindToolType(toolTypes, "FRAMECOUNT"))
				frameCount = atoi(toolType);
			
			if (toolType = FindToolType(toolTypes, "FRAMEDELAY"))
				frameDelay = atoi(toolType);
			
			if (toolType = FindToolType(toolTypes, "COLORKEY"))
				colorKey = atoi(toolType);
			
			if (toolType = FindToolType(toolTypes, "QUIET"))
				quiet = TRUE;
		}
		
		FreeDiskObject(diskObject);
	}
	else
	{
		PrintFault(IoErr(), APP_NAME);
	}
}

/**********************************************************
 *
 * main()
 * 
 **********************************************************/

ULONG main(ULONG argc, char *argv[])
{
	ULONG rc = RETURN_WARN;
	
	App_Init();
	
	if (_WBenchMsg)
	{
		WBStart();
	}
	else
	{
		CLIStart();
	}
	
	if (!quiet)
	{
		printf("%s\n", (VERSTRING+6));
		printf("directory   = %s\n",    directory);
		printf("colorKey    = %04lx\n", colorKey);
		printf("frameCount  = %lu\n",   frameCount);
		printf("frameDelay  = %lu\n",   frameDelay);
		printf("frameWidth  = %lu\n",   frameWidth);
		printf("frameHeight = %lu\n",   frameHeight);
	}
	
	if (broker_mp = CreateMsgPort())
	{
		newbroker.nb_Port = broker_mp;
		
		if (broker = CxBroker(&newbroker, NULL))
		{
			CxMsg * msg;
			
			ActivateCxObj(broker, 1L);
			
			if (LoadFrames())
			{
				while (TRUE)
				{
					if (ProcessMsg() & SIGBREAKF_CTRL_C)
					{
						SetSignal(0L, SIGBREAKF_CTRL_C);
						break;
					}
					
					if (bRender)
					{
						UpdateFrames();
						ShowPiP();
					}
					
					Delay(frameDelay);
				}
				
				HidePiP();
				FreeFrames();
				rc = RETURN_OK;
			}
			
			DeleteCxObj(broker);
			
			while(msg = (CxMsg *)GetMsg(broker_mp))
				ReplyMsg((struct Message *)msg);
		}
		
		DeleteMsgPort(broker_mp);
	}
	
	App_Cleanup();
	
	return (rc);
}

/**********************************************************
 *
 * END OF FILE
 * 
 **********************************************************/
