#include<lpc17xx.h>
#include<string.h>

char val=0;
char gsm_msg[200];


void delayMS(unsigned int milliseconds);
void delayUS(unsigned int microseconds);
unsigned int stopTimer0(void);
void startTimer0(void);
void initTimer0(void);

#define TRIG1 (5<<28) //P4.28 
#define ECHO1 (5<<29) //P4.29
int echoTime1=0;
float distance11=0; 
int sensor(void);

char keypad[4][4]={'1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'};
char key_val;
char key_val1;
char keyread(void);

void center(char *str);
void lcdwrt(char y);
void delay(unsigned long int x);
void dispint(void);
void disp(char *mesg);
char cmd[]={0x38,0x0e,0x06,0x01,0x80};

char msg1[]="WELCOME";
char msg2[]="Vendtech";
char msg3[]="CHOOSE the item";	
char msg4[]="selected:";
char msg5[]="AMOUNT IS 50";
char msg6[]="AMOUNT IS 100";
char msg7[]="AMOUNT IS 150";
char msg8[]="AMOUNT IS 200";
char msg9[]="PAYMENT";
char msg10[]="SUCCESSFUL";
char msg11[]="INVALID";
char msg12[]="TRY AGAIN";
char msg13[]="EXITING";
char msg14[]="done :)";
char msg15[]="PAYMENT OPTION";
char msg16[]="0 : RFID";
char msg17[]="1 : PAYTM";
char msg18[]=" SCAN the TAG";
char msg19[]="Pay online";
char msg20[]="Dispensing";
char msg21[]="PRESS 0 or 1";
char msg22[]="SELECT THE"; 

void uart0_init(void);
void uart1_init(void);

char rfid_val[12];
char val_050[12]="5400C3746C8F";
char val_100[12]="5400C3961110";
char val_150[12]="5400C396D9D8";
char val_200[12]="5400C3753CDE";

int payment(void);

void motor(void);

