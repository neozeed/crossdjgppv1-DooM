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
#include <stdarg.h>
#include <sys/types.h>

#include <signal.h>

#include "doomstat.h"
#include "i_system.h"
#include "v_video.h"
#include "m_argv.h"
#include "d_main.h"

#include "doomdef.h"

#include <graph.h>
#define INCL_VIO
#define INCL_DOS
#define INCL_DOSERRORS
#define INCL_DOSMONITORS
#define INCL_KBD
#define INCL_MOU
#include <os2.h>

#include "keys.h"

VIOMODEINFO graph_mode;
VIOPHYSBUF phys;



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



#define KBD_BUFFER_SIZE 256  /* size of buffer for incoming keys */


#define OS2_ALIGN(var) (_THUNK_PTR_STRUCT_OK(&var##1) ? &var##1 : &var##2)

HMONITOR os2_hmon = -1;

struct os2_key_packet {
	USHORT mon_flag;
	UCHAR chr;
	UCHAR scan;
	UCHAR dbcs;
	UCHAR dbcs_shift;
	USHORT shift_state;
	ULONG ms;
	USHORT dd_flag;
};

struct os2_buffer {
	USHORT cb;
	struct os2_key_packet packet;
	char pad[20];
};

struct os2_key_packet *os2_key;
struct os2_buffer *os2_in;
struct os2_buffer *os2_out;

int os2_read_mon(unsigned char *data, int len)
{
	static int prefix = 0;
	int bytes_read = 0;
	int r;
	USHORT count = sizeof(struct os2_key_packet);
	while (bytes_read < len) {
		int scan;
		r = DosMonRead((void *)os2_in, IO_NOWAIT, (void *)os2_key, &count);
		if (r == ERROR_MON_BUFFER_EMPTY) break;
		if (r) {
			fprintf(stderr, "DosMonRead: %d\n", r);
			sleep(1);
			break;
		}
		/*fprintf(stderr, "monflag: %04x, scan %02x, shift %04x\n", os2_key->mon_flag, os2_key->scan, os2_key->shift_state);*/
		scan = os2_key->mon_flag >> 8;
		/*fprintf(stderr, "scan: %02x\n", scan); fflush(stderr);*/
		if (scan == 0xE0) {
			prefix = 1;
			c:continue;
		}
		if (scan == 0xE1) {
			prefix = 2;
			continue;
		}
		if (prefix == 1) {
			prefix = 0;
			switch (scan & 0x7f) {
			case 29:	scan = (scan & 0x80) | 97; break;
			case 71:	scan = (scan & 0x80) | 102; break;
			case 72:	scan = (scan & 0x80) | 103; break;
			case 73:	scan = (scan & 0x80) | 104; break;
			case 75:	scan = (scan & 0x80) | 105; break;
			case 77:	scan = (scan & 0x80) | 106; break;
			case 79:	scan = (scan & 0x80) | 107; break;
			case 80:	scan = (scan & 0x80) | 108; break;
			case 81:	scan = (scan & 0x80) | 109; break;
			case 82:	scan = (scan & 0x80) | 110; break;
			case 83:	scan = (scan & 0x80) | 111; break;
			/* !!! FIXME: pause */
			default:	
		/*fprintf(stderr, "scan: %02x\n", scan); fflush(stderr);*/
					goto c;
			}
		}
		if (prefix == 3) {
			prefix = 0;
			continue;
		}
		if (prefix == 2) {
			prefix = 0;
			switch (scan & 0x7f) {
			case 29:	scan = (scan & 0x80) | 101; prefix = 3; break;
			default:	
		/*fprintf(stderr, "scan: %02x\n", scan); fflush(stderr);*/
					goto c;
			}
		}
		/*Con_Printf("%d\n", scan);*/
		data[bytes_read++] = scan;
	}
	return bytes_read;
}

static unsigned char scantokey[128];

