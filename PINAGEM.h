/**
 * @file PINAGEM.h
 * @brief Definição da pinagem do microcontrolador
 * @author Ricardo Estefano Rosa (restefano)
 * @date 09/08/14
 * <p>(C) Copyright <b> WEG CRITICAL POWER </b> - Todos os direitos reservados.</p>
 * 
 */

#ifndef PINAGEM_H
#define	PINAGEM_H

#include <PIC18F46K80.H>

/* ===================== */
/* DEFINIÇÕES DA PORTA A */
/* ===================== */

/// Não Conectado
#define _RA7_NC   PORTAbits.RA7 /// Não Conectado

/// Não Conectado
#define _RA6_NC   PORTAbits.RA6

/// Bit 0 do Endereço e LED 1 do Painel
#define _ADDR0     PORTAbits.RA5
#define _LED1      PORTAbits.RA5
#define _LED1_TRIS TRISAbits.TRISA5
#define Desliga_LED1() _LED1=0;LATA5=0;
#define Liga_LED1() _LED1=1;LATA5=1;

/// Usado como VDDCORE/VCAP

#define _VDDCORE  PORTAbits.RA4 
/// Referência Analógica 5.00V

#define _ANREF    PORTAbits.RA3 
/// Amostra da temperatura interna (10mV por grau Celsius)

#define _ATI      PORTAbits.RA2
/// Amostra da corrente de saída (XXXmV por Ampère)

#define _ACS      PORTAbits.RA1 
/// Amostra da tensão de saída (40mV por decimal)

#define _ATS      PORTAbits.RA0 

/// Direçao dos pinos da porta (1 = Entrada, 0 = Saída)
#define INICIAL_TRISA   0b11111111  
/// Estado inicial dos pinos da porta (1 = Ligado, 0 = Desligado)
#define INICIAL_PORTA   0b00000000  // Estado inicial


/* ===================== */
/* DEFINIÇÕES DA PORTA B */
/* ===================== */

/// Dado da programação In-Circuit
#define _PGD      PORTBbits.RB7

/// Clock da programação in-circuit
#define _PGC      PORTBbits.RB6

/// LED 8 (Painel)
#define _LED8     PORTBbits.RB5
#define Desliga_LED8() _LED8=0;LATB5=0;
#define Liga_LED8() _LED8=1;LATB5=1;

/// LED 7 (Painel)
#define _LED7     PORTBbits.RB4
#define Desliga_LED7() _LED7=0;LATB4=0;
#define Liga_LED7() _LED7=1;LATB4=1;

/// RX da interface CAN
#define _CANRX    PORTBbits.RB3

/// TX da interface CAN
#define _CANTX    PORTBbits.RB2

/// Bit 1 do Endereço / LED 2 (Painel)
#define _ADDR1    PORTBbits.RB1
#define _LED2     PORTBbits.RB1
#define _LED2_TRIS TRISBbits.TRISB1
#define Desliga_LED2() _LED2=0;LATB1=0;
#define Liga_LED2() _LED2=1;LATB1=1;

/// Termostato -> 0=OK; 1 = SOBRETEMPERATURA!
#define _TMTT     PORTBbits.RB0 

/// Direçao dos pinos da porta (1 = Entrada, 0 = Saída)
#define INICIAL_TRISB   0b11001011
/// Estado inicial dos pinos da porta (1 = Ligado, 0 = Desligado)
#define INICIAL_PORTB   0b00000000  

/* ===================== */
/* DEFINIÇÕES DA PORTA C */
/* ===================== */

/// RX da interface 485
#define _RXD        PORTCbits.RC7

/// TX da interface 485
#define _TXD        PORTCbits.RC6

/// DAT do ICSP / LED 5 (Painel)
#define _DAT        PORTCbits.RC5
#define _LED5       PORTCbits.RC5
#define _LED5_TRIS  TRISCbits.TRISC5
#define Desliga_LED5() _LED5=0;LATC5=0;
#define Liga_LED5() _LED5=1;LATC5=1;

/// Bit 2 do Endereço e DI da interface 485
#define _ADDR2      PORTCbits.RC4
#define _DI         PORTCbits.RC4
#define _DI_TRIS    TRISCbits.TRISC4
#define Desliga_DI() _DI=0;LATC4=0;
#define Liga_DI() _DI=1;LATC4=1;

/// CLK do ICSP / LED 4 (Painel)
#define _CLK        PORTCbits.RC3
#define _LED4       PORTCbits.RC3
#define _LED4_TRIS TRISCbits.TRISC3
#define Desliga_LED4() _LED4=0;LATC3=0;
#define Liga_LED4() _LED4=1;LATC3=1;

