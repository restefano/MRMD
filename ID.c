/**
 * @file ID.c
 * @brief Identifica o endereço do módulo e salva no reg. global id_modulo
 * @author estefano
 * @date 27 de Setembro de 2014
 * <p>(C) Copyright <b> WEG CRITICAL POWER </b> - Todos os direitos reservados.</p>
 * @see http://www.weg.net
 */

#include "ID.h"

void Identifica_Modulo(void)
/**@brief Identifica o endereço do módulo de potência no barramento.<br>
 * E altera a direção dos dados das portas lidas.<br>
 * @date 24/09/14
 * @param
 *      nenhum
 * @return nenhum
 * @warning Deve estar logo após a configuração do I/O por utilizar portas
 * compartilhadas.
 * @author Ricardo Estefano Rosa (restefano)
 */
{

    id_modulo = 0;

    /* TODO: ADDR:Aguardando alteração no Hardware. Por enquanto, ID = 0

    if (_ADDR0) id_modulo+=1;
    if (_ADDR1) id_modulo+=2;
    if (_ADDR2) id_modulo+=4;
    if (_ADDR3) id_modulo+=8;

     */

}



