/**********************************************************
 * 
 * Project:   V4JoyTest
 * Short:     Vampire V4 Joypad Tester program
 * Version:   1.0
 * Author:    flype
 * Copyright: (C) 2015-2021 APOLLO-Team
 * Compiler:  SAS/C 6.59 for AmigaOS 3.x
 * V4 Core:   SA_7925.jic
 * 
 **********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dos/dos.h>
#include <exec/exec.h>
#include <exec/types.h>
#include <exec/execbase.h>
#include <graphics/gfxbase.h>
#include <intuition/intuitionbase.h>
#include <libraries/gadtools.h>

#include <proto/dos.h>
#include <proto/exec.h>
#include <proto/gadtools.h>
#include <proto/graphics.h>
#include <proto/intuition.h>

#include "ControllerTest.h"

/**********************************************************
 *
 * DEFINES
 *
 **********************************************************/

#define MAX_PORT   2
#define MAX_BUTTON 16

#define GRIDW  20
#define GRIDH  16
#define SCALE  10

#define WIDTH  ((((GRIDW-1)*SCALE)*MAX_PORT)+SCALE)
#define HEIGHT (GRIDH*SCALE)

/**********************************************************
 *
 * GLOBALS
 *
 **********************************************************/

extern struct ExecBase *SysBase;
extern struct DOSBase *DOSBase;

struct IntuitionBase *IntuitionBase = NULL;
struct Library *GadToolsBase = NULL;
struct GfxBase *GfxBase = NULL;
struct Screen *screen = NULL;
struct Window *window = NULL;
struct VisualInfo *visualInfo = NULL;

/**********************************************************
 *
 * GLOBALS
 *
 **********************************************************/

USHORT JoyPos[ MAX_BUTTON ][ 4 ] = {
//	{  X,  Y, W, H },
	{  2,  2, 1, 1 }, // Plugged
	{ 14, 10, 2, 2 }, // A
	{ 16,  8, 2, 2 }, // B
	{ 12,  8, 2, 2 }, // X
	{ 14,  6, 2, 2 }, // Y
	{  4,  3, 2, 1 }, // LT
	{ 14,  3, 2, 1 }, // RT
	{  4,  4, 2, 1 }, // LB
	{ 14,  4, 2, 1 }, // RB
	{  8,  5, 1, 1 }, // Back
	{ 11,  5, 1, 1 }, // Start
	{ 17,  2, 1, 1 }, // Unassigned
	{  4,  6, 2, 2 }, // Up
	{  4, 10, 2, 2 }, // Down
	{  2,  8, 2, 2 }, // Left
	{  6,  8, 2, 2 }  // Right
};

STRPTR JoyStr[ MAX_BUTTON ] = {
	NULL,"A","B","X","Y",
	NULL,NULL,
	NULL,NULL,
	NULL,NULL,
	NULL,"U","D","L","R"
};

UWORD JoyState[ MAX_PORT ];

/**********************************************************
 * 
 * IsVampireV4()
 * 
 **********************************************************/

LONG IsVampireV4(void)
{
	LONG result = FALSE;
	
	if (BTST(SysBase->AttnFlags, AFF_68080))
	{
		UBYTE model = (*(volatile UWORD*)VREG_BOARD) >> 8;
		
		result = (model == VMODEL_V4SA);
	}
	
	return(result);
}

/**********************************************************
 * 
 * DrawButton()
 * 
 **********************************************************/

void DrawButton(
	USHORT x, USHORT y,
	USHORT w, USHORT h, 
	STRPTR s, ULONG type)
{
	if (window)
	{
		struct RastPort *rp;
		
		if (rp = window->RPort)
		{
			x *= SCALE;
			y *= SCALE;
			w *= SCALE;
			h *= SCALE;
			
			SetDrMd(rp, JAM1);
			
			// Draw Button Frame
			
			switch (type)
			{
			case 0: // Released
				DrawBevelBox(rp, x, y, w, h,
				//	GTBB_Recessed,  FALSE,
					GTBB_FrameType, BBFT_BUTTON,
					GT_VisualInfo,  visualInfo, 
					TAG_END);
				SetAPen(rp, FILLPEN);
				RectFill(rp, x+1, y+1, x+w-2, y+h-2);
				break;
			case 1: // Pressed
				DrawBevelBox(rp, x, y, w, h,
					GTBB_Recessed,  TRUE,
					GTBB_FrameType, BBFT_BUTTON,
					GT_VisualInfo,  visualInfo, 
					TAG_END);
				SetAPen(rp, SHINEPEN);
				RectFill(rp, x+1, y+1, x+w-2, y+h-2);
				break;
			case 2: // Background
				DrawBevelBox(rp, x, y, w, h,
					GTBB_Recessed,  TRUE,
					GTBB_FrameType, BBFT_RIDGE,
					GT_VisualInfo,  visualInfo, 
					TAG_END);
				SetAPen(rp, DETAILPEN);
				RectFill(rp, x+4, y+4, x+w-4, y+h-4);
				break;
			}
			
			// Draw Button Text
			
			if (s)
			{
				Move(rp, x + 7, y + 13);
				SetAPen(rp, BLOCKPEN);
				Text(rp, s, strlen(s));
			}
		}
	}
}

