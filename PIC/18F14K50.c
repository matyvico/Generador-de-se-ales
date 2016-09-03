#INCLUDE <18F14K50.h>

#FUSES HS
#FUSES PLLEN
#FUSES PCLKEN
#FUSES CPUDIV1
#FUSES USBDIV1
#FUSES NOFCMEN
#FUSES NOWDT
#FUSES WDT128
#FUSES NOIESO
#FUSES MCLR
#FUSES NOLVP
#FUSES NOXINST
#FUSES NODEBUG
#FUSES PUT
#FUSES NODEBUG
#FUSES NOBROWNOUT
#FUSES NOPROTECT

#USE delay(clock=48000000)
#USE rs232(baud=57600, parity=N, xmit=PIN_B7, rcv=PIN_B5 ,bits=8, ERRORS)

#DEFINE USB_HID_DEVICE     FALSE             // Deshabilitamos el uso de las directivas HID
#DEFINE USB_EP1_TX_ENABLE  USB_ENABLE_BULK   // Turn on EP1(EndPoint1) for IN bulk/interrupt transfers
#DEFINE USB_EP1_RX_ENABLE  USB_ENABLE_BULK   // Turn on EP1(EndPoint1) for OUT bulk/interrupt transfers
#DEFINE USB_EP1_TX_SIZE    32                // Size to allocate for the tx endpoint 1 buffer
#DEFINE USB_EP1_RX_SIZE    32                // Size to allocate for the rx endpoint 1 buffer

#INCLUDE <pic18_usb.h>                       // Microchip PIC18Fxx5x Hardware layer for CCS's PIC USB driver
#INCLUDE <rr2_USB_Monitor.h>                 // Configuración del USB y los descriptores para este dispositivo
#INCLUDE <usb.c>                             // Handles usb setup tokens and get descriptor reports
#INCLUDE <math.h>
#INCLUDE <stdlib.h>

#DEFINE Enciende    Output_High
#DEFINE Apaga       Output_Low
#DEFINE Conmuta     Output_Toggle

#DEFINE LED_ROJO    PIN_C0                   //Conectado
#DEFINE LED_VERDE   PIN_C1                   //Reconocido
#DEFINE P4550       PIN_C2                   //Reset Digital

#DEFINE Tipo        recbuf[0]
#DEFINE Frec_4      recbuf[1]
#DEFINE Frec_3      recbuf[2]
#DEFINE Frec_2      recbuf[3]
#DEFINE Frec_1      recbuf[4]
#DEFINE Frec_0      recbuf[5]
#DEFINE Offset_3    recbuf[6]
#DEFINE Offset_2    recbuf[7]
#DEFINE Offset_1    recbuf[8]
#DEFINE Offset_0    recbuf[9]
#DEFINE Control     recbuf[10]

#byte ANSEL = 0xF7E
#byte ANSELH = 0xF7F

const int8 Lenbuf = 12;                     // 11 bytes + caracter nulo

int i;
int j;

char recbuf[Lenbuf] = {'0','0','0','0','0','0','0','0','0','0','0'};
char envbuf[Lenbuf] = {'0','0','0','0','0','0','0','0','0','0','0'};
char letra;

void main(void){
    
    ANSEL = 0x00; 
    ANSELH = 0x00;

    Enciende(P4550);
    
    Apaga(LED_ROJO);
    Apaga(LED_VERDE);
    delay_ms(100);
    Enciende(LED_ROJO);

    usb_init();                             //Se inicia el perisferico USB
    usb_task();                             //Se configura el perisferico USB
    usb_wait_for_enumeration();             //Se espera la respuesta del SO

    while (TRUE){
        if(usb_enumerated()){
            Apaga(LED_ROJO);
            Enciende(LED_VERDE);
            if (usb_kbhit(1)){              //Se verifica la existencia de un nuevo paquete en el ENDPOINT
                usb_get_packet(1, recbuf, Lenbuf); // Recibe Packet

                strcpy(envbuf,recbuf); 
                recbuf[0] = '0';
                recbuf[1] = '0';
                recbuf[2] = '0';
                recbuf[3] = '0';
                recbuf[4] = '0';
                recbuf[5] = '0';
                recbuf[6] = '0';
                recbuf[7] = '0';
                recbuf[8] = '0';
                recbuf[9] = '0';
                recbuf[10] = '0';
                
                Apaga(P4550); //Se resetea el PIC18F4550
                delay_ms(5);
                Enciende(P4550); //Se inicia el PICF4550
                delay_ms(200); //Se espera al inicio completo del PIC18F4550
                
                for(i=0;i<=10;i++){ //Se inicia el envio de la configuracion de la señal a generar
                    letra = envbuf[i]; 
                    putc(letra);
                }
            }
        }
    }
}
