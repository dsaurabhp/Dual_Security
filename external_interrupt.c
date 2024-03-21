#include "dual_Security.h"
#include "external_interrupt.h"
extern u32 EINTSW;
extern u8 rbyte[8];
extern u8 kpmLUT[4][4];
extern u8 rx,invalid;
extern u32 EINTSW;
extern u32 c,i;
extern u32 rNo,cNo;
extern u8 pswd[5];
extern u8 var;
void Init_ExtIns(void)
{
	IODIR0|=1<<EINT2_LED;
	IOCLR0|=1<<EINT2_LED;
	PINSEL0=0x0000C000;
	VICIntSelect|=0; 
	VICVectAddr5|=(unsigned)  eint2_isr;
	VICVectCntl5|=1<<5|16;
	VICIntEnable|=1<<16;	
	EXTINT=4;
	EXTMODE=4;
	//EXTPOLAR=4;
}
void eint2_isr(void)__irq
{
	EINTSW=1;
	EXTINT|=4;
	VICVectAddr=0;
}
