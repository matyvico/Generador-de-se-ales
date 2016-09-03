#INCLUDE <18F4550.h>

#FUSES HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL4,CPUDIV1,VREGEN
//REVISAR ESTOS FUSES, FUERON AGREGADOS DESPUES, SIN ESTOS ANDA TODO BIEN
#FUSES BROWNOUT_NOSL, NOBROWNOUT, PUT, NOCPD, NOSTVREN
#FUSES NOWRT, NOWRTD, IESO, NOFCMEN, NOPBADEN, NOWRTC, NOWRTB
#FUSES NOEBTR, NOEBTRB, NOCPB, MCLR, NOLPT1OSC, NOXINST
#FUSES NOICPRT

#USE delay(clock=48000000)
#USE fast_io(d)

#DEFINE USB_HID_DEVICE     FALSE             // deshabilitamos el uso de las directivas HID
#DEFINE USB_EP1_TX_ENABLE  USB_ENABLE_BULK   // turn on EP1(EndPoint1) for IN bulk/interrupt transfers
#DEFINE USB_EP1_RX_ENABLE  USB_ENABLE_BULK   // turn on EP1(EndPoint1) for OUT bulk/interrupt transfers
#DEFINE USB_EP1_TX_SIZE    32                // size to allocate for the tx endpoint 1 buffer
#DEFINE USB_EP1_RX_SIZE    32                // size to allocate for the rx endpoint 1 buffer

#INCLUDE <pic18_usb.h>                      // Microchip PIC18Fxx5x Hardware layer for CCS's PIC USB driver
#INCLUDE ".\include\rr2_USB_Monitor.h"      // Configuración del USB y los descriptores para este dispositivo
#INCLUDE <usb.c>                            // handles usb setup tokens and get descriptor reports
#INCLUDE <string.h>
#INCLUDE <stdlib.h>
#INCLUDE <math.h>

#DEFINE LED_VERDE    PIN_E0 //Conectado
#DEFINE DAC_B        PIN_E1 //DAC B
#DEFINE LED_AMARILLO PIN_E1 //DAC B
#DEFINE WR           PIN_E2 //#WR
#DEFINE LED_ROJO     PIN_E2 //#WR

#DEFINE Enciende     Output_High
#DEFINE Apaga        Output_Low
#DEFINE Conmuta      Output_Toggle

#DEFINE Tipo        recbuf[0]
#DEFINE Frec_4      recbuf[1] //MSB DE FREC
#DEFINE Frec_3      recbuf[2]
#DEFINE Frec_2      recbuf[3]
#DEFINE Frec_1      recbuf[4]
#DEFINE Frec_0      recbuf[5]
#DEFINE Offset_3    recbuf[6] //MSB DE OFF (Signo)
#DEFINE Offset_2    recbuf[7] 
#DEFINE Offset_1    recbuf[8]
#DEFINE Offset_0    recbuf[9]
#DEFINE Control     recbuf[10]

#DEFINE DOSPI 2*pi

#BYTE FSR0L = 0XFE9
#BYTE FSR0H = 0XFEA
#BYTE INDF0 = 0XFEF
#BYTE PORTB = 0XF81
#BYTE PORTD = 0XF83
#BYTE STATUS = 0XFD8

const int8 Lenbuf = 32;

int i;
int j;

int recbuf[Lenbuf];
//int8 sndbuf[Lenbuf];
float temp; //ver si es si o si obligatorio que sea float
long Frec = 0;
int Off = 0;

int1 estado = 1;
//int f = 9;

