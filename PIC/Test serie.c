#include <Test serie.h>
#include <ctype.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

int i=0;
char letra;
char recbuf[]={'0','0','5','0','0','0','+','0','0','0','1'};
//char recbuf[]={'0','1','2','3','4','5','6','7','8','9','A'};
//char recbuf[]="005000+0001";

#int_RDA
void  RDA_isr(void) 
{
    if(kbhit()){
        letra = getchar();
        recbuf[i] = letra;
        i++;
        if(i=10){
            i=0;
        }
    }
}

void main()
{
   enable_interrupts(INT_RDA);
   enable_interrupts(GLOBAL);
   
   //set_tris_b(0xFF);
   delay_ms(200);
   
   printf("Buffer de entrada:\n");
   
    while(TRUE)
    {
        if(bit_test(PORTB,0))
        {
            while(bit_test(PORTB,0)){delay_ms(50);}
            //for(i=0;i<=10;i++)
            //{
                
                putc(recbuf[i++]);
                //printf("%s \n\r", recbuf[i++]);
                if(i==10){
                    i=0;
                    printf("\n\r");
                }
                

            //}
        }
    }
}
