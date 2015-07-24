/**
 * @file INT.c
 * @brief Rotinas relacionadas às interrupções do sistema
 * @author estefano
 * @date 27 de Setembro de 2014
 * <p>(C) Copyright <b> WEG CRITICAL POWER </b> - Todos os direitos reservados.</p>
 * @see http://www.weg.net
 */

#include <xc.h>
#include "INT.h"
#include "RSERIAL.H"
#include "PINAGEM.H"
#include "GLOBALS.h"
#include "PWM.h"

    unsigned char passo_maqAD = 0;
    enum PMAD
    {
        PMADInicioSolicitaATI,
        PMADSalvaATI_SolicitaATS,
        PMADSalvaATS_SolicitaACS,
        PMADSalvaACS_SolicitaATSouATI
    };



void Configura_INT(void)
/**@brief Configura as interrupções do sistema.<br>
 * DESCREVER
 * São elas:<br>
 * - TIMER 0, a cada 50us
 * @date 24/09/14
 * @param
 *      nenhum
 * @return nenhum
 * @author Ricardo Estefano Rosa (restefano)
 */
{

    PIE1 = 0; // Desabilita interrupções dos periféricos
    PIE2 = 0;
    PIE3 = 0;
    PIE4 = 0;
    PIE5 = 0;

    RCONbits.IPEN = 1;

    IPR1bits.RC1IP = 0; // Baixa prioridade
    IPR1bits.TX1IP = 0; // Baixa prioridade
    PIR1bits.RC1IF = 0; // Esquece recebimentos espúrios

    PIE1bits.TX1IE = 0; // Habilita interrupçao por transmissão
    PIE1bits.RC1IE = 1; // Habilita interrupçao por recepçao

//  Configura o Timer 0 para interromper a cada 50us (800 instruções)

    T0CON = 0b11000001;  // 16 bits com prescaler 1:4
    TMR0IF = 0;
    TMR0IE = 1;
    INTCON2bits.TMR0IP = 1;

    INTCONbits.PEIE = 1;
    INTCONbits.GIEL = 1;
    INTCONbits.GIEH = 1;
    INTCONbits.GIE = 1;

}

// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX


void interrupt VETOR_INTHIGH(void)
/**@brief Vetor de interrupção de alta prioridade.<br>
 * Trata as interrupções de alta prioridade do sistema.<br>
 * São elas:<br>
 * - TIMER 0, a cada 50us
 * @date 16/09/14
 * @param
 *      nenhum
 * @return nenhum
 * @author Ricardo Estefano Rosa (restefano)
 */
{
    //Liga_LED1();

    //==============
    //==============
    // INT TIMER 0
    //==============
    //==============


    // AJUSTE DE TEMPO DO TIMER 0. NÃO ALTERAR ATÉ XXXXXXXXXXXXXXX
    if (TMR0IE && TMR0IF)
    {
        TMR0IF=0;
        NOP();NOP();NOP();
        TMR0L =+ 59;
    // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    //  Trata a temporização de sinal
        conta_sinal++;
        if (conta_sinal == 10000)
        {

            // PASSOU MEIO SEGUNDO

            conta_sinal = 0;
            /* TODO: Fazer sincronismo da sinalização dos módulos pelo CAN */
            /* Basta receber broadcast e zerar _sinal e conta_sinal...     */
            _sinal=!_sinal;
            _passouMSegSinal = 1; // Avisa que passou meio segundo para sinal.

            // Solicita a leitura da temperatura
            _ler_ATI = 1;
        }

    //  Trata a temporização de 1ms
        conta_ms++;
        if (conta_ms == 20)
        {
            conta_ms = 0;
            _passou_1ms = 1; // Avisa que passou 1ms

            // Trata o timeout da serial

            conta_modbus++;
            if (conta_modbus >= 2)
            {
                conta_modbus = 0;
                _timeout_modbus = 1;
            }

        }

        // MÁQUINA DOS A/Ds
        // ACS e ATS lidos constantemente de forma alternada
        // ATI lido conforme comando externo (a cada X segundos, por exemplo)

        switch (passo_maqAD)
        {
            case PMADInicioSolicitaATI:
                // Solicita leitura de ATI
                ADCON0 = 0b00001001; // Canal 2, A/D ligado
                GODONE = 1;
                passo_maqAD = PMADSalvaATI_SolicitaATS;
                break;
            case PMADSalvaATI_SolicitaATS:
                // Salva ATI
                ATI_lido = ADRES>>2;
                _ler_ATI = 0;
                // Comanda a leitura de ATS
                ADCON0 = 0b00000001; // Canal 0, A/D ligado
                GODONE = 1;
                passo_maqAD = PMADSalvaATS_SolicitaACS;
                break;
            case PMADSalvaATS_SolicitaACS:
                // Salva ATS
                ATS_raw = ADRES;
                ATS_lido = (ATS_raw-calib_ATS)>>2;
                if (ATS_lido > 1000) ATS_lido = 0; // Filtro para ATS
                // Solicita leitura de ACS
                ADCON0 = 0b00000101; // Canal 1, A/D ligado
                GODONE = 1;
                passo_maqAD = PMADSalvaACS_SolicitaATSouATI;
                break;
            case PMADSalvaACS_SolicitaATSouATI:
                // Salva ACS
                ACS_raw = ADRES;
                ACS_lido = (ACS_raw-calib_ACS)>>2;
                if (ACS_lido > 1000) ACS_lido = 0; // Filtro para ATS

                // Verifica se houve sobrecorrente
                if (ACS_lido > c_SOBRECORRENTE)
                {
                    _inibePWM = 1;
                    Altera_PWM(0,1); // CORTA OS PULSOS IMEDIATAMENTE
                }


                // Verifica se deve ler ATI
                if (_ler_ATI)
                {
                    // Solicita leitura de ATI
                    ADCON0 = 0b00001001; // Canal 2, A/D ligado
                    passo_maqAD = PMADSalvaATI_SolicitaATS;
                }
                else
                {
                    // Solicita leitura de ATS
                    ADCON0 = 0b00000001; // Canal 0, A/D ligado
                    passo_maqAD = PMADSalvaATS_SolicitaACS;
                }
                GODONE = 1;
                break;
            default:
                passo_maqAD = PMADInicioSolicitaATI;
                break;
        }
    }
}

// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void interrupt low_priority VETOR_INTLOW(void)
/**@brief Vetor de interrupção de baixa prioridade.<br>
 * Trata as interrupções de baixa prioridade do sistema.<br>
 * São elas:<br>
 * - TX e RX da serial
 * @date 18/09/14
 * @param
 *      nenhum
 * @return nenhum
 * @author Ricardo Estefano Rosa (restefano)
 */
{

    unsigned char temp; // Variável temporária para uso local

    //Liga_LED2();

    //==============
    //==============
    // INT SERIAL 1
    //==============
    //==============

    if (PIR1bits.RC1IF) // Recebeu um byte pela serial ???
    {
        temp = RCREG1;
        PIR1bits.RC1IF = 0;                             // Sim, avisa e trata.
        if (qbytesS1E < tamanho_bufferS1E)              // Verifica se o buffer encheu... (se ninguém esvaziou)
        {
        // Chegou novo caractere e há espaço no buffer
            ponteiroS1Ei++;                             // Incrementa ponteiro de entrada
            if (ponteiroS1Ei == tamanho_bufferS1E)      // Verifica se passou do tamanho do buffer
            {
                ponteiroS1Ei = 0;                       // Passou. Começa do zero
            }
            bufferS1E[ponteiroS1Ei]=temp;               // Recebe o byte da UART
            qbytesS1E++;                                // Avisa que tem um byte a mais neste buffer
        } // Se encheu, ignora a entrada
    }
    else if (PIR1bits.TX1IF && PIE1bits.TX1IE  ) // Terminou de enviar um byte pela serial e está habilitada a interrupçao ???
    {
        if (qbytesS1S > 0)             // Verifica se há algo a transmitir...
        {
        // Há algo a transmitir. Envia e processa.
            ponteiroS1Si++;                             // Incrementa ponteiro de saída
            if (ponteiroS1Si == tamanho_bufferS1S)      // Verifica se passou do tamanho do buffer
            {
                ponteiroS1Si = 0;                       // Passou. Começa do zero
            }

            TXREG1 = bufferS1S[ponteiroS1Si];
            qbytesS1S--;                                // Transmitiu.
        }
        else
        {
            PIE1bits.TX1IE = 0;                         // Habilita a interrupçao
        }
//Desliga_LED2();
        return;
    }

}

