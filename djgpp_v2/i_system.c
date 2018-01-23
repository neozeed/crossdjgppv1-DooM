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
/* */
/*----------------------------------------------------------------------------- */
/*
static const char
        rcsid[] = "$Id: m_bbox.c,v 1.1 1997/02/03 22:45:10 b1 Exp $";
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <stdarg.h>
#include <sys/time.h>
#include <unistd.h>

#include <pc.h>

#include <go32.h>
#include <dos.h>
#include <dpmi.h>
#include <sys/nearptr.h>


#include <allegro.h>

#include "doomdef.h"
#include "m_misc.h"
#include "i_video.h"
#include "i_sound.h"

#include "d_net.h"
#include "g_game.h"

#ifdef __GNUG__
#pragma implementation "i_system.h"
#endif
#include "i_system.h"

//unsigned int ticcount;	/*our counter*/

//How much memory to allocate to Zone
int mb_used = 32;

/* need a 1MB stack */
unsigned _stklen = 1048576;

void
I_Tactile
        ( int on,
        int off,
        int total )
{
	/* UNUSED. */
	on = off = total = 0;
}

ticcmd_t emptycmd;
ticcmd_t*       I_BaseTiccmd(void)
{
	return &emptycmd;
}


int  I_GetHeapSize (void)
{
	return mb_used*1024*1024;
}

byte* I_ZoneBase (int*  size)
{
	*size = mb_used*1024*1024;
	return (byte *) malloc (*size);
}


//allegro?///////////////////////////////////////////////////
//dm_defs.h
//
// DOOM keyboard definition.
// This is the stuff configured by Setup.Exe.
// Most key data are simple ascii (uppercased).
//
#define KEYD_TILDE      ('`')
#define KEYD_RIGHTARROW 0xae
#define KEYD_LEFTARROW  0xac
#define KEYD_UPARROW    0xad
#define KEYD_DOWNARROW  0xaf
#define KEYD_ESCAPE     27
#define KEYD_ENTER      13
#define KEYD_TAB        9
#define KEYD_F1         (0x80+0x3b)
#define KEYD_F2         (0x80+0x3c)
#define KEYD_F3         (0x80+0x3d)
#define KEYD_F4         (0x80+0x3e)
#define KEYD_F5         (0x80+0x3f)
#define KEYD_F6         (0x80+0x40)
#define KEYD_F7         (0x80+0x41)
#define KEYD_F8         (0x80+0x42)
#define KEYD_F9         (0x80+0x43)
#define KEYD_F10        (0x80+0x44)
#define KEYD_F11        (0x80+0x57)
#define KEYD_F12        (0x80+0x58)
#define KEYD_BACKSPACE  127
#define KEYD_PAUSE      0xff
#define KEYD_EQUALS     0x3d
#define KEYD_MINUS      0x2d
#define KEYD_RSHIFT     (0x80+0x36)
#define KEYD_RCTRL      (0x80+0x1d)
#define KEYD_RALT       (0x80+0x38)
#define KEYD_LALT       KEYD_RALT
#define KEYD_PRTSCR     (0x80+84)
#define KEYD_PGUP       (0x80+73)
#define KEYD_PGDN       (0x80+81)
#define KEYD_HOME       (0x80+71)
#define KEYD_END        (0x80+79)
#define KEYD_INSERT     (0x80+82)
#define KEYD_DELETE     (0x80+83)


/////////////////////////////////////////////////////////////


char oldkeystate[128];