int main()
{
	unsigned int count;
	SystemInit();
	
	//uarts
	LPC_PINCON->PINSEL0|=0x00000050;
	LPC_PINCON->PINSEL4|=0x0000000A;
	
	//lcd
	LPC_GPIO0->FIOMASKL=0Xf00f;
	LPC_GPIO0->FIOMASK2=0X8F;
	LPC_GPIO0->FIODIRL=0XFFFF;   
	LPC_GPIO0->FIODIR2=0XFF;
	
	//motor
	LPC_GPIO2->FIOMASK0=0X00;
	LPC_GPIO2->FIODIR0=0XFF;
	LPC_GPIO2->FIOMASK1=0Xc3;
	LPC_GPIO2->FIODIR1=0XFF;
	LPC_GPIO2->FIOPINL=0X00;
	
	//keypad
	LPC_GPIO1->FIOMASKH=0Xf00f;
	LPC_GPIO1->FIODIRH=0XFF0F;

	//ultrasonic sensor
	LPC_GPIO4->FIOMASK3=0x00;
  LPC_GPIO4->FIODIR |= TRIG1;    //Set P0.21(TRIG) as output 
  LPC_GPIO4->FIODIR &= ~(ECHO1); //Set P0.22(ECHO) as input (explicitly) 
  LPC_GPIO4->FIOCLR |= TRIG1;    //Set P0.21 LOW initially 

while(1)
{
	inv:LPC_GPIO2->FIOPINL=0X00;
	
	uart0_init();
	uart1_init();
	
	dispint();
	center(msg1);
	disp(msg1);
	LPC_GPIO0->FIOCLR2=0X10;
	lcdwrt(0xc0);
	center(msg2);
	disp(msg2);
	delay(0xfffffC);
	
	LPC_GPIO0->FIOCLR2=0X10;
	lcdwrt(0x01);
	center(msg3);
	disp(msg3);
	delay(0xfffffC);
	
	//LPC_GPIO0->FIOCLR2=0X10;
	//lcdwrt(0xc0);
	key_val=keyread();
	
	LPC_GPIO0->FIOCLR2=0X10;
	lcdwrt(0x01);
	center(msg4);
	disp(msg4);
	lcdwrt(key_val);
	delay(0xfffffC);
	
	LPC_GPIO0->FIOCLR2=0X10;
	lcdwrt(0x01);
	if(key_val=='0'  || key_val=='4' )
	{
		center(msg5);
		disp(msg5);
	}
	else if(key_val=='1' || key_val=='5')
	{
		center(msg6);
		disp(msg6);
	}
	else if(key_val=='2' || key_val=='6')
	{
		center(msg7);
		disp(msg7);
	}
	else if(key_val=='3' || key_val=='7')
	{
		center(msg8);
		disp(msg8);
	}
	
	else
		{
		center(msg11);
		disp(msg11);
		delay(0xffffff);
		
		dispint();
		center(msg13);
		disp(msg13);
		delay(0xffffff);
		
		goto inv;
	  }
	delay(0xffffff);
	
	LPC_GPIO0->FIOCLR2=0X10;
	lcdwrt(0x01);
	center(msg22);
	disp(msg22);
	LPC_GPIO0->FIOCLR2=0X10;
	lcdwrt(0x01);
	center(msg15);
	disp(msg15);
	delay(0xffffff);
	
	LPC_GPIO0->FIOCLR2=0X10;
	lcdwrt(0x01);
	center(msg16);
	disp(msg16);
	LPC_GPIO0->FIOCLR2=0X10;
	lcdwrt(0xc0);
	center(msg17);
	disp(msg17);
	delay(0xffffff);
	
	count=0;
	up:	key_val1=keyread();
	LPC_GPIO0->FIOCLR2=0X10;
	lcdwrt(0x01);
	center(msg4);
	disp(msg4);
	lcdwrt(key_val1);
	delay(0xfffffC);
	
	if(key_val1=='0' || key_val1=='1')
		payment();
	else
	{
		if(count==0)
		{
			dispint();
			center(msg11);
			disp(msg11);
			LPC_GPIO0->FIOCLR2=0X10;
			lcdwrt(0xc0);
			center(msg21);
			disp(msg21);
			delay(0xffffff);
		}
		count++;
		//dispint();
		if (count < 2)
			goto up;
		else
		{
			dispint();
			center(msg13);
			disp(msg13);
			delay(0xffffff);			
		}
	}
}
}



