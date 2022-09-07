#include<W78E052.h>
#define PORT_01 P1
#define PORT_00 P0
#define PORT_02 P2
#define USER_LED_DELAY 0x7ff

void bit_left_to_right(void) interrupt 2
{ 
	unsigned int i,j,k;
	for(i=1;i<= 0x80;i=i<<1)
		{
			PORT_01 = i;
			for(j=0;j<USER_LED_DELAY;j++)
			{
				for(k=0;k<USER_LED_DELAY;k++);
			}
		}
	//	IE1=0;
}

void bit_left_to_right2(void)
{ 
	unsigned int i,j,k;
	for(i=1;i<= 0x80;i=i<<1)
		{
			PORT_02 = i;
			for(j=0;j<USER_LED_DELAY;j++)
			{
				for(k=0;k<USER_LED_DELAY;k++);
			}
		}
}


void bit_right_to_left(void) interrupt 0
{
	unsigned int i,j,k;
	for(i=0x80;i>= 0x1;i=i>>1)
		{
			PORT_00 = i;
			for(j=0;j<USER_LED_DELAY;j++)
			{
				for(k=0;k<USER_LED_DELAY;k++);
			}
		}
		//IE0=0;
}



void main()
{
	EX1 = 1;
	EX0=1;
	IT0= 1;
	IT1=1;
	EA = 1;
	
	
	while (1)
	{
		//bit_left_to_right();
		bit_left_to_right2();
		//bit_right_to_left();
	}
}