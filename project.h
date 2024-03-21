#include "dual_Security.h"
#include "lcd.h"
#include "types.h"
#include "kpm.h"
#include "i2c_define.h"
#include "defines.h"
#include "external_interrupt.h"
#include "string.h"
#include "rtc.h"
#include "uart0.h"
#include "R305.h"
#include "delay.h"

void change_FNG(u32 n);

u8 rbyte[8]; //;// __attribute__((at(0x40000010)));
u8 kpmLUT[4][4]=
{
	{'1','2','3','/'},
  {'4','5','6','x'},
  {'7','8','9','-'},
  {'C','0','=','+'}
};
u32 cnt=0,K=0,Del=0;
s32 n;
u32 rNo=0,cNo=0;
u8 pswd[5];
u8 usrpswd[10]={0x00,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80,0x90};
u8 useri2c[10]={0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9};
u8 *lcdstr[]={"Enter Password","2.CNGPSWD","2.UPD FNG","Enter new PSWD","Ent Confm PSWD","pswd not match","PSWD CHG SUC","same prev pswd","Enter USERID","Invalid USERID","1.ENROLL","  3.DEL"," 4.DALL"};
u8 EINTSW=0,EIN=0;

/*extern u8 *lcdstr[];
extern u8 EINTSW,EIN;
extern u32 rNo,cNo;
extern u8 useri2c[10];
extern u8 kpmLUT[4][4];
extern u32 cnt,K,Del;
extern s32 n;
extern u8 pswd[5],usrpswd[10];
extern u8 rbyte[8];*/