int payment()
{
	unsigned int i,j,k;
	unsigned int counter;
	uart0_init();
	uart1_init();
	
	LPC_GPIO0->FIOCLR2=0X10;
	lcdwrt(0x01);
	
	if(key_val1=='0')//rfid
	{
		center(msg18);
		disp(msg18);
		for(i=0;i<3;i++)
		{
			uart1_init();
		
		  for(k=0;k<12;k++)
		  {
		  		counter=0;
				while(1)
				{
					if((LPC_UART1->LSR & 0x01)==0x01)
						break;
					if(counter>=0x1000aff)
						goto exit;
					counter++;
				 }
				rfid_val[k]=LPC_UART1->RBR;
				LPC_UART1->THR=rfid_val[k];
				while((LPC_UART1->LSR&0X20)!=0X20);
				delay(5000);
		  }
		
			if(key_val=='0' || key_val=='4')
			{
				for(j=0;j<12;j++)
				{
					if(rfid_val[j] != val_050[j])
					 break;
					if(j==11)
					 goto success;
				}
			}
			
			else if(key_val=='1' || key_val=='5')
		  {
				for(j=0;j<12;j++)
			  {
					if(rfid_val[j] != val_100[j])
						break;
					if(j==11)
						goto success;
				}
		  }
			
			else if(key_val=='2' || key_val=='6')
			{
			 for(j=0;j<12;j++)
			 {
				if(rfid_val[j] != val_150[j])
					break;
				if(j==11)
					goto success;
			 }
		  }
			else if(key_val=='3' || key_val=='7')
		  {
				for(j=0;j<12;j++)
				{
					if(rfid_val[j] != val_200[j])
						break;
					if(j==11)
					 goto success;
				}
		  }
			else
				goto exit1;
		 if(i==2)
			goto exit;
     
     LPC_GPIO0->FIOCLR2=0X10;
		 lcdwrt(0x01);
		 center(msg11);
		 disp(msg11);
		 LPC_GPIO0->FIOCLR2=0X10;
		 lcdwrt(0xc0);
		 center(msg12);
		 disp(msg12);
		 delay(0xfffffc);	
	 }
   } 	
	 else if(key_val1=='1') // gsm
	 {
		
	  unsigned char cmd1[]="AT\r\n";
	  unsigned char cmd2[]="AT+CREG?\r\n";
	  unsigned char cmd3[]="AT+CMGF=1\r\n";
	  unsigned char cmd4[]="AT+CNMI=1,2,0,0,0\r\n";
	  unsigned int i;
	  unsigned char val;
	
		center(msg19);
		disp(msg19);
		for(i=0;cmd1[i]!='\0';i++)
		{
			LPC_UART0->THR=cmd1[i];
			while((LPC_UART0->LSR & 0x20)!=0x20);
		}
		delay(500000);
		
		for(i=0;cmd2[i]!='\0';i++)
		{
			LPC_UART0->THR=cmd2[i];
			while((LPC_UART0->LSR & 0x20)!=0x20);
			
		}
		delay(500000);
		
		for(i=0;cmd3[i]!='\0';i++)
		{
			LPC_UART0->THR=cmd3[i];
			while((LPC_UART0->LSR & 0x20)!=0x20);	
		}
		delay(500000);
		
		for(i=0;cmd4[i]!='\0';i++)
		{
			LPC_UART0->THR=cmd4[i];
			while((LPC_UART0->LSR & 0x20)!=0x20);
		}
		delay(500000);
			
		for(i=0;i<50;i++)
		{
			counter=0;
			while(1)
			{
				if((LPC_UART0->LSR & 0x01)==0x01)
					break;
				if(counter>0x1000fff)
					goto exit;
				counter++;
			}
			val=LPC_UART0->RBR;
			LPC_UART1->THR=val;
			while((LPC_UART1->LSR & 0x20)!=0x20);
		}
		
		for(i=50;i<162;i++)
		{
			while((LPC_UART0->LSR & 0x01)!=0x01);
			gsm_msg[i-50]=LPC_UART0->RBR;
			LPC_UART1->THR=gsm_msg[i-50];
			while((LPC_UART1->LSR & 0x20)!=0x20);
		}
		gsm_msg[i]='\0';
		delay(0x500000);
		
	
			dispint();
		LPC_GPIO0->FIOSET2=0X10;
			lcdwrt(gsm_msg[6]);
			lcdwrt(gsm_msg[7]);
		  lcdwrt(gsm_msg[8]);
		  lcdwrt(gsm_msg[10]);
		  delay(0xFFFf);
		
		
		if(key_val=='0' || key_val=='4')
			{
				if(gsm_msg[6]=='0' && gsm_msg[7]=='5' && gsm_msg[8]=='0' && gsm_msg[10]=='t')
					goto success;
				else goto exit;
			}
		else if(key_val=='1' || key_val=='5')
			{
				if(gsm_msg[6]=='1' && gsm_msg[7]=='0' && gsm_msg[8]=='0' && gsm_msg[10]=='t')
					goto success;
				else goto exit;
			}
		else if(key_val=='2' || key_val=='6')
			{
				if(gsm_msg[6]=='1' && gsm_msg[7]=='0' && gsm_msg[8]=='0' && gsm_msg[10]=='t')
					goto success;
				else goto exit;
			}
		else if(key_val=='3' || key_val=='7')
			{
				if(gsm_msg[6]=='1' && gsm_msg[7]=='0' && gsm_msg[8]=='0' && gsm_msg[10]=='t')
					goto success;
				else goto exit;
			}
		else
			goto exit1;
		}
success:LPC_GPIO0->FIOCLR2=0X10;
				lcdwrt(0x01);
				center(msg9);
				disp(msg9);
				LPC_GPIO0->FIOCLR2=0X10;
				lcdwrt(0xc0);
				center(msg10);
				disp(msg10);
				delay(0xfffffa);
					
		    motor();
				//goto exit1;
		exit:LPC_GPIO0->FIOCLR2=0X10;
				 lcdwrt(0x01);
				 center(msg13);
				 disp(msg13);
				 delay(0xffffff);
		exit1: return 0;
}



