#include "types.h"
#ifndef _KPM_H_
#define _KPM_H_

#define ROW0 16
#define ROW1 17
#define ROW2 18
#define ROW3 19
#define COL0 20
#define COL1 21
#define COL2 22
#define COL3 23

//#define READBIT(WORD,BITPOS) ((WORD>>BITPOS)&1)
//#define READNIBBLE(WORD,BITPOS) ((WORD>>BITPOS)&0xF)
//#define WRITENIBBLE(WORD,BITPOS,BYTE) (WORD=((WORD&~(0xF<<BITPOS))|(BYTE<<BITPOS)))

void Init_KPM(void);
u8 ColScan(void);
u32 Col_Check(void);
u32 Row_Check(void);
#endif
