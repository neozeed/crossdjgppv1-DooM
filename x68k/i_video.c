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

static const char
        rcsid[] = "$Id: i_x.c,v 1.6 1997/02/03 22:45:10 b1 Exp $";

#include <stdlib.h>
/*
#include <unistd.h>
#include <stdarg.h>
#include <sys/time.h>
#include <sys/types.h>
*/
/*
 #include <netinet/in.h>
 #include <signal.h>
 */


#include "doomstat.h"
#include "i_system.h"
#include "v_video.h"
#include "m_argv.h"
#include "d_main.h"

#include "doomdef.h"


#include <iocslib.h>
#include <doslib.h>
#include "vc.h"
#include "crtc.h"
#include "tvram.h"
#include "mfp.h"

#include "serial.h"
char errmsg[1024];
#include <string.h>

/* create an x68000 color from 3 bytes
// TODO: fix this to use F8 as a mask instead of 1F*/
#define X68_GRB(r, g, b, i) ( ( ((b&0xF8)>>2) | (((g)&0xF8)<<8) | (((r)&0xF8)<<3) ) | i )
/*#define X68_GRB(r, g, b) ( (  ((b&0x1F)<<1) | (((g)&0x1F)<<11) | (((r)&0x1F)<<6) ) )*/

/* the original game code just shifts an 8-bit value 2 bits to the right = 6-bit value */
/*gammatable[usegamma][*palette++]&0x1F<<6 | gammatable[usegamma][*palette++]&0x1F<<11 | \
			gammatable[usegamma][*palette++]&0x1F<<1;*/

#define GVRAM_BASE (0xC00000)
#define GVRAM0 (GVRAM_BASE)
#define GVRAM1 (GVRAM_BASE+0x80000)
#define GVRAM2 (GVRAM_BASE+0x100000)
#define GVRAM3 (GVRAM_BASE+0x180000)

short *gvram0 = (short*)GVRAM0;
byte oldTVPal[32];


/*dosstuff -newly added */
/*byte* dascreen;*/

char keydown[128];
volatile char extendedkeydown[128];
volatile char nextkeyextended;
void initkeyhandler();
void killkeyhandler();
void keyhandler();
static char oldkeystate[128];
static char oldextendedkeystate[128];

void initkeyhandler()
{
	int i;
	memset(keydown, 0, 128);
	memset(oldkeystate, 0, 128);
	/*for (i=0; i<128; i++) keydown[i]=0;*/
	for (i=0; i<128; i++) extendedkeydown[i]=0;
	for (i=0; i<128; i++) oldkeystate[i]=0;
	for (i=0; i<128; i++) oldextendedkeystate[i]=0;
	nextkeyextended=0;

}

void killkeyhandler()
{
}

void keyhandler()
{
	unsigned char keyhandlercurrkey;



	if (nextkeyextended)
	{
		if ((keyhandlercurrkey&0x80)==0)
			extendedkeydown[keyhandlercurrkey&0x7f]=1;
		else
			extendedkeydown[keyhandlercurrkey&0x7f]=0;
		nextkeyextended=0;
	}
	else
	{
		if ((keyhandlercurrkey&0x80)==0)
			keydown[keyhandlercurrkey&0x7f]=1;
		else
			keydown[keyhandlercurrkey&0x7f]=0;
	}

	if (keyhandlercurrkey==0xe0)
		nextkeyextended=1;



}

/* create a 2d translation table to avoid redefining this one
   X68XlatTable[0xf][0x8] = ASCII_Scan_Code */