//Definido como const para ahorrar en RAM, ver si funciona!
const unsigned int8 SENO_ALTA[256] = {128,131,134,137,140,143,146,149,153,156,159,162,
165,168,171,174,177,180,182,185,188,191,194,196,199,201,204,207,209,211,214,216,
218,220,223,225,227,229,231,232,234,236,238,239,241,242,243,245,246,247,248,249,
250,251,252,253,253,254,254,255,255,255,255,255,255,255,255,255,255,254,254,253,
253,252,251,251,250,249,248,247,245,244,243,241,240,238,237,235,233,232,230,228,
226,224,222,219,217,215,213,210,208,205,203,200,198,195,192,189,187,184,181,178,
175,172,169,166,163,160,157,154,151,148,145,142,139,135,132,129,126,123,120,116,
113,110,107,104,101,98,95,92,89,86,83,80,77,74,71,68,66,63,60,57,55,52,50,47,45,
42,40,38,36,33,31,29,27,25,23,22,20,18,17,15,14,12,11,10,8,7,6,5,4,4,3,2,2,1,1,
0,0,0,0,0,0,0,0,0,0,1,1,2,2,3,4,5,6,7,8,9,10,12,13,14,16,17,19,21,23,24,26,28,
30,32,35,37,39,41,44,46,48,51,54,56,59,61,64,67,70,73,75,78,81,84,87,90,93,96,
99,102,106,109,112,115,118,121,124,127};
const unsigned int8 SENO_BAJA[1024] = {128,128,129,130,131,131,132,133,134,135,135,136,
137,138,138,139,140,141,142,142,143,144,145,146,146,147,148,149,149,150,151,152,
152,153,154,155,156,156,157,158,159,159,160,161,162,162,163,164,165,165,166,167,
168,168,169,170,171,171,172,173,174,174,175,176,177,177,178,179,179,180,181,182,
182,183,184,184,185,186,187,187,188,189,189,190,191,191,192,193,193,194,195,195,
196,197,197,198,199,199,200,201,201,202,203,203,204,204,205,206,206,207,208,208,
209,209,210,211,211,212,212,213,214,214,215,215,216,216,217,218,218,219,219,220,
220,221,221,222,222,223,223,224,224,225,226,226,227,227,228,228,228,229,229,230,
230,231,231,232,232,233,233,234,234,234,235,235,236,236,237,237,237,238,238,239,
239,239,240,240,240,241,241,242,242,242,243,243,243,244,244,244,245,245,245,246,
246,246,246,247,247,247,248,248,248,248,249,249,249,249,250,250,250,250,250,251,
251,251,251,252,252,252,252,252,252,253,253,253,253,253,253,254,254,254,254,254,
254,254,254,254,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,254,254,254,254,254,254,254,254,253,253,253,253,253,253,253,
252,252,252,252,252,251,251,251,251,251,250,250,250,250,249,249,249,249,248,248,
248,248,247,247,247,247,246,246,246,245,245,245,244,244,244,243,243,243,242,242,
242,241,241,241,240,240,240,239,239,238,238,238,237,237,236,236,235,235,235,234,
234,233,233,232,232,232,231,231,230,230,229,229,228,228,227,227,226,226,225,225,
224,224,223,223,222,222,221,221,220,219,219,218,218,217,217,216,216,215,214,214,
213,213,212,211,211,210,210,209,208,208,207,207,206,205,205,204,203,203,202,202,
201,200,200,199,198,198,197,196,196,195,194,194,193,192,192,191,190,190,189,188,
188,187,186,185,185,184,183,183,182,181,180,180,179,178,178,177,176,175,175,174,
173,173,172,171,170,170,169,168,167,167,166,165,164,164,163,162,161,161,160,159,
158,157,157,156,155,154,154,153,152,151,151,150,149,148,147,147,146,145,144,144,
143,142,141,140,140,139,138,137,137,136,135,134,133,133,132,131,130,129,129,128,
127,126,126,125,124,123,122,122,121,120,119,118,118,117,116,115,115,114,113,112,
111,111,110,109,108,108,107,106,105,104,104,103,102,101,101,100,99,98,98,97,96,
95,94,94,93,92,91,91,90,89,88,88,87,86,85,85,84,83,82,82,81,80,80,79,78,77,77,76,
75,75,74,73,72,72,71,70,70,69,68,67,67,66,65,65,64,63,63,62,61,61,60,59,59,58,57,
57,56,55,55,54,53,53,52,52,51,50,50,49,48,48,47,47,46,45,45,44,44,43,42,42,41,41,
40,39,39,38,38,37,37,36,36,35,34,34,33,33,32,32,31,31,30,30,29,29,28,28,27,27,26,
26,25,25,24,24,23,23,23,22,22,21,21,20,20,20,19,19,18,18,17,17,17,16,16,15,15,15,
14,14,14,13,13,13,12,12,12,11,11,11,10,10,10,9,9,9,8,8,8,8,7,7,7,7,6,6,6,6,5,5,5,
5,4,4,4,4,4,3,3,3,3,3,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,2,2,2,2,
2,2,3,3,3,3,3,3,4,4,4,4,5,5,5,5,5,6,6,6,6,7,7,7,7,8,8,8,9,9,9,9,10,10,10,11,11,
11,12,12,12,13,13,13,14,14,15,15,15,16,16,16,17,17,18,18,18,19,19,20,20,21,21,21,
22,22,23,23,24,24,25,25,26,26,27,27,27,28,28,29,29,30,31,31,32,32,33,33,34,34,35,
35,36,36,37,37,38,39,39,40,40,41,41,42,43,43,44,44,45,46,46,47,47,48,49,49,50,51,
51,52,52,53,54,54,55,56,56,57,58,58,59,60,60,61,62,62,63,64,64,65,66,66,67,68,68,
69,70,71,71,72,73,73,74,75,76,76,77,78,78,79,80,81,81,82,83,84,84,85,86,87,87,88,
89,90,90,91,92,93,93,94,95,96,96,97,98,99,99,100,101,102,103,103,104,105,106,106,
107,108,109,109,110,111,112,113,113,114,115,116,117,117,118,119,120,120,121,122,
123,124,124,125,126,127,127};

