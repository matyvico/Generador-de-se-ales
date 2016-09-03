/*#INCLUDE "18F4550.h"

//#INCLUDE <stdio.h>
//#INCLUDE <stdlib.h>
//#INCLUDE <string.h>
//#INCLUDE <math.h>

#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL4,CPUDIV1,VREGEN

#use delay(clock=16000000)
//#device adc=8 //Usa resolución de 8 bits

         //handles usb setup tokens and get descriptor reports




#define USB_HID_DEVICE     FALSE             //deshabilitamos el uso de las directivas HID
#define USB_EP1_TX_ENABLE  USB_ENABLE_BULK   //turn on EP1(EndPoint1) for IN bulk/interrupt transfers
#define USB_EP1_RX_ENABLE  USB_ENABLE_BULK   //turn on EP1(EndPoint1) for OUT bulk/interrupt transfers
#define USB_EP1_TX_SIZE    1                 //size to allocate for the tx endpoint 1 buffer
#define USB_EP1_RX_SIZE    3                 //size to allocate for the rx endpoint 1 buffer



#use fast_io(b) //Hay que configurar manualmente el trisb

#include "usb.c"
#include "usb_desc_scope.h" //Configuración del USB por Enumerador Palitroquez
#include "pic18_usb.h"     //Microchip PIC18Fxx5x Hardware layer for CCS's PIC USB driver


void main(void) {

   int8 recibe[2];                  //declaramos variables
   int8 envia[1];
   
   set_tris_d(0x00); //Todo salidas
   set_tris_a(0xff); //Todo entradas
   output_b(0x00);
   
   output_low(PIN_D7); //Apaga led para indicar que no ha sido detectado por pc
   
   setup_adc (adc_clock_div_32); //Enciende ADC
   setup_adc_ports (AN0_ANALOG); //Elige terminales análogicas
   set_adc_channel (0); //Elige canal a medir
   
   usb_init();                      //inicializamos el USB
   usb_task();                      //habilita periferico usb e interrupciones
   usb_wait_for_enumeration();      //esperamos hasta que el PicUSB sea configurado por el host
   
   output_high(PIN_D7); //Ya fue detectado por PC
   delay_ms(500);
   output_low(PIN_D7); //Ya fue detectado por PC
   delay_ms(500);
   output_high(PIN_D7); //Ya fue detectado por PC
   delay_ms(500);
   output_low(PIN_D7); //Ya fue detectado por PC
   delay_ms(500);
   output_high(PIN_D7); //Ya fue detectado por PC
   delay_ms(500);
   output_low(PIN_D7); //Ya fue detectado por PC
   delay_ms(500);
   
   while (1)
   {
      if(usb_enumerated())          //si el PicUSB está configurado
      {
         if (usb_kbhit(1))          //si el endpoint de salida contiene datos del host
         {
            usb_get_packet(1, recibe, 2);
            if(recibe[0] == 0xff) // Debe activar BCD
               output_b(~recibe[1]); //display ánodo común
            if(recibe[0]==0x00)
               {
               envia[0]=read_adc (); //Hace conversión AD
               usb_put_packet(1, envia, 1, USB_DTS_TOGGLE);
               }
         }
      }
   }
}
#ignore_warnings 216,203
#include <18f4550.h>//DIRECTIVA PARA ESPECIFICAR EL CHIP A UTILIZAR
#fuses PLL5,CPUDIV1,USBDIV,HSPLL,VREGEN,NOWDT,NOMCLR,NODEBUG,NOPROTECT,NOLVP,NOPBADEN//FUSIBLES
#device ADC=8//DEFINIMOS LA CANTIDAD DE BITS UTILIZADOS POR EL ADC
#use delay(clock=48000000)//FRECUENCIA A CONSIDERAR EN LOS COMANDOS DE RETARDO 
#define PACK 32//TAMAÑO DEL PAQUETE A TRANSMITIR A LA PC, DEPENDE DE LOS DISPOSITIVOS QUE HALLAMOS CREADO EN EL REPORTE DEL DESCRIPTOR

//DIRECTIVAS PARA TRABAJO USB HID
#define Vid 0x04d8//DEFINE EL VID
#define pid 0x0019//DEFINE EL PID
#define USB_HID_DIVICE TRUE//habilitamos el uso de las directivas HID
#define USB_EP1_TX_ENABLE  USB_ENABLE_INTERRUPT//turn on EP1(EndPoint1) for IN interrupt transfers     
#define USB_EP1_RX_ENABLE  USB_ENABLE_INTERRUPT//turn on EP1(EndPoint1) for OUT interrupt transfers    
#define USB_EP1_TX_SIZE    PACK//size to allocate for the tx endpoint 1 buffer (MAXIMO 64)
#define USB_EP1_RX_SIZE    PACK//size to allocate for the rx endpoint 1 buffer (MAXIMO 64)     
#define USB_USE_FULL_SPEED TRUE  //para utilizar full speed de 12 Mbps
#include <pic18_usb.h>//Microchip PIC18Fxx5x Hardware layer for CCS's PIC USB driver
#include <usb_desc_hid basico.h>//USB Configuration and Device descriptors for this UBS device
#include <usb.c>//handles usb setup tokens and get descriptor reports
#include <alux.h>

//DEFINICIONES ADICIONALES SE DEFINEN PARA FACILITAR LA LECTURA DEL CÓDIGO
#define LEDR PIN_C1
#define LEDV PIN_C0
#define LED_ON        output_high
#define LED_OFF       output_low
#BYTE TRISA = 0xf92
#BYTE PORTA = 0xf80
#BYTE TRISB = 0xf93
#BYTE PORTB = 0xf81
#BYTE TRISC = 0xf94
#BYTE PORTC = 0xf82
//DECLARACION DE VARIABLES
int salida[PACK];
int entrada[PACK];
void main(void)
{
 TRISC=0X00;
 TRISA=0B00000001;
 TRISC=0X00;
 LED_ON(LEDR);//ENCENDEMOS EL LED DE POWER
 LED_OFF(LEDV);
  setup_adc(ADC_CLOCK_INTERNAL);//ESTABLECE A INTERNO EL RELOJ DEL ADC
 setup_adc_ports(AN0);//CONFIGURA EL PIN A0 COMO ENTRADA ANALÓGICA (AN0)
 usb_init();//INICIALIZA LOS PARAMETROS DEL USB
 usb_task();//HABILITA LA CONEXION USB
 usb_wait_for_enumeration();//ESPERAR HASTA QUE EL PIC SEA CONFIGURADO POR LA PC
LED_OFF(LEDR);
 LED_ON(LEDV);//ENCIENDE EL LED DE OK
  while(TRUE)
 {

  if(usb_enumerated())//VERDADERO SI EL PIC HA SIDO CONFIGURADO LA PC
  {
   if(usb_kbhit(1))//VERDADERO SI EL ENDPOINT TIENE DATOS RECIBIDOS DESDE LA PC
   {
    usb_get_packet(1,entrada,PACK);//ALMACENAMOS ENTRADA[PACK] RECIBIDO DESDE LA PC
    set_adc_channel(0);//SELECCIONA EL CANAL AN0
    delay_us(10);
    salida[0]=read_adc();
    usb_put_packet(1,salida,PACK,USB_DTS_TOGGLE);//ENVIAMOS SALIDA[PACK] HACIA LA PC
   }
  }
 }
}






*/#ignore_warnings 216,203