/* keyboard tab = - */
unsigned char X68XlatTable[16][8] = { \
{ 0, 1, 2, 3, 4, 5, 6, 7},\
{ 8, 9,10,11,12, 0, 0,14},\
{15,16,17,18,19,20,21,22},\
{23,24,25, 0,26,28,30,31},\
{32,33,34,35,36,37,38,13},\
{ 14,27,44,45,46,47,48,49},\
{50,51,52,53, 0, 57, 0, 0},\
{ 0, 0, 0,88,89,90,91, 0},\
{ 0, 74, 0,71, 72, 73, 78, 81},\
{ 76, 77, 0, 79, 80, 81, 0, 82},\
{ 0, 0, 0, 0, 0,104, 103, 0},\
{ 0, 0, 0, 0, 0, 0, 0, 0},\
{ 0, 0, 0, 92,93,94,95,96},\
{ 97,98,99,100,101, 0, 0, 0},\
{ 102, 0, 0, 0, 0, 0, 0, 0},\
{ 0, 0, 0, 0, 0, 0, 0, 0}};

unsigned char ASCIINames[] =             /* Unshifted ASCII for scan codes */
{
/*	 0   1   2   3   4   5   6   7 */
	 0, 27, '1','2','3','4','5','6', /* 8  */
	'7','8','9','0','-','=', 8,  9 , /* 16 */
	'q','w','e','r','t','y','u','i', /* 24 */
	'o','p','[',']',13,  0,'a', 's', /* 32 */ 
	'd','f','g','h','j','k','l',';', /* 40 */
	 39,'`', 0, 92, 'z','x','c','v', /* 48 */
	'b','n','m',',','.','/', 0, '*', /* 56 */
	 0, ' ', 0,  0,  0,  0,  0,  0 , /* 64 */
	 0,  0,  0,  0,  0,  0,  0, '7', /* 72 */
	'8','9','-','4','5','6','+','1', /* 80 */
	'2','3','0',127, 0,  0,  0,  0,  /* 88 */
	 172,173,174,175, 187,188,189,190,  /* 96 */
	 191,192,193,194,195,196,182,184,  /* 104*/
	 157,  0,  0,  0,  0,  0,  0,  0,  /* 112*/
	 0,  0,  0,  0,  0,  0,  0,  0,  /* 120*/
	 0,  0,  0,  0,  0,  0,  0,  0   /* 128*/
};

/*end of newly added stuff */


void I_ShutdownGraphics(void)
{
	*crtc_r21 = 0;
/* 768x512, 16 color*/
	*crtc_r20 = 0xB16;
	*crtc_r00 = 0x89;
	*crtc_r01 = 0x0e;
	*crtc_r02 = 0x1c;
	*crtc_r03 = 0x7c;
	*crtc_r04 = 0x237;
	*crtc_r05 = 0x05;
	*crtc_r06 = 0x28;
	*crtc_r07 = 0x228;
	*crtc_r08 = 0x1B;

	/* restore text mode priorities*/
	*vidcon_r1 = 0x06E4;
	/* restore text layer visibility*/
	*vidcon_r2 = 0x20;
	
	

	/*FB_clear();*/
	memset(tvram0, 0, 512*1024);
	_iocs_tgusemd(1, 1);			/* enable soft mouse, keyboard, etc*/
	_iocs_tgusemd(0, 1);			/* ""*/
	_dos_c_curon();					/* turn on DOS cursor*/
	memcpy(tpal, &oldTVPal, 32);	/* restore old text pal*/
	SER_debug("Video shutdown finished.\n", SER_DEBUG_INFO);
}



/* */
/* I_StartFrame */
/* */
void I_StartFrame (void)
{
	/* er? */
	grWaitVSync();
}


