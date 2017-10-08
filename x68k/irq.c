#define uint32_t unsigned long


#include <stdio.h>
#include "irq.h"
#include "mfp.h"
#include "crtc.h"

volatile uint32_t oldVDispFP = 0;
volatile uint32_t *vec_vdisp = (uint32_t*)IRQ_46;
uint32_t blanks = 0;

void IRQ_vdisp(){
	/* do stuff that gets done during VBLANK */
	
	/* we can check IPRB to see if we're already handling a VDISP */
	/* and drop this frame */
	
	blanks++;
	*crtc_r12 = blanks;
	*crtc_r13 = blanks;
	*crtc_r14 = blanks;
	*crtc_r15 = -blanks;
	/* we can either set MFP flag that says int is handled */
	/* or call the old FP and see what happens */
	
	/* ISRB bit 6 = 0; // do this at the end to say we're done */
	*mfp_isrb ^= 64;
}

/* function pointer to handler, address of irq vector */
void IRQ_insVDisp(void (*func)()){
	/* backup old function pointer */
	/* set interrupt vector to this function pointer */
	/* set up MFP stuff */
	/* goooooo */

	oldVDispFP = *vec_vdisp;
	*vec_vdisp = func;
	
	*mfp_gpip ^= 8;
	*mfp_ierb ^= 64;
	*mfp_imrb ^= 64;
	/* GPIP bit 4 = 1; // enable vdisp interrupt source */
	/* IERB bit 6 = 1; // enable vdisp handling */
	/* IMRB bit 6 = 1; // unmask interrupt */
}


void IRQ_uninsVDisp(){
	/* restore old MFP state */
	/* restore old FP */
	*mfp_gpip ^= 8;
	*mfp_ierb ^= 64;
	*mfp_imrb ^= 64;

	*vec_vdisp = oldVDispFP;
}
