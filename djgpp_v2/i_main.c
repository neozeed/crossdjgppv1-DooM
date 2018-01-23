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
/*	Main program, simply calls D_DoomMain high level loop. */
/* */
/*----------------------------------------------------------------------------- */
/*
static const char
        rcsid[] = "$Id: i_main.c,v 1.4 1997/02/03 22:45:10 b1 Exp $";
*/

#include <stdio.h>
#include <sys/nearptr.h>
#include <allegro.h>
#include "doomdef.h"

#include "m_argv.h"
#include "d_main.h"

int
main
        ( int argc,
        char**        argv )
{
       int b_ssp;
	myargc = argc;
	myargv = argv;

    if (__djgpp_nearptr_enable())  //handle nearptr now
      D_DoomMain ();
    else
      printf ("Failed trying to allocate DOS near pointers.\n");

	printf("exiting i_main.c: main()\n");
	allegro_exit();

	return 0;
}
