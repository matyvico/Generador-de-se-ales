
//DEFINICION DE UNA  ESTRUCTURA
typedef union {                                            //SE DEFINE UNA UNION PARA PODER MANEJAR UNA MISMA VARIABLE
   unsigned char _byte;                            //POR BYTE Y TAMBIEN POR BIT INDIVIDUAL
   struct {
     short _0:1;
     short _1:1;
     short _2:1;
     short _3:1;
     short _4:1;
     short _5:1;
     short _6:1;
     short _7:1;
   };
}DUAL;

DUAL PORTA,PORTB,PORTC;

#byte TRISC = 0xF94
#byte TRISB = 0xF93
#byte TRISA = 0xF92
#byte LATC = 0xF8B
#byte LATB = 0xF8A
#byte LATA = 0xF89
#byte PORTE = 0xF84
#byte PORTC = 0xF82
#byte PORTB = 0xF81
#byte PORTA = 0xF80

#byte TOSU = 0xFFF
#bit    TOSU0 = TOSU.0
#bit    TOSU1 = TOSU.1
#bit    TOSU2 = TOSU.2
#bit    TOSU3 = TOSU.3
#bit    TOSU4 = TOSU.4
#byte TOSH = 0xFFE
#byte TOSL = 0xFFD
#byte STKPTR = 0xFFC
#bit    STKPTR0 = STKPTR.0
#bit    STKPTR1 = STKPTR.1
#bit    STKPTR2 = STKPTR.2
#bit    STKPTR3 = STKPTR.3
#bit    STKPTR4 = STKPTR.4
#bit    STKUNF = STKPTR.6
#bit    STKFUL = STKPTR.7
#byte PCLATU = 0xFFB
#bit    PCU0 = PCLATU.0
#bit    PCU1 = PCLATU.1
#bit    PCU2 = PCLATU.2
#bit    PCU3 = PCLATU.3
#bit    PCU4 = PCLATU.4
#byte PCLATH = 0xFFA
#byte PCL = 0xFF9
#byte TBLPTRU = 0xFF8
#bit    TBLPTRU0 = TBLPTRU.0
#bit    TBLPTRU1 = TBLPTRU.1
#bit    TBLPTRU2 = TBLPTRU.2
#bit    TBLPTRU3 = TBLPTRU.3
#bit    TBLPTRU4 = TBLPTRU.4
#byte TBLPTRH = 0xFF7
#byte TBLPTRL = 0xFF6
#byte TABLAT = 0xFF5
#byte PRODH = 0xFF4
#byte PRODL = 0xFF3
#byte INTCON = 0xFF2
#bit    RBIF = INTCON.0
#bit    INT0IF = INTCON.1
#bit    TMR0IF = INTCON.2
#bit    RBIE = INTCON.3
#bit    INT0IE = INTCON.4
#bit    TMR0IE = INTCON.5
#bit    PEIE = INTCON.6
#bit    GIE = INTCON.7
#byte INTCON2 = 0xFF1
#bit    RBIP = INTCON2.0
#bit    TMR0IP = INTCON2.2
#bit    INTEDG2 = INTCON2.4
#bit    INTEDG1 = INTCON2.5
#bit    INTEDG0 = INTCON2.6
#bit    RBPU = INTCON2.7
#byte INTCON3 = 0xFF0
#bit    INT1IF = INTCON3.0
#bit    INT2IF = INTCON3.1
#bit    INT1IE = INTCON3.3
#bit    INT2IE = INTCON3.4
#bit    INT1IP = INTCON3.6
#bit    INT2IP = INTCON3.7
#byte INDF0 = 0xFEF
#byte POSTINC0 = 0xFEE
#byte POSTDEC0 = 0xFED
#byte PREINC0 = 0xFEC
#byte PLUSW0 = 0xFEB
#byte FSR0H = 0xFEA
#bit    FSR0H0 = FSR0H.0
#bit    FSR0H1 = FSR0H.1
#bit    FSR0H2 = FSR0H.2
#bit    FSR0H3 = FSR0H.3
#byte FSR0L = 0xFE9
#byte WREG = 0xFE8
#byte INDF1 = 0xFE7
#byte POSTINC1 = 0xFE6
#byte POSTDEC1 = 0xFE5
#byte PREINC1 = 0xFE4
#byte PLUSW1 = 0xFE3
#byte FSR1H = 0xFE2
#bit    FSR1H0 = FSR1H.0
#bit    FSR1H1 = FSR1H.1
#bit    FSR1H2 = FSR1H.2
#bit    FSR1H3 = FSR1H.3
#byte FSR1L = 0xFE1
#byte BSR = 0xFE0
#bit    BSR0 = BSR.0
#bit    BSR1 = BSR.1
#bit    BSR2 = BSR.2
#bit    BSR3 = BSR.3
#byte INDF2 = 0xFDF
#byte POSTINC2 = 0xFDE
#byte POSTDEC2 = 0xFDD
#byte PREINC2 = 0xFDC
#byte PLUSW2 = 0xFDB
#byte FSR2H = 0xFDA
#bit    FSR2H0 = FSR2H.0
#bit    FSR2H1 = FSR2H.1
#bit    FSR2H2 = FSR2H.2
#bit    FSR2H3 = FSR2H.3
#byte FSR2L = 0xFD9
#byte STATUS = 0xFD8
#bit    DC = STATUS.1
#bit    OV = STATUS.3
#byte TMR0H = 0xFD7
#byte TMR0L = 0xFD6
#byte T0CON = 0xFD5
#bit    T0PS0 = T0CON.0
#bit    T0PS1 = T0CON.1
#bit    T0PS2 = T0CON.2
#bit    PSA = T0CON.3
#bit    T0SE = T0CON.4
#bit    T0CS = T0CON.5
#bit    T08BIT = T0CON.6
#bit    TMR0ON = T0CON.7
#byte OSCCON = 0xFD3
#bit    SCS0 = OSCCON.0
#bit    SCS1 = OSCCON.1
#bit    IOFS = OSCCON.2
#bit    OSTS = OSCCON.3
#bit    IRCF0 = OSCCON.4
#bit    IRCF1 = OSCCON.5
#bit    IRCF2 = OSCCON.6
#bit    IDLEN = OSCCON.7
#byte HLVDCON = 0xFD2
#bit    HLVDL0 = HLVDCON.0
#bit    HLVDL1 = HLVDCON.1
#bit    HLVDL2 = HLVDCON.2
#bit    HLVDL3 = HLVDCON.3
#bit    HLVDEN = HLVDCON.4
#bit    IRVST = HLVDCON.5
#bit    VDIRMAG = HLVDCON.7
#byte WDTCON = 0xFD1
#bit    SWDTEN = WDTCON.0
#byte RCON = 0xFD0
#bit    BOR = RCON.0
#bit    POR = RCON.1
#bit    PD = RCON.2
#bit    TO = RCON.3
#bit    RI = RCON.4
#bit    SBOREN = RCON.6
#bit    IPEN = RCON.7
#byte TMR1H = 0xFCF
#byte TMR1L = 0xFCE
#byte T1CON = 0xFCD
#bit    TMR1ON = T1CON.0
#bit    TMR1CS = T1CON.1
#bit    T1SYNC = T1CON.2
#bit    T1OSCEN = T1CON.3
#bit    T1CKPS0 = T1CON.4
#bit    T1CKPS1 = T1CON.5
#bit    T1RUN = T1CON.6
#bit    RD16 = T1CON.7
#byte TMR2 = 0xFCC
#byte PR2 = 0xFCB
#byte T2CON = 0xFCA
#bit    T2CKPS0 = T2CON.0
#bit    T2CKPS1 = T2CON.1
#bit    TMR2ON = T2CON.2
#bit    TOUTPS0 = T2CON.3
#bit    TOUTPS1 = T2CON.4
#bit    TOUTPS2 = T2CON.5
#bit    TOUTPS3 = T2CON.6
#byte SSPBUF = 0xFC9
#byte SSPADD = 0xFC8
#byte SSPSTAT = 0xFC7
#bit    BF = SSPSTAT.0
#bit    UA = SSPSTAT.1
#bit    CKE = SSPSTAT.6
#bit    SMP = SSPSTAT.7
#byte SSPCON1 = 0xFC6
#bit    SSPM0 = SSPCON1.0
#bit    SSPM1 = SSPCON1.1
#bit    SSPM2 = SSPCON1.2
#bit    SSPM3 = SSPCON1.3
#bit    CKP = SSPCON1.4
#bit    SSPEN = SSPCON1.5
#bit    SSPOV = SSPCON1.6
#bit    WCOL = SSPCON1.7
#byte SSPCON2 = 0xFC5
#bit    SEN = SSPCON2.0
#bit    RSEN = SSPCON2.1
#bit    PEN = SSPCON2.2
#bit    RCEN = SSPCON2.3
#bit    ACKEN = SSPCON2.4
#bit    ACKDT = SSPCON2.5
#bit    ACKSTAT = SSPCON2.6
#bit    GCEN = SSPCON2.7
#byte ADRESH = 0xFC4
#byte ADRESL = 0xFC3
#byte ADCON0 = 0xFC2
#bit    ADON = ADCON0.0
#bit    GO = ADCON0.1
#bit    CHS0 = ADCON0.2
#bit    CHS1 = ADCON0.3
#bit    CHS2 = ADCON0.4
#bit    CHS3 = ADCON0.5
#byte ADCON1 = 0xFC1
#bit    PCFG0 = ADCON1.0
#bit    PCFG1 = ADCON1.1
#bit    PCFG2 = ADCON1.2
#bit    PCFG3 = ADCON1.3
#bit    VCFG0 = ADCON1.4
#bit    VCFG1 = ADCON1.5
#byte ADCON2 = 0xFC0
#bit    ADCS0 = ADCON2.0
#bit    ADCS1 = ADCON2.1
#bit    ADCS2 = ADCON2.2
#bit    ACQT0 = ADCON2.3
#bit    ACQT1 = ADCON2.4
#bit    ACQT2 = ADCON2.5
#bit    ADFM = ADCON2.7
#byte CCPR1H = 0xFBF
#byte CCPR1L = 0xFBE
#byte CCP1CON = 0xFBD
#bit    CCP1M0 = CCP1CON.0
#bit    CCP1M1 = CCP1CON.1
#bit    CCP1M2 = CCP1CON.2
#bit    CCP1M3 = CCP1CON.3
#bit    DC1B0 = CCP1CON.4
#bit    DC1B1 = CCP1CON.5
#byte CCPR2H = 0xFBC
#byte CCPR2L = 0xFBB
#byte CCP2CON = 0xFBA
#bit    CCP2M0 = CCP2CON.0
#bit    CCP2M1 = CCP2CON.1
#bit    CCP2M2 = CCP2CON.2
#bit    CCP2M3 = CCP2CON.3
#bit    DC2B0 = CCP2CON.4
#bit    DC2B1 = CCP2CON.5
#byte BAUDCON = 0xFB8
#bit    ABDEN = BAUDCON.0
#bit    WUE = BAUDCON.1
#bit    BRG16 = BAUDCON.3
#bit    TXCKP = BAUDCON.4
#bit    RXDTP = BAUDCON.5
#bit    RCMT = BAUDCON.6
#bit    ABDOVF = BAUDCON.7
#byte ECCP1DEL = 0xFB7
#bit    PRSEN = ECCP1DEL.7
#byte ECCP1AS = 0xFB6
#bit    PSSAC0 = ECCP1AS.2
#bit    PSSAC1 = ECCP1AS.3
#bit    ECCPAS0 = ECCP1AS.4
#bit    ECCPAS1 = ECCP1AS.5
#bit    ECCPAS2 = ECCP1AS.6
#bit    ECCPASE = ECCP1AS.7
#byte CVRCON = 0xFB5
#bit    CVR0 = CVRCON.0
#bit    CVR1 = CVRCON.1
#bit    CVR2 = CVRCON.2
#bit    CVR3 = CVRCON.3
#bit    CVRSS = CVRCON.4
#bit    CVRR = CVRCON.5
#bit    CVROE = CVRCON.6
#bit    CVREN = CVRCON.7
#byte CMCON = 0xFB4
#bit    CM0 = CMCON.0
#bit    CM1 = CMCON.1
#bit    CM2 = CMCON.2
#bit    CIS = CMCON.3
#bit    C1INV = CMCON.4
#bit    C2INV = CMCON.5



