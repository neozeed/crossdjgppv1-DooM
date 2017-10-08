#ifndef __PCG_H__
#define __PCG_H__

#include <stdint.h>

#define PCG_BASE	(0xEB8000)
#define PCG_PATTERNS	(PCG_BASE)

// BG's are a list of uint16_t as define below
//
// VR(1),HR(1),unused(2),color(4),PCG#(8)
// VR = vertical flip
// HR = horizontal flip
// COLOR = top 4 bits of color code (low 4 bits from PCG)
//         this is which palette to use
#define PCG_BG0		(PCG_BASE+0x2000)
#define PCG_BG1		(PCG_BASE+0x4000)

// patterns are 4-bit indexed color
// color palette is in the Sprite RAM or in PCG_BGn
// there is room for up to 256 8x8 or 64 16x16
// 8x8 patterns are linear and sequential in memory
// 16x16 patterns are made of 8x8 patterns like this:

/*
address|16x16| 8x8 |
-------+-----+-----+
EB8000 |  0  |  0  |
EB8020 |     |  1  |
EB8040 |     |  2  |
EB8060 |     |  3  |
-------+-----+-----+
.......|  n  |  n  |
.......|     | n+1 |
.......|     | n+2 |
.......|     | n+3 |
-------+-----+-----+

+-----+-----+
-     |     -
-  0  |  2  -
-     |     -
+-----+-----+
-     |     -
-  1  |  3  -
-     |     -
+-----+-----+
*/

#define SPRITE_BASE	(0xEB0000)
// 0xEBFFFF

#define BG_BASE		(0xEB0800)
#define BG0_YPOS	(BG_BASE)
#define BG0_XPOS	(BG_BASE+0x2)
#define BG1_YPOS	(BG_BASE+0x4)
#define BG1_XPOS	(BG_BASE+0x6)
#define BG0_TILES	(0xEBC000)
#define BG1_TILES	(0xEBE000)

// unused(6),DISP/CPU(1),unused(3),BG1TXSEL(2), BG1ON(1),BG0TXSEL(2),BG0ON(1)
//
// DISP/CPU = toggles sprite/BG display on/off. off (0) allows fast access to PCG.
// BGnTXSEL = use BGn data area 0, 1 (2, 3 = undefined)
// BGnON = BGn on(1), off(0)
#define BG_CONTROL	(BG_BASE+0x8) 

#define CYNTHIA_BASE	(0xEB080A)
#define CYNTHIA_HTOTAL	(CYNTHIA_BASE)
#define CYNTHIA_HDISP	(CYNTHIA_BASE+0x02)
#define CYNTHIA_VTOTAL	(CYNTHIA_BASE+0x04)

// unused(13),L/H(1),V-RES(2),H-RES(2)
//
// L/H = low res/high res
// V-RES = these both probably need to match CRTC r20
// H-RES = 0 0 = 256x256 tile, 512x512 virtual, 8x8 tiles
//	   0 1 = 512x512 tile, 1025x1024 virtual, 16x16 tiles		
#define CYNTHIA_RES	(CYNTHIA_BASE+0x06)

// VR = horizontal flip
// HR = vertical flip
// COLOR = top 4 bits of color code(bottom 4 bits is from PCG)
//         this is which pallete to use
// SPAT# = sprite pattern number (out of the PCG area)
// PRW = priority
// 0 0 = sprite not shown
// 0 1 = bg0, bg1, sp
// 1 0 = bg0, sp, bg1
// 1 1 = sp, bg0, bg1
typedef struct SPR{
	uint16_t xpos;	// low 10 bits
	uint16_t ypos;	// low 10 bits
	uint16_t data1;	// VR(2)|HR(2)|(2)|color(4)|spat #(8)
	uint16_t data2; // |(13)|'0'(1)|PRW[priority bits](2)
}SPR; // 0~127

extern uint16_t *bg_control;
extern uint16_t *bg0_xpos;
extern uint16_t *bg0_ypos;
extern uint16_t *bg1_xpos;
extern uint16_t *bg1_ypos;
extern uint16_t *bg0_tiles;
extern uint16_t *bg1_tiles;

extern volatile uint16_t *cynthia_res;
extern uint16_t *cynthia_htotal;
extern uint16_t *cynthia_hdisp;
extern uint16_t *cynthia_vtotal;

// these overlap depending on if BG's are enabled or not
extern uint16_t *pcg_patterns;

// 64x64 total
// 32x32 visible
// this does not depend on the cynthia res
// each 16-bits is:
// VR(1)|HR(2)|unused(2)|COLOR(low 4-bits)|PCG pattern number(8)

extern uint16_t *pcg_BG0;
extern uint16_t *pcg_BG1;

extern SPR *sprites; 

#endif