static void set_scantokey(void)
{
	int i;
	for (i=0 ; i<128 ; i++)
		scantokey[i] = ' ';

	scantokey[  1] = K_ESCAPE;
	scantokey[  2] = '1';
	scantokey[  3] = '2';
	scantokey[  4] = '3';
	scantokey[  5] = '4';
	scantokey[  6] = '5';
	scantokey[  7] = '6';
	scantokey[  8] = '7';
	scantokey[  9] = '8';
	scantokey[ 10] = '9';
	scantokey[ 11] = '0';
	scantokey[ 12] = '-';
	scantokey[ 13] = '=';
	scantokey[ 14] = K_BACKSPACE;
	scantokey[ 15] = K_TAB;
	scantokey[ 16] = 'q';       
	scantokey[ 17] = 'w';       
	scantokey[ 18] = 'e';       
	scantokey[ 19] = 'r';       
	scantokey[ 20] = 't';       
	scantokey[ 21] = 'y';       
	scantokey[ 22] = 'u';       
	scantokey[ 23] = 'i';       
	scantokey[ 24] = 'o';       
	scantokey[ 25] = 'p';       
	scantokey[ 26] = '[';
	scantokey[ 27] = ']';
	scantokey[ 28] = K_ENTER;
	scantokey[ 29] = K_CTRL; //left
	scantokey[ 30] = 'a';
	scantokey[ 31] = 's';       
	scantokey[ 32] = 'd';       
	scantokey[ 33] = 'f';       
	scantokey[ 34] = 'g';       
	scantokey[ 35] = 'h';       
	scantokey[ 36] = 'j';       
	scantokey[ 37] = 'k';       
	scantokey[ 38] = 'l';       
	scantokey[ 39] = ';';
	scantokey[ 40] = '\'';
	scantokey[ 41] = '`';
	scantokey[ 42] = K_SHIFT; //left
	scantokey[ 43] = '\\';
	scantokey[ 44] = 'z';       
	scantokey[ 45] = 'x';  
	scantokey[ 46] = 'c';
	scantokey[ 47] = 'v';       
	scantokey[ 48] = 'b';
	scantokey[ 49] = 'n';       
	scantokey[ 50] = 'm';       
	scantokey[ 51] = ',';
	scantokey[ 52] = '.';
	scantokey[ 53] = '/';
	scantokey[ 54] = K_SHIFT; //right
	scantokey[ 55] = '*'; //keypad
	scantokey[ 56] = K_ALT; //left
	scantokey[ 57] = ' ';
	// 58 caps lock
	scantokey[ 59] = K_F1;
	scantokey[ 60] = K_F2;
	scantokey[ 61] = K_F3;
	scantokey[ 62] = K_F4;
	scantokey[ 63] = K_F5;
	scantokey[ 64] = K_F6;
	scantokey[ 65] = K_F7;
	scantokey[ 66] = K_F8;
	scantokey[ 67] = K_F9;
	scantokey[ 68] = K_F10;
	// 69 numlock
	// 70 scrollock
	scantokey[ 71] = K_HOME;
	scantokey[ 72] = K_UPARROW;
	scantokey[ 73] = K_PGUP;
	scantokey[ 74] = '-';
	scantokey[ 75] = K_LEFTARROW;
	scantokey[ 76] = '5';
	scantokey[ 77] = K_RIGHTARROW;
	scantokey[ 79] = K_END;
	scantokey[ 78] = '+';
	scantokey[ 80] = K_DOWNARROW;
	scantokey[ 81] = K_PGDN;
	scantokey[ 82] = K_INS;
	scantokey[ 83] = K_DEL;
	// 84 to 86 not used
	scantokey[ 87] = K_F11;
	scantokey[ 88] = K_F12;
	// 89 to 95 not used
	scantokey[ 96] = K_ENTER; //keypad enter
	scantokey[ 97] = K_CTRL; //right
	scantokey[ 98] = '/';
	scantokey[ 99] = K_F12; // print screen, bind to screenshot by default
	scantokey[100] = K_ALT; // right

	scantokey[101] = K_PAUSE; // break
	scantokey[102] = K_HOME;
	scantokey[103] = K_UPARROW;
	scantokey[104] = K_PGUP;
	scantokey[105] = K_LEFTARROW;
	scantokey[106] = K_RIGHTARROW;
	scantokey[107] = K_END;
	scantokey[108] = K_DOWNARROW;
	scantokey[109] = K_PGDN;
	scantokey[110] = K_INS;
	scantokey[111] = K_DEL;

	scantokey[119] = K_PAUSE;
}



