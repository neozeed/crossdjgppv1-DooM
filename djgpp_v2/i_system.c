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


int mb_used = 6;


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


//allegro?
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

 

#define VBLCOUNTER              34000           /* hardware tics to a frame */

void IO_SetTimer0(int speed)
{
	if (speed > 0 && speed < 150)
		I_Error ("INT_SetTimer0: %i is a bad value",speed);

	outportb(0x43,0x36);                            /* Change timer 0 */
	outportb(0x40,speed);
	outportb(0x40,speed >> 8);
}

/* */
/* I_Init */
/* */
void I_Init (void)
{
  I_InitSound();
  /*  I_InitGraphics(); */

  //init timer
  LOCK_VARIABLE(mselapsed);
  LOCK_FUNCTION(I_Timer);
  install_timer();
  //install_int(I_Timer,5);
  install_int_ex(I_Timer,BPS_TO_TIMER(TICRATE));

  // Init music

    i_love_bill = FALSE; // My machine doesn't work without it.  As you might have guessed,
    // my machine is pretty shit. :-(

  I_InitMusic();

  // Load entire midi patch set.
  load_midi_patches();

S_StartMusic (mus_intro);

}

/* */
/* I_Quit */
/* */
void I_Quit (void)
{
	unsigned char * scr;


	D_QuitNetGame ();
	I_ShutdownSound();
	I_ShutdownMusic();
	M_SaveDefaults ();
	I_ShutdownGraphics();

	scr = (byte *)W_CacheLumpName("ENDOOM", 101); /*PU_CACHE 101*/
//	memcpy((void *)0xd0000000, scr, 80*25*2);	


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
