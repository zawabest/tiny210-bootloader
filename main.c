#include "uart.h"
#include "gpio.h"
#include "my_printf.h"
#include "my_shell.h"
int mode_select(void)
{
	int i,flag=1;
	/*
	for (i = 0; i < 3; i++) {
		//�жϴ���״̬�Ĵ���,��flag��ֵ;
		delay(1);
	}
	*/
	return flag;
	
}
void load_os(void)
{

}
void loader(void)
{
	int res;
	
//	uart0_init();
	res = mode_select();
	if (res == 1)
		my_shell();
	else
		load_os();
}