#include <18f4550.h>//DIRECTIVA PARA ESPECIFICAR EL CHIP A UTILIZAR
#fuses PLL5,CPUDIV1,USBDIV,HSPLL,VREGEN,NOWDT,NOMCLR,NODEBUG,NOPROTECT,NOLVP,NOPBADEN//FUSIBLES
#device ADC=8//DEFINIMOS LA CANTIDAD DE BITS UTILIZADOS POR EL ADC
#use delay(clock=48000000)//FRECUENCIA A CONSIDERAR EN LOS COMANDOS DE RETARDO 
#define PACK 32//TAMAÑO DEL PAQUETE A TRANSMITIR A LA PC, DEPENDE DE LOS DISPOSITIVOS QUE HALLAMOS CREADO EN EL REPORTE DEL DESCRIPTOR

//DIRECTIVAS PARA TRABAJO USB HID
#define Vid 0x04d8//DEFINE EL VID
#define pid 0x0019//DEFINE EL PID
#define USB_HID_DIVICE TRUE//habilitamos el uso de las directivas HID
#define USB_EP1_TX_ENABLE  USB_ENABLE_INTERRUPT//turn on EP1(EndPoint1) for IN interrupt transfers     
#define USB_EP1_RX_ENABLE  USB_ENABLE_INTERRUPT//turn on EP1(EndPoint1) for OUT interrupt transfers    
#define USB_EP1_TX_SIZE    PACK//size to allocate for the tx endpoint 1 buffer (MAXIMO 64)
#define USB_EP1_RX_SIZE    PACK//size to allocate for the rx endpoint 1 buffer (MAXIMO 64)     
#define USB_USE_FULL_SPEED TRUE  //para utilizar full speed de 12 Mbps
#include <pic18_usb.h>//Microchip PIC18Fxx5x Hardware layer for CCS's PIC USB driver
#include <usb_desc_hid basico.h>//USB Configuration and Device descriptors for this UBS device
#include <usb.c>//handles usb setup tokens and get descriptor reports
#include <alux.h>