//
// -MH- 1998/07/02  USABLE_EXTKEY, I_DoomCode2ScanCode, I_ScanCode2DoomCode
// -MH- 1998/08/18  Placed up here so defined _before_ use
//
// Doom seems to have some of the special keys usable as control keys and
// others not. Whether or not a special key is is determined by the two
// functions mentioned above and a LONG if statement further down in this
// module. For example, if the functions have them but they are not in the
// if-statement, the DEFAULT.CFG logic can handle them but Doom can't; that
// is to say, you can't assign them in the CONTROLS menu.
//
// I have changed the contents of the if-statement into a macro to make it
// more manageable and I have move the functions from i_video.c to here so
// that they are all defined together. Who knows what the hell they were
// doing in i_video.c since they are nothing to do with video logic.
//
// I have cleaned up the functions and the macro and made these changes
// because the menu won't recognise some of the keys especially insert and
// delete. I guess we should really do same for function keys as well, but
// one possible bug-introducer at a time, eh :-)
//

// Extended keys that can be used in menus
// -ES- 1999/03/28 Added Enter on numeric keypad
#define USABLE_EXTKEY(i)      \
        (  (i==0x48)          \
         ||(i==0x50)          \
         ||(i==0x4b)          \
         ||(i==0x4d)          \
         ||(i==0x0e)          \
         ||(i==0x2a)          \
         ||(i==KEY_ENTER)     \
         ||(i==KEY_RCONTROL)  \
         ||(i==KEY_ALTGR)     \
         ||(i==KEY_PGUP)      \
         ||(i==KEY_PGDN)      \
         ||(i==KEY_HOME)      \
         ||(i==KEY_END)       \
         ||(i==KEY_INSERT)    \
         ||(i==KEY_DEL)       \
         ||(i==KEY_PRTSCR))

int I_ScanCode2DoomCode (int a)
{
  switch (a)
  {
    case 0x48:         return KEYD_UPARROW;
    case 0x50:         return KEYD_DOWNARROW;
    case 0x4b:         return KEYD_LEFTARROW;
    case 0x4d:         return KEYD_RIGHTARROW;
    case 0x0e:         return KEYD_BACKSPACE;
    case 0x2a:         return KEYD_RSHIFT;
    case KEY_RCONTROL: return KEYD_RCTRL;
    case KEY_ALTGR:    return KEYD_RALT;
    case KEY_PGUP:     return KEYD_PGUP;
    case KEY_PGDN:     return KEYD_PGDN;
    case KEY_HOME:     return KEYD_HOME;
    case KEY_END:      return KEYD_END;
    case KEY_INSERT:   return KEYD_INSERT;
    case KEY_DEL:      return KEYD_DELETE;
    case KEY_PRTSCR:   return KEYD_PRTSCR;  // -MH- 1998/08/18 Added "Print Screen"
  }

  if ((key_shifts & KB_NUMLOCK_FLAG) && (a == KEY_5_PAD))
      return '5';

  if (a>=0x100)
    return a;
  else if (key_ascii_table[a]>8)
    return key_ascii_table[a];
  else
    return a+0x80;
}

int I_DoomCode2ScanCode (int a)
{
  int i;

  switch (a)
  {
    case KEYD_UPARROW:    return 0x48;
    case KEYD_DOWNARROW:  return 0x50;
    case KEYD_LEFTARROW:  return 0x4b;
    case KEYD_RIGHTARROW: return 0x4d;
    case KEYD_BACKSPACE:  return 0x0e;
    case KEYD_RSHIFT:     return 0x2a;
    case KEYD_RALT:       return KEY_ALTGR;
    case KEYD_PGUP:       return KEY_PGUP;
    case KEYD_PGDN:       return KEY_PGDN;
    case KEYD_HOME:       return KEY_HOME;
    case KEYD_END:        return KEY_END;
    case KEYD_INSERT:     return KEY_INSERT;
    case KEYD_DELETE:     return KEY_DEL;
    case KEYD_PRTSCR:     return KEY_PRTSCR;  // -MH- 1998/08/18 Added "Print Screen"
  }

  if (a>=0x100)
    return a;
  else if (a>=0x80)
    return (a-0x80);
  else
  {
    for (i=0;i<128;i++)
      if (key_ascii_table[i]==a)
        return i;
    return 0;
  }
}



