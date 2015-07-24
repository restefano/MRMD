/**
 * @file PWM.C
 * @brief Funções relacionadas ao PWM
 * @author Ricardo Estefano Rosa (restefano)
 * @date 09/08/14
 * <p>(C) Copyright <b> WEG CRITICAL POWER </b> - Todos os direitos reservados.</p>
 * @see http://www.weg.net
 */

#include <plib/pwm.h>
#include <plib/timers.h>
#include "PWM.h"
#include "PINAGEM.h"

extern bit _inibePWM;
 
void Configura_PWM(){
/**@brief Habilita a geração de PWM em 25kHz.<br>
 * A base do PWM1 é TMR2. A base do PWM2 é TMR4.<br>
 * Habilita o PWM1, aguarda TMR2 avançar 180 graus e habilita o PWM2.
 * @date 15/08/15
 * @param Nenhum.
 * @return Nada.
 * @warning Desabilita as interrupções para garantir a temporização.
 * @author Ricardo Estefano Rosa (restefano)
 */

    unsigned char Timer2Config;    ///< PWM clock source A
    unsigned char Timer4Config;    ///<PWM clock source B
    union PWMDC DCycle;
    unsigned char dutycycle = 0;

    INTCONbits.GIE = 0;
    CCPTMRS = 0b00000010;

    Timer2Config = T2_PS_1_4 || TIMER_INT_OFF; //prescale 4
    OpenTimer2(Timer2Config);

    T2CONbits.TMR2ON = 0;  // STOP TIMER2 registers to POR state
    PR2 = 159;          // Set period
    TMR2 = 0;
    T2CONbits.TMR2ON = 1;  // Turn on PWM1

    // Save the dutycycle value in the union
    DCycle.lpwm = dutycycle << 6;
    // Write the high byte into CCPR1L
    CCPR1L = DCycle.bpwm[1];
    // Write the low byte into CCP1CON5:4
    CCP1CON = (CCP1CON & 0xCF) | ((DCycle.bpwm[0] >> 2) & 0x30);
    CCP1CON |= 0b00001100;    //ccpxm3:ccpxm0 11xx=pwm mode

    while (TMR2 < 73) //73, confirmado para 1/2T de 25kHz
    {
    }
    NOP();
    NOP();
    NOP();

    Timer4Config = T4_PS_1_4 || TIMER_INT_OFF; //prescale 4
    OpenTimer4(Timer4Config);

    T4CONbits.TMR4ON = 0;  // STOP TIMER2 registers to POR state
    PR4 = 159;          // Set period
    T4CONbits.TMR4ON = 1;  // Turn on PWM1

    // Save the dutycycle value in the union
    DCycle.lpwm = dutycycle << 6;
    // Write the high byte into CCPR1L
    CCPR2L = DCycle.bpwm[1];
    // Write the low byte into CCP1CON5:4
    CCP2CON = (CCP2CON & 0xCF) | ((DCycle.bpwm[0] >> 2) & 0x30);
    CCP2CON |= 0b00001100;    //ccpxm3:ccpxm0 11xx=pwm mode

    INTCONbits.GIE = 1;

}

void Altera_PWM(unsigned int dutycycle, unsigned char na_int){
/**@brief Altera a largura de pulso do PWM, caso não esteja inibido.
 * @date 26/08/15
 * @param dutycycle Razão cíclica desejada (x62,5ns). Máx:320, Mín:0.
 * @param na_int 1 = está na interrupção, 0 = está fora dela.
 * @return Nada.
 * @author Ricardo Estefano Rosa (restefano)
 */

    if (!na_int) GIE = 0;

    union PWMDC DCycle;
    unsigned char temp;

    if (_inibePWM) { dutycycle = 0; } // Se sobrecorrente, largura = nula

    // Save the dutycycle value in the union
    DCycle.lpwm = dutycycle << 6;

    // Write the high byte into CCPR1L
    CCPR1L = DCycle.bpwm[1];
    CCPR2L = DCycle.bpwm[1];
    temp = ((DCycle.bpwm[0] >> 2) & 0x30);

    // Write the low byte into CCP1CON5:4
    CCP1CON = (CCP1CON & 0xCF) | temp;
    // Write the low byte into CCP1CON5:4
    CCP2CON = (CCP2CON & 0xCF) | temp;

    if (!na_int) GIE = 1;

}