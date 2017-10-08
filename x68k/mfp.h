#ifndef __MFP_H__
#define __MFP_H__

#define uint8_t unsigned char

#define MFP_BASE	(0xE88000)

/* GPIP control*/
#define MFP_GPIP	(MFP_BASE+0x01) /* GPIO*/
#define MFP_AER		(MFP_BASE+0x03) /* active edge*/
#define MFP_DDR		(MFP_BASE+0x05) /* data direction*/

/* interrupt control*/
#define MFP_IERA	(MFP_BASE+0x07) /* irq enable A*/
#define MFP_IERB	(MFP_BASE+0x09) /* irq enable B*/
#define MFP_IPRA	(MFP_BASE+0x0B) /* irq pending A*/
#define MFP_IPRB	(MFP_BASE+0x0D) /* irq pending B*/
#define MFP_ISRA	(MFP_BASE+0x0F) /* irq service A*/
#define MFP_ISRB	(MFP_BASE+0x11) /* irq service B*/
#define MFP_IMRA	(MFP_BASE+0x13) /* irq mask A*/
#define MFP_IMRB	(MFP_BASE+0x15) /* irq mask B*/
#define MFP_VR		(MFP_BASE+0x17) / vector register*/

/* timer control*/
#define MFP_TACR	(MFP_BASE+0x19)	/* timer control A*/
#define MFP_TBCR	(MFP_BASE+0x1B) /* timer control B*/
#define MFP_TCDCR	(MFP_BASE+0x1D) /* timer control C&D*/
#define MFP_TADR	(MFP_BASE+0x1F) /* timer A data*/
#define MFP_TBDR	(MFP_BASE+0x21) /* timer B data*/
#define MFP_TCDR	(MFP_BASE+0x23) /* timer C data*/
#define MFP_TDDR	(MFP_BASE+0x25) /* timer D data*/

/* USART control*/
#define MFP_SCR		(MFP_BASE+0x27)	/* SYNC character*/
#define MFP_UCR		(MFP_BASE+0x29) /* USART control*/
#define MFP_RSR		(MFP_BASE+0x2B) /* receiver status*/
#define MFP_TSR		(MFP_BASE+0x2D) /* transmitter status*/
#define MFP_USR		(MFP_BASE+0x2F) /* USART data*/

extern volatile uint8_t *mfp_gpip;
extern volatile uint8_t *mfp_ierb;
extern volatile uint8_t *mfp_iprb;
extern volatile uint8_t *mfp_isrb;
extern volatile uint8_t *mfp_imrb;

#endif