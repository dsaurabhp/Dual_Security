/*#include "dual_Security.h"
s32 password_handle(s32);
u8 rbyte[8]; //;// __attribute__((at(0x40000010)));
u8 kpmLUT[4][4]=
{
	{'1','2','3','/'},
  {'4','5','6','x'},
  {'7','8','9','-'},
  {'C','0','=','+'}
};
u32 c,i,invalid=0;
u32 rNo=0,cNo;
u8 pswd[5];
u8 user[10];
u8 usrpswd[10]={0x00,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80,0x90};
u8 useri2c[10]={0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9};
u32 K=0;
u8 *lcdstr[]={"Enter Password","1)Edit Password","2)Edit Finger","Enter new PSWD","Ent Confm PSWD","pswd not match","PSWD CHG SUC","same prev pswd","Enter USERID","Invalid USERID","1)Enroll USERID"};
u32 EINTSW,EIN=0;
u8 var;
void interrupt_handler(void);
s32 check_user(void)
{
	CmdLCD(0x01);
	Str_lcd(lcdstr[8]);
	CmdLCD(0xc0);
	CCR=0x01;
	SEC=0;
	while((ColScan()) && (SEC!=3) &&(EINTSW!=1));
	if(EINTSW==1)
	{
		EINTSW=0;
		return -1;
			//goto pswdcheck;
	}
	if(SEC==3)
	{
		SEC=0;
		CCR=0x00;
		return -2;
		//goto pswdcheck;
	}//wait for 3 sec if no interrupt
	rNo=Row_Check();
	cNo=Col_Check();
	if((var=i2c_eeprom_randomread(0x50,useri2c[(kpmLUT[rNo][cNo]-48)-1]))=='Y')
	{
		while((READNIBBLE(IOPIN1,COL0)!=15)?1:0);
		return kpmLUT[rNo][cNo]-48;
	}
	while((READNIBBLE(IOPIN1,COL0)!=15)?1:0);
	return 0;
}
s32 password_handle(s32 n)
{
	c=0;
	
	i=0;
	
	CmdLCD(0x01);
	Str_lcd(lcdstr[0]);
	CmdLCD(0xc0);
	while(c!=5)
		{
			CCR=0x01;
			SEC=0;
			//if(EIN==0)
			//{
				while(ColScan()&& (SEC!=3) &&(EINTSW!=1));
				if(EINTSW==1)
				{
					return -1;
				//	goto pswdcheck;
				}
				if(SEC==3)
				{
					SEC=0;
					CCR=0x00;
					return -2;
					//goto pswdcheck;
				}//wait for 3 sec if no interrupt
			//}// check if interruot is occur or not
			if(EIN)											  oiiiiiiiiiii
			{
				while(ColScan()&&SEC!=5);
				if(SEC==5)
				{
					SEC=0;
					EIN=0;
					CCR=0x00;
					//goto pswdcheck;
				}//wait for 5 sec if interrupt occur
			}//check if interrupt is occur or not 						iiiiiiiiiii
			CCR=0x00;
			SEC=0;
			rNo=Row_Check();
			cNo=Col_Check();
			delay_ms(100);
			while((READNIBBLE(IOPIN1,COL0)!=15)?1:0);
			pswd[i]=kpmLUT[rNo][cNo];
			if((pswd[i]=='/') && (i==0))
			{
				//Char_LCD('\0');
				//CmdLCD(0x13);
				c=0;
				i=0;
				//while((READNIBBLE(IOPIN1,COL0)!=15)?1:0);
				continue;
			}//if you pressed del initially
			if((pswd[i]=='/') && (i!=0))
			{
				i-=2;
				c-=2;
				CmdLCD(0x13);
				Char_LCD(32);
				CmdLCD(0x13);
				//while((READNIBBLE(IOPIN1,COL0)!=15)?1:0);
			}// like memmove the cursor
			else
			{
				if(c!=4)
				Char_LCD('*');
			}// print '*' in place of pswd
			if((c==4)&&(pswd[i]=='C'))
			{
				pswd[i]='\0';
			}//in place of 'C' null character
			else if(c==4)
			{
				CmdLCD(0x01);
				Str_lcd("Invalid Size");
				EIN=0;
				EINTSW=0;
				CCR=0x01;
				SEC=0;
				while((SEC!=1)&&(EINTSW!=1));
				if(EINTSW==1)
					return -1;
				//	goto pswdcheck;
				//delay_ms(800);
				//goto pswdcheck;
			}//check pin size valid or not
			c++;
			i++;
			//while((READNIBBLE(IOPIN1,COL0)!=15)?1:0);
		}// input done
		//if(EIN==3)
		//i2c_eeprom_seqread(0x50,0xA0,rbyte,8);
		//else
			i2c_eeprom_seqread(0x50,usrpswd[n-1],rbyte,8);
		if(strcmp((const char*)pswd,(const char*)rbyte)==0)
		{
			//if(EIN==1)
			//{
			//	EIN=2;
				//goto pswdcheck;
			//}
			//if(EIN==2)
			//{
			//	CmdLCD(0x01);
			//	Str_lcd(lcdstr[7]);
			//	EIN=0;
			//	CCR=0x01;
			//	SEC=0;
			//	while((SEC!=1) &&(EINTSW!=1));
			//	if(EINTSW==1)
			//		return -1;
				//	goto pswdcheck;
				//delay_ms(500);
				//goto pswdcheck;
			//}
			//if(EIN==3)
			//{
			//	EIN=0;
				//CmdLCD(0x01);
			//	Str_lcd(lcdstr[6]);
			//	i2c_eeprom_pagewrite(0x50,usrpswd[n-1],(s8*)pswd,8);
			//	CCR=0x01;
			//	SEC=0;
			//	while((SEC!=1) && (EINTSW!=1));
			//	if(EINTSW==1)
			//		return -1;
				//	goto pswdcheck;
				//delay_ms(500);
				//goto pswdcheck;
			//}
			CmdLCD(0x01);
			Str_lcd("Password Correct");
			CmdLCD(0xc0);
			Str_lcd("FNG check");
			invalid=0;
			while((READNIBBLE(IOPIN1,COL0)!=15)?1:0);
			CCR=0x01;
			SEC=0;
			while(SEC!=3 && EINTSW!=1);
			if(EINTSW==1)
				return -1;
			return 1;
		}
		else
		{
			//if(EIN==2)
			//{
			//	EIN=3;
			//	i2c_eeprom_pagewrite(0x50,0xA0,(s8*)pswd,8);
				//goto pswdcheck;
			//}
			//EIN=0;
			//CmdLCD(0xc0);
			CmdLCD(0x01);
			Str_lcd("Invalid Password");
			invalid++;
			CCR=0x01;
			SEC=0;
			while((SEC!=3)&&(EINTSW!=1));
			if(EINTSW==1)
					return -1;
			return 0;
		}
		CCR=0x01;				iiiiiiii
		SEC=0;
		while((SEC!=1)&&(EINTSW!=1));
		if(EINTSW==1)
			return -1;*/
		//	goto pswdcheck;
		//goto pswdcheck;
		//delay_ms(800);
	/*pswdcheck:c=0;
	SEC=0;
		CCR=0x00;
		i=0;
		CmdLCD(0x01);
		if(invalid==3)
		{
			u8 sec=30;
			CmdLCD(0x01);
			Str_lcd("Time Left");
			while(sec && (EINTSW!=1))
			{
				CmdLCD(0xc0);
				Str_lcd("  ");
				CmdLCD(0xc0);
				U32LCD(sec);
				CCR=0x01;
				SEC=0;
				while((SEC!=1)&&(EINTSW!=1));
				if(EINTSW==1)
					goto pswdcheck;
				//delay_ms(500);
				sec--;
			}
			invalid=0;
			goto pswdcheck;
		}
		if(EINTSW==0)
		{
			if(EIN==2)
				Str_lcd(lcdstr[3]);
			else if(EIN==3)
				Str_lcd(lcdstr[4]);
			else
				Str_lcd(lcdstr[0]);
			CmdLCD(0xc0);
		}//if interrupt not occur
		if(EINTSW==1)
		{
			Str_lcd(lcdstr[1]);
			CmdLCD(0xc0);
			Str_lcd(lcdstr[10]);
			EINTSW=0;
			CCR=0x01;
			SEC=0;
			while(ColScan()&&SEC!=5);
			if(SEC==5)
			{
				EIN=0;
				goto pswdcheck;
			}
			rNo=Row_Check();
			cNo=Col_Check();
			if(kpmLUT[rNo][cNo]=='1')
			{
				SEC=0;
				CCR=0x00;
				EIN++;
				//goto start;
			}*/
			/*else if(kpmLUT[rNo][cNo]=='2')
			{
				CCR=0x01;
				SEC=0;
				while(ColScan()&&(SEC!=2) && (EINTSW!=1));
				if(SEC==2)
					goto pswdcheck;
				if(EINTSW==1)
					goto pswdcheck;
				rNo=Row_Check();
				cNo=Col_Check();
				for(K=0;user[K];K++);
				if(K==10)
				{
					CCR=0x01;
					Str_lcd("USER LIMIT 10");
				}
				else
				{
					CmdLCD(0x01);
					i2c_eeprom_bytewrite(0x50,useri2c[K],'Y');
					U32LCD(K);
					Str_lcd(" USERID SUCCES");
				}
				while((READNIBBLE(IOPIN1,COL0)!=15)?1:0);
					while((SEC!=1)&&(EINTSW!=1));
					if(EINTSW==1)
						goto pswdcheck;
			}*/
			/*else
			{
				CmdLCD(0x01);
				Str_lcd("INVALID INPUT");
				CCR=0x01;
				SEC=0;
				while((SEC!=1) && (EINTSW!=1));
				if(EINTSW==1)
					goto pswdcheck;
			}
			while((READNIBBLE(IOPIN1,COL0)!=15)?1:0);
			goto pswdcheck;
		}//if interrupt occur
		while(c!=5)
		{
			if(EIN==0)
			{
				while(ColScan()&&SEC!=3 &&(EINTSW!=1));
				if(EINTSW==1)
				{
					goto pswdcheck;
				}
				if(SEC==3)
				{
					SEC=0;
					CCR=0x00;
					goto pswdcheck;
				}//wait for 3 sec if no interrupt
			}// check if interruot is occur or not
			if(EIN)
			{
				while(ColScan()&&SEC!=5);
				if(SEC==5)
				{
					SEC=0;
					EIN=0;
					CCR=0x00;
					goto pswdcheck;
				}//wait for 5 sec if interrupt occur
			}//check if interrupt is occur or not
			CCR=0x01;
			SEC=0;
			rNo=Row_Check();
			cNo=Col_Check();
			pswd[i]=kpmLUT[rNo][cNo];
			if((pswd[i]=='/') && (i==0))
			{
				while((READNIBBLE(IOPIN1,COL0)!=15)?1:0);
				continue;
			}//if you pressed del initially
			if((pswd[i]=='/') && (i!=0))
			{
				i-=2;
				c-=2;
				CmdLCD(0x13);
				Char_LCD(0);
				CmdLCD(0x13);
			}// like memmove the cursor
			else
			{
				if(c!=4)
				Char_LCD('*');
			}// print '*' in place of pswd
			if((c==4)&&(pswd[i]=='C'))
			{
				pswd[i]='\0';
			}//in place of 'C' null character
			else if(c==4)
			{
				CmdLCD(0x01);
				Str_lcd("Invalid Size");
				EIN=0;
				EINTSW=0;
				CCR=0x01;
				SEC=0;
				while((SEC!=1)&&(EINTSW!=1));
				if(EINTSW==1)
					goto pswdcheck;
				//delay_ms(800);
				goto pswdcheck;
			}//check pin size valid or not
			c++;
			i++;
			while((READNIBBLE(IOPIN1,COL0)!=15)?1:0);
		}// input done
		if(EIN==3)
			i2c_eeprom_seqread(0x50,0xA0,rbyte,8);
		else
			i2c_eeprom_seqread(0x50,0x00,rbyte,8);
		if(strcmp((const char*)pswd,(const char*)rbyte)==0)
		{
			if(EIN==1)
			{
				EIN=2;
				goto pswdcheck;
			}
			if(EIN==2)
			{
				CmdLCD(0x01);
				Str_lcd(lcdstr[7]);
				EIN=0;
				CCR=0x01;
				SEC=0;
				while((SEC!=1) &&(EINTSW!=1));
				if(EINTSW==1)
					goto pswdcheck;
				//delay_ms(500);
				goto pswdcheck;
			}
			if(EIN==3)
			{
				EIN=0;
				CmdLCD(0x01);
				Str_lcd(lcdstr[6]);
				i2c_eeprom_pagewrite(0x50,0x00,(s8*)pswd,8);
				CCR=0x01;
				SEC=0;
				while((SEC!=1) && (EINTSW!=1));
				if(EINTSW==1)
					goto pswdcheck;
				//delay_ms(500);
				goto pswdcheck;
			}
			CmdLCD(0x01);
			Str_lcd("Password Correct");
			CmdLCD(0xc0);
			Str_lcd("FNG check");
			invalid=0;
			while((READNIBBLE(IOPIN1,COL0)!=15)?1:0);
		}
		else
		{
			if(EIN==2)
			{
				EIN=3;
				i2c_eeprom_pagewrite(0x50,0xA0,(s8*)pswd,8);
				goto pswdcheck;
			}
			EIN=0;
			CmdLCD(0xc0);
			Str_lcd("Invalid Password");
			invalid++;
		}
		CCR=0x01;
		SEC=0;
		while((SEC!=1)&&(EINTSW!=1));
		if(EINTSW==1)
			goto pswdcheck;
		goto pswdcheck;
		//delay_ms(800);        iiiiiiiiiiii
}
s8 n=0;
void change_pswd(u32 id)
{
	n=password_handle(id);
	CmdLCD(0x01);
	if(n==-1)
	{
			Str_lcd("handle Interrupt");
			interrupt_handler();
	}
	if(n==1)
	{
		repeat:
		CmdLCD(0x01);
		if(EIN==0)
			Str_lcd(lcdstr[3]);
		else
			Str_lcd(lcdstr[4]);
		c=0;
		i=0;
		CmdLCD(0xc0);
		while(c!=5)
		{
			CCR=0x01;
			SEC=0;
			while(ColScan()&& (SEC!=5) &&(EINTSW!=1));
			if(EINTSW==1)
			{
				SEC=0;
				CCR=0x00;
				return;
			}
			if(SEC==5)
			{
				SEC=0;
				CCR=0x00;
				return;
			}
			CCR=0x00;
			SEC=0;
			rNo=Row_Check();
			cNo=Col_Check();
			delay_ms(100);
			while((READNIBBLE(IOPIN1,COL0)!=15)?1:0);
			pswd[i]=kpmLUT[rNo][cNo];
			if((pswd[i]=='/') && (i==0))
			{
				c=0;
				i=0;
				continue;
			}//if you pressed del initially
			if((pswd[i]=='/') && (i!=0))
			{
				i-=2;
				c-=2;
				CmdLCD(0x13);
				Char_LCD(32);
				CmdLCD(0x13);
			}// like memmove the cursor
			else
			{
				if(c!=4)
				Char_LCD('*');
			}// print '*' in place of pswd
			if((c==4)&&(pswd[i]=='C'))
			{
				pswd[i]='\0';
			}//in place of 'C' null character
			else if(c==4)
			{
				CmdLCD(0x01);
				Str_lcd("Invalid Size");
				EIN=0;
				EINTSW=0;
				CCR=0x01;
				SEC=0;
				while((SEC!=1)&&(EINTSW!=1));
				if(EINTSW==1)
					return ;
				return;
			}//check pin size valid or not
			c++;
			i++;
			//while((READNIBBLE(IOPIN1,COL0)!=15)?1:0);
		}// input done
		if(EIN==1)
			i2c_eeprom_seqread(0x50,0xA0,rbyte,8);
		else
			i2c_eeprom_seqread(0x50,usrpswd[id-1],rbyte,8);
		if(strcmp((const char*)pswd,(const char*)rbyte)==0)
		{
			if(EIN==1)
			{
				CmdLCD(0x01);
				Str_lcd(lcdstr[6]);
				i2c_eeprom_pagewrite(0x50,usrpswd[id-1],(s8*)pswd,8);
			}
			else
			{
				CmdLCD(0x01);
				Str_lcd(lcdstr[7]);
			}
		}
		else
		{
			if(EIN==1)
			{
				CmdLCD(0x01);
				Str_lcd("Wrong PSWD");
			}
			else
			{
				EIN=1;
				i2c_eeprom_pagewrite(0x50,0xA0,(s8*)pswd,8);
				goto repeat;
			}
		}
	}
	CCR=0x01;
	SEC=0;
	while(SEC!=2);
	SEC=0;
	CCR=0x00;
	return;
}
void interrupt_handler(void)
{
	CmdLCD(0x01);
	Str_lcd(lcdstr[10]);
	CmdLCD(0xc0);
	Str_lcd(lcdstr[1]);
	CCR=0x01;
	SEC=0;
	while(ColScan()&&SEC!=5);
	if(SEC==5)
	{
		EINTSW=0;
		CCR=0x00;
		SEC=0;
		return;
	}
	rNo=Row_Check();
	cNo=Col_Check();
	while((READNIBBLE(IOPIN1,COL0)!=15)?1:0);
	if(kpmLUT[rNo][cNo]=='1')
	{
		for(K=0;K<10;K++)
		{
			if((var=i2c_eeprom_randomread(0x50,useri2c[K]))=='Y');
			else
			{
				CmdLCD(0x01);
				i2c_eeprom_bytewrite(0x50,useri2c[K],'Y');
				i2c_eeprom_pagewrite(0x50,usrpswd[K],"3690",8);
				U32LCD(K);
				Str_lcd(" USERID SUCCES");
				break;
			}
		}
		if(K==10)
		{
			CmdLCD(0x01);
			Str_lcd("USER LIMIT 10");
		}
		CCR=0x01;
		SEC=0;
		while(SEC!=2);
		CCR=0x00;
		SEC=0;
	}
	else if(kpmLUT[rNo][cNo]=='2')
	{
		CmdLCD(0x01);
		Str_lcd("En USERID CNG PSWD");
		CmdLCD(0xc0);
		CCR=0x01;
		SEC=0;
		while(ColScan()&&SEC!=5);
		if(SEC==5)
		{
			EIN=0;
			return;
		}
		rNo=Row_Check();
		cNo=Col_Check();
		while((READNIBBLE(IOPIN1,COL0)!=15)?1:0);
		if((var=i2c_eeprom_randomread(0x50,useri2c[kpmLUT[rNo][cNo]-48]))=='Y')
		{
			//EIN=1;
			change_pswd(kpmLUT[rNo][cNo]-48);
		}
		else
		{
			return;
		}
		CCR=0x00;
		SEC=0;
	}
	else
	{
		CmdLCD(0x01);
		Str_lcd("Invalid Choice");
		CCR=0x01;
		SEC=0;
		while(SEC!=2);
		CCR=0x00;
		SEC=0;
	}
}
int main()
{
	Init_LCD();
	Init_ExtIns();
	Init_KPM();
	i2c_eeprom_pagewrite(0x50,0x00,"1234",8);	iiii
	i2c_eeprom_seqread(0x50,0x00,rbyte,8);
	Str_lcd(rbyte);
	delay_ms(1000);
	i2c_eeprom_bytewrite(0x50,0xB0,'Y');
	i2c_eeprom_bytewrite(0x50,0xB1,'N');
	i2c_eeprom_bytewrite(0x50,0xB2,'N');
	i2c_eeprom_bytewrite(0x50,0xB3,'N');
	i2c_eeprom_bytewrite(0x50,0xB4,'N');
	i2c_eeprom_bytewrite(0x50,0xB5,'N');
	i2c_eeprom_bytewrite(0x50,0xB6,'N');
	i2c_eeprom_bytewrite(0x50,0xB7,'N');
	i2c_eeprom_bytewrite(0x50,0xB8,'N');
	i2c_eeprom_bytewrite(0x50,0xB9,'N');
	
	CmdLCD(0xc0);
	for(i=0;i<10;i++)
	{
		var=i2c_eeprom_randomread(0x50,useri2c[i]);
		Char_LCD(var);
	}	
	while(1);  iiiiiiiiiiiii
	//i2c_eeprom_pagewrite(0x50,0x10,"3690",8);
	//i2c_eeprom_bytewrite(0x50,0xB1,'Y');
	UART0_init();
	PREINT=PREINT_VAL;
  PREFRAC=PREFRAC_VAL;
	while(1)
	{
		CCR=0x00;
		SEC=0;
		n=check_user();
		if(n==-1)
		{
			CCR=0x01;
			SEC=0;
			CmdLCD(0x01);
			Str_lcd("handle Interrupt");
			EINTSW=0;
			interrupt_handler();
			continue;
			//while((SEC!=3)&&(EINTSW!=1));
		}
		if(n==-2)
			continue;
		if(n>0)
		{
			CmdLCD(0x01);
			U32LCD(n);
			CmdLCD(0xc0);
			//Str_lcd("check_pswd");
			n=password_handle(n);
			if(n==-1)
			{
				Str_lcd("handle Interrupt");
				interrupt_handler();
			}
			if(n==-2)
				continue;
			if(n==0)
				Str_lcd(lcdstr[5]);
			continue;
		}
		if(n==0)
		{
			CCR=0x01;
			SEC=0;
			CmdLCD(0x01);
			Str_lcd(lcdstr[9]);
			while((SEC!=3)&&(EINTSW!=1));
		}
		//password_handle();
		//interrupt_handler();
	}
}
*/
