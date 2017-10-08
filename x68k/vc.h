#ifndef __VC_H__
#define __VC_H__

/*#include <stdint.h>*/



#define VIDCON_BASE	(0xE82000)
#define GRAPHICS_PAL	(VIDCON_BASE)	/* graphics layers*/
#define TEXT_PAL	(VIDCON_BASE+0x200)	/* text, sprite, bg*/
#define VIDCON_R0	(VIDCON_BASE+0x400)	
#define VIDCON_R1	(VIDCON_BASE+0x500)	
#define VIDCON_R2	(VIDCON_BASE+0x600)	

#define RGB(r,g,b,i) (g<<11 | r<<6 | b<<1 | i)

/* cfd9
//   G     R     B   I
// 11001 11111 01100 1
// 11111 11001 01100 0
//   R     G     B   A(ignored)
*/
extern short *vidcon_r0;
extern short *vidcon_r1;
extern short *vidcon_r2;

/* palette entries are 5551 GRBi*/
extern short *gpal;
extern short *tpal;


#endif