void initkeyhandler()
{
	int i;
	static struct os2_key_packet os2_key1, os2_key2;
	static struct os2_buffer os2_in1, os2_in2, os2_out1, os2_out2;
	int r;

	set_scantokey();
	if ((r = DosMonOpen("KBD$", &os2_hmon))) 
		I_Error("DosMonOpen: %d\n", r);
	os2_key = OS2_ALIGN(os2_key);
	os2_in = OS2_ALIGN(os2_in);
	os2_out = OS2_ALIGN(os2_out);
	os2_in->cb = sizeof(struct os2_buffer) - sizeof(USHORT);
	os2_out->cb = sizeof(struct os2_buffer) - sizeof(USHORT);
	if ((r = DosMonReg(os2_hmon, (void *)os2_in, (void *)os2_out, MONITOR_END, -1))) {
		if (r == ERROR_MONITORS_NOT_SUPPORTED) {
			I_Error("VIO window not supported");
		} else {
			I_Error("DosMonReg: %d\n", r);
		}
	}

	/*old key state thing*/
	for (i=0; i<128; i++) keydown[i]=0;
	for (i=0; i<128; i++) extendedkeydown[i]=0;
	for (i=0; i<128; i++) oldkeystate[i]=0;
	for (i=0; i<128; i++) oldextendedkeystate[i]=0;
	nextkeyextended=0;
}

void killkeyhandler()
{
	if (os2_hmon != (HMONITOR)-1) DosMonClose(os2_hmon);
	os2_hmon = -1;
}

void keyhandler()
{
	unsigned char keyhandlercurrkey;

	keyhandlercurrkey=0x0;	/*inportb(0x60);*/

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

	if ((keydown[0x1d])&&(keyhandlercurrkey==0x2e))
	{
	}

/*reset keyoard */
}

byte ASCIINames[] =             /* Unshifted ASCII for scan codes */
{
/*	 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F */
	0,27,'1','2','3','4','5','6','7','8','9','0','-','=',8,9,               /* 0 */
	'q','w','e','r','t','y','u','i','o','p','[',']',13,0,'a','s',           /* 1 */
	'd','f','g','h','j','k','l',';',39,'`',0,92,'z','x','c','v',            /* 2 */
	'b','n','m',',','.','/',0,'*',0,' ',0,0,0,0,0,0,                        /* 3 */
	0,0,0,0,0,0,0,'7','8','9','-','4','5','6','+','1',                      /* 4 */
	'2','3','0',127,0,0,0,0,0,0,0,0,0,0,0,0,                                /* 5 */
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                                        /* 6 */
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0                                         /* 7 */
};

/*end of newly added stuff */


