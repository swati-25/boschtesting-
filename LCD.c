#include<W78E052.H>
#define LCD_DATA P1
#define busy P17
#define rs   P20
#define rw   P21
#define en   P22
void LCD_delay(unsigned char ms)
{
	unsigned char n;
	unsigned int i;
	for(n=0;n<ms;n++)
		for(i=0;i<1275;i++); /*for 1.7ms delay */ 
}
void LCD_ready()
{
	busy=1;
	rs=0;
	rw=1;
	LCD_delay(1);
	while(busy==1)
	{
		en=0;
		LCD_delay(1);
		en=1;
	}
}
void LCD_command(unsigned char command )
{
	LCD_ready();
	LCD_DATA=command;
	rs=0;
	rw=0;
	en=1;
	LCD_delay(1);
	en=0;
}
void LCD_data(unsigned char ascii)
{
	LCD_ready();
	LCD_DATA=ascii;
	rs=1;
	rw=0;
	en=1;
	LCD_delay(1);
	en=0;
}
void LCD_puts(unsigned char *lcd_string)
{
	while(*lcd_string)
	{
		LCD_data(*lcd_string++);
	}
}
void LCD_init()
{
	LCD_command(0x38);
	LCD_command(0x0F);
	LCD_command(0x06);
	LCD_command(0x01);
	LCD_command(0x80);
	lcd_delay(1);
}
main()
{
	LCD_init();
	LCD_data('S');
	LCD_data('H');
	LCD_data('E');
	LCD_data('N');
	LCD_data('O');
	LCD_data('Y');
	LCD_data(':');
	LCD_puts("JAI HIND !!!");
	LCD_command(0x04);
	LCD_command(0xCF);
	LCD_puts("! ayadohbuhS");
	while(1);
}