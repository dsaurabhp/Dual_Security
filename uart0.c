#include "dual_Security.h"
#include "uart0.h"
u8 rx;
char rec_buff[25],dummy;
unsigned char global=0,ch,r_flag;

extern unsigned char buf[25]; 

void UART0_isr(void) __irq
{
  if((U0IIR & 0x04)) //check if receive interrupt
  {
		ch = U0RBR;	// Read to Clear Receive Interrupt
		if(global<25)
			rec_buff[global++] = ch; 
  }
  else
  {
      dummy=U0IIR; //Read to Clear transmit interrupt
  
  }
   VICVectAddr = 0; // dummy write 
}
void InitUART0(void)
{
	PINSEL0 = PINSEL0 | 0x00000005;	// Enable UART0 Rx0 and Tx0 pins of UART0
	U0LCR = 0x83;	// DLAB = 1, 1 stop bit, 8-bit character length 
	U0DLM = 0x00;	// For baud rate of 9600 with Pclk = 15MHz 
	U0DLL = 0x61;	// We get these values of U0DLL and U0DLM from formula 
	U0LCR = 0x03; // DLAB = 0 
	U0IER = 0x00000003;	//Enable THRE and RBR interrupt 
	VICVectAddr0|= (unsigned) UART0_isr;	// UART0 ISR Address 
	VICVectCntl0|= 0x00000026;	// Enable UART0 IRQ slot
	VICIntEnable|= 0x00000040;	// Enable UART0 interrupt
	VICIntSelect|= 0x00000000;	// UART0 configured as IRQ 
}

void UART0_Tx(char ch)  /* Write character to Serial Port    */  
{ 
  while (!(U0LSR & 0x20));
  U0THR = ch;                
}

char UART0_Rx(void)    /* Read character from Serial Port   */
{                     
  while (!(U0LSR & 0x01));
  return (U0RBR);
}
void UART0_Str1(unsigned char n)
{
 	unsigned char j;   
    for(j=0;j<n;j++)
    {
        UART0_Tx(buf[j]);
    }   
}