void uart0_init()
{
	LPC_UART0->LCR=0X83;
	LPC_UART0->DLM=0X00;
	LPC_UART0->DLL=0X75;
	LPC_UART0->FCR=0X10;
	LPC_UART0->LCR=0X03;
}

void uart1_init()
{
	LPC_UART1->LCR=0X83;
	LPC_UART1->DLM=0X00;
	LPC_UART1->DLL=0X75;
	LPC_UART1->FCR=0X10;
	LPC_UART1->LCR=0X03;
}

void delay(unsigned long int x)
{
	unsigned long int i;
	for(i=0;i<x;i++);
}

void lcdwrt(char y)
{   
	  LPC_GPIO0->FIOPINL=y<<4;
	  LPC_GPIO0->FIOSET2=0X40;
	  delay(0xf000);
	  LPC_GPIO0->FIOCLR2=0X40;
	  delay(0xf000);
}

void dispint()
{
	unsigned long int i;
	LPC_GPIO0->FIOCLR2=0X20;
	LPC_GPIO0->FIOCLR2=0X10;	
	for(i=0;i<5;i++)
		lcdwrt(cmd[i]);
}
	
void disp(char *mesg)
{   unsigned long int i;
	  LPC_GPIO0->FIOSET2=0X10;
		for(i=0;mesg[i]!='\0';i++)
		lcdwrt(mesg[i]);
}

void center(char *str)
{
	unsigned long int i,skip;
	char space=' ';
	LPC_GPIO0->FIOSET2=0X10;
	skip=(16-strlen(str))/2;
	for(i=0;i<skip;i++)
		lcdwrt(space);
}

char keyread()
{
	unsigned char row,col;
	//while(1)
	//{
	
	LPC_GPIO1->FIOPIN3=0X0f;
	delay(50000);
	while(1)
		{ 
			//dispint();
			//LPC_GPIO0->FIOSET2=0X10;	
			LPC_GPIO1->FIOPIN3=0XFE;
			row=LPC_GPIO1->FIOPIN2;
			row&=0xf0;
			if(row!=0xf0)
			{
				col=3;
				break;
			}
			LPC_GPIO1->FIOPIN3=0XFD;
			row=LPC_GPIO1->FIOPIN2;
			row&=0xf0;
			if(row!=0xf0)
			{
				col=0;
				break;
			}
			LPC_GPIO1->FIOPIN3=0XFB;
			row=LPC_GPIO1->FIOPIN2;
			row&=0xf0;
			if(row!=0xf0)
			{
				col=1;
				break;
			}
			LPC_GPIO1->FIOPIN3=0XF7;
			row=LPC_GPIO1->FIOPIN2;
			row&=0xf0;
			if(row!=0xf0)
			{
				col=2;
				break;
			}
		}
					
		if(row==0xe0)
		{ //lcdwrt(keypad[0][col]);
			delay(0xffffff);
			return(keypad[0][col]);
		}
    else if(row==0xd0)
		{	//lcdwrt(keypad[1][col]);
			delay(0xffffff);
			return(keypad[1][col]);
		}
		else if(row==0xb0)
		{
			//lcdwrt(keypad[2][col]);	
			delay(0xffffff);
			return(keypad[2][col]);
		}
		else 
		{
			//lcdwrt(keypad[3][col]);
			delay(0xffffff);
			return(keypad[3][col]);
		}
}

