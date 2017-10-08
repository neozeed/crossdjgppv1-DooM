#include <stdint.h>

#include "cynthia.h"

uint16_t *bg_control = (uint16_t*)BG_CONTROL;
uint16_t *bg0_xpos = (uint16_t*)BG0_XPOS;
uint16_t *bg0_ypos = (uint16_t*)BG0_YPOS;
uint16_t *bg1_xpos = (uint16_t*)BG1_XPOS;
uint16_t *bg1_ypos = (uint16_t*)BG1_YPOS;
uint16_t *bg0_tiles = (uint16_t*)BG0_TILES;
uint16_t *bg1_tiles = (uint16_t*)BG1_TILES;

uint16_t volatile *cynthia_res = (uint16_t*)CYNTHIA_RES;
uint16_t *cynthia_htotal = (uint16_t*)CYNTHIA_HTOTAL;
uint16_t *cynthia_hdisp = (uint16_t*)CYNTHIA_HDISP;
uint16_t *cynthia_vtotal = (uint16_t*)CYNTHIA_VTOTAL;

uint16_t *pcg_patterns = (uint16_t*)PCG_PATTERNS;
uint16_t *pcg_BG0 = (uint16_t*)PCG_BG0;
uint16_t *pcg_BG1 = (uint16_t*)PCG_BG1;

SPR *sprites = SPRITE_BASE;