/*
Programa: prueba oscilador interno y módulo usart
Pic: 18f46j50
Cristal: NO
CPU: 8Mhz (valor establecido dentro de main cambiando los bits IRCF2, IRCF1 e IRCF0 del registro OSCCON)
en RA6 sale la frecuencia de Fosc/4   ------->   8Mhz/4 = 2Mhz
 * File:   newmain.c
 * Autor: Joaquín
 * Created on 5 de septiembre de 2013, 14:52
 */

// PIC18F46J50 Configuration Bit Settings

#include <xc.h>
#include <stdio.h>
#include <plib/usart.h> //Libreria para el manejo del modulo USART
#include <pic18f46j50.h>
#include <pps.h>
#include <string.h>

// CONFIG1L
#pragma config WDTEN = OFF      // Watchdog Timer (Disabled - Controlled by SWDTEN bit)
#pragma config PLLDIV = 1       // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config STVREN = OFF     // Stack Overflow/Underflow Reset (Disabled)
#pragma config XINST = ON       // Extended Instruction Set (Enabled)

// CONFIG1H
#pragma config CPUDIV = OSC4_PLL6// CPU System Clock Postscaler (CPU system clock divide by 6)
#pragma config CP0 = OFF        // Code Protect (Program memory is not code-protected)

// CONFIG2L
#pragma config OSC = ECPLL      // Oscillator (EC+PLL (CLKO-RA6), USB-EC+PLL)
#pragma config T1DIG = OFF      // T1OSCEN Enforcement (Secondary Oscillator clock source may not be selected)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator (High-power operation)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor (Disabled)
#pragma config IESO = OFF       // Internal External Oscillator Switch Over Mode (Disabled)

// CONFIG2H
#pragma config WDTPS = 32768    // Watchdog Postscaler (1:32768)

// CONFIG3L
#pragma config DSWDTOSC = INTOSCREF// DSWDT Clock Select (DSWDT uses INTRC)
#pragma config RTCOSC = T1OSCREF// RTCC Clock Select (RTCC uses T1OSC/T1CKI)
#pragma config DSBOREN = OFF    // Deep Sleep BOR (Disabled)
#pragma config DSWDTEN = OFF    // Deep Sleep Watchdog Timer (Disabled)
#pragma config DSWDTPS = G2     // Deep Sleep Watchdog Postscaler (1:2,147,483,648 (25.7 days))

// CONFIG3H
#pragma config IOL1WAY = OFF    // IOLOCK One-Way Set Enable bit (The IOLOCK bit (PPSCON<0>) can be set and cleared as needed)
#pragma config MSSP7B_EN = MSK7 // MSSP address masking (7 Bit address masking mode)

// CONFIG4L
#pragma config WPFP = PAGE_63   // Write/Erase Protect Page Start/End Location (Write Protect Program Flash Page 63)
#pragma config WPEND = PAGE_WPFP// Write/Erase Protect Region Select (valid when WPDIS = 0) (Page WPFP<5:0> through Configuration Words erase/write protected)
#pragma config WPCFG = OFF      // Write/Erase Protect Configuration Region (Configuration Words page not erase/write-protected)

// CONFIG4H
#pragma config WPDIS = OFF      // Write Protect Disable bit (WPFP<5:0>/WPEND region ignored)
#include <delays.h> /* Para utilizar demoras en nuestro código debemos incluir la librería delays.h.*/
#define _XTAL_FREQ 4000000 // Linea necesario para los cálculos de la función
                           // de delay. No es para la configuración
                           // de la frecuencia de trabajo del cpu del pic.
                           // eso se determina en otro lado.

char CaracterRx2;

// Variables globales.
char CaracterRx1=0X00,a=0,coma=0;              //Se guarda cada uno de los byte resividos por rs232.
unsigned char bufferGPS[100];                    // Buffer de recepcion de datos serie GPS.
unsigned int auxGPS[15];                    //Baffer genaral
//int8 flag=0x00;
unsigned int p1,p2;                        //Se utiliza para detectar las comas
unsigned int hora=0x00;
unsigned int minutos=0x00;
unsigned int segundos=0x00;
float latitud=0.00000;
char direccion_NS=0x00;
float longitud=0.00000;
char direccion_EO=0x00;
unsigned int dia=0x00;
unsigned int mes=0x00;
unsigned int anio=0x00;
unsigned int posicion_escritura=0;
unsigned int posicion_lectura=0; 
unsigned char temporal;
unsigned int bandera_contador=0;
unsigned int *puntero;

//Declaracion de funciones
//unsigned int primera_coma(unsigned char,unsigned char,unsigned int); //Encuentra ccaracter(vector char, char a encontrar, posision donde se arranca a buscar)
//unsigned int segunda_coma(unsigned char,unsigned char,unsigned int);

