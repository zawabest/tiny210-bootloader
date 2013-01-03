#include "my_stdio.h"
#include "my_command.h"
#include "uart.h"
#include "my_printf.h"


#define	MAX_CMD_LEN	128
#define	MAX_ARGS	MAX_CMD_LEN/4
typedef int (*cmdproc)(int argc, char *argv[]);
typedef struct {
	const char *cmd;
	const char *hlp;
	cmdproc proc;
}CMD_STRUC;


CMD_STRUC CMD_INNER[] =
				{ 
					{"help", "    show help",help},	
					{"led_test", "test the led's status", led_test},
					{"beep_test", "test the beep's status", beep_test},
					{"loadx", "   download the image use Xmodem protocol", loadx},
					{"go", "      go at your input,default is 0x20000", go},
					{"nandr", "      nandr nandaddr dramaddr size", nandr},
					{"md", "      md dramaddr", md},
					{NULL, NULL, NULL}
				};


/*
CMD_STRUC CMD_INNER[] =
				{ 
					{"help", "    show help",help},
					{"dn", "      download the image from UART0", uue_receive},	
					{"xdown", "   download the image use Xmodem protocol", xmodem_receive},
					{"go", "      go at your input,default is 0x20000", go},
					{"testled", "test the led's status", led_test},
					{"testseg7", "test the seg7's status", seg7_test},
					{"testbeep", "test the beep's status", beep_test},
					{"testdip4", "test the dip4's status", dip4_test},
					{"testtime0", "test the time0's status", timer0_on},
					{"testint0", "test the int0's status", int0_on},
					{"flashw",    "  write the image to flash", flashw},					
					{NULL, NULL, NULL}
				};
								*/
/************************************************/

/************************************************/
static void ParseArgs(char *cmdline, int *argc, char **argv)
{
#define STATE_WHITESPACE	0
#define STATE_WORD			1

	char *c;
	int state = STATE_WHITESPACE;
	int i;

	*argc = 0;

	if(my_strlen(cmdline) == 0)
		return;

	/* convert all tabs into single spaces */
	c = cmdline;
	while(*c != '\0')
	{
		if(*c == '\t')
			*c = ' ';
		c++;
	}
	
	c = cmdline;
	i = 0;

	/* now find all words on the command line */
	while(*c != '\0')
	{
		if(state == STATE_WHITESPACE)
		{
			if(*c != ' ')
			{
				argv[i] = c;		//��argv[i]ָ��c
				i++;
				state = STATE_WORD;
			}
		}
		else
		{ /* state == STATE_WORD */
			if(*c == ' ')
			{
				*c = '\0';
				state = STATE_WHITESPACE;
			}
		}
		c++;
	}
	
	*argc = i;
	
#undef STATE_WHITESPACE
#undef STATE_WORD
}
static int GetCmdMatche(char *cmdline)
{
	int i;	
	
	for(i=0; CMD_INNER[i].cmd!=NULL; i++)
	{
		if(my_strncmp(CMD_INNER[i].cmd, cmdline, my_strlen(CMD_INNER[i].cmd))==0)
			return i;
	}
	
	return -1;
}

static int ParseCmd(char *cmdline, int cmd_len)
{
	int argc, num_commands;
	char *argv[MAX_ARGS];

	ParseArgs(cmdline, &argc, argv);

	/* only whitespace */
	if(argc == 0) 
		return 0;
	
	num_commands = GetCmdMatche(argv[0]);
	if(num_commands<0)
		return -1;
		
	if(CMD_INNER[num_commands].proc!=NULL)	
		CMD_INNER[num_commands].proc(argc, argv);
				
	return 0;			
}

 void my_diplay(void)
{
	int j;
	char ss[] = "aka210_Boot>";
	for(j=0; ss[j]!='\0';j++)
	{
		uart0_putchar(ss[j]);
	}
}

void my_shell(void)
{
	
	char s[100];
	char *p ;
	int len;
	led_init();	
	led_on(2);	
	uart0_init();
	while(1)
	{
		p = s;
		my_diplay();
		my_gets(s);
		len = my_strlen(s);
		//my_printf("%d",len);/////////////////////////////////
		if(ParseCmd(s,len)== -1) 
			{
			my_putchar('\r');
			my_puts("  bad command,please enter \"help\" to get manual") ;
			}
		my_putchar('\r');
	}
}


int help(int argc, char *argv[])
{
	int i;	
	my_putchar('\r');
	for(i=0; CMD_INNER[i].cmd!=NULL; i++)
	{
		if(CMD_INNER[i].hlp!=NULL)
		{
			my_puts(CMD_INNER[i].cmd);
			my_puts(" ------ ");
			my_puts(CMD_INNER[i].hlp);
			my_putchar('\r');
		}
	}
	
	return 0;
}

