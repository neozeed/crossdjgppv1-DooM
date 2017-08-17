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
#include <unistd.h>

#include <pc.h>
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


static union REGS r;
static struct SREGS s;
/*
end go32 / dpmi
*/



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

unsigned int ticcount;	/*our counter*/


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


_go32_dpmi_seginfo old_clock,new_clock;

void IO_TimerISR (void)
{
	ticcount++;
	outportb(0x20,0x20);                            /* Ack the interrupt */
}


/* */
/* I_GetTime */
/* returns time in 1/70th second tics */
/* */
int  I_GetTime (void)
{
return(ticcount);
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

/* setup the new clock*/
  _go32_dpmi_get_protected_mode_interrupt_vector(8, &old_clock);

  new_clock.pm_offset = (int)IO_TimerISR;
  new_clock.pm_selector = _go32_my_cs();
  _go32_dpmi_chain_protected_mode_interrupt_vector(8, &new_clock);
	IO_SetTimer0 (VBLCOUNTER);
}

/* */
/* I_Quit */
/* */
void I_Quit (void)
{
unsigned char * scr;
	_go32_dpmi_set_protected_mode_interrupt_vector(8, &old_clock);
	IO_SetTimer0 (0);              /* back to 18.4 ips */
	D_QuitNetGame ();
	I_ShutdownSound();
	I_ShutdownMusic();
	M_SaveDefaults ();
	I_ShutdownGraphics();

	scr = (byte *)W_CacheLumpName("ENDOOM", 101); /*PU_CACHE 101*/
	memcpy((void *)0xd0000000, scr, 80*25*2);	


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
