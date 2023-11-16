#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include "rs232.h"
#include "rs232.c"
#include <curses.h>

int le_teclado(char *newbyte);
int filtro(char newbyte);
int comport_number;
int StartingRS232();
int kbhit();
char val;

int main()
{
    char byte;
    char newbyte;
    StartingRS232();

    while(1)
    {
        if (le_teclado(&newbyte))
        {
           
           printf("Press 'Esc' for exit\n");
	   byte = newbyte;
           break;
        }
    }  

    while(1)
        {
            printf("\r\033[2K%c\n\r",byte);
            if (le_teclado(&newbyte))
            {
                byte = newbyte;
            }
            else
            {
	
                if(newbyte == 27)
                {
                    break;
                }
            }
	    system("sleep 0.01");
            RS232_SendByte(comport_number,byte);
	    //RS232_SendByte(comport_number,10);
	    //RS232_SendByte(comport_number,8);
         }

}

int le_teclado(char *newbyte)
{
	if (kbhit())
	{
		*newbyte = getchar();
		//system("sleep 0.1");
		if(*newbyte >= '0' && *newbyte <= '9')
		{
			return 1;
		}

		else
		{
			return 0;
		}
	}

	else 
	{
		return 0;
	}

}

int StartingRS232 ()
{

    char mode[]={'8','N','1',0};

    printf ("Type de COM number:\n");
    scanf  ("%d",&comport_number);
    printf ("Try open COM %d @9600 - 8N1\n",comport_number);

    comport_number = comport_number - 1;

    if (RS232_OpenComport(comport_number , 9600,mode,0))
    {
        printf ("Open Com error\n");
        return (0);
    }
    printf ("Open Com success\n");
    return (1);
}

int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;
  
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
 

  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
  
  ch = getchar();
  
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
  
  if (ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }
  
  return 0;
}
