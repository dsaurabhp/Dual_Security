#include "uart0.h"     
#include "lcd.h"   
#include "delay.h"  
#include<lpc21xx.h>
unsigned char ret,j,buf[25]; 

extern unsigned char global,rec_buff[25];


unsigned char collect(void)
{     
    for(j=0;j<12;j++)
        buf[j]=0x00;    
    buf[0]=0xEF; //MSB
    buf[1]=0x01;//LSB Header---2 BYTES    
    buf[2]=0xff;
    buf[3]=0xff;
    buf[4]=0xff;
    buf[5]=0xff; //LSB DEVICE ADRESS----4 BYTES     
    buf[6]=0x01;//Package identifier--1 BYTE    
    buf[8]=0x03;//LSB Package length--2 BYTES  
    buf[9]=0x01; //Instruction code--1 BYTE    
    buf[11]=0x05;//LSBChecksum--2 BYTES  

    global=0;
    UART0_Str1(12);  
    while(global<12);
    if(rec_buff[9]==0x00)
        return 0;
    else 
        return 1;        
}  


unsigned char store(unsigned char id)
{
    for(j=0;j<13;j++)
        buf[j]=0x00;    
    buf[0]=0xEF; //MSB
    buf[1]=0x01;//LSB Header---2 BYTES    
    buf[2]=0xff;
    buf[3]=0xff;
    buf[4]=0xff;
    buf[5]=0xff; //LSB DEVICE ADRESS----4 BYTES     
    buf[6]=0x01;//Package identifier--1 BYTE    
    buf[8]=0x04;//LSB Package length--2 BYTES 
    buf[9]=0x02; //Instruction code--1 BYTE    
    buf[10]=id;//BufferID    
    buf[12]=0x07+id;//LSBChecksum--2 BYTES
    global=0;
    UART0_Str1(13);
	CCR=0x01;
	SEC=0; 
    while(global<12&&(SEC!=2));
	CCR=0x00;
	SEC=0;
    if(rec_buff[9]==0x00)
        return 0;
    else 
        return 1;    
} 


unsigned char gen_template(unsigned int pageid)
{ 
    for(j=0;j<15;j++)
        buf[j]=0x00;    
    buf[0]=0xEF; //MSB
    buf[1]=0x01;//LSB Header---2 BYTES
    
    buf[2]=0xff;
    buf[3]=0xff;
    buf[4]=0xff;
    buf[5]=0xff; //LSB DEVICE ADRESS----4 BYTES
    
     
    buf[6]=0x01;//Package identifier--1 BYTE
    
    
    buf[8]=0x06;//LSB Package length--2 BYTES
  
  
    buf[9]=0x06; //Instruction code--1 BYTE  
    
    buf[10]=1;//BufferID    
    
    buf[12]=pageid;//PAGEID---2 BYTES
    
    j=0x0E+pageid;
    buf[13]=j>>8;
    buf[14]=j;//LSBChecksum--2 BYTES  
    global=0;
    UART0_Str1(15);   
 
    while(global<12);
    if(rec_buff[9]==0x00)
        return 0;
    else 
        return 1;   
    
}


unsigned char enroll(unsigned int pageid)
{
      CmdLCD(0x01);
      CmdLCD(0x80);
      Str_lcd("PLACE FINGER  ");
      delay_ms(1000);
      

      ret=collect();     
      //delay_ms(500);
      CmdLCD(0x80);
      Str_lcd("REMOVE FINGER "); 
      delay_ms(500);  
      CmdLCD(0x01);
      CmdLCD(0x80);
      if(ret==0)
      {
        ret=store(1);     
        if(ret==0)
        {
            Str_lcd("PLACE FINGER  ");
            delay_ms(1000);
            ret=collect(); 
            delay_ms(100);
            CmdLCD(0x80);
            Str_lcd("REMOVE FINGER "); 
            delay_ms(500);
            CmdLCD(0x80);
            if(ret==0)
            {  
               ret=store(2);  
               ret=gen_template(pageid); 
               if(ret==0)  
                    return 1;
               else
			   {
			   //CmdLCD(0x80);
            	//Str_lcd("Gen template");
				//delay_ms(1000);
				return 0;
				}
            }
            else
			{
            	CmdLCD(0x80);
            	Str_lcd("Collect issue ");
				delay_ms(1000);
                 return 0;
			}
        } 
        else
            return 0; 
         
      }                    
      else
         return 0;
}


