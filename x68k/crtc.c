
#include "crtc.h"

short *crtc_r00 = (short*)CRTC_R00;	/* htotal*/
short *crtc_r01 = (short*)CRTC_R01;	/* hsync end*/
short *crtc_r02 = (short*)CRTC_R02;	/* hdisp start*/
short *crtc_r03 = (short*)CRTC_R03;	/* hdisp end*/

/* vertical control*/
short *crtc_r04 = (short*)CRTC_R04;	/* vtotal*/
short *crtc_r05 = (short*)CRTC_R05;	/* vsync end*/
short *crtc_r06 = (short*)CRTC_R06;	/* vdisp end*/
short *crtc_r07 = (short*)CRTC_R07;	/* vdisp start*/

/* external hsync adjust/hposition fine tune*/
short *crtc_r08 = (short*)CRTC_R08;	

/* raster numbers: raster interrupt(?)*/
short *crtc_r09 = (short*)CRTC_R09;	

/* text screen scrolling-*/
short *crtc_r10 = (short*)CRTC_R09;	/* x position*/
short *crtc_r11 = (short*)CRTC_R10;	/* y position*/

/* graphic screen scroll*/
short *crtc_r12 = (short*)CRTC_R12;	/* x0*/
short *crtc_r13 = (short*)CRTC_R13;	/* y0*/
short *crtc_r14 = (short*)CRTC_R14;	/* x1*/
short *crtc_r15 = (short*)CRTC_R15;	/* y1*/
short *crtc_r16 = (short*)CRTC_R16;	/* x2*/
short *crtc_r17 = (short*)CRTC_R17;	/* y2*/
short *crtc_r18 = (short*)CRTC_R18;	/* x3*/
short *crtc_r19 = (short*)CRTC_R19;	/* y3*/

/* graphic mode setting*/
volatile short *crtc_r20 = (short*)CRTC_R20;

/* tvram plane simultaneous access enable/tvram raster copy enable/fast clear gvram enable*/
volatile short *crtc_r21 = (short*)CRTC_R21;

/* raster copy source/destination*/
short *crtc_r22 = (short*)CRTC_R22;

/* text screen access mask pattern*/
short *crtc_r23 = (short*)CRTC_R23;

/* image capture/fast clear/raster copy control*/
volatile char *crtc_mem = (char*)CRTC_MEM;