void I_ShutdownGraphics(void)
{
	g_unlock();
	g_mode(G_MODE_OFF);
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


	char buf[KBD_BUFFER_SIZE];
	int i;
	if (os2_hmon == (HMONITOR)-1) return;
	while ((i = os2_read_mon(buf, KBD_BUFFER_SIZE))) {
		int j;
		for (j = 0; j < i; j++)
			{
			if(buf[j] & 0x80)
				event.type=ev_keydown;
			else
				event.type=ev_keyup;
			switch(buf[j]&0x80){
				case 72: event.data1=KEY_UPARROW;break;
				case 77: event.data1=KEY_RIGHTARROW;break;
				case 80: event.data1=KEY_DOWNARROW;break;
				case 75: event.data1=KEY_LEFTARROW;break;
				default:
				event.data1=scantokey[buf[j] & 0x7f];break;
				}
			D_PostEvent(&event);
			//Key_Event(scantokey[buf[j] & 0x7f], !(buf[j] & 0x80));
			}
	}

#if 0
	char tempkey[128];
	char tempextendedkey[128];
	int i;
	int xmickeys,ymickeys,buttons;
	static int lastbuttons=0;


	/*key presses */
	for (i=0; i<128; i++)
	{
		tempkey[i]=keydown[i];
		tempextendedkey[i]=extendedkeydown[i];
	}

	for (i=0; i<128; i++)
	{
		if ((tempkey[i]==1)&&(oldkeystate[i]==0))
		{
			event.type=ev_keydown;
			if (ASCIINames[i]!=0)
				event.data1=ASCIINames[i];
			else
				event.data1=i+0x80;
			D_PostEvent(&event);
		}
		if ((tempkey[i]==0)&&(oldkeystate[i]==1))
		{
			event.type=ev_keyup;
			if (ASCIINames[i]!=0)
				event.data1=ASCIINames[i];
			else
				event.data1=i+0x80;
			D_PostEvent(&event);
		}
		if ((tempextendedkey[i]==1)&&(oldextendedkeystate[i]==0))
		{
			event.type=ev_keydown;
			switch (i)
			{
			case 0x48: event.data1=KEY_UPARROW; D_PostEvent(&event); break;
			case 0x4d: event.data1=KEY_RIGHTARROW; D_PostEvent(&event); break;
			case 0x50: event.data1=KEY_DOWNARROW; D_PostEvent(&event); break;
			case 0x4b: event.data1=KEY_LEFTARROW; D_PostEvent(&event); break;
			}
		}
		if ((tempextendedkey[i]==0)&&(oldextendedkeystate[i]==1))
		{
			event.type=ev_keyup;
			switch (i)
			{
			case 0x48: event.data1=KEY_UPARROW; D_PostEvent(&event); break;
			case 0x4d: event.data1=KEY_RIGHTARROW; D_PostEvent(&event); break;
			case 0x50: event.data1=KEY_DOWNARROW; D_PostEvent(&event); break;
			case 0x4b: event.data1=KEY_LEFTARROW; D_PostEvent(&event); break;
			}
		}
	}
	memcpy(oldkeystate,tempkey,128);
	memcpy(oldextendedkeystate,tempextendedkey,128);
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
	unsigned char * graph_phys;
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
	graph_phys=MAKEP(phys.asel[0],0);
	memcpy(graph_phys,screens[0],SCREENWIDTH*SCREENHEIGHT);
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
int shiftcomponents;
shiftcomponents=2;
_outp8(0x3c8,0);
for(i=0;i<768;i++)
	_outp8(0x3c9,palette[i]>>shiftcomponents);
}


void I_InitGraphics(void)
{
	static int firsttime=1;
	int status;

	if (!firsttime)
		return;
	firsttime=0;

	/*enter graphics mode */
#if 0
	g_mode(G_MODE_VGA_L);
#else
	graph_mode.cb=13;
	graph_mode.fbType=3;
	graph_mode.color=8;		/*bits depth*/
	graph_mode.col=40;		/*text mode*/
	graph_mode.row=25;
	graph_mode.hres=320;	/*graphics resolutions*/
	graph_mode.vres=200;
	graph_mode.fmt_ID=0;
	graph_mode.attrib=1;
	status=VioSetMode(&graph_mode,0);
	if(status!=0)
	{
		I_Error("error with VIOSETMODE\n");
	}	
#endif
	g_lock();
	dascreen=(PBYTE)0xa0000;   /*(byte *)(__djgpp_conventional_base+0xa0000);*/
	phys.pBuf=(PBYTE)0xa0000;
	phys.cb=0x10000;
	status=VioGetPhysBuf(&phys,0);
	if(status!=0)
	{
		I_Error("error with VioGetPhysBuf\n");
	}
	screens[0]=(byte *)malloc(SCREENWIDTH*SCREENHEIGHT);

	/*init the mouse */

	/*init keyboard */
	initkeyhandler();
}



int inited;


