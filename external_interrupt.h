#ifndef _EXT_H_
#define _EXT_H_
#define EINT2_LED 4//P0.4
void eint2_isr(void)__irq;
void Init_ExtIns(void);
void external_interrupt_handler(void);
#endif
