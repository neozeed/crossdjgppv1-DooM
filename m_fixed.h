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
/* DESCRIPTION: */
/*	Fixed point arithemtics, implementation. */
/* */
/*----------------------------------------------------------------------------- */


#ifndef __M_FIXED__
#define __M_FIXED__


#ifdef __GNUG__
#pragma interface
#endif


/* */
/* Fixed point, 32bit as 16.16. */
/* */
#define FRACBITS                16
#define FRACUNIT                (1<<FRACBITS)

typedef int fixed_t;

#ifndef USEASM
fixed_t FixedMul        (fixed_t a, fixed_t b);
fixed_t FixedDiv        (fixed_t a, fixed_t b);
fixed_t FixedDiv2       (fixed_t a, fixed_t b);
#endif

#ifdef __WATCOMC__	/* FS: Heretic Merge */
#pragma aux FixedMul =	\
	"imul ebx",			\
	"shrd eax,edx,16"	\
	parm	[eax] [ebx] \
	value	[eax]		\
	modify exact [eax edx]

#pragma aux FixedDiv2 =	\
	"cdq",				\
	"shld edx,eax,16",	\
	"sal eax,16",		\
	"idiv ebx"			\
	parm	[eax] [ebx] \
	value	[eax]		\
	modify exact [eax edx]
#endif



#ifdef __DJGPP
/* multiple definition of `FixedMul'
I think later versions treat __inline__ vs inline 
not to mention static or const
inline __inline__ whatever. they all duplicate
*/
__inline__ static const int D_abs(x)
{
  int _t = (x),_s;
  _s = _t >> (8*sizeof _t-1);
  return (_t^_s)-_s;
}

__inline__ static const int FixedMul(int a, int b)
{
  int result;

  asm (
      "  imull %2 ;"
      "  shrdl $16,%%edx,%0 ;"
      : "=a" (result)           /* eax is always the result */
      : "0" (a),                /* eax is also first operand */
        "rm" (b)                /* second operand can be reg or mem */
      : "%edx", "%cc"           /* edx and condition codes clobbered */
      );

  return result;
}


/* killough 5/10/98: In djgpp, use inlined assembly for performance
 * killough 9/5/98: optimized to reduce the number of branches
 * CPhipps - made __inline__ to inline, as specified in the gcc docs
 * Also made const, also __asm__ to asm as in docs.
 * Replaced inline asm with Julian's version for Eternity dated 6/7/2001
 */
#define INT_MAX          ((int)0x7fffffff)
__inline__ static const int FixedDiv(int a, int b)
{
  //e6y: zg is a master of engineer science
  if ((unsigned)D_abs(a) >> 14 < (unsigned)D_abs(b))
    {
      int result;
      asm (
          " idivl %3 ;"
	  : "=a" (result)
	  : "0" (a<<16),
	    "d" (a>>16),
	    "rm" (b)
	  : "%cc"
	  );
      return result;
    }
  return ((a^b)>>31) ^ INT_MAX;
}

#endif

#endif
/*----------------------------------------------------------------------------- */
/* */
/* $Log:$ */
/* */
/*----------------------------------------------------------------------------- */