unsigned int16 Alux_ADC_1(void) {
unsigned int8 BK_TRISA,BK_ADCON1;
      
        BK_TRISA=TRISA;
        BK_ADCON1=ADCON1;
        TRISA=TRISA|0x01;
        setup_adc(ADC_CLOCK_INTERNAL);             //ESTABLECE A INTERNO, EL RELOJ DEL ADC
        setup_adc_ports(AN0);                      //CONFIGURA EL PIN A0 COMO ENTRADA ANALÓGICA (AN0)
        set_adc_channel(0);                        //SELECCIONA EL CANAL AN0
        delay_us(10);
        ADCON1=BK_ADCON1;
        TRISA=BK_TRISA;
        
return  read_adc();
}

unsigned int16 Alux_ADC_2(void) {
unsigned int8 BK_TRISA,BK_ADCON1;

        BK_TRISA=TRISA;
        BK_ADCON1=ADCON1;
        TRISA=TRISA|0x20;
        setup_adc(ADC_CLOCK_INTERNAL);                 //ESTABLECE A INTERNO, EL RELOJ DEL ADC
        setup_adc_ports(AN0_TO_AN4);                   //CONFIGURA EL PIN A0 A ENTRADA ANALÓGICA (AN0)
        set_adc_channel(4);                            //SELECCIONA EL CANAL AN4 PARA LA SIGUIENTES LECTURAS
        delay_us(10);
        ADCON1=BK_ADCON1;
        TRISA=BK_TRISA;

return  read_adc();
}