//DEFINICIONES ADICIONALES SE DEFINEN PARA FACILITAR LA LECTURA DEL CÓDIGO
#define LEDR PIN_C1
#define LEDV PIN_C0
#define LED_ON        output_high
#define LED_OFF       output_low
#BYTE TRISA = 0xf92
#BYTE PORTA = 0xf80
#BYTE TRISB = 0xf93
#BYTE PORTB = 0xf81
#BYTE TRISC = 0xf94
#BYTE PORTC = 0xf82
//DECLARACION DE VARIABLES
int salida[PACK];
int entrada[PACK];
void main(void)
{
 TRISC=0X00;
 TRISA=0B00000001;
 TRISC=0X00;
 LED_ON(LEDR);//ENCENDEMOS EL LED DE POWER
 LED_OFF(LEDV);
  setup_adc(ADC_CLOCK_INTERNAL);//ESTABLECE A INTERNO EL RELOJ DEL ADC
 setup_adc_ports(AN0);//CONFIGURA EL PIN A0 COMO ENTRADA ANALÓGICA (AN0)
 usb_init();//INICIALIZA LOS PARAMETROS DEL USB
 usb_task();//HABILITA LA CONEXION USB
 usb_wait_for_enumeration();//ESPERAR HASTA QUE EL PIC SEA CONFIGURADO POR LA PC
LED_OFF(LEDR);
 LED_ON(LEDV);//ENCIENDE EL LED DE OK
  while(TRUE)
 {

  if(usb_enumerated())//VERDADERO SI EL PIC HA SIDO CONFIGURADO LA PC
  {
   if(usb_kbhit(1))//VERDADERO SI EL ENDPOINT TIENE DATOS RECIBIDOS DESDE LA PC
   {
    usb_get_packet(1,entrada,PACK);//ALMACENAMOS ENTRADA[PACK] RECIBIDO DESDE LA PC
    set_adc_channel(0);//SELECCIONA EL CANAL AN0
    delay_us(10);
    salida[0]=read_adc();
    usb_put_packet(1,salida,PACK,USB_DTS_TOGGLE);//ENVIAMOS SALIDA[PACK] HACIA LA PC
   }
  }
 }
}







