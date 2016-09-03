#include <18F4550.h>
#include <stdlib.h>
//#device adc=16

#FUSES NOWDT                     //No Watch Dog Timer
//#FUSES WDT128                    //Watch Dog Timer uses 1:128 Postscale
#FUSES PLL4                     //Divide By 4(16MHz oscillator input)
#FUSES CPUDIV4                   //System Clock by 4
#FUSES HSPLL                        //High speed Osc (> 4mhz for PCM/PCH) (>10mhz for PCD)
#FUSES PUT                       //Power Up Timer
#FUSES BORV45                    //Brownout reset at 4.5V
#FUSES NOPBADEN                  //PORTB pins are configured as digital I/O on RESET
#FUSES NOLPT1OSC                 //Timer1 configured for higher power operation
#FUSES NOLVP                     //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NOXINST                   //Extended set extension and Indexed Addressing mode disabled (Legacy mode)
#BYTE PORTB = 0XF81 //18F4550

//#use delay(crystal=48000000,  clock=16000000)
#use delay(clock=48000000)
//#use delay(crystal=4000000)

//#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8)
#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8)//,FORCE_SW,BRGH1OK)
//#priority rda,rtcc

/*
#include <16F877.h>

#FUSES NOWDT                     //No Watch Dog Timer
//#FUSES WDT128                    //Watch Dog Timer uses 1:128 Postscale
//#FUSES PLL4                      //Divide By 4(16MHz oscillator input)
//#FUSES CPUDIV1                   //System Clock by 4
//#FUSES HSPLL                        //High speed Osc (> 4mhz for PCM/PCH) (>10mhz for PCD)
#FUSES PUT                       //Power Up Timer
//#FUSES BORV45                    //Brownout reset at 4.5V
//#FUSES NOPBADEN                  //PORTB pins are configured as digital I/O on RESET
//#FUSES NOLPT1OSC                 //Timer1 configured for higher power operation
#FUSES NOLVP                     //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
//#FUSES NOXINST                   //Extended set extension and Indexed Addressing mode disabled (Legacy mode)
#BYTE PORTB = 0X106 //16F877
#use delay(clock=4000000)
//#include <stdlib.h>
//#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8)
#use rs232(baud=125000,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8)*/