void motor()
{
	int val_sensor;

upp:initTimer0();
	
	switch(key_val)
		{
	  	case '1': LPC_GPIO2->FIOPIN0=0X08; break;
			case '2': LPC_GPIO2->FIOPIN0=0X20; break;
			case '3': LPC_GPIO2->FIOPIN0=0X40; break;
			case '4': LPC_GPIO2->FIOPIN0=0X80; break;	
			case '5': LPC_GPIO2->FIOPIN1=0X04; break;
			case '6': LPC_GPIO2->FIOPIN1=0X08; break;
			case '7': LPC_GPIO2->FIOPIN1=0X10; break;
			case '8': LPC_GPIO2->FIOPIN1=0X20; break;	
	  }
	
	LPC_GPIO0->FIOCLR2=0X10;
	lcdwrt(0x01);
	center(msg20);
	disp(msg20);
		
	delayMS(900); 
	LPC_GPIO2->FIOPINL=0X00;
	delayMS(900); 

	val_sensor=sensor();
		
	if(val_sensor==0)
		goto upp;
	
		 
	LPC_GPIO0->FIOCLR2=0X10;
	lcdwrt(0x01);
	center(msg14);
	disp(msg14);
	delay(0xfffffa);
}



void startTimer0(void) 
{ 
	LPC_TIM0->TCR = 0x02; //Reset Timer 
	LPC_TIM0->TCR = 0x01; //Enable timer 
} 


unsigned int stopTimer0(void) 
{ 
	LPC_TIM0->TCR = 0x00; //Disable timer 
	return LPC_TIM0->TC; 
} 


void delayUS(unsigned int microseconds) //Using Timer0 
{ 
	LPC_TIM0->TCR = 0x02; //Reset Timer 
	LPC_TIM0->TCR = 0x01; //Enable timer 
	while(LPC_TIM0->TC < microseconds); //wait until timer counter reaches the desired delay 
	LPC_TIM0->TCR = 0x00; //Disable timer 
} 
  
void delayMS(unsigned int milliseconds) //Using Timer0 
{ 
  delayUS(milliseconds * 1000); 
} 
	
void initTimer0(void) //PCLK must be = 25Mhz! 
{ 
	LPC_SC->PCONP |= (1<<1); //Power up TIM0. By default TIM0 and TIM1 are enabled.
	LPC_SC->PCLKSEL0 &= ~(0x3<<2); //Set PCLK for timer = CCLK/4 = 100/4 (default)
  LPC_TIM0->CTCR = 0x0; 
  LPC_TIM0->PR = (25-1); //Increment TC at every 24999+1 clock cycles 
  LPC_TIM0->TCR = 0x02; //Reset Timer 
} 

int sensor()
{
	int i,val;
	int echoTime1=0;
  float distance11=0; 
	//unsigned int distance1,i,j,val;
	//int msg1[5],no1[4];
	uart0_init(); //Initialize UART0 for retargeted printf() - defined in ocf_lpc176x_lib.c 
  initTimer0(); //Init Timer for delay functions - defined in ocf_lpc176x_lib.c 
	
	distance11=0;
	//Output 10us HIGH on TRIG pin 
	LPC_GPIO4->FIOPIN |= TRIG1; 
	delayUS(10); 
	LPC_GPIO4->FIOCLR |= TRIG1; 
	
	while(!(LPC_GPIO4->FIOPIN & ECHO1)); //Wait for a HIGH on ECHO pin 
	startTimer0(); //Start counting 
	while(LPC_GPIO4->FIOPIN & ECHO1); //Wait for a LOW on ECHO pin 
	echoTime1 = stopTimer0(); //Stop counting and save value(us) in echoTime 
 	
	distance11 = (0.0343 * echoTime1)/2.0; //Find the distance

		LPC_UART0->THR=distance11;
    while((LPC_UART0->LSR&0X20)!=0X20);
		delay(0x5000);
		LPC_UART0->THR=0x0d;
    while((LPC_UART0->LSR&0X20)!=0X20);

				
	if(distance11 <= 40.00)
	{
		
		LPC_UART0->THR='y';
			while((LPC_UART0->LSR&0X20)!=0X20);
		LPC_UART0->THR='e';
			while((LPC_UART0->LSR&0X20)!=0X20);
		LPC_UART0->THR='s';
		val=1;
	}
	
	else
	{
		LPC_UART0->THR='n';
			while((LPC_UART0->LSR&0X20)!=0X20);
		LPC_UART0->THR='o';
		while((LPC_UART0->LSR&0X20)!=0X20);
		val=0;
		}
	LPC_UART0->THR=0x0D;
		while((LPC_UART0->LSR&0X20)!=0X20);
	delay(0x150000);
	return val;
}
