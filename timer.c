#include <W78E052.h>
#define TIMER0_H 0x19
#define TIMER0_L 0x19
#define LED      P10
//#define LED_01   P11

short i=0;
short j=0;

void timer0_interrupt() interrupt 1
{
	if(i== 4000)
	{
		LED = !LED ;
		i=0;
	}
	else
		i++;
}

void main()
{
	TMOD = 0x02; //setting timer 0 in 8 bit mode
	TH0 = TIMER0_H;
	TL0 = TIMER0_L;
	ET0 = 1;
	EA =1;
	TR0 = 1; //start timer 
	while(1)
	{
		;
	}
}