/// Saída PWM 2
#define _PWM2       PORTCbits.RC2

/// Indicador de Pré-Carga. 1=OK!
#define _PCOK       PORTCbits.RC1 

/// LED DEFEITO - VERDE
#define _LED_DEF_VD PORTCbits.RC0
#define Desliga_LED_DEF_VD() _LED_DEF_VD=0;LATC0=0;
#define Liga_LED_DEF_VD() _LED_DEF_VD=1;LATC0=1;

/// Direçao dos pinos da porta (1 = Entrada, 0 = Saída)
#define INICIAL_TRISC   0b10111010  
/// Estado inicial dos pinos da porta (1 = Ligado, 0 = Desligado)
#define INICIAL_PORTC   0b00000000  


/* ===================== */
/* DEFINIÇÕES DA PORTA D */
/* ===================== */

/// LED 10 (Painel)
#define _LED10    PORTDbits.RD7
#define Desliga_LED10() _LED10=0;LATD7=0;
#define Liga_LED10() _LED10=1;LATD7=1;

/// LED 9 (Painel)
#define _LED9     PORTDbits.RD6
#define Desliga_LED9() _LED9=0;LATD6=0;
#define Liga_LED9() _LED9=1;LATD6=1;

/// LED 6 (Painel)
#define _LED6     PORTDbits.RD5
#define Desliga_LED6() _LED6=0;LATD5=0;
#define Liga_LED6() _LED6=1;LATD5=1;

/// Saída PWM 1
#define _PWM1     PORTDbits.RD4 

/// Bit 3 do Endereço / LED 3 (Painel)
#define _ADDR3      PORTDbits.RD3
#define _LED3       PORTDbits.RD3
#define _LED3_TRIS  TRISDbits.TRISD3
#define Desliga_LED3() _LED3=0;LATD3=0;
#define Liga_LED3() _LED3=1;LATD3=1;

/// Habilita Relé de Entrada
#define _HRLE     PORTDbits.RD2 
#define Desliga_RLE() _HRLE=0;LATD2=0;
#define Liga_RLE() _HRLE=1;LATD2=1;

/// Habilita Relé de Consumidor
#define _HRLC     PORTDbits.RD1 
#define Desliga_RLC() _HRLC=0;LATD1=0;
#define Liga_RLC() _HRLC=1;LATD1=1;

/// Habilita Relé de Pré-Carga
#define _HRPC     PORTDbits.RD0 
#define Desliga_RPC() _HRPC=0;LATD0=0;
#define Liga_RPC() _HRPC=1;LATD0=1;

/// Direçao dos pinos da porta (1 = Entrada, 0 = Saída)
#define INICIAL_TRISD   0b00001000  
/// Estado inicial dos pinos da porta (1 = Ligado, 0 = Desligado)
#define INICIAL_PORTD   0b00000000  


/* ===================== */
/* DEFINIÇÕES DA PORTA E */
/* ===================== */

/// LED OPERAÇÃO - VERMELHO
#define _LED_OP_VM  PORTEbits.RE0
#define Desliga_LED_OP_VM() _LED_OP_VM=0;LATE0=0;
#define Liga_LED_OP_VM() _LED_OP_VM=1;LATE0=1;

/// LED OPERAÇÃO - VERDE
#define _LED_OP_VD  PORTEbits.RE1
#define Desliga_LED_OP_VD() _LED_OP_VD=0;LATE1=0;
#define Liga_LED_OP_VD() _LED_OP_VD=1;LATE1=1;

/// LED DEFEITO - VERMELHO
#define _LED_DEF_VM PORTEbits.RE2
#define Desliga_LED_DEF_VM() _LED_DEF_VM=0;LATE2=0;
#define Liga_LED_DEF_VM() _LED_DEF_VM=1;LATE2=1;

/// Direçao dos pinos da porta (1 = Entrada, 0 = Saída)
#define INICIAL_TRISE   0b11111000  
/// Estado inicial dos pinos da porta (1 = Ligado, 0 = Desligado)
#define INICIAL_PORTE   0b00000000  

/* ===================== */
/* DEFINIÇÕES PINOS A/D  */
/* ===================== */

/// Configurações individuais das portas ANX
#define INICIAL_ANCON0  0b00000111 
/// Configurações individuais das portas ANX
#define INICIAL_ANCON1  0b00000000 


#endif	/* PINAGEM_H */

