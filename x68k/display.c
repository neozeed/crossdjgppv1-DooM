// use two 256-color graphics layers
// toggle them on and off for double buffering

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vc.h"
#include "crtc.h"
#include "tvram.h"
#include "cynthia.h"
#include "dos.h"

#ifdef DEBUG
#include "serial.h"
char debugstr[65535];
#endif


#define GVRAM_BASE (0xC00000)
#define GVRAM0 (GVRAM_BASE)
#define GVRAM1 (GVRAM_BASE+0x80000)
#define GVRAM2 (GVRAM_BASE+0x100000)
#define GVRAM3 (GVRAM_BASE+0x180000)

short *gvram0 = (short*)GVRAM0;
short oldTVPal[16];

static inline void FB_clear(){
	memset(gvram0, 0x02, 0x100000);		// clear all gvram
	memset(tvram0, 0, 512*1024);		// clear all tvram
	
	memset(gpal, 0x0, 256*2);				// clear all palettes
	memset(gpal, 0xC9, 2);
	memset(gpal+1, 0xF0, 2);
	memset(gpal+2, 0x1, 2);
	memset(tpal, 0, 256*2);
	memset(pcg_patterns, 0, 0x8000);	// clear all patterns from pcg
	memset(bg0_tiles, 0, 0x2000);	// clear all bg's
}

void FB_init(){
	// 512x512, 256 color, gv/tvram graphics enabled
	/**crtc_r20 = 0x115; 
	
	// various display timings
	*crtc_r00 = 0x5B;
	*crtc_r01 = 0x09;
	*crtc_r02 = 0x11;
	*crtc_r03 = 0x51;
	*crtc_r04 = 0x237;
	*crtc_r05 = 0x05;
	*crtc_r06 = 0x28;
	*crtc_r07 = 0x228;
	*crtc_r08 = 0x1B;
	*/
	
	// 256x256 256 color, gv/tvram enabled
	*crtc_r20 = 0x110; 
	
	// various display timings
	*crtc_r00 = 0x2D;
	*crtc_r01 = 0x04;
	*crtc_r02 = 0x06;
	*crtc_r03 = 0x26;
	*crtc_r04 = 0x237;
	*crtc_r05 = 0x05;
	*crtc_r06 = 0x28;
	*crtc_r07 = 0x228;
	*crtc_r08 = 0x1B;
	
	// gvram settings
	// 0 = 512x512, 16 color 
	// 4 = 1024x1024, 16
	// 1 = 512x512, 256
	// 2 = dont use
	// 3 = 512x512, 65536(swizzled)
	*vidcon_r0 = 0x1;	

	// set scrolls to 0
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

	// text, graphics, bg/sprite, 
	// graphics layers priority, 0, 1, 2, 3
	*vidcon_r1 = 0x34A4; // 0x31A4 = text, graphics, bg/sprite
	// set text and GVRAM0 layers visible, enable transparency stuff
	*vidcon_r2 = 0x1B61;//0x21; // no transparency
	
	// 8x8 31khz BGn
	*cynthia_res = 0x10;
	
	// disable soft mouse stuff (right click)
	_iocs_tgusemd(1, 2);
	_iocs_tgusemd(0, 2);
	_dos_c_curoff();
	
	memcpy(&oldTVPal, tpal, 32);		// backup old text palette
	FB_clear();
	
	// 0 is transparent color for tvram. maybe for pcg also.
	// white seems to be transparent color for gvram
	memset(tpal, 0, 32);
}

void FB_deinit(){

	// 768x512, 16 color
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

	// restore text mode priorities
	*vidcon_r1 = 0x06E4;
	// restore text layer visibility
	*vidcon_r2 = 0x20;

	//FB_clear();
	memset(tvram0, 0, 512*1024);		// clear all tvram
	_iocs_tgusemd(1, 1);			// enable soft mouse, keyboard, etc
	_iocs_tgusemd(0, 1);			// ""
	_dos_c_curon();					// turn on DOS cursor
	memcpy(tpal, &oldTVPal, 32);	// restore old text pal
}

extern inline void FB_putpixel(char color, short x, short y){
	// same for 4 and 8 bit 512x512
	// address = GVRAMn+y*1024+x*2
}

// need to check file length
void FB_loadpal(char *fn, short *dest, short offset){
	FILE *in = NULL;
	int i = 0;
	int filesize = 0;
	short *buf = NULL;
	dest = (short*)(dest+offset);
	
	#ifdef DEBUG
	sprintf(debugstr, "%16x\n", dest);
	SER_debug(debugstr, SER_DEBUG_INFO);
	memset(debugstr, 1, 65535);
	#endif
	
	in = fopen(fn, "rb");

	fseek(in, 0, SEEK_END);
	filesize = ftell(in);
	fseek(in, 0, SEEK_SET);

	buf = (short*)malloc(filesize);	
	fread(buf, 1, filesize, in);
	memcpy(dest, buf, filesize);
//	for(i = 0;i<filesize;i++){
//		*gpal = buf[i];
//		gpal++;
//	}

	fclose(in);
}

// 512x512
// will extend later
void FB_loadimg(char *fn, short *dest, short x, short y){
	FILE *in = NULL;
	uint32_t i = 0;
	
	uint32_t filesize = 0;
	short *buf = NULL;
	
	short p0ofs = 0;
	short p1ofs = 0;
	short p2ofs = 0;
	short p3ofs = 0;

	short width = 0;
	short height = 0;
	short modebpp = 0;			
	short stride = 64;

	in = fopen(fn, "rb");
	
	fread(&modebpp, 1, 1, in);
	fread(&width, 1, 2, in);
	fread(&height, 1, 2, in);
	
	if(modebpp == 0x2400){
		
	}
	
	// TODO: need to fix this up
	#ifdef DEBUG
	sprintf(debugstr, "%x %i %i\n", modebpp, width, height);
	SER_debug(debugstr, SER_DEBUG_INFO);
	memset(debugstr, 1, 65535);
	#endif
	fseek(in, 0, SEEK_END);
	filesize = ftell(in)-4;
	fseek(in, 5, SEEK_SET);

	buf = (short*)malloc(filesize);
	fread(buf, 1, filesize, in);
	
	if(dest == tvram0){
		// this is planar
		width = width/8;
		short w2 = width/2;
		p0ofs = (filesize)/8;
		p1ofs = p0ofs;
		p2ofs = p0ofs*2;
		p3ofs = p0ofs*3;
		

		#ifdef DEBUG
		sprintf(debugstr, "%x %x %x %x %x\n", filesize, p1ofs, p2ofs, p3ofs, w2);
		SER_debug(debugstr, SER_DEBUG_INFO);
		memset(debugstr, 1, 65535);
		#endif
		
		for(i = 0; i<height; i++){
			// TODO update page offsets to be dynamic. see p0ofs and friends above.
			
			// TODO: figure what the deal is with 64 vs. 128 byte stride
			memcpy(&tvram0[i*stride], &buf[i*w2], width);
			memcpy(&tvram1[i*stride], &buf[i*w2+p1ofs], width);
			memcpy(&tvram2[i*stride], &buf[i*w2+p2ofs], width);
			memcpy(&tvram3[i*stride], &buf[i*w2+p3ofs], width);
		}
	} else if(dest == pcg_patterns){
		// TODO: make this correct...
		memcpy(dest, buf, filesize-1);
		//printf("Attempting PCG load...\n");
	} else {
		memcpy(dest, buf, filesize-1);
	}
	
	
	free(buf);
	fclose(in);
}




