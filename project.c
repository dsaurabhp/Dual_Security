#include "project.h"
u8 chng_fng=0,dsp=0;
void change_FNG(u32 n)
{
	/*if(delete_fp(n)==0)
	{
		CmdLCD(0X01);
		U32LCD(n);
		Str_lcd("Del Succes");
		delay_ms(1000);
	}  */
	if((dsp=enroll(n-1))==1)
	{
		CmdLCD(0x01);
//		U32LCD(n-1);
		//U32LCD(K);
		//i2c_eeprom_bytewrite(0x50,useri2c[n-1],'Y');
		//i2c_eeprom_pagewrite(0x50,usrpswd[n-1],"3690",4);
		//Str_lcd("Your Id: ");
//		U32LCD(n);
//		CmdLCD(0xc0);
		Str_lcd("FNG CHNG SUCCES");
		delay_ms(1000);
	}
	else
	{
		CmdLCD(0x01);
		Str_lcd("ERROR In ENROLL");
		CmdLCD(0xc0);
		Str_lcd("try again");
		delay_ms(1000);
	}
}

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
	}
	if(SEC==3)
	{
		SEC=0;
		CCR=0x00;
		return -2;
	}//wait for 3 sec if no interrupt
	rNo=Row_Check();
	cNo=Col_Check();
	if((i2c_eeprom_randomread(0x50,useri2c[(kpmLUT[rNo][cNo]-48)-1]))=='Y')
	{
		delay_ms(100);
		while((READNIBBLE(IOPIN1,COL0)!=15)?1:0);
		return kpmLUT[rNo][cNo]-48;
	}
	delay_ms(100);
	while((READNIBBLE(IOPIN1,COL0)!=15)?1:0);
	return 0;
}
s32 password_handle(s32 n,u8 D)
{
	cnt=0;
	K=0;
	CmdLCD(0x01);
	Str_lcd(lcdstr[0]);
	CmdLCD(0xc0);
	while(cnt!=5)
	{
		CCR=0x01;
		SEC=0;
		while(ColScan()&& (SEC!=3) &&(EINTSW!=1));
		if(EINTSW==1)
		{
			return -1;
		}
		if(SEC==3)
		{
			SEC=0;
			CCR=0x00;
			return -2;
		}//wait for 3 sec if no interrupt
		CCR=0x00;
		SEC=0;
		rNo=Row_Check();
		cNo=Col_Check();
		delay_ms(100);
		while((READNIBBLE(IOPIN1,COL0)!=15)?1:0);
		pswd[K]=kpmLUT[rNo][cNo];
		if((pswd[K]=='/') && (K==0))
		{
			cnt=0;
			K=0;
			continue;
		}//if you pressed del initially
		if((pswd[K]=='/') && (K!=0))
		{
			K-=2;
			cnt-=2;
			CmdLCD(0x13);
			Char_LCD(32);
			CmdLCD(0x13);
		}// like memmove the cursor
		else
		{
			if(cnt!=4)
			Char_LCD('*');
		}// print '*' in place of pswd
		if((cnt==4)&&(pswd[K]=='C'))
		{
			pswd[K]='\0';
		}//in place of 'C' null character
		else if(cnt==4)
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
		}//check pin size valid or not
		cnt++;
		K++;
	}// input done
	i2c_eeprom_seqread(0x50,usrpswd[n-1],rbyte,4);
	//CmdLCD(0x01);
	//Str_lcd(rbyte);
	//delay_ms(1000);
	if(strcmp((const char*)pswd,(const char*)rbyte)==0)
	{
		CmdLCD(0x01);
		Str_lcd("Password Correct");
		CmdLCD(0xc0);
		Str_lcd("FNG check");
		//invalid=0;
		while((READNIBBLE(IOPIN1,COL0)!=15)?1:0);
		if((K=search_fp())==n)
		{
			CmdLCD(0x01);
			Str_lcd("match succ");
			//Str_lcd("USER FOUND");
			delay_ms(1000);
			if(D==0)
			{
				for(K=0;K<4;K++)
				{
					IOSET0|=1<<19+K;
					delay_ms(500);
					IOCLR0|=1<<19+K;
					delay_ms(500);
				}
			}
			else
			{
				delay_ms(500);
			}
			//while(SEC!=3 && EINTSW!=1);
				return 1;
		}
		/*if((K=search_fp())!=0)
		{
			CmdLCD(0x01);
			Str_lcd("USER FOUND");
			CmdLCD(0xc0);
			U32LCD(K);
			//CCR=0x01;
			//SEC=0;
			if(D==0)
			{
				for(K=0;K<4;K++)
				{
					IOSET0|=1<<19+K;
					delay_ms(500);
					IOCLR0|=1<<19+K;
					delay_ms(500);
				}
			}
			else
			{
				delay_ms(500);
			}
			//while(SEC!=3 && EINTSW!=1);
				return 1;
		} */
		else
		{
			CmdLCD(0x01);
//			U32LCD(K);
			Str_lcd("FNG Invalid");
		}
		CCR=0x01;
		SEC=0;
		while(SEC!=3 && EINTSW!=1);
		if(EINTSW==1)
			return -1;
		return 0;
	}
	else
	{
		CmdLCD(0x01);
		Str_lcd("Invalid Password");
		//invalid++;
		CCR=0x01;
		SEC=0;
		while((SEC!=3)&&(EINTSW!=1));
		if(EINTSW==1)
				return -1;
		return 0;
	}
}
void change_pswd(u32 id)
{
	n=password_handle(id,1);
	if(n==0)
		return;
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
		cnt=0;
		K=0;
		CmdLCD(0xc0);
		while(cnt!=5)
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
			pswd[K]=kpmLUT[rNo][cNo];
			if((pswd[K]=='/') && (K==0))
			{
				cnt=0;
				K=0;
				continue;
			}//if you pressed del initially
			if((pswd[K]=='/') && (K!=0))
			{
				K-=2;
				cnt-=2;
				CmdLCD(0x13);
				Char_LCD(32);
				CmdLCD(0x13);
			}// like memmove the cursor
			else
			{
				if(cnt!=4)
				Char_LCD('*');
			}// print '*' in place of pswd
			if((cnt==4)&&(pswd[K]=='C'))
			{
				pswd[K]='\0';
			}//in place of 'C' null character
			else if(cnt==4)
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
			cnt++;
			K++;
			//while((READNIBBLE(IOPIN1,COL0)!=15)?1:0);
		}// input done
		if(EIN==1)
			i2c_eeprom_seqread(0x50,0xA0,rbyte,4);
		else
			i2c_eeprom_seqread(0x50,usrpswd[id-1],rbyte,4);
		if(strcmp((const char*)pswd,(const char*)rbyte)==0)
		{
			if(EIN==1)
			{
				CmdLCD(0x01);
				Str_lcd(lcdstr[6]);
				i2c_eeprom_pagewrite(0x50,usrpswd[id-1],(s8*)pswd,4);
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
				i2c_eeprom_pagewrite(0x50,0xA0,(s8*)pswd,4);
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
	Str_lcd(lcdstr[11]);
	CmdLCD(0xc0);
	Str_lcd(lcdstr[1]);
	Str_lcd(lcdstr[12]);
	//CmdLCD(0xc0);
	CCR=0x01;
	SEC=0;
	EINTSW=0;
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
	delay_ms(100);
	while((READNIBBLE(IOPIN1,COL0)!=15)?1:0);
	if(kpmLUT[rNo][cNo]=='1')
	{
		for(K=0;K<9;K++)
		{
			if((i2c_eeprom_randomread(0x50,useri2c[K]))=='Y');
			else break;
		}
		if(K==9)
		{
			CmdLCD(0x01);
			Str_lcd("USER LIMIT 10");
		}
		else
		{
			CmdLCD(0x01);
			Str_lcd("FINGER PRINT ");
			delay_ms(2000);
//			U32LCD(K);
			if((enroll(K))==1)
			{
					CmdLCD(0x01);
					//U32LCD(dsp);
					i2c_eeprom_bytewrite(0x50,useri2c[K],'Y');
					i2c_eeprom_pagewrite(0x50,usrpswd[K],"3690",4);
					Str_lcd("Your Id: ");
					U32LCD(K+1);
					CmdLCD(0xc0);
					Str_lcd(" USERID SUCCES");
			}
			else
			{
					CmdLCD(0x01);
					Str_lcd("ERROR In ENROLL");
					CmdLCD(0xc0);
					Str_lcd("try again");
			}
		}
		EINTSW=0;
		CCR=0x01;
		SEC=0;
		while(SEC!=2);
		CCR=0x00;
		SEC=0;
	}
	else if(kpmLUT[rNo][cNo]=='2')
	{
		CmdLCD(0x01);
		Str_lcd("1.EDIT PSWD");
		CmdLCD(0xc0);
		Str_lcd("2.EDIT FNG");
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
		delay_ms(100);
		while((READNIBBLE(IOPIN1,COL0)!=15)?1:0);
		if(kpmLUT[rNo][cNo]=='1')
		{
			CmdLCD(0x01);
			Str_lcd("Enter USERID");
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
			delay_ms(100);
			while((READNIBBLE(IOPIN1,COL0)!=15)?1:0);	
			if((i2c_eeprom_randomread(0x50,useri2c[(kpmLUT[rNo][cNo]-48)-1]))=='Y')
			{
				//EIN=1;
				change_pswd(kpmLUT[rNo][cNo]-48);
			}
			else
			{
				CmdLCD(0x01);
				Str_lcd("Invalid User ID");
				delay_ms(1000);
			}
		}
		else if(kpmLUT[rNo][cNo]=='2')
		{
			CmdLCD(0x01);
			Str_lcd("Enter USERID");
			CmdLCD(0xc0);
			//Str_lcd("2.EDIT FNG");
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
			delay_ms(100);
			while((READNIBBLE(IOPIN1,COL0)!=15)?1:0);
			chng_fng=kpmLUT[rNo][cNo]-48;	
			if((i2c_eeprom_randomread(0x50,useri2c[(kpmLUT[rNo][cNo]-48)-1]))=='Y')
			{
				//EIN=1;
				n=password_handle((kpmLUT[rNo][cNo]-48),1);
				if(n==1)
					change_FNG(chng_fng);
			}
			else
			{
				CmdLCD(0x01);
				Str_lcd("Invalid User ID");
				delay_ms(1000);
			}
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
	else if(kpmLUT[rNo][cNo]=='3')
	{
		CmdLCD(0x01);
		Str_lcd("En USERID To DEL");
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
		delay_ms(100);
		while((READNIBBLE(IOPIN1,COL0)!=15)?1:0);
		n=(kpmLUT[rNo][cNo]-48);
		Del=kpmLUT[rNo][cNo]-48-1;
		if((i2c_eeprom_randomread(0x50,useri2c[n-1]))=='Y')
		{
			n=password_handle(n,1);
			if(n==1)
			{
				n=kpmLUT[rNo][cNo]-48;
				
				if(delete_fp(n)==0)
				{
					i2c_eeprom_bytewrite(0x50,useri2c[Del],'N');
					//i2c_eeprom_bytewrite(0x50,0xB0,'N');
					CmdLCD(0x01);
					//U32LCD(Del);
					Str_lcd("Succesfully Del");
					CmdLCD(0xc0);
					//U32LCD(n);
					delay_ms(1000);
				}
				else
				{
					CmdLCD(0x01);
					Str_lcd("Not Del USER ID");
					CmdLCD(0xc0);
					U32LCD(n);
					delay_ms(1000);
				}
			}
		}
		else
		{
			CmdLCD(0x01);
			Str_lcd("Invalid User ID");
			delay_ms(1000);
		}
	}
	else if(kpmLUT[rNo][cNo]=='4')
	{
		CmdLCD(0x01);
		Str_lcd("En USERID");
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
		delay_ms(100);
		while((READNIBBLE(IOPIN1,COL0)!=15)?1:0);
		n=kpmLUT[rNo][cNo]-48;
		if((i2c_eeprom_randomread(0x50,useri2c[kpmLUT[rNo][cNo]-48]-1))=='Y')
		{
			n=password_handle(n,1);
			if(n==1)
			{
				if(deleteall_fp() == 0x00)
				{
					CmdLCD(0x01);
					Str_lcd("Del Success");
					i2c_eeprom_bytewrite(0x50,0xB0,'N');
					i2c_eeprom_bytewrite(0x50,0xB1,'N');
					i2c_eeprom_bytewrite(0x50,0xB2,'N');
					i2c_eeprom_bytewrite(0x50,0xB3,'N');
					i2c_eeprom_bytewrite(0x50,0xB4,'N');
					i2c_eeprom_bytewrite(0x50,0xB5,'N');
					i2c_eeprom_bytewrite(0x50,0xB6,'N');
					i2c_eeprom_bytewrite(0x50,0xB7,'N');
					i2c_eeprom_bytewrite(0x50,0xB8,'N');
					i2c_eeprom_bytewrite(0x50,0xB9,'N');
					delay_ms(1000);
				}
				else
				{
					CmdLCD(0x01);
					Str_lcd("Fail To Del");
					delay_ms(1000);
				}
			}
		}
		else
		{
			CmdLCD(0x01);
			Str_lcd("You can't Del");
			delay_ms(1000);
		}
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
void Dual_Security_Init(void)
{
	IODIR0|=0xf<<19;
  	Init_LCD();
	Init_ExtIns();
	Init_KPM();
	InitUART0();
	PREINT=PREINT_VAL;
	PREFRAC=PREFRAC_VAL;
}
void Dual_security_loop(void)
{
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
			}
			if(n==-2)
				continue;
			if(n>0)
			{
				CmdLCD(0x01);
				U32LCD(n);
				CmdLCD(0xc0);
				n=password_handle(n,0);
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
		}
}

/*
int main()
{
//	unsigned int pageid; 
   	IODIR0|=0xf<<19;
    Init_LCD();
    InitUART0();
    CmdLCD(0x01);
    CmdLCD(0x80);
    Str_lcd("FINGER PRINT ");

    delay_ms(2000);
	for(K=1;K<11;K++)
	{
		if(enroll(K)==1)
		{
			CmdLCD(0x01);
			Str_lcd("Success");
		}
		else
		{
			CmdLCD(0x01);
			Str_lcd("ERROR");
		}
		delay_ms(1000);
	}
	while(1);
	while(1)
	{
		if((K=search_fp()-1)==1)
		{
			Str_lcd("UPTO HERE FINE");
			CmdLCD(0xc0);
			U32LCD(K);
			delay_ms(3000);
		}
		else
		{
			Str_lcd("NO FG FOUND");
			CmdLCD(0xc0);
			U32LCD(K);
			delay_ms(3000);
		}
	}
	Init_LCD();
	Init_ExtIns();
	Init_KPM();
	InitUART0();
	i2c_eeprom_pagewrite(0x50,0x00,"1234",8);
	i2c_eeprom_seqread(0x50,0x00,rbyte,8);
	Str_lcd(rbyte);
	delay_ms(1000);
	i2c_eeprom_bytewrite(0x50,0xB0,'N');
	i2c_eeprom_bytewrite(0x50,0xB1,'N');
	i2c_eeprom_bytewrite(0x50,0xB2,'N');
	i2c_eeprom_bytewrite(0x50,0xB3,'N');
	i2c_eeprom_bytewrite(0x50,0xB4,'N');
	i2c_eeprom_bytewrite(0x50,0xB5,'N');
	i2c_eeprom_bytewrite(0x50,0xB6,'N');
	i2c_eeprom_bytewrite(0x50,0xB7,'N');
	i2c_eeprom_bytewrite(0x50,0xB8,'N');
	i2c_eeprom_bytewrite(0x50,0xB9,'N');
	
	
	for(K=0;K<10;K++)
	{
		var=i2c_eeprom_randomread(0x50,useri2c[K]);
		Char_LCD(var);
	}
	CmdLCD(0xc0);	
	while(1);
	//i2c_eeprom_pagewrite(0x50,0x10,"3690",8);
	//i2c_eeprom_bytewrite(0x50,0xB1,'Y');
	//UART0_init();
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
		}
		if(n==-2)
			continue;
		if(n>0)
		{
			CmdLCD(0x01);
			U32LCD(n);
			CmdLCD(0xc0);
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
	}
}

*/
