#ifndef __TVRAM_H__
#define __TVRAM_H__

/*#include <stdint.h>*/



#define TVRAM_BASE	(0xE00000)
#define TVRAM_0		(TVRAM_BASE)	
#define TVRAM_1		(TVRAM_BASE+0x20000)
#define TVRAM_2		(TVRAM_BASE+0x40000)
#define TVRAM_3		(TVRAM_BASE+0x60000)

/* TVRAM is 4-bit indexed color
// each pixel is packed into 16-bits so there
// are 4 pixels per WORD
// there are 4 pages to write and this area can be scrolled
// each page is a linear buffer*/

extern short *tvram0;
extern short *tvram1;
extern short *tvram2;
extern short *tvram3;

#endif