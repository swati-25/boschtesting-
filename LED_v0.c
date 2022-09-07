#include<W78E052.h>
void main()
{
	int i,j;
	while(1)
	{
		for(i=1;i<0x80;i=i<<1)
		{
		P1=i;
			for(j=0;j<0xff;j++);
		}
	}
}