#include <W78E052.H>
#define Ldata P1
#define rs P20
#define rw P21
#define en P22									// Not checking the busy flag(P2_7) of LCD
#define scl P00
#define sda P01
/*sbit scl=P00;
sbit sda=P01;
*/
void write(unsigned char,unsigned char);  		// To write initial data to RTC
unsigned char datatran(unsigned char);		    // To write 8bit data(without start & stop) to RTC in I2C  formact
void time(void);								// To read time from RTC and display on the LCD
void date(void);								// To read date from RTC and display on the LCD	 
void d_write(unsigned char,unsigned char);		// Sent DI and Regstr-address to RTC, for RTC regstr read
unsigned char read(unsigned char);				// For RTC read and non ack fron MC to RTC
void start(void);
void stop(void);
void rtc_delay(unsigned int);

void lcd_init();
void lcd_cmd(unsigned char);
void lcd_write(unsigned char);
void lcd_mes(unsigned char *);
void LCD_delay(unsigned char ms);

unsigned char r=0,ap=0;

unsigned char  z[]={0x50,0x56,0x61,0x03,0x07,0x09,0x22};   //50sec 59min 23hr/11PM,monday,31-12-'10
/*
	For hour
	11AM------> 0,1,0,1 0001----> 51
	11PM------> 0,1,1,1 0001----> 71
	23Hr------> 0,0,1,0 0011----> 23(directly)	
*/
unsigned char *s[]={"SUN","MON","TUE","WED","THU","FRI","SAT"};

///////////////////////////////////////
void main()
{	   
	lcd_init();
	lcd_write('S');
	write(0xd0,0x00);	// To send z[i] contents to registers from 00 to 06
  
	while(1)
	{
		time();
		date();
    }	
}

///////////////////////////////////////		
void write(unsigned char devi_id,unsigned char add)
{
	unsigned char a,i;						
	start();
	a=datatran(devi_id);		       //device address	 1101 000_,last 0 for write
	a=datatran(add);	               //First register address(e2rom location) 

	for(i=0;i<=6;i++)
	{
		a=datatran(z[i]);		       //data send to RTC in sequal order
	}
  	stop();
}

unsigned char datatran(unsigned char c)
{
	unsigned char mask1,d=0;
	unsigned int i;
	mask1=0x80;
	
	for(i=0;i<=7;i++)
	{
		if(c&mask1)
		{
			sda=1;
		}

		else
		{
			sda=0;
		}
		
		scl=1;
		rtc_delay(1);
		scl=0;
		rtc_delay(1);
		mask1=(mask1>>1);
	}
			
	sda=1;
	scl=1;
	rtc_delay(1);
	d=sda;
	scl=0;
	rtc_delay(1);

	return 0;	
} 

void time(void)
{
	unsigned char r1,r2;
	lcd_cmd(0x80);	  		// 1st line

	// For RTC hours..........
	d_write(0xD0,0x02);	    // ["D0" means writing OxO2 to RTC]
	r=read(0xd1);			// ["D1" means reading data from RTC] 
	ap=r;		

	//	Filtering bits 
	/*	For 12hr clock (B6=1, 2nd bit from MSB)						
			B5=1 means PM, B5=0 means AM
		For 24hr clock(B6=0)
			B5 and B4 together gives tenth position digit in  BCD */

	if(ap&0x40)		  //for 12hr clock
	{
		r1=r&0x0f;	// D3-D0(4nos),unit position
		r2=r&0x10;   // filtering 3rd bit from MSB(D4)
		r2=r2>>4;
		// Convertion of HOURS from bcd to ASCII value
		lcd_write(r2+48); 
		lcd_write(r1+48);  
		lcd_mes(":");
	}
	else	    //for 24hr clock
	{
		r1=r&0x0f;	// D3-D0(4nos),unit position
		r2=r&0x30;   // filtering 3rd bit from MSB(D4)
		r2=r2>>4;
		// Convertion of HOURS from bcd to ASCII value
		lcd_write(r2+48); 
		lcd_write(r1+48);  
		lcd_mes(": ");
	}	

	// For RTC minutes.......... 		
	d_write(0xd0,0x01);	  	
	r=read(0xd1);
	// Filtering bits
	r1=r&0x0f;	 // D3-D0(4nos),unit position
	r2=r&0x70;   // D6-D4(3nos),tenth position [can use 0xf0 instead of 0x70]
	r2=r2>>4;
	// Convertion of MINUTES from bcd to ASCII value
	lcd_write((r2)+48);  //minutes
	lcd_write((r1)+48);
	lcd_mes(":");

	// For RTC seconds.......... 	
	d_write(0xd0,0x00);
	r=read(0xd1);
	// Filtering bits
	r1=r&0x0f;	 // D3-D0(4nos),unit position
	r2=r&0x70;   // D6-D4(3nos),tenth position
	r2=r2>>4;
	// Convertion of SECONDS from bcd to ASCII value
	lcd_write((r2)+48);  //seconds
	lcd_write((r1)+48);
	lcd_mes(" ");

	// For RTC AM/PM.......... 
	if(ap&0x40)		  //for 12hr clock
	{
		if(ap&0x20)		  // filtering 3rd bit from MSB	 
			lcd_mes("PM");
		else
			lcd_mes("AM");

		lcd_mes(" ");
	}
	// For RTC DAY.......... 	
	d_write(0xd0,0x03);
	r=read(0xd1); // r=r&0x07;

	lcd_mes(s[r]);        //day

	lcd_mes(" ");
}