unsigned char delete_fp(unsigned int pageid)
{
    for(j=0;j<16;j++)
        buf[j]=0x00;    
    
    buf[0]=0xEF; //MSB
    buf[1]=0x01;//LSB Header---2 BYTES
    
    buf[2]=0xff;
    buf[3]=0xff;
    buf[4]=0xff;
    buf[5]=0xff; //LSB DEVICE ADRESS----4 BYTES
    
     
    buf[6]=0x01;//Package identifier--1 BYTE
    
    
    buf[8]=0x07;//LSB Package length--2 BYTES
  
  
    buf[9]=0x0C; //Instruction code--1 BYTE  
    
    buf[10]=pageid>>8;//pageID MSB    
    
    buf[11]=pageid;//PAGEID---2 BYTES   
    
    buf[13]=1; //No.OF ID's to be deleted
    
    
    j=0x15+pageid;
    buf[14]=j>>8;
    buf[15]=j;//LSBChecksum--2 BYTES  
    global=0;
    UART0_Str1(16);   
 
    while(global<12);
    if(rec_buff[9]==0x00)
        return 0;  //SUCCESSFULLY DELETED
    else 
        return 1;  //ERROR IN DELETION

} 


unsigned char deleteall_fp(void)
{
    for(j=0;j<12;j++)
        buf[j]=0x00;    
    
    buf[0]=0xEF; //MSB
    buf[1]=0x01;//LSB Header---2 BYTES
    
    buf[2]=0xff;
    buf[3]=0xff;
    buf[4]=0xff;
    buf[5]=0xff; //LSB DEVICE ADRESS----4 BYTES
    
     
    buf[6]=0x01;//Package identifier--1 BYTE
    
    
    buf[8]=0x03;//LSB Package length--2 BYTES
  
  
    buf[9]=0x0D; //Instruction code--1 BYTE  
    
       
    buf[11]=0x11;//LSBC hecksum--2 BYTES  
    global=0;
    UART0_Str1(12);   
 
    while(global<12);
    if(rec_buff[9]==0x00)
        return 0;  //SUCCESSFULLY DELETED ALL
    else 
        return 1;  //ERROR IN DELETION

} 


unsigned int identify_fp(void)       
{    
    for(j=0;j<12;j++)
        buf[j]=0x00;    
    
    buf[0]=0xEF; //MSB
    buf[1]=0x01;//LSB Header---2 BYTES    
    buf[2]=0xff;
    buf[3]=0xff;
    buf[4]=0xff;
    buf[5]=0xff; //LSB DEVICE ADRESS----4 BYTES      
    buf[6]=0x01;//Package identifier--1 BYTE     
    buf[8]=0x08;//LSB Package length--2 BYTES  
    buf[9]=0x04; //Instruction code--1 BYTE     
    buf[10]=0x01;//BUFFER ID    
    buf[12]=0x00;//START PAGE ADDRESS--2 BYTES    
    buf[14]=0x0A;//PAGENUM---2 BYTES
    buf[16]=0x18;//LSBC hecksum--2 BYTES 
		global=0;
    UART0_Str1(17);   
 
    while(global<16);
    if(rec_buff[9]==0x00)
    {
        j=rec_buff[10];
        j=(j<<8)|rec_buff[11];
        return (j+1);  //SUCCESSFULLY IDENTIFIED FP
    }
    else 
        return 0;  //ERROR IN DELETION
}
unsigned int search_fp()  
{  
    CmdLCD(0x01);
    CmdLCD(0x80);
    Str_lcd("PLACE FINGER  ");
    delay_ms(1000);    

    ret=collect();     
    delay_ms(100);
    CmdLCD(0x80);
    Str_lcd("REMOVE FINGER"); 
    delay_ms(500);  
    CmdLCD(0x01);
    CmdLCD(0x80);
    if(ret==0)
    {
        ret=store(1);     
        if(ret==0)
        {
            ret=identify_fp();  
            return(ret);
        }                  
        else
        {
            Str_lcd("FAILED IN STORING");
            delay_ms(2000); 
            return 0;    
        } 
    }   
    else
    {
        Str_lcd("FAIL DENTIFTN");
        delay_ms(2000); 
        return 0; 
    } 
}