float Alux_pwm_init(char TimerPrescaler, char PR2) {
float value,f_PR2,f_TmrPr;

   switch (TimerPrescaler) {
      case 0:  setup_timer_2(T2_DISABLED,PR2,1);  break;
      case 1:  setup_timer_2(T2_DIV_BY_1,PR2,1);  break;
      case 4:  setup_timer_2(T2_DIV_BY_4,PR2,1);  break;
      case 16: setup_timer_2(T2_DIV_BY_16,PR2,1); break;
      default: setup_timer_2(T2_DISABLED,PR2,1);  break;
   }
   setup_comparator(NC_NC_NC_NC);               // Se apagan los modulos comparadores.
   
   value=getenv("CLOCK");
   f_PR2=PR2;
   f_TmrPr=TimerPrescaler;
   return value/((f_PR2+1)*4*f_TmrPr);
}

void Alux_pwm1(unsigned int16 Duty) {  //Duty va de 0 a 1023 Máximo
unsigned int8 HighDuty,LowDuty;     
unsigned int16 NewDuty;

   setup_ccp1(CCP_PWM);             //pwm 1
   NewDuty=Duty<<6;                 //el bit 9 pasa a ser bit 15, bit 0 pasa al bit 6.
   HighDuty=MAKE8(NewDuty,1);       //8 bits altos destinados a CCPRxL
   LowDuty= MAKE8(NewDuty,0);       //parte baja de NewDuty con 2 bits: 7 y 6.
   LowDuty= (LowDuty>>2)&0x30;      //bit 7 y 6 son ahora bit 5 y 4 destinados a CCPxCON<5:4>.
   CCPR1L=HighDuty;
   CCP1CON=(CCP1CON&0xCF)|LowDuty;
}

void Alux_pwm2(unsigned int16 Duty) {  //Duty va de 0 a 1023 Máximo
unsigned int8 HighDuty,LowDuty;     
unsigned int16 NewDuty;

   setup_ccp2(CCP_PWM);             //pwm 2
   NewDuty=Duty<<6;                 //el bit 9 pasa a ser bit 15, bit 0 pasa al bit 6.
   HighDuty=MAKE8(NewDuty,1);       //8 bits altos destinados a CCPRxL
   LowDuty= MAKE8(NewDuty,0);       //parte baja de NewDuty con 2 bits: 7 y 6.
   LowDuty= (LowDuty>>2)&0x30;      //bit 7 y 6 son ahora bit 5 y 4 destinados a CCPxCON<5:4>.
   CCPR2L=HighDuty;
   CCP2CON=(CCP2CON&0xCF)|LowDuty;
}