///////////////////////////////////////
void date()
{
	unsigned char j,add,add1,k;
	unsigned char r1,r2;
	
	add=add1=0x04;
	j=0xc0;
	k=0;
	while(k<3)
	{
		d_write(0xd0,add++);
		r=read(0xd1);
	// Filtering bits
		r1=r&0x0f;
		r2=r&0xf0;	 // [instead of 0xf0 we can use 0x30 for date,0x10 for month and 0xf0 for year seperately]
		r2=r2>>4;
		
	// Convertion of DATE from bcd to ASCII value
		lcd_cmd(j++);
		lcd_write(r2+48);
		lcd_cmd(j++);
		lcd_write(r1+48);
		lcd_cmd(j++);
		
		if(k==2)
			lcd_mes(".");
		else
			lcd_mes("-");
		
		k++;
	}
}

void d_write(unsigned char devi_id,unsigned char add)
{
	unsigned char a;

	start();
	a=datatran(devi_id); //device address
   	a=datatran(add);
    stop();
}      

unsigned char read(unsigned char add)
{
	unsigned char c,a,mask1,v;
	unsigned int l;

	mask1=0x80;
	c=0;
	v=0;

	start();
	a=datatran(add);
	for(l=0;l<=7;l++)
	{		
		if(sda==1)
		{c=(c|mask1);}
		else
		c=c;

		scl=1;
		rtc_delay(1);
		scl=0;
		rtc_delay(1);
		mask1=(mask1>>1);
				
	}
  	
	scl=1;
	rtc_delay(1);
	sda=1;		//non ack
	scl=0;
	rtc_delay(1);

	return c;
	stop();
}	

void start()
{
	sda=1;
	scl=1;
	sda=0;	//start condition.
	rtc_delay(1);
	scl=0;
	rtc_delay(1);
}

void stop()
{
	sda=0;	//stop condition
	scl=1;
	sda=1;	 
	rtc_delay(1);  //OPTIONAL
	scl=0;
	rtc_delay(1);
}

void rtc_delay(unsigned int j)
{
	unsigned int i;
	while(j-->0)
		for(i=0; i<100; i++);			
}

void lcd_init()
{
	lcd_cmd(0x38);
	lcd_cmd(0x01);
	lcd_cmd(0x06);			 
	lcd_cmd(0x0c);
}

void LCD_delay(unsigned char ms)
{
	unsigned char n;
	unsigned int i;
	for (n=0; n<ms; n++)
	for (i=0; i<1275; i++); /* For 1 ms */
}

void lcd_cmd(unsigned char command)
{
	Ldata =command ; /* Clear bit P2.5 */
	rs=0;//CND
	rw=0;	  //WR
	en=1;
	LCD_delay(1);
	en=0;
}

void lcd_write(unsigned char ascii)
{
	Ldata=ascii;
	rs=1;
	rw=0;
	en=1;
	LCD_delay(1);
	en=0;
}

void lcd_mes(unsigned char *s)
{
	while(*s)
	{
		lcd_write(*s++);
	}
} 