/**********************************************************
 * 
 * OnJoyDrawChange()
 * 
 **********************************************************/

void OnJoyDrawChange(ULONG port)
{
	static char buf[16];
	
	if (window)
	{
		struct RastPort *rp;
		
		if (rp = window->RPort)
		{
			int x = (GRIDW - 1) * port;
			
			sprintf(buf, "0x%04x\0", JoyState[ port ]);
			SetDrMd(rp, JAM2);
			SetAPen(rp, BLOCKPEN);
			SetBPen(rp, DETAILPEN);
			Move(rp, ((x + 8) * SCALE) + 2, (13 * SCALE) - 3);
			Text(rp, buf, 6);
		}
	}
}

/**********************************************************
 * 
 * OnJoyDrawPressed()
 * 
 **********************************************************/

void OnJoyDrawPressed(ULONG port, ULONG button)
{
	int x = (GRIDW - 1) * port;
	
	// Draw Joy Button
	
	DrawButton(
		JoyPos[ button ][ 0 ] + x,
		JoyPos[ button ][ 1 ],
		JoyPos[ button ][ 2 ],
		JoyPos[ button ][ 3 ],
		JoyStr[ button ], 1);
	
	// Draw Bit Button
	
	DrawButton(
		(MAX_BUTTON - button - 1) + x + 2, 
		13, 1, 1, NULL, 1);
}

/**********************************************************
 * 
 * OnJoyDrawReleased()
 * 
 **********************************************************/

void OnJoyDrawReleased(ULONG port, ULONG button)
{
	int x = (GRIDW - 1) * port;
	
	// Draw Joypad Button
	
	DrawButton(
		JoyPos[ button ][ 0 ] + x,
		JoyPos[ button ][ 1 ],
		JoyPos[ button ][ 2 ],
		JoyPos[ button ][ 3 ],
		JoyStr[ button ], 0);
	
	// Draw Joypad Bit Number
	
	DrawButton(
		(MAX_BUTTON - button - 1) + x + 2, 
		13, 1, 1, NULL, 0);
}

/**********************************************************
 *
 * OnJoyDraw()
 * 
 **********************************************************/

void OnJoyDraw(ULONG port)
{
	static char buf[16];
	
	if (window)
	{ 
		struct RastPort *rp;
		
		if (rp = window->RPort)
		{
			int button;
			int x = (GRIDW - 1) * port;
			
			// Draw Joypad Background
			
			DrawButton(1 + x, 1, 18, 14, NULL, 2);
			
			// Draw Joypad Port Number
			
			sprintf(buf, "Joypad %ld\0", port + 1);
			
			SetAPen(rp, BLOCKPEN);
			Move(rp, ((x + 8) * SCALE) - 3, (SCALE * 3) - 2);
			Text(rp, buf, 8);
			
			SetAPen(rp, TEXTPEN);
			Move(rp, ((x + 8) * SCALE) - 4, (SCALE * 3) - 3);
			Text(rp, buf, 8);
			
			// Draw Joypad Register
			
			sprintf(buf, "JOY%ldBTN\0", port + 1);
			
			SetAPen(rp, TEXTPEN);
			Move(rp, ((x + 8) * SCALE) - 2, (SCALE * (GRIDH - 4)) - 3);
			Text(rp, buf, 7);
			
			// Draw Joypad Buttons
			
			for (button = 0; button < MAX_BUTTON; button++)
			{       
				OnJoyDrawReleased(port, button);
			}
			
			// Draw Joypad State value
			
			OnJoyDrawChange(port);
		}
	}
}

/**********************************************************
 * 
 * GetJoyState()
 * 
 **********************************************************/

void GetJoyState(void)
{
	int port;
	
	for (port = 0; port < MAX_PORT; port++)
	{
		UWORD oldState = JoyState[ port ];
		UWORD newState = *(volatile UWORD *)(VREG_JOYP1 + (port * sizeof(UWORD)));
		
		if (newState != oldState)
		{
			ULONG button;
			
			for (button = 0; button < MAX_BUTTON; button++)
			{
				int a = 1 << button;
				int b = newState & a;
				int c = oldState & a;
				
				if (b == a && c == 0)
				{
					OnJoyDrawPressed(port, button);
				}
				else
				if (b == 0 && c == a)
				{
					OnJoyDrawReleased(port, button);
				}
			}
			
			JoyState[ port ] = newState;
			
			OnJoyDrawChange(port);
		}
	}
}

