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

static const char
        rcsid[] = "$Id: m_bbox.c,v 1.1 1997/02/03 22:45:10 b1 Exp $";


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <stdarg.h>
#include <sys/time.h>
/*#include <sys/timeb.h>*/
#include <unistd.h>

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

#include <sys/resource.h>
#include <sys/time.h>
#include <sys/times.h>
/* */
/* I_GetTime */
/* returns time in 1/70th second tics */
/* */
static int klock;
int  I_GetTime (void)
{
#if 0
	struct timeval tp;
#ifdef _WIN32
	struct timezone tzp;
#else
	short tzp;
#endif
	int newtics;
	static int basetime=0;

	/*gettimeofday(&tp, &tzp);*/
	if (!basetime)
		basetime = tp.tv_sec;
	newtics = (tp.tv_sec-basetime)*TICRATE + tp.tv_usec*TICRATE/1000000;


	return newtics;

#else
struct rusage rusage;
long ret;
getrusage(1, &rusage);
ret=rusage.ru_stime.tv_usec;
ret+=rusage.ru_utime.tv_usec;
ret=ret/1000;
if(ret<0)
	ret=ret*-1;
return((int)ret);
#endif
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
	IO_SetTimer0 (VBLCOUNTER);
}

/* */
/* I_Quit */
/* */
void I_Quit (void)
{
	IO_SetTimer0 (0);              /* back to 18.4 ips */
	D_QuitNetGame ();
	I_ShutdownSound();
	I_ShutdownMusic();
	M_SaveDefaults ();
	I_ShutdownGraphics();
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

	exit(-1);
}
