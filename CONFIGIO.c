/** 
 * @file CONFIGIO.c
 * @brief Configuração das I/Os com base em CONFIGIO.h
 * @author estefano
 * @date 15 de Agosto de 2014
 * <p>(C) Copyright <b> WEG CRITICAL POWER </b> - Todos os direitos reservados.</p>
 * @see http://www.weg.net
 */

#include "CONFIGIO.h"
#include "PINAGEM.h"

void Configura_IO(void)
/**@brief Configura as portas de E/S do controlador, inclusive os A/Ds.<br>
 * @date 20/09/14
 * @param
 *      nenhum
 * @return nenhum
 * @author Ricardo Estefano Rosa (restefano)
 */
{

//  Configurações iniciais do controlador

    PORTA = INICIAL_PORTA;
    PORTB = INICIAL_PORTB;
    PORTC = INICIAL_PORTC;
    PORTD = INICIAL_PORTD;
    PORTE = INICIAL_PORTE;

    TRISA = INICIAL_TRISA;
    TRISB = INICIAL_TRISB;
    TRISC = INICIAL_TRISC;
    TRISD = INICIAL_TRISD;
    TRISE = INICIAL_TRISE;

//  Configura os A/Ds

    ANCON0 = INICIAL_ANCON0;
    ANCON1 = INICIAL_ANCON1;

    ADCON0 = 0b00000000; // Canal 0, A/D ligado
    ADCON1 = 0b00010000; // VREF+, AGND, Single
    ADCON2 = 0b10011110; // Justif. à direita, 64Tosc, 6Tad aquisition time
                         // Tad = 1us, total = 23us/aquisição


}

