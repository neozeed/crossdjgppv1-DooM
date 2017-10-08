
#include "mfp.h"

#define uint8_t unsigned char

volatile uint8_t *mfp_gpip = (uint8_t*)MFP_GPIP;
volatile uint8_t *mfp_ierb = (uint8_t*)MFP_IERB;
volatile uint8_t *mfp_iprb = (uint8_t*)MFP_IPRB;
volatile uint8_t *mfp_isrb = (uint8_t*)MFP_ISRB;
volatile uint8_t *mfp_imrb = (uint8_t*)MFP_IMRB;