void I_GetEvent()
{

#if 0
	/*mouse movement */

	r.x.ax=0x0b;
	__dpmi_int(0x33,&r);
	xmickeys=(signed short)r.x.cx;
	ymickeys=(signed short)r.x.dx;
	r.x.ax=0x03;
	__dpmi_int(0x33,&r);
	buttons=r.x.bx;

	event.type=ev_mouse;
	event.data1=buttons; /*??? */
	event.data2=xmickeys;
	event.data3=-ymickeys;
	if ((xmickeys!=0)||(ymickeys!=0)||(buttons!=lastbuttons))
		D_PostEvent(&event);
	lastbuttons=buttons;
#endif
	char key;
	unsigned char tempkey[128];	
	unsigned char bitsnsmask=1;
	unsigned char i;
	unsigned char j;
	event_t event;
	
	memset(tempkey, 0, 128);

	for(i=0;i<16;i++)
	{
		key = _iocs_bitsns(i);
		for(j=0;j<8;j++)
		{
			if(key&bitsnsmask)
				tempkey[X68XlatTable[i][j]] = 1;
			else
				tempkey[X68XlatTable[i][j]] = 0;
			bitsnsmask<<=1;	
		}
		bitsnsmask = 1;
	}
	
	bitsnsmask = 1;
	
	for (i=0; i<128; i++)
	{
		if ((tempkey[i]==1)&&(oldkeystate[i]==0))
		{
			event.type=ev_keydown;
			if (ASCIINames[i]!=0)
				event.data1=ASCIINames[i];
			
			sprintf(&errmsg, "ASCII %i\n", event.data1);
			SER_debug(errmsg, SER_DEBUG_INFO);
			sprintf(&errmsg, "X68K  %i\n", i);
			SER_debug(errmsg, SER_DEBUG_INFO);
			event.type=ev_keydown;

			D_PostEvent(&event);
			
		}
		if ((tempkey[i]==0)&&(oldkeystate[i]==1))
		{
			event.type=ev_keyup;
			if (ASCIINames[i]!=0)
				event.data1=ASCIINames[i];	
			
			sprintf(&errmsg, "ASCII %i\n", event.data1);
			SER_debug(errmsg, SER_DEBUG_WARN);
			sprintf(&errmsg, "X68K  %i\n", i);
			SER_debug(errmsg, SER_DEBUG_WARN);
			event.type=ev_keyup;
			
			D_PostEvent(&event);
		}
		
	}
	memcpy(oldkeystate,tempkey,128);
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
	int y;
	int x;
	int x2;
	int y2;
	
	
	short writemask=0;
	short planemask = 0x300;
	unsigned long pixelnum=0;
	unsigned long wordnum=0;
	char  bitnum=0;
	char lastcolor=0;
	char pixel=0;
	unsigned short r21=0;
	unsigned long r23=0;
	
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

	
	/* optimize out the multiples in this */
	for(y=0,y2=0;y<SCREENHEIGHT;y++,y2+=2){
		for(x=0;x<SCREENWIDTH;x++){			
			pixel = screens[0][x+y*SCREENWIDTH];
			
			/* if the pixel is the first in its row in palette ram */
			if(!(pixel%0x10)){
				/* fancy math */
				pixelnum = x+y2*1024;
				wordnum = pixelnum/16;
				bitnum = pixelnum%16;
				
				/* set the text access mask */
				*crtc_r23 = (short)(~(1<<15>>bitnum));
				/* set current this pixel to 0 */
				*crtc_r21 = 0x3F0;
				tvram0[wordnum] = 0;
				/* now set the pixel to the color */
				*crtc_r21 = 0x300|(pixel&0xF0);
				tvram0[wordnum] = 0xFFFF;
			}
			gvram0[x+y2*512] = pixel;
		}
	}
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
	int i;
	

		for(i=0; i<256*3; i+=3){
			/* put the whole first column in text palette */
			if(!(i%0x10)){
				tpal[((i/3)>>4)&0xF] = X68_GRB(gammatable[usegamma][palette[i]],gammatable[usegamma][palette[i+1]], \
				gammatable[usegamma][palette[i+2]], 0);
				gpal[i/3] = X68_GRB(gammatable[usegamma][palette[i]],gammatable[usegamma][palette[i+1]], \
				gammatable[usegamma][palette[i+2]], 0);
				/**/
			} else {
				gpal[i/3] = X68_GRB(gammatable[usegamma][palette[i]],gammatable[usegamma][palette[i+1]], \
				gammatable[usegamma][palette[i+2]], 0);
			}
		}
		

}


