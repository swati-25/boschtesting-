#include<W78E052.h>
#define BAUD_RATE_9600 0xFD
bit tx;
bit rx;
unsigned char serialdata;
void main(void);
void SerialInitialize (void);
void SendByteSerially(unsigned char serialdata);
unsigned char ReceiveByteSerially(void);

void serial_interrupt(void) interrupt 4
{
	if(TI !=0 )
	{
		 
	}
	if(RI == 1)
	{
		 
		 serialdata = SBUF;
		RI=0;
		 rx=1;
	}
}

void main(void)

{
	//unsigned char serialdata;
	SerialInitialize();
	ES =1;
	EA=1;
	tx=0;
	rx=0;
	
	for(;;)
	{
	/*	serialdata = ReceiveByteSerially();
		SendByteSerially(serialdata);*/
		if(rx)
		{
			SBUF = serialdata;
			rx=0;
			//tx=0;
		}
		if (tx)
		{
			
	}
}
}

void SerialInitialize (void)
{
	TMOD = ((TMOD & 0x0f) | 0x20);
	SCON = 0x50;
	TH1 = BAUD_RATE_9600;
	TR1 = 1;
}

/*void SendByteSerially(unsigned char serialdata)
{
	SBUF = '*';
	while(TI ==0);
	TI =0;
}

unsigned char ReceiveByteSerially(void)
{
	while(RI ==0);
	RI =0;
	return SBUF;
}
*/