// -KM- 1998/09/01 Handles keyboard
static void Keyboard_Event()
{
  event_t event;
  char keystate[128];
  int i;

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
      if USABLE_EXTKEY(i)              /* -MH- 1998/08/18 Fixed keys for up/down etc */
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
      if USABLE_EXTKEY(i)
      {
        event.type=ev_keyup;
        event.data1=I_ScanCode2DoomCode(i);
        D_PostEvent(&event);
      }
    }
  }

  memcpy(oldkeystate,keystate,128);
}



volatile int mselapsed=0;
void I_Timer(void)
{	
	mselapsed++;
}
END_OF_FUNCTION(I_Timer);

//
// I_GetTime
// returns time in 1/TICRATE second tics
//
int I_GetTime (void)
{
  if (mselapsed>0)
  {
    return (mselapsed);
  }
  else
  {
    struct timeval        tp;
    struct timezone       tzp;
    int                   newtics;
    static int            basetime=0;

    gettimeofday(&tp, &tzp);

    if (!basetime)
      basetime = tp.tv_sec;

    newtics = (tp.tv_sec-basetime)*TICRATE + tp.tv_usec*TICRATE/1000000;
    return newtics;
  }
}

 

// #define VBLCOUNTER              34000           /* hardware tics to a frame */



/* */
/* I_Init */
/* */
void I_Init (void)
{
    i_love_bill = TRUE; // My machine doesn't work without it.  As you might have guessed,
    // my machine is pretty shit. :-(

    //something something FPU exceptions.
    //MaskExceptions();

    //load and init Allegro?
  fprintf( stdout, "I_InitSound: Allegro init\n");
    allegro_init();

  I_InitSound();
  /*  I_InitGraphics(); */

  //init timer
  LOCK_VARIABLE(mselapsed);
  LOCK_FUNCTION(I_Timer);
  install_timer();
  //install_int(I_Timer,5);
  install_int_ex(I_Timer,BPS_TO_TIMER(TICRATE));

  // Init music

  I_InitMusic();

  // Load entire midi patch set.
  load_midi_patches();

  memset(oldkeystate,0,128);
  install_keyboard();


//S_StartMusic (mus_intro);

}

/* */
/* I_Quit */
/* */
void I_Quit (void)
{
	int i;
	unsigned char *scr;

	D_QuitNetGame ();
	I_ShutdownSound();
	I_ShutdownMusic();
	M_SaveDefaults ();
	I_ShutdownGraphics();
	remove_keyboard();
	remove_timer();
	allegro_exit();

	scr = (byte *)W_CacheLumpName("ENDOOM", 101); /*PU_CACHE 101*/
//	for (i=0;i<=22;i++) printf("\n");
	   memcpy((void *)(__djgpp_conventional_base)+0xb8000,scr,4000);

	exit(0);
}

void I_WaitVBL(int count)
{
while ((inportb(0x3da)&8)!=8);
while ((inportb(0x3da)&8)==8);
}

void I_BeginRead(void)
{
}

void I_EndRead(void)
{
}

byte*   I_AllocLow(int length)
{
	byte*       mem;

	mem = (byte *)malloc (length);
	memset (mem,0,length);
	return mem;

}


/* */
/* I_Error */
/* */
extern boolean demorecording;

void I_Error (char *error, ...)
{
	va_list argptr;

	/* Message first. */
	va_start (argptr,error);
	fprintf (stderr, "Error: ");
	vfprintf (stderr,error,argptr);
	fprintf (stderr, "\n");
	va_end (argptr);

	fflush( stderr );

	/* Shutdown. Here might be other errors. */
	if (demorecording)
		G_CheckDemoStatus();

	D_QuitNetGame ();
	I_ShutdownGraphics();

	va_start (argptr,error);
	fprintf (stdout, "Error: ");
	vfprintf (stdout,error,argptr);
	fprintf (stdout, "\n");
	va_end (argptr);

	fflush( stdout );


	exit(-1);
}