void I_InitGraphics(void)
{
	static int firsttime=1;


	if(!firsttime)
		return;
	firsttime=0;


	/* 512x512 256 color, gv/tvram enabled*/
	*crtc_r20 = 0x115; 
	
	/* various display timings*/
	*crtc_r00 = 0x5B;
	*crtc_r01 = 0x09;
	*crtc_r02 = 0x11;
	*crtc_r03 = 0x51;
	*crtc_r04 = 0x237;
	*crtc_r05 = 0x05;
	*crtc_r06 = 0x28;
	*crtc_r07 = 0x228;
	*crtc_r08 = 0x1B;
	/**crtc_r00 = 0x2d;
	*crtc_r01 = 0x04;
	*crtc_r02 = 0x06;
	*crtc_r03 = 0x26;
	*crtc_r04 = 0xF0;
	*crtc_r05 = 0x05;
	*crtc_r06 = 0x28;
	*crtc_r07 = 0x228;
	*crtc_r08 = 0x1B;*/
	
	/* gvram settings
	// 0 = 512x512, 16 color 
	// 4 = 1024x1024, 16
	// 1 = 512x512, 256
	// 2 = dont use
	// 3 = 512x512, 65536(swizzled)*/
	*vidcon_r0 = 0x1;	

	/* set scrolls to 0*/
	*crtc_r10 = 0;
	*crtc_r11 = 0;
	*crtc_r12 = 0;
	*crtc_r13 = 0;
	*crtc_r14 = 0;
	*crtc_r15 = 0;
	*crtc_r16 = 0;
	*crtc_r17 = 0;
	*crtc_r18 = 0;
	*crtc_r19 = 0;

	/* text, graphics, bg/sprite, 
	// graphics layers priority, 0, 1, 2, 3*/
	*vidcon_r1 = 0x24E4;
	/* set text and GVRAM0 layers visible, enable transparency stuff*/
	*vidcon_r2 = 33; /*0x1;/*0x21; // no transparency*/
	
	/* 8x8 31khz BGn*/
	/**cynthia_res = 0x10;*/
	
	/* disable soft mouse stuff (right click)*/
	_iocs_tgusemd(1, 2);
	_iocs_tgusemd(0, 2);
	_dos_c_curoff();
	
	memcpy(&oldTVPal, tpal, 32);		/* backup old text palette*/
	memset(tvram0, 0, 512*1024);
	memset(gvram0, 0x02, 0x100000);		/* clear all gvram*/
	/*memset(tvram0, 0, 512*1024);		/* clear all tvram*/
	
	/*memset(gpal, 0x0, 256*2);				/* clear all palettes
	memset(gpal, 0xC9, 2);
	memset(gpal+1, 0xF0, 2);
	memset(gpal+2, 0x1, 2);
	memset(tpal, 0, 256*2);*/
	
	/* 0 is transparent color for tvram. maybe for pcg also.
	// white seems to be transparent color for gvram*/
	


	/*dascreen=(char *)(calloc(64*1024, 2));*/
	screens[0]=(char *)calloc(SCREENWIDTH*SCREENHEIGHT, 1);
	

	/*init keyboard */
	SER_debug("Video inited\n", SER_DEBUG_INFO);
	initkeyhandler();
	
}



int inited;


void grWaitVSync()
{
#ifdef human68k
	while( !( *mfp_gpip & 0x40 ) );
	while( *mfp_gpip & 0x40 );
	
#endif
}


void grEraseGraphicScreen()
{
#ifdef human68k
	short	iOld21;

	/* 現在の値を退避	*/
	/*iOld21 = CRTC_REG(21);*/

	/* CRTCの機能を使用してクリア	*/
	grWaitVSync();
	/*CRTC_REG(21) = 0x0F;
	*CRTC_MODE = 0x02;*/
	grWaitVSync();

	/* 退避した内容を戻す	*/
	/*CRTC_REG(21) = iOld21;*/

/*text mode?
*VCR2 |= 0x0020;
*VCR2 &= 0xFFDF;*/
#endif
}