void main(void)
{
    TRISA = 0X00;                             // puertos A B y C como salida. Recordar Tip: el 0 es una o de ouput y el 1 una I de input!!!
    TRISB = 0X00;
    TRISC = 0X80;
    TRISD = 0X04;
    LATA = 0X00;                              // ponemos los puertos en cero
    LATB = 0X00;
    LATC = 0X00;
    LATD = 0X00;
  
    // configuramos el puerto serie

    Open1USART(USART_TX_INT_OFF &
            USART_RX_INT_ON &       //Activar la interrupcion por la recepcion de dato del buffer de Rx del USART
            USART_ASYNCH_MODE &     //Modo asincrono (fullduplex)
            USART_EIGHT_BIT &       //8 bits de datos
            USART_CONT_RX &         //Recepción continua
            USART_BRGH_HIGH, 51);  //9600 Baudios

    Open2USART(USART_TX_INT_OFF &
            USART_RX_INT_ON &       //Activar la interrupcion por la recepcion de dato del buffer de Rx del USART
            USART_ASYNCH_MODE &     //Modo asincrono (fullduplex)
            USART_EIGHT_BIT &       //8 bits de datos
            USART_CONT_RX &         //Recepción continua
            USART_BRGH_HIGH, 51);  //9600 Baudios

    //Configuración del PPS
    PPSUnLock();                                        //inicialización del pps
    iPPSOutput(OUT_PIN_PPS_RP20,OUT_FN_PPS_TX2CK2);     //configuración del pin rp20 como salida de TX2.
    iPPSInput(IN_FN_PPS_RX2DT2,IN_PIN_PPS_RP19);        //configuración del pin rp19 como entrada de RX2.
    PPSLock();                                          //finalizacion del pps
    
  // (frecuencia a la que trabaja el cpu/dbaudrate)/16)-1
  // (8.000.000/9600)/16)-1=51
    
    Write1USART(0);         //Se coloca un cero para inicializar el buffer de recepción.
    Write2USART(0);

    INTCONbits.PEIE = 1; //Activamos interrupcion de perifericos
    INTCONbits.GIE = 1;  //Activamos las interrupciones globales

    while(1)
        {
        if(a==0x0D) //si finalizo una trama
            {
            a=0;
            puntero=&bufferGPS[0];
            // Determinamos si hay una trama $GPRMC válida..
            if(bufferGPS[0]=='$' && bufferGPS[1]=='G' && bufferGPS[2]=='P' && bufferGPS[3]=='R' && bufferGPS[4]=='M' && bufferGPS[5]=='C' && bufferGPS[18]=='A' )
                {
                //titila led de status gps avisando que hubo trama valida
                LATD6=1;
                __delay_ms(100);
                __delay_ms(100);
                __delay_ms(100);
                
                LATD6=0;
//                //Encontrar primera coma
//                for(bandera_contador=0;bandera_contador<=100;bandera_contador++)
//                    {
//                    if(bufferGPS[bandera_contador]==',')
//                        {
//                        p1=bandera_contador;
//                        break;
//                        }
//                    }
//                Write1USART(p1);

                coma=*puntero;
                if(coma==',')
                {
                    p1=&puntero;
                    break;
                }
                    puntero++;
                }
            }
            if(CaracterRx1!=0x00)   //si hay datos en el buffer de entrada
        {
        Write2USART(CaracterRx1);
        a=CaracterRx1;
        CaracterRx1=0x00;
        }
     }
}

void interrupt Interrupcion()   //Funcion que atiende las interrupciones
{
    if(PIR3bits.RC2IF ==1)
    {
        CaracterRx2 = Read2USART();   //Se lee el dato que esta en el buffer de Rx del USART
        PIR3bits.RC2IF = 0;  //Desactivamos la bandera de recepción en el buffer de entrada del USART

    }
    
    if(PIR1bits.RC1IF ==1)
    {
        CaracterRx1 = Read1USART();                 //Se lee el dato que esta en el buffer de Rx del USART del GPS
//esto lee la trama gps desde que detesta $ hasta enter y lo guarda en el vector buffer gps
        if(CaracterRx1=='$')posicion_escritura=0;   // Si llega un comienzo de trama, resetea el contador.
        else posicion_escritura++;                  // si no es comienzo d etrama incrementamos el contador.
        bufferGPS[posicion_escritura]=CaracterRx1;  // Caso contrario, sigue leyendo datos y los agrega al buffer.


//        if(CaracterRx1==13) //si finalizo una trama
//            {
//            posicion_lectura=0;
//            while(bufferGPS[posicion_lectura]!=13)
//                {
//                Write2USART(bufferGPS[posicion_lectura]);
//                posicion_lectura++;
//                }
//            }

        

        PIR1bits.RC1IF = 0;                         //Desactivamos la bandera de interrupción
    }
}