void generar();
//long concatenate();

#INT_EXT NOCLEAR
void EXT_isr(){
    //ESPERO LA NO PRESIÓN DE UNA TECLA
    while(bit_test(PORTB,0)&&bit_test(PORTB,1)&&bit_test(PORTB,2)){delay_ms(200);}
    while(!bit_test(PORTB,0)||!bit_test(PORTB,1)||!bit_test(PORTB,2)){delay_ms(200);}
    //output_toggle(PIN_C2);
    //estado=!estado;
    clear_interrupt(INT_EXT);
    disable_interrupts(INT_EXT);
}

void main(void) 
{

    //clear_interrupt (INT_EXT);
    //ext_int_edge (0,H_TO_L);
    //enable_interrupts (GLOBAL);
    
    //CONFIGURACIÓN DE PUERTOS
    set_tris_a (0b00000000);
    port_b_pullups (TRUE);
    set_tris_b (0x07);
    set_tris_c (0b00000000);
    output_c (0x00);
    set_tris_d (0b00000000);
    output_d (0x80); // 0V del DAC
    set_tris_e (0b00000000);
    output_e (0x00);
    
    //INICIALIZACIÓN DEL DISPLAY
    //lcd_init();
    //delay_ms(200);
    //lcd_clear();
    //gotoxy(0,0);
    
    //while (bit_test(PORTB,0)){delay_ms(200);}
    //while (!bit_test(PORTB,0)){delay_ms(200);}
    
    //===============================================
    
    delay_ms(500);
    Apaga(LED_VERDE);
    Apaga(LED_AMARILLO);
    //Enciende(LED_ROJO);

    usb_init();
    usb_task();
    usb_wait_for_enumeration();

    enable_interrupts(global);

    while (TRUE){
        if(usb_enumerated()){
            //Apaga(LED_ROJO);
            Enciende(LED_VERDE);
            if (usb_kbhit(1)){
                usb_get_packet(1, recbuf, Lenbuf); // Recibe Packet

                if(Control=='1'){ //Se puede generar la señal
                    for(i=0;i<=4;i++){
                        switch(recbuf[i+1]){
                            case '0':
                                temp = 0;
                                break; 
                            case '1':
                                temp = 1;
                                break; 
                            case '2':
                                temp = 2;
                                break; 
                            case '3':
                                temp = 3;
                                break; 
                            case '4':
                                temp = 4;
                                break; 
                            case '5':
                                temp = 5;
                                break; 
                            case '6':
                                temp = 6;
                                break; 
                            case '7':
                                temp = 7;
                                break;
                            case '8':
                                temp = 8;
                                break;
                            case '9':
                                temp = 9;
                                break; 
                        }
                        if(i==0){ //MSB de frecuencia
                            Frec = temp * 10000;
                        }
                        if(i==1){
                            Frec = Frec + temp * 1000;
                        }
                        if(i==2){
                            Frec = Frec + temp * 100;
                        }
                        if(i==3){
                            Frec = Frec + temp * 10;
                        }
                        if(i==4){ //LSB de frecuencia
                            Frec = Frec + temp;
                        }
                    }
                    
                    for(i=0;i<=2;i++){
                        switch(recbuf[i+7]){
                            case '0':
                                temp = 0;
                                break; 
                            case '1':
                                temp = 1;
                                break; 
                            case '2':
                                temp = 2;
                                break; 
                            case '3':
                                temp = 3;
                                break; 
                            case '4':
                                temp = 4;
                                break; 
                            case '5':
                                temp = 5;
                                break; 
                            case '6':
                                temp = 6;
                                break; 
                            case '7':
                                temp = 7;
                                break;
                            case '8':
                                temp = 8;
                                break;
                            case '9':
                                temp = 9;
                                break; 
                        }
                        if(i==0){ //MSB del Offset
                            Off = temp * 100;
                        }
                        if(i==1){
                            Off = Off + temp * 10;
                        }
                        if(i==2){ //LSB del Offset
                            Off = Off + temp;
                        }
                    }
                    if(Offset_3 == '+'){
                        Off = ceil(Off*127/100) + 128;
                        Apaga(WR); //WR debe estar bajo para cambiar de DAC
                        Enciende(DAC_B); //Selecciono el DAC B
                        output_d(Off); //Offset DAC
                        Enciende(WR); //Latch al DAC B (Offset)
                        Apaga(WR); //WR debe estar bajo para cambiar de DAC
                        Apaga(DAC_B); //Apago el DAC B (selecciono el DAC A - Señal)
                    }
                    if(Offset_3 == '-'){
                        Off = ceil(-Off*128/100) + 128;
                        Apaga(WR);
                        Enciende(DAC_B);
                        output_d(Off); // Offset DAC
                        Enciende(WR);
                        Apaga(WR);
                        Apaga(DAC_B);
                    }
                    generar();
                }
                if(Control=='0') // Control es 0 y no se puede generar
                {
                    //continue; //Revisar si no es necesario pausar la señal
                    //poner a cero los dos dac
                    output_d (0x80); // 0V DAC
                }
            }
        }
        else
        {
            Apaga(LED_VERDE);
            delay_ms(20);
            Enciende(LED_VERDE);
            delay_ms(20);
            Apaga(LED_VERDE);
            delay_ms(20);
            Enciende(LED_VERDE);
            delay_ms(20);
            Apaga(LED_VERDE);
            delay_ms(20);
            Enciende(LED_VERDE);
            delay_ms(20);
            Apaga(LED_VERDE);
            delay_ms(20);
            Enciende(LED_VERDE);
            delay_ms(20);
            Apaga(LED_VERDE);
            delay_ms(20);
            Enciende(LED_VERDE);
            delay_ms(20);
            //Enciende(LED_ROJO);
        }
    }
}

