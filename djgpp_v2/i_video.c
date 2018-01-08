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

/*dosstuff -newly added */
byte* dascreen;
volatile char keydown[128];
volatile char extendedkeydown[128];
volatile char nextkeyextended;
void initkeyhandler();
void killkeyhandler();
void keyhandler();
char oldkeystate[128];
char oldextendedkeystate[128];

#define KBDQUESIZE 32
byte keyboardque[KBDQUESIZE];
int kbdtail, kbdhead;
int lastpress;
_go32_dpmi_seginfo old_keyboard,new_keyboard;


void initkeyhandler()
{
	int i;

	for (i=0; i<128; i++) keydown[i]=0;
	for (i=0; i<128; i++) extendedkeydown[i]=0;
	for (i=0; i<128; i++) oldkeystate[i]=0;
	for (i=0; i<128; i++) oldextendedkeystate[i]=0;
	nextkeyextended=0;

  _go32_dpmi_get_protected_mode_interrupt_vector(9, &old_keyboard);
  new_keyboard.pm_offset = (int)keyhandler;
  new_keyboard.pm_selector = _go32_my_cs();
  _go32_dpmi_chain_protected_mode_interrupt_vector(9, &new_keyboard);

}

/* keyboard interface bits */
#define bit(n) (1<<(n)) /* Set to 1 bit n */
#define KBRD_KDATA bit(0) /* keyboard data is (not) in buffer (bit 0) */
#define KBRD_UDATA bit(1) /* user data is (not) in buffer (bit 1) */
#define KBRD_IO 0x60 /* keyboard IO port */
#define KBRD_RESET 0xFE /* reset CPU command */
#define KBRD_INTRFC 0x64
void killkeyhandler()
{
unsigned int temp;
        _go32_dpmi_set_protected_mode_interrupt_vector(9, &old_keyboard);

/* flush the keyboard controller */
   do /* empty user data in buffer */
   {
      temp = inportb(KBRD_INTRFC);
      if ((temp & KBRD_KDATA) != 0)
         inportb(KBRD_IO); /* empty keyboard data in buffer */
   } while ((temp & KBRD_UDATA) != 0);
	fflush(stdin);
}

void keyhandler(void)
{
keyboardque[kbdhead&(KBDQUESIZE-1)] = lastpress = inportb(0x60);
kbdhead++;

/* acknowledge the interrupt	*/

	outportb(0x20,0x20);
}



#define SC_UPARROW              0x48
#define SC_DOWNARROW    0x50
#define SC_LEFTARROW            0x4b
#define SC_RIGHTARROW   0x4d
#define SC_RSHIFT       0x36
#define SC_LSHIFT       0x2a

#define KEY_LSHIFT      0xfe
#define KEY_INS         (0x80+0x52)
#define KEY_DEL         (0x80+0x53)
#define KEY_PGUP        (0x80+0x49)
#define KEY_PGDN        (0x80+0x51)
#define KEY_HOME        (0x80+0x47)
#define KEY_END         (0x80+0x4f)

byte        scantokey[128] =
					{
/*  0           1       2       3       4       5       6       7
    8           9       A       B       C       D       E       F		*/
	0  ,    27,     '1',    '2',    '3',    '4',    '5',    '6',
	'7',    '8',    '9',    '0',    '-',    '=',    KEY_BACKSPACE, 9, /* 0	*/
	'q',    'w',    'e',    'r',    't',    'y',    'u',    'i',
	'o',    'p',    '[',    ']',    13 ,    KEY_RCTRL,'a',  's',      /* 1	*/
	'd',    'f',    'g',    'h',    'j',    'k',    'l',    ';',
	39 ,    '`',    KEY_LSHIFT,92,  'z',    'x',    'c',    'v',      /* 2	*/
	'b',    'n',    'm',    ',',    '.',    '/',    KEY_RSHIFT,'*',
	KEY_RALT,' ',   0  ,    KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5,   /* 3	*/
	KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10,0  ,    0  , KEY_HOME,
	KEY_UPARROW,KEY_PGUP,'-',KEY_LEFTARROW,'5',KEY_RIGHTARROW,'+',KEY_END, /*4	*/
	KEY_DOWNARROW,KEY_PGDN,KEY_INS,KEY_DEL,0,0,             0,     KEY_F11,
	KEY_F12,0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0,        /* 5	*/
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0,
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0,        /* 6	*/
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0,
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0         /* 7	*/
					};


/*end of newly added stuff */


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

	static int firsttime=1;

	if (!firsttime)
		return;
	firsttime=0;

	/*enter graphics mode */
#ifdef GRAPHICS
      __dpmi_regs r;

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
	initkeyhandler();
}



int inited;


