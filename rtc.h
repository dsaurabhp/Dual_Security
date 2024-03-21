#ifndef _RTC_H_
#define _RTC_H_

#define FOSC      12000000
#define CCLK  	  5*FOSC
#define PCLK  	  CCLK/4
#define PREINT_VAL ((PCLK/32768)-1)
#define PREFRAC_VAL (PCLK - ((PREINT_VAL + 1) * 32768))
#endif
