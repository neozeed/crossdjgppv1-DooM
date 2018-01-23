#define GRAPHICS
/* Emacs style mode select   -*- C++ -*- */
/*----------------------------------------------------------------------------- */
/* */
/* $Id:$ */
/* */
/* Copyright (C) 1993-1996 by id Software, Inc. */
/* */
/* This program is free software; you can redistribute it and/or */
/* modify it under the terms of the GNU General Public License */
/* as published by the Free Software Foundation; either version 2 */
/* of the License, or (at your option) any later version. */
/* */
/* This program is distributed in the hope that it will be useful, */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/* GNU General Public License for more details. */
/* */
/* $Log:$ */
/* */
/* DESCRIPTION: */
/*	DOOM graphics stuff for X11, UNIX. */
/* */
/*----------------------------------------------------------------------------- */
/*
static const char
        rcsid[] = "$Id: i_x.c,v 1.6 1997/02/03 22:45:10 b1 Exp $";
*/
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/time.h>
#include <sys/types.h>

#include <signal.h>
//#include <graphics.h>
#include <allegro.h>

#include <pc.h>
#include <go32.h>

/*
GO32 bits
 */
typedef struct {
  unsigned long size_of_this_structure_in_bytes;
  unsigned long linear_address_of_primary_screen;
  unsigned long linear_address_of_secondary_screen;
  unsigned long linear_address_of_transfer_buffer;
  unsigned long size_of_transfer_buffer; /* >= 4k */
  unsigned long pid;
  unsigned char master_interrupt_controller_base;
  unsigned char slave_interrupt_controller_base;
  unsigned short selector_for_linear_memory;
  unsigned long linear_address_of_stub_info_structure;
  unsigned long linear_address_of_original_psp;
  unsigned short run_mode;
  unsigned short run_mode_info;
} Go32_Info_Block;

#include <dos.h>

#include <dpmi.h>


#include "doomstat.h"
#include "i_system.h"
#include "v_video.h"
#include "m_argv.h"
#include "d_main.h"

#include "doomdef.h"
//doomdef.h
#define KEYD_PAUSE    0xff
/*dosstuff -newly added */
char oldkeystate[128];
byte* dascreen;


void I_ShutdownGraphics(void)
{
#ifdef GRAPHICS
      __dpmi_regs r;

      r.x.ax = 3;
      __dpmi_int(0x10, &r);
#endif
}



/* */
/* I_StartFrame */
/* */
void I_StartFrame (void)
{
	/* er? */

}


void I_GetEvent()
{

  event_t event;
  int i,j;

  char keystate[128];
  int xmickeys,ymickeys,buttons;
  static int lastbuttons=0;

  //key presses
  for (i=0;i<128;i++)
    keystate[i]=key[i];
  for (i=0;i<128;i++)
    {
    char normkey,extkey,oldnormkey,oldextkey;

    normkey=keystate[i]&KB_NORMAL; extkey=keystate[i]&KB_EXTENDED;
    oldnormkey=oldkeystate[i]&KB_NORMAL; oldextkey=oldkeystate[i]&KB_EXTENDED;

    if ((normkey!=0)&&(oldnormkey==0))
      {
      event.type=ev_keydown;
      event.data1=I_ScanCode2DoomCode(i);
      D_PostEvent(&event);
      }
    if ((normkey==0)&&(oldnormkey!=0))
      {
      event.type=ev_keyup;
      event.data1=I_ScanCode2DoomCode(i);
      D_PostEvent(&event);
      }
    if ((extkey!=0)&&(oldextkey==0))
      {
      if ((i==0x48)||(i==0x4d)||(i==0x50)||(i==0x4b)||(i==KEY_ALTGR)||(i==KEY_RCONTROL)||(i==KEY_PGDN)||(i==KEY_PGUP)||(i==KEY_HOME)||(i==KEY_PRTSCR))
        {
        event.type=ev_keydown;
        event.data1=I_ScanCode2DoomCode(i);
        D_PostEvent(&event);
        }
      else if (i==0x7b)
        {
        event.type=ev_keydown;
        event.data1=KEYD_PAUSE;
        D_PostEvent(&event);
        key[0x7b]=0;break;
        event.type=ev_keyup;
        event.data1=KEYD_PAUSE;
        D_PostEvent(&event);
        }
      }
    if ((extkey==0)&&(oldextkey!=0))
      {
      if ((i==0x48)||(i==0x4d)||(i==0x50)||(i==0x4b)||(i==KEY_ALTGR)||(i==KEY_RCONTROL)||(i==KEY_PGDN)||(i==KEY_PGUP)||(i==KEY_HOME)||(i==KEY_PRTSCR))
        {
        event.type=ev_keyup;
        event.data1=I_ScanCode2DoomCode(i);
        D_PostEvent(&event);
        }
      }
    }
  memcpy(oldkeystate,keystate,128);

#if 0


	event_t event;
	char tempkey[128];
	char tempextendedkey[128];
	int i,k;
	while (kbdtail < kbdhead)
	{
		k = keyboardque[kbdtail&(KBDQUESIZE-1)];
		kbdtail++;

		/* extended keyboard shift key bullshit	*/
		if ( (k&0x7f)==SC_LSHIFT || (k&0x7f)==SC_RSHIFT )
		{
			if ( keyboardque[(kbdtail-2)&(KBDQUESIZE-1)]==0xe0 )
				continue;
			k &= 0x80;
			k |= SC_RSHIFT;
		}

		if (k==0xe0)
			continue;               /* special / pause keys */
		if (keyboardque[(kbdtail-2)&(KBDQUESIZE-1)] == 0xe1)
			continue;                               /* pause key bullshit */

		if (k==0xc5 && keyboardque[(kbdtail-2)&(KBDQUESIZE-1)] == 0x9d)
		{
			event.type = ev_keydown;
			event.data1 = KEY_PAUSE;
			D_PostEvent (&event);
			continue;
		}

		if (k&0x80)
			event.type = ev_keyup;
		else
			event.type = ev_keydown;
		k &= 0x7f;
		switch (k)
		{
		case SC_UPARROW:
			event.data1 = KEY_UPARROW;
			break;
		case SC_DOWNARROW:
			event.data1 = KEY_DOWNARROW;
			break;
		case SC_LEFTARROW:
			event.data1 = KEY_LEFTARROW;
			break;
		case SC_RIGHTARROW:
			event.data1 = KEY_RIGHTARROW;
			break;
		default:
			event.data1 = scantokey[k];
			break;
		}
		D_PostEvent (&event);
	}
#endif



	/*mouse movement */

}


