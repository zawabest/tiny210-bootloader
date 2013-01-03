#include "gpio.h"
#include "my_stdio.h"
#include "my_printf.h"
#include "nand.h"



//in my_shell.c
extern int help(int argc, char *argv[]);

int  go(int argc, char *argv[])
{
	unsigned int addr;
	void (*fp)(void);
	if(argc > 1) addr =  atox(argv[1]);
	else addr = 0x22000000;
	fp = (void (*)(void)) addr;
	my_putchar('\r');
	my_puts("go at address:");
	my_printf("0x%x ......",addr);
	my_putchar('\r');
	(*fp)();  
	return 0;
}

//in my_xmodem.c
extern int loadx(int argc, char *argv[]);

int led_test(int argc, char *argv[])
{
	int i;
	led_init();
	for (i = 0; i < 4; i++) {
		led_on(i);
		delay(1);
		led_off(i);
		delay(1);
	} 
	return 0;
}

int beep_test(int argc, char *argv[])
{
	int i;
	beep_init();
	for (i = 0; i < 4; i++) {
		beep_on();
		delay(1);
		beep_off();
		delay(1);
	} 
	return 0;
}

int nandr(int argc, char *argv[])
{
	int nandaddr,dramaddr,size;
	
	if (argc < 3) {
		my_printf("nandr nandaddr dramaddr size\n");
		return -1;
	}
	
	nand_init();
	nandaddr= atox(argv[1]);
	dramaddr=atox(argv[2]);
	size=atox(argv[3]);
	
	my_printf("\n");
	my_printf("%x  \n",nandaddr);
	my_printf("%x  \n",dramaddr);
	my_printf("%x  \n",size	);
	
	nand_read(nandaddr,(char *)dramaddr,size);
	return 0;
}
int md(int argc, char *argv[])
{
	int size = 0x100;
	int i,j;
	int addr = atox(argv[1]);
	//my_printf("%x  \n",addr);
	my_printf("\n");
	for (i = 0,j = 1; i < size; i+=4,j++) {
		my_printf("%x  ",*((unsigned int *)(addr+i)));
		if(!(j & 0x3))
			my_printf("\n");
	}
	return 0;
}
