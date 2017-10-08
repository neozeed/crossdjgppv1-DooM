#ifndef __CRTC_H__
#define __CRTC_H__

/*#include <stdint.h>*/


#define CRTC_BASE (0xE80000)
#define CRTC_R00 (CRTC_BASE)
#define CRTC_R01 (CRTC_BASE+0x2)
#define CRTC_R02 (CRTC_BASE+0x4)
#define CRTC_R03 (CRTC_BASE+0x6)
#define CRTC_R04 (CRTC_BASE+0x8)
#define CRTC_R05 (CRTC_BASE+0xA)
#define CRTC_R06 (CRTC_BASE+0xC)
#define CRTC_R07 (CRTC_BASE+0xE)
#define CRTC_R08 (CRTC_BASE+0x10)
#define CRTC_R09 (CRTC_BASE+0x12)
#define CRTC_R10 (CRTC_BASE+0x14)
#define CRTC_R11 (CRTC_BASE+0x16)
#define CRTC_R12 (CRTC_BASE+0x18)
#define CRTC_R13 (CRTC_BASE+0x1A)
#define CRTC_R14 (CRTC_BASE+0x1C)
#define CRTC_R15 (CRTC_BASE+0x1E)
#define CRTC_R16 (CRTC_BASE+0x20)
#define CRTC_R17 (CRTC_BASE+0x22)
#define CRTC_R18 (CRTC_BASE+0x24)
#define CRTC_R19 (CRTC_BASE+0x26)
#define CRTC_R20 (CRTC_BASE+0x28)
#define CRTC_R21 (CRTC_BASE+0x2A)
#define CRTC_R22 (CRTC_BASE+0x2C)
#define CRTC_R23 (CRTC_BASE+0x2E)
#define CRTC_MEM (CRTC_BASE+0x421)

/* horizontal control*/
extern short *crtc_r00;	/* htotal*/
extern short *crtc_r01;	/* hsync end*/
extern short *crtc_r02;	/* hdisp start*/
extern short *crtc_r03;	/* hdisp end*/

/* vertical control*/
extern short *crtc_r04;	/* vtotal*/
extern short *crtc_r05;	/* vsync end*/
extern short *crtc_r06;	/* vdisp end*/
extern short *crtc_r07;	/* vdisp start*/

/*external hsync adjust/hposition fine tune*/
extern short *crtc_r08;	

/* raster numbers: raster interrupt(?)*/
extern short *crtc_r09;	

/* text screen scrolling*/
extern short *crtc_r10;	/* x position*/
extern short *crtc_r11;	/* y position*/

/* graphic screen scroll*/
extern short *crtc_r12;	/* x0*/
extern short *crtc_r13;	/* y0*/
extern short *crtc_r14;	/* x1*/
extern short *crtc_r15;	/* y1*/
extern short *crtc_r16;	/* x2*/
extern short *crtc_r17;	/* y2*/
extern short *crtc_r18;	/* x3*/
extern short *crtc_r19;	/* y3*/

/* graphic mode setting*/
extern volatile short *crtc_r20;

/* tvram plane simultaneous access enable/tvram raster copy enable/fast clear gvram enable*/
extern volatile short *crtc_r21;

/* raster copy source/destination*/
extern short *crtc_r22;

/* text screen access mask pattern*/
extern short *crtc_r23;

/* image capture/fast clear/raster copy control*/
extern volatile char *crtc_mem;

#endif