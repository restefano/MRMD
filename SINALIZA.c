/**
 * @file SINALIZA.c
 * @brief Descrição do arquivo
 * @author estefano
 * @date 27 de Setembro de 2014
 * <p>(C) Copyright <b> WEG CRITICAL POWER </b> - Todos os direitos reservados.</p>
 * @see http://www.weg.net
 */

#include "SINALIZA.h"

void Liga_Todos_LEDPOT(void)
/**@brief Liga imediatamente todos os LEDs de potência.<br>
 * @date 25/09/14
 * @param
 *      nenhum
 * @return nenhum
 * @author Ricardo Estefano Rosa (restefano)
 */
{

    Liga_LED1();
    Liga_LED2();
    Liga_LED3();
    Liga_LED4();
    Liga_LED5();
    Liga_LED6();
    Liga_LED7();
    Liga_LED8();
    Liga_LED9();
    Liga_LED10();

}

void Desliga_Todos_LEDPOT(void)
/**@brief Desliga imediatamente todos os LEDs de potência.<br>
 * @date 27/09/14
 * @param
 *      nenhum
 * @return nenhum
 * @author Ricardo Estefano Rosa (restefano)
 */
{

    Desliga_LED1();
    Desliga_LED2();
    Desliga_LED3();
    Desliga_LED4();
    Desliga_LED5();
    Desliga_LED6();
    Desliga_LED7();
    Desliga_LED8();
    Desliga_LED9();
    Desliga_LED10();

}

void Aguarda_Sinal(void)
{
    _passouMSegSinal = 0;
    while(_passouMSegSinal == 0)
    {
    }
}

void Sinaliza_Inicio(void)
/**@brief Sinaliza o início do funcionamento do sistema.<br>
 * Pisca por 3 vezes todos os LEDs de potência e acende
 * cada um dos LEDs de controle.
 * @date 27/09/14
 * @param
 *      nenhum
 * @return nenhum
 * @warning Função bloqueante: Bloqueia o fluxo por aprox. 5 segundos.
 * @author Ricardo Estefano Rosa (restefano)
 */
{

    _LED1_TRIS = 0;
    _LED2_TRIS = 0;
    _LED3_TRIS = 0;
    _LED4_TRIS = 0;
    _LED5_TRIS = 0;

    Liga_Todos_LEDPOT();
    Aguarda_Sinal();
    Desliga_Todos_LEDPOT();
//    Aguarda_Sinal();
//    Liga_Todos_LEDPOT();
//    Aguarda_Sinal();
//    Desliga_Todos_LEDPOT();
//    Aguarda_Sinal();
//    Liga_Todos_LEDPOT();
      Aguarda_Sinal();
//    Desliga_Todos_LEDPOT();

/*
    Aguarda_Sinal();

    Liga_Todos_LEDPOT();
    Aguarda_Sinal();
    Desliga_Todos_LEDPOT();
    Aguarda_Sinal();

    Liga_Todos_LEDPOT();
    Aguarda_Sinal();
    Desliga_Todos_LEDPOT();
    Aguarda_Sinal();
*/
    
    Liga_LED_OP_VD();
    Liga_LED_DEF_VD();
    Aguarda_Sinal();
    Liga_LED_OP_VM();
    Liga_LED_DEF_VM();
    Aguarda_Sinal();
    Desliga_LED_OP_VD();
    Desliga_LED_DEF_VD();
    Aguarda_Sinal();
    Desliga_LED_OP_VM();
    Desliga_LED_DEF_VM();
}