/**********************************************************
 * 
 * OpenUI()
 * 
 **********************************************************/

BOOL OpenUI(void)
{
	BOOL result = FALSE;
	
	if (screen = LockPubScreen(NULL))
	{
		visualInfo = GetVisualInfoA(screen, NULL);
		
		window = OpenWindowTags(
			NULL, 
			WA_Activate,      TRUE,
			WA_AutoAdjust,    TRUE,
			WA_CloseGadget,   TRUE,
			WA_DepthGadget,   TRUE,
			WA_DragBar,       TRUE,
			WA_GimmeZeroZero, TRUE,
			WA_RMBTrap,       TRUE,
			WA_NoCareRefresh, TRUE,
			WA_Title,         TITLE,
			WA_Left,          (screen->Width - WIDTH) / 2,
			WA_Top,           (screen->Height - HEIGHT) / 2,
			WA_InnerWidth,    WIDTH,
			WA_InnerHeight,   HEIGHT,
			WA_MinWidth,      WIDTH,
			WA_MinHeight,     HEIGHT,
			WA_MaxWidth,      WIDTH,
			WA_MaxHeight,     HEIGHT,
			WA_IDCMP,
				IDCMP_CLOSEWINDOW | 
				IDCMP_REFRESHWINDOW | 
				IDCMP_RAWKEY,
			TAG_DONE);
		
		if (window)
		{
			result = TRUE;
		}
		
		UnlockPubScreen(NULL, screen);
	}
	
	return(result);
}

/**********************************************************
 * 
 * CloseUI()
 * 
 **********************************************************/

BOOL CloseUI(void)
{
	BOOL result = FALSE;
	
	if (window)
		CloseWindow(window);
	
	if (visualInfo)
		FreeVisualInfo(visualInfo);
	
	return(result);
}

/**********************************************************
 *
 * DrawUI()
 * 
 **********************************************************/

void DrawUI(void)
{
	if (window)
	{ 
		ULONG port;
		
		SetDrMd(window->RPort, JAM1);
		SetRast(window->RPort, DETAILPEN);
		
		for (port = 0; port < MAX_PORT; port++)
		{
			OnJoyDraw(port);
		}
	}
}

/**********************************************************
 *
 * ProcessUI()
 * 
 **********************************************************/

LONG ProcessUI(void)
{
	struct Message *msg;
	
	LONG sigs = SetSignal(0L, 0L);
	
	if (!window)
		return(sigs |= SIGBREAKF_CTRL_C);
	
	while (msg = GetMsg(window->UserPort))
	{
		struct IntuiMessage *imsg = (struct IntuiMessage *)msg;
		
		ReplyMsg(msg);
		
		switch (imsg->Class)
		{
		case IDCMP_REFRESHWINDOW:
			DrawUI();
			break;
		case IDCMP_CLOSEWINDOW:
			sigs |= SIGBREAKF_CTRL_C;
			break;
		case IDCMP_RAWKEY:
			switch (imsg->Code)
			{
			case 0x45: // ESCAPE
				sigs |= SIGBREAKF_CTRL_C;
				break;
			}
			break;
		}
	}
	
	return(sigs);
}

/**********************************************************
 * 
 * OpenLibs()
 * 
 **********************************************************/

BOOL OpenLibs(void)
{
	BOOL result = FALSE;
	
	GfxBase = (struct GfxBase*)OpenLibrary(
		"graphics.library", 0L);
	
	GadToolsBase = (struct Library*)OpenLibrary(
		"gadtools.library", 0L);
	
	IntuitionBase = (struct IntuitionBase*)OpenLibrary(
		"intuition.library", 0L);
	
	return(result);
}

/**********************************************************
 * 
 * CloseLibs()
 * 
 **********************************************************/

BOOL CloseLibs(void)
{
	BOOL result = FALSE;
	
	if (IntuitionBase)
		CloseLibrary((struct Library*)IntuitionBase);
	
	if (GadToolsBase)
		CloseLibrary((struct Library*)GadToolsBase);
	
	if (GfxBase)
		CloseLibrary((struct Library*)GfxBase);
	
	return(result);
}

/**********************************************************
 * 
 * main()
 * 
 **********************************************************/

ULONG main(ULONG argc, char *argv[])
{
	ULONG result = RETURN_WARN;

	if (IsVampireV4())
	{
		OpenLibs();
		
		if (OpenUI())
		{
			DrawUI();
			
			while (TRUE)
			{
				GetJoyState();
				
				if (ProcessUI() & SIGBREAKF_CTRL_C)
				{
					SetSignal(0L, SIGBREAKF_CTRL_C);
					break;
				}
				
				Delay(1);
			}
			
			CloseWindow(window);
			
			result = RETURN_OK;
		}
		
		CloseLibs();
	}
	
	return(result);
}

/**********************************************************
 *
 * END
 *
 **********************************************************/
