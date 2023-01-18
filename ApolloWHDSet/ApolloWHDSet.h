/********************************************************************
 *** 
 *** Program:  ApolloWHDSet.h
 *** Purpose:  GUI to set WHDLoad game tooltypes
 *** Authors:  Flype and TuKo
 *** Target:   AmigaOS 3.x
 *** Compiler: GCC 2.95.3, SAS/C 6.59 
 *** 
 ********************************************************************/

#include <exec/types.h>

/********************************************************************
 *** SASC
 ********************************************************************/

#ifdef __SASC
#define REG(x) register __ ## x
#define ASM    __asm
#define SAVEDS __saveds
int CXBRK(void) { return (0); }
int _CXBRK(void) { return (0); }
void chkabort(void) { }
#endif

/********************************************************************
 *** MUI
 ********************************************************************/

#ifndef IPTR
#define IPTR ULONG
#endif

#ifndef MAKE_ID
#define MAKE_ID(a,b,c,d) ((ULONG) (a)<<24 | (ULONG) (b)<<16 | (ULONG) (c)<<8 | (ULONG) (d))
#endif

/********************************************************************
 *** APP
 ********************************************************************/

#define APP_NAME        "ApolloWHDSet"
#define APP_BASE        "APOLLOWHDSET"
#define APP_DATE        "18.1.2023"
#define APP_VERSION     "0.2.2"
#define APP_VERSTRING   APP_NAME " " APP_VERSION " (" APP_DATE ")"
#define APP_AUTHORS     "Flype and TuKo"
#define APP_COPYRIGHT   "Copyright 2023 by " APP_AUTHORS
#define APP_DESCRIPTION "GUI to set WHDLoad game tooltypes"
#define APP_HELPFILE    "PROGDIR:ApolloWHDSet.guide"

/********************************************************************
 *** WHDLOAD
 ********************************************************************/

#define WHDLOAD_NAME     "WHDLoad"
#define WHDLOAD_TT_NAME  "ExecutePostDisk"
#define WHDLOAD_TT_TOOL  "C:ApolloControl"

/********************************************************************
 *** EVENTS
 ********************************************************************/

enum eventIds
{
	EVENT_OPEN = 100,
	EVENT_SAVE,
	EVENT_ENABLE_ALL,
	EVENT_DISABLE_ALL,
	EVENT_INFO,
	EVENT_ABOUT,
	EVENT_ABOUT_MUI,
	EVENT_ICONIFY,
	EVENT_SETTINGS,
	EVENT_QUIT,
	EVENT_PRESET,
};

/********************************************************************
 *** PRESETS
 ********************************************************************/

typedef struct Preset
{
	LONG	pr_GameSpeed;
	LONG	pr_Superscalar;
	LONG	pr_DisplayMode;
	LONG	pr_ZoomMode;
	LONG	pr_ZoomShift;
	LONG	pr_Scanlines;
	APTR	pr_MN_Item;
	STRPTR	pr_MN_Shortcut;
	STRPTR	pr_MN_Label;
} Preset_t;

/********************************************************************
 *** OBJAPP
 ********************************************************************/

struct ObjApp
{
	APTR	App;
	APTR	MN_Main;
	APTR	WI_Main;
	
	CONST_STRPTR	GameSpeed[4];
	CONST_STRPTR	Superscalar[3];
	CONST_STRPTR	DisplayMode[3];
	CONST_STRPTR	ZoomMode[5];
	CONST_STRPTR	Scanlines[3];
};

/********************************************************************
 *** END OF FILE
 ********************************************************************/
