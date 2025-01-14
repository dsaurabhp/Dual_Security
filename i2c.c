#include "i2c_define.h"
#include "defines.h"
#include <lpc21xx.h>
#include "delay.h"
void i2cStart(void)
{
	SSETBIT(IOSET0,SDA);
	delay_us(1);
	SSETBIT(IOSET0,SCL);
	delay_us(1);
	SETBIT(IODIR0,SDA);
	delay_us(1);
	SETBIT(IODIR0,SCL);
  delay_us(1);	
	SSETBIT(IOCLR0,SDA);
  delay_us(1);	
}

void i2cWrite(u8 sDat)
{
	s32 i;
	for(i=7;i>=0;i--)
	{
		SCLRBIT(IOCLR0,SCL);
    delay_us(1);		
    READWRITEBIT2(IOPIN0,SDA,sDat,i);	
    delay_us(1);		
		SSETBIT(IOSET0,SCL);
		delay_us(1);
	}
}

void i2cAck(void)
{
	SCLRBIT(IOCLR0,SCL);
  delay_us(1);	
	SSETBIT(IOSET0,SDA);
  delay_us(1);	
	CLRBIT(IODIR0,SDA);
  delay_us(1);  
	SSETBIT(IOSET0,SCL);
  delay_us(1); 	
	while(READBIT(IOPIN0,SDA)==1);
	SETBIT(IODIR0,SDA);
  delay_us(1);	
}

void i2cStop(void)
{
	SCLRBIT(IOCLR0,SCL); 
  delay_us(1);	
	SCLRBIT(IOCLR0,SDA); 
  delay_us(1);	
	SSETBIT(IOSET0,SCL); 
  delay_us(1);	
	SSETBIT(IOSET0,SDA);
  delay_us(1);	
}

void i2cReStart(void)
{
	SCLRBIT(IOCLR0,SCL); 
  delay_us(1);	
	SSETBIT(IOSET0,SDA);
  delay_us(1);	
	SSETBIT(IOSET0,SCL); 
  delay_us(1);	
	SCLRBIT(IOCLR0,SDA); 
}

void i2cNAck(void)
{
	SCLRBIT(IOCLR0,SCL); 
  delay_us(1);	
	SSETBIT(IOSET0,SDA); 
  delay_us(1);	
	SSETBIT(IOSET0,SCL); 
  delay_us(1);	
}

u8 i2cRead(void)
{
	s32 i;
	u8 buff;
	//cfg sda line as gpio in
	CLRBIT(IODIR0,SDA); 
  delay_us(1);	
  for(i=7;i>=0;i--)
	{
		SCLRBIT(IOCLR0,SCL);//M makes scl low so S can change sda		
		delay_us(1);
		SSETBIT(IOSET0,SCL);//M makes scl high to validate/read sda
	  delay_us(1);	
		READWRITEBIT2(buff,i,IOPIN0,SDA);
	  delay_us(1);	
	}
	SETBIT(IODIR0,SDA); 
  delay_us(1);	
  return buff;
}

void i2cMAck(void)
{
	SCLRBIT(IOCLR0,SCL);//M makes scl low 
  delay_us(1);	
	SCLRBIT(IOCLR0,SDA);//M make sda low , while slave should release sda
  delay_us(1);	
	SSETBIT(IOSET0,SCL);//for slave to read ack given by master
  delay_us(1);	
	SCLRBIT(IOCLR0,SCL);//M make scl low
  delay_us(1);	
	SSETBIT(IOSET0,SDA);//to release SDA
}

void i2c_eeprom_bytewrite(u8 slaveAddr,u16 wBuffAddr,u8 Dat)
{
	 i2cStart();            //begins this comm algo/session
	 i2cWrite(slaveAddr<<1);//Slave Address + W
	 i2cAck();
	 i2cWrite(wBuffAddr>>8);   //Address in eeprom location for writing into
	 i2cAck();
	 i2cWrite(wBuffAddr);   //Address in eeprom location for writing into
	 i2cAck();
	 i2cWrite(Dat);         //Data for that eeprom location
	 i2cAck();
	 i2cStop();            //ends this comm algo/session
	 delay_ms(10);         //wait period self timed write cycle in eeprom
}

u8 i2c_eeprom_randomread(u8 slaveAddr,u8 rBuffAddr)
{
	 u8 rDat;
	 i2cStart();            //begins this comm algo/session
	 i2cWrite(slaveAddr<<1);//Slave Address + W
	 i2cAck();
	 i2cWrite(rBuffAddr>>8);   //Address in eeprom location for reading from 
	 i2cAck();
	 i2cWrite(rBuffAddr);   //Address in eeprom location for reading from 
	 i2cAck();
	 i2cReStart();
	 i2cWrite((slaveAddr<<1)|1);//Slave Address + R
	 i2cAck();
	 rDat=i2cRead();
	 i2cNAck();
	 i2cStop();
	 return rDat;
}

void i2c_eeprom_pagewrite(u8 slaveAddr,u16 wBuffStartAddr,s8 *ptr,u8 nBytes)
{
	 i2cStart();            //begins this comm algo/session
	 i2cWrite(slaveAddr<<1);//Slave Address + W
	 i2cAck();
	i2cWrite(wBuffStartAddr>>8);   //Address in eeprom location for writing into
	 i2cAck();
	 i2cWrite(wBuffStartAddr);   //Start Address in eeprom location for writing into
	 i2cAck();
	 while(nBytes--)
	 {	 
	   i2cWrite(*ptr);         //Data for that eeprom location
	   i2cAck();
	   ptr++; 	 
	 }	 
	 i2cStop();            //ends this comm algo/session
	 delay_ms(10);         //wait period self timed write cycle in eeprom
}
  
void i2c_eeprom_seqread(u8 slaveAddr,u16 rBuffStartAddr,u8 *ptr,u8 nBytes)
{
	 u32 i=0;
   i2cStart();            //begins this comm algo/session
	 i2cWrite(slaveAddr<<1);//Slave Address + W
	 i2cAck();
	i2cWrite(rBuffStartAddr>>8);   //Address in eeprom location for writing into
	 i2cAck();
	 i2cWrite(rBuffStartAddr);   //Address in eeprom location for reading from 
	 i2cAck();
	 i2cReStart();
	 i2cWrite((slaveAddr<<1)|1);//Slave Address + R
	 i2cAck();
	
	 while(nBytes > 1)
	 {
		 ptr[i++]=i2cRead();
		 i2cMAck();
		 nBytes--;
   }		 
	 ptr[i++]=i2cRead();
	 i2cNAck();
	 ptr[i]='\0';
	 i2cStop();
}