void generar()
{
    //output_d (0x80); // 0V del DAC ME PARECE QUE ESTA DUPLICADO, VER SI HACE FALTA PARA DESPUES DE HABER GENERADO UNA SEAÑAL
    /*if (!bit_test(PORTB,1)) { //SUBO FRECUENCIA
        while(!bit_test(PORTB,0) || !bit_test(PORTB,1) || !bit_test(PORTB,2)){delay_ms(200);}
        if (f <= 18){
            f +=1;
            //lcd_clear();
            //gotoxy(10,3);
            //printf(print_char,"%s",frecuencia[f]);
        }
    }
    if (!bit_test(PORTB,2)){ //BAJO FRECUENCIA
        while(!bit_test(PORTB,0) || !bit_test(PORTB,1) || !bit_test(PORTB,2)){delay_ms(200);}
        if (f >= 0){
            f -= 1;
            //lcd_clear();
            //gotoxy(10,3);
            //printf(print_char,"%s",frecuencia[f]);
        }
    }
    if (!bit_test(PORTB,0)) { //SELECCIONO LA FRECUENCIA
        estado = 0;
    }*/
    
    clear_interrupt(INT_EXT);
    enable_interrupts (INT_EXT);
    
    //GENERAR EL OFFSET
    
    
    switch(Tipo){
        case '0': //Señal senoidal
            Enciende(LED_AMARILLO);
            delay_ms(200);
            Apaga(LED_AMARILLO);
            delay_ms(500);
            
            while(1){
                //switch(Frec){
                if(Frec >= 0 && Frec <= 49){
                    //case 0: //0 Hz
                        RETORNO_0:
                            output_d (0x80);
                        GOTO RETORNO_0;}
                       //break;
                    //case 100: //100 Hz
                if(Frec >= 50 && Frec <=149){
                        #ASM ASIS
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            
                            RETORNO_100:
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCFSZ FSR0L, 1
                            GOTO SALTO_1_100
                            INCF FSR0H, 1
                            MOVLW 6
                            SUBWF FSR0H, 0
                            BTFSS STATUS, 2
                            GOTO SALTO_1_100
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            SALTO_1_100:
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCFSZ FSR0L, 1
                            GOTO SALTO_2_100
                            INCF FSR0H, 1
                            MOVLW 6
                            SUBWF FSR0H, 0
                            BTFSS STATUS, 2
                            GOTO SALTO_2_100
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            SALTO_2_100:
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCFSZ FSR0L, 1
                            GOTO SALTO_3_100
                            INCF FSR0H, 1
                            MOVLW 6
                            SUBWF FSR0H, 0
                            BTFSS STATUS, 2
                            GOTO SALTO_3_100
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            SALTO_3_100:
                            
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            GOTO RETORNO_100
                        #ENDASM}
                        //break;
                    //case 200: //200 Hz
                if(Frec >= 150 && Frec <=249){
                        #ASM ASIS
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            
                            RETORNO_200:
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCFSZ FSR0L, 1
                            GOTO SALTO_1_200
                            INCF FSR0H, 1
                            MOVLW 6
                            SUBWF FSR0H, 0
                            BTFSS STATUS, 2
                            GOTO SALTO_1_200
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            SALTO_1_200:
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCFSZ FSR0L, 1
                            GOTO SALTO_2_200
                            INCF FSR0H, 1
                            MOVLW 6
                            SUBWF FSR0H, 0
                            BTFSS STATUS, 2
                            GOTO SALTO_2_200
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            SALTO_2_200:
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCFSZ FSR0L, 1
                            GOTO SALTO_3_200
                            INCF FSR0H, 1
                            MOVLW 6
                            SUBWF FSR0H, 0
                            BTFSS STATUS, 2
                            GOTO SALTO_3_200
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            SALTO_3_200:
                            
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            GOTO RETORNO_200
                        #ENDASM}
                        //break;
                    //case 300: //300 Hz
                if(Frec >= 250 && Frec <=349){
                        #ASM ASIS
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            
                            RETORNO_300:
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCFSZ FSR0L, 1
                            GOTO SALTO_1_300
                            INCF FSR0H, 1
                            MOVLW 6
                            SUBWF FSR0H, 0
                            BTFSS STATUS, 2
                            GOTO SALTO_1_300
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            SALTO_1_300:
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCFSZ FSR0L, 1
                            GOTO SALTO_2_300
                            INCF FSR0H, 1
                            MOVLW 6
                            SUBWF FSR0H, 0
                            BTFSS STATUS, 2
                            GOTO SALTO_2_300
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            SALTO_2_300:
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCFSZ FSR0L, 1
                            GOTO SALTO_3_300
                            INCF FSR0H, 1
                            MOVLW 6
                            SUBWF FSR0H, 0
                            BTFSS STATUS, 2
                            GOTO SALTO_3_300
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            SALTO_3_300:
                            
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP 
                            NOP
                            GOTO RETORNO_300
                        #ENDASM}
                        //break;
                if(Frec >= 350 && Frec <=449){
                    //case 400: //400 Hz
                        #ASM ASIS
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            
                            RETORNO_400:
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCFSZ FSR0L, 1
                            GOTO SALTO_1_400
                            INCF FSR0H, 1
                            MOVLW 6
                            SUBWF FSR0H, 0
                            BTFSS STATUS, 2
                            GOTO SALTO_1_400
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            SALTO_1_400:
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCFSZ FSR0L, 1
                            GOTO SALTO_2_400
                            INCF FSR0H, 1
                            MOVLW 6
                            SUBWF FSR0H, 0
                            BTFSS STATUS, 2
                            GOTO SALTO_2_400
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            SALTO_2_400:
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCFSZ FSR0L, 1
                            GOTO SALTO_3_400
                            INCF FSR0H, 1
                            MOVLW 6
                            SUBWF FSR0H, 0
                            BTFSS STATUS, 2
                            GOTO SALTO_3_400
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            SALTO_3_400:
                            
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP
                            GOTO RETORNO_400
                        #ENDASM}
                        //break;
                if(Frec >= 450 && Frec <=549){
                    //case 500: //500 Hz
                        #ASM ASIS
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            
                            RETORNO_500:
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCFSZ FSR0L, 1
                            GOTO SALTO_1_500
                            INCF FSR0H, 1
                            MOVLW 6
                            SUBWF FSR0H, 0
                            BTFSS STATUS, 2
                            GOTO SALTO_1_500
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            SALTO_1_500:
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCFSZ FSR0L, 1
                            GOTO SALTO_2_500
                            INCF FSR0H, 1
                            MOVLW 6
                            SUBWF FSR0H, 0
                            BTFSS STATUS, 2
                            GOTO SALTO_2_500
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            SALTO_2_500:
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCFSZ FSR0L, 1
                            GOTO SALTO_3_500
                            INCF FSR0H, 1
                            MOVLW 6
                            SUBWF FSR0H, 0
                            BTFSS STATUS, 2
                            GOTO SALTO_3_500
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            SALTO_3_500:
                            
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP
                            GOTO RETORNO_500
                        #ENDASM}
                        //break;
                if(Frec >= 550 && Frec <=649){
                    //case 600: //600 Hz
                        #ASM ASIS
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            
                            RETORNO_600:
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCFSZ FSR0L, 1
                            GOTO SALTO_1_600
                            INCF FSR0H, 1
                            MOVLW 6
                            SUBWF FSR0H, 0
                            BTFSS STATUS, 2
                            GOTO SALTO_1_600
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            SALTO_1_600:
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCFSZ FSR0L, 1
                            GOTO SALTO_2_600
                            INCF FSR0H, 1
                            MOVLW 6
                            SUBWF FSR0H, 0
                            BTFSS STATUS, 2
                            GOTO SALTO_2_600
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            SALTO_2_600:
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCFSZ FSR0L, 1
                            GOTO SALTO_3_600
                            INCF FSR0H, 1
                            MOVLW 6
                            SUBWF FSR0H, 0
                            BTFSS STATUS, 2
                            GOTO SALTO_3_600
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            SALTO_3_600:
                            
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            GOTO RETORNO_600
                        #ENDASM}
                        //break;
                if(Frec >= 650 && Frec <=749){
                    //case 700: //700 Hz
                        #ASM ASIS
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            
                            RETORNO_700:
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCFSZ FSR0L, 1
                            GOTO SALTO_1_700
                            INCF FSR0H, 1
                            MOVLW 6
                            SUBWF FSR0H, 0
                            BTFSS STATUS, 2
                            GOTO SALTO_1_700
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            SALTO_1_700:
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCFSZ FSR0L, 1
                            GOTO SALTO_2_700
                            INCF FSR0H, 1
                            MOVLW 6
                            SUBWF FSR0H, 0
                            BTFSS STATUS, 2
                            GOTO SALTO_2_700
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            SALTO_2_700:
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCFSZ FSR0L, 1
                            GOTO SALTO_3_700
                            INCF FSR0H, 1
                            MOVLW 6
                            SUBWF FSR0H, 0
                            BTFSS STATUS, 2
                            GOTO SALTO_3_700
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            SALTO_3_700:
                            
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            GOTO RETORNO_700
                        #ENDASM}
                        //break;
                if(Frec >= 750 && Frec <=849){
                    //case 800: //800 Hz
                        #ASM ASIS
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            
                            RETORNO_800:
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCFSZ FSR0L, 1
                            GOTO SALTO_1_800
                            INCF FSR0H, 1
                            MOVLW 6
                            SUBWF FSR0H, 0
                            BTFSS STATUS, 2
                            GOTO SALTO_1_800
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            SALTO_1_800:
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCFSZ FSR0L, 1
                            GOTO SALTO_2_800
                            INCF FSR0H, 1
                            MOVLW 6
                            SUBWF FSR0H, 0
                            BTFSS STATUS, 2
                            GOTO SALTO_2_800
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            SALTO_2_800:
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCFSZ FSR0L, 1
                            GOTO SALTO_3_800
                            INCF FSR0H, 1
                            MOVLW 6
                            SUBWF FSR0H, 0
                            BTFSS STATUS, 2
                            GOTO SALTO_3_800
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            SALTO_3_800:
                            
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            GOTO RETORNO_800
                        #ENDASM}
                        //break;
                if(Frec >= 850 && Frec <=949){
                    //case 900: //900 Hz
                        #ASM ASIS
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            
                            RETORNO_900:
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCFSZ FSR0L, 1
                            GOTO SALTO_1_900
                            INCF FSR0H, 1
                            MOVLW 6
                            SUBWF FSR0H, 0
                            BTFSS STATUS, 2
                            GOTO SALTO_1_900
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            SALTO_1_900:
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCFSZ FSR0L, 1
                            GOTO SALTO_2_900
                            INCF FSR0H, 1
                            MOVLW 6
                            SUBWF FSR0H, 0
                            BTFSS STATUS, 2
                            GOTO SALTO_2_900
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            SALTO_2_900:
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCFSZ FSR0L, 1
                            GOTO SALTO_3_900
                            INCF FSR0H, 1
                            MOVLW 6
                            SUBWF FSR0H, 0
                            BTFSS STATUS, 2
                            GOTO SALTO_3_900
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            SALTO_3_900:
                            
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP
                            GOTO RETORNO_900
                        #ENDASM}
                        //break;
                if(Frec >= 950 && Frec <=1499){
                    //case 1000: //1000 Hz
                        #ASM ASIS
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            
                            RETORNO_1000:
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCFSZ FSR0L, 1
                            GOTO SALTO_1_1000
                            INCF FSR0H, 1
                            MOVLW 6
                            SUBWF FSR0H, 0
                            BTFSS STATUS, 2
                            GOTO SALTO_1_1000
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            SALTO_1_1000:
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCFSZ FSR0L, 1
                            GOTO SALTO_2_1000
                            INCF FSR0H, 1
                            MOVLW 6
                            SUBWF FSR0H, 0
                            BTFSS STATUS, 2
                            GOTO SALTO_2_1000
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            SALTO_2_1000:
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCFSZ FSR0L, 1
                            GOTO SALTO_3_1000
                            INCF FSR0H, 1
                            MOVLW 6
                            SUBWF FSR0H, 0
                            BTFSS STATUS, 2
                            GOTO SALTO_3_1000
                            MOVLW 2
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            SALTO_3_1000:
                            
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            GOTO RETORNO_1000
                        #ENDASM}
                        //break;
                if(Frec >= 1500 && Frec <=2499){
                    //case 2000: //2000 Hz
                        #ASM ASIS
                        MOVLW 1
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            
                            RETORNO_2000:
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                                               
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            GOTO RETORNO_2000
                        #ENDASM}
                        //break;
                if(Frec >= 2500 && Frec <=3499){
                    //case 3000: //3000 Hz
                        #ASM ASIS
                            MOVLW 1
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            
                            RETORNO_3000:
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP      
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP      
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP
                            GOTO RETORNO_3000
                        #ENDASM}
                        //break;
                if(Frec >= 3500 && Frec <=4499){
                    //case 4000: //4000 Hz
                        #ASM ASIS
                            MOVLW 1
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            
                            RETORNO_4000:
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                                               
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP
                            GOTO RETORNO_4000
                        #ENDASM}
                        //break;
                if(Frec >= 4500 && Frec <=5499){
                   // case 5000: //5000 Hz
                        #ASM ASIS
                            MOVLW 1
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            
                            RETORNO_5000:
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                           
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            GOTO RETORNO_5000
                        #ENDASM}
                        //break;
                if(Frec >= 5500 && Frec <=6499){
                    //case 6000: //6000 Hz
                        #ASM ASIS
                            MOVLW 1
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            
                            RETORNO_6000:
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                                               
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP
                            GOTO RETORNO_6000
                        #ENDASM}
                        //break;
                if(Frec >= 6500 && Frec <=7499){
                    //case 7000: //7000 Hz
                        #ASM ASIS
                            MOVLW 1
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            
                            RETORNO_7000:
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                                               
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            GOTO RETORNO_7000
                        #ENDASM}
                        //break;
                if(Frec >= 7500 && Frec <=8499){
                    //case 8000: //8000 Hz
                        #ASM ASIS
                            MOVLW 1
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            
                            RETORNO_8000:
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                                               
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            GOTO RETORNO_8000
                        #ENDASM}
                        //break;
                if(Frec >= 8500 && Frec <=9499){
                    //case 9000: //9000 Hz
                        #ASM ASIS
                            MOVLW 1
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            
                            RETORNO_9000:
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                                                
                            NOP NOP NOP
                            NOP NOP NOP
                            NOP NOP NOP
                            GOTO RETORNO_9000                
                        #ENDASM}
                        //break;
                if(Frec >= 9500){
                    //case 10000: //10000 Hz
                        #ASM ASIS
                            MOVLW 1
                            MOVWF FSR0H
                            MOVLW 0
                            MOVWF FSR0L
                            
                            RETORNO_10000:
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                            
                            MOVF INDF0, W
                            MOVWF PORTD
                            INCF FSR0L
                                                
                            NOP NOP NOP
                            GOTO RETORNO_10000
                        #ENDASM}
                        //break;
                }
            break;
            
        case '1': //Señal triangular
            Enciende(LED_AMARILLO);
            delay_ms(200);
            Apaga(LED_AMARILLO);
            delay_ms(200);
            
            Enciende(LED_AMARILLO);
            delay_ms(200);
            Apaga(LED_AMARILLO);
            delay_ms(500);
            
            while(TRUE){
                for(i=0;i<=255;i++){
                    output_d(i);
                }
                for(i=255;i>=0;i--){
                    output_d(i);
                }
            }
            break;
        
        case '2': //Señal rampa
            Enciende(LED_AMARILLO);
            delay_ms(200);
            Apaga(LED_AMARILLO);
            delay_ms(200);
            
            Enciende(LED_AMARILLO);
            delay_ms(200);
            Apaga(LED_AMARILLO);
            delay_ms(200);
            
            Enciende(LED_AMARILLO);
            delay_ms(200);
            Apaga(LED_AMARILLO);
            delay_ms(500);
            
            while(TRUE){
                for(j=0;j<=255;j++){
                    output_d(i); //Se podrian poner la lista de valores para lograr la f mas rapida
                }
            }            
            break;
            
        case '3': //Señal cuadrada
            Enciende(LED_AMARILLO);
            delay_ms(200);
            Apaga(LED_AMARILLO);
            delay_ms(200);
            
            Enciende(LED_AMARILLO);
            delay_ms(200);
            Apaga(LED_AMARILLO);
            delay_ms(200);
            
            Enciende(LED_AMARILLO);
            delay_ms(200);
            Apaga(LED_AMARILLO);
            delay_ms(200);
            
            Enciende(LED_AMARILLO);
            delay_ms(200);
            Apaga(LED_AMARILLO);
            delay_ms(500);
            
            while(Estado){
                output_d(0x00);
                output_d(0xFF);
            }
            
            break;
    }
}
