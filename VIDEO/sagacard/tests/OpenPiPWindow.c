/******************************************************************************
** OpenPiPWindow.c
** Picasso96/SAGA.card
******************************************************************************/

#include <intuition/screens.h>
#include <graphics/rastport.h>
#include <proto/exec.h>
#include <proto/graphics.h>
#include <proto/intuition.h>
#include <proto/picasso96.h>

/*****************************************************************************/

struct Library *P96Base;

/*****************************************************************************/

ULONG main(void)
{
    ULONG result = 5;
    
    if (P96Base = OpenLibrary("Picasso96API.library", 2))
    {
        LONG Width  = 256;
        LONG Height = 256;
        
        struct Window *win;
        
        if (win = p96PIP_OpenTags(
            P96PIP_SourceWidth,   Width,
            P96PIP_SourceHeight,  Height,
            P96PIP_SourceFormat,  RGBFB_R5G6B5,
            P96PIP_Type,          PIPT_MemoryWindow,
            WA_InnerWidth,        Width,
            WA_InnerHeight,       Height,
            WA_Activate,          TRUE,
            WA_RMBTrap,           TRUE,
            WA_DragBar,           TRUE,
            WA_DepthGadget,       TRUE,
            WA_SimpleRefresh,     TRUE,
            WA_CloseGadget,       TRUE,
            WA_IDCMP,             IDCMP_CLOSEWINDOW,
            WA_PubScreenName,     "Workbench",
            WA_Title,             "SAGA.Card PiP Test",
            TAG_DONE))
        {
            BOOL   running = TRUE;
            struct IntuiMessage	*imsg;
            struct BitMap *bm = NULL;
            struct RastPort *rp = NULL;
            
            /* Get PiP-Window properties */
            
            p96PIP_GetTags(win, 
                P96PIP_SourceBitMap, (ULONG)&bm, 
                TAG_END);
            
            p96PIP_GetTags(win, 
                P96PIP_SourceRPort, (ULONG)&rp, 
                TAG_END);
            
            /* Draw in the PiP-Window RastPort */
            
            if (rp)
            {
                UWORD x, y;
                
                for (y = 0; y < Height; y++)
                {
                    for (x = 0; x < Width; x++)
                    {
                        ULONG color = ((x * Height + y) * Width);
                        
                        p96WritePixel(rp, x, y, color);
                    }
                }
                
                SetAPen(rp, 2);
                SetDrMd(rp, JAM1);
                DrawEllipse(rp, 80, 80, 30, 30);
                
                SetAPen(rp, 1);
                SetDrMd(rp, COMPLEMENT);
                
                SetOutlinePen(rp, 3);
                DrawEllipse(rp, 95, 95, 40, 40);
            }
            
            /* Event Loop */
            
            while (running)
            {
                WaitPort(win->UserPort);
                
                while (imsg = (struct IntuiMessage *)GetMsg(win->UserPort))
                {
                    switch (imsg->Class)
                    {
                        case IDCMP_CLOSEWINDOW:
                            running = FALSE;
                            break;
                    }
                    
                    ReplyMsg((struct Message *)imsg);
                }
            }
            
            p96PIP_Close(win);
            
            result = 0;
        }
        
        CloseLibrary(P96Base);
    }
    
    return(result);
}

/*****************************************************************************/
