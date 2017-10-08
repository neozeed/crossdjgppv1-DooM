
#include "vc.h"

short *vidcon_r0 = (short*)VIDCON_R0;	/* screen mode*/
short *vidcon_r1 = (short*)VIDCON_R1;	/* priority control*/
short *vidcon_r2 = (short*)VIDCON_R2;	/* on/off control / special priority*/

/* palette entries are 5551 GRBi*/
short *gpal = (short*)GRAPHICS_PAL;
short *tpal = (short*)TEXT_PAL;