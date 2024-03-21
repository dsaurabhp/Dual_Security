#include <lpc21xx.h>
#include "types.h"
#include "defines.h"
#include  "kpm.h"
void Init_KPM(void)
{
	IODIR1=0xF<<ROW0;
	//IOSET1|=0xF<<COL0;
}
u32 Col_Check(void)
{
	u32 i;
	for(i=0;i<4;i++)
	{
		if(READBIT(IOPIN1,(COL0 + i))==0)
			break;
	}
	return i;
}
u32 Row_Check(void)
{
	u32 i;
	for(i=0;i<4;i++)
	{
		WRITENIBBLE(IOPIN1,ROW0,(~(1<<i)));
		if(READNIBBLE(IOPIN1,COL0)!=15)
			break;
	}
	WRITENIBBLE(IOPIN1,ROW0,0);
	return i;
}

u8 ColScan(void)
{
	return ((READNIBBLE(IOPIN1,COL0)==15)?1:0);
}