/* */
/* I_StartTic */
/* */
void I_StartTic()
{
	I_GetEvent();
	/*i dont think i have to do anything else here */

}



/* */
/* I_UpdateNoBlit */
/* */
void I_UpdateNoBlit (void)
{
	/* what is this? */
}

void I_FinishUpdate(void)
{
	static int lasttic;
	int tics;
	int i;
	/* UNUSED static unsigned char *bigscreen=0; */

	/* draws little dots on the bottom of the screen */
	if (devparm)
	{

		i = I_GetTime();
		tics = i - lasttic;
		lasttic = i;
		if (tics > 20) tics = 20;

		for (i=0; i<tics*2; i+=2)
			screens[0][ (SCREENHEIGHT-1)*SCREENWIDTH + i] = 0xff;
		for (; i<20*2; i+=2)
			screens[0][ (SCREENHEIGHT-1)*SCREENWIDTH + i] = 0x0;

	}

	/*blast it to the screen */
	//  memcpy(dascreen,screens[0],SCREENWIDTH*SCREENHEIGHT);
	dosmemput(screens[0], 320*200, 0xA0000);
}

/* */
/* I_ReadScreen */
/* */
void I_ReadScreen (byte* scr)
{
	memcpy (scr, screens[0], SCREENWIDTH*SCREENHEIGHT);
}


void I_SetPalette (byte* palette)
{
#ifdef GRAPHICS
	int c,i;
  outportb(0x3c8,0);
  for (i=0;i<256;i++)
    {
    c=gammatable[usegamma][*palette++];
    outportb(0x3c9,c>>2);
    c=gammatable[usegamma][*palette++];
    outportb(0x3c9,c>>2);
    c=gammatable[usegamma][*palette++];
    outportb(0x3c9,c>>2);
    }
#endif
}


void I_InitGraphics(void)
{
      __dpmi_regs r;

	static int firsttime=1;

	if (!firsttime)
		return;
	firsttime=0;

	/*enter graphics mode */
#ifdef GRAPHICS
      r.x.ax = 0x13;
      __dpmi_int(0x10, &r);
//dascreen= (byte *)(__djgpp_conventional_base+0xa0000);
dascreen=(byte *)malloc(SCREENWIDTH*SCREENHEIGHT);
#else
dascreen=(byte *)malloc(SCREENWIDTH*SCREENHEIGHT);
#endif

	screens[0]=(byte *)malloc(SCREENWIDTH*SCREENHEIGHT);

	/*init the mouse */

	/*init keyboard */
	//initkeyhandler();
}



int inited;


