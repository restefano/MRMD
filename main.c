//#define SEMPRECARGA

/**
 * @file main.c
 * @brief Arquivo principal do firmware do Módulo do Retificador Modular Digital
 * @author Ricardo Estefano Rosa (restefano)
 * @date 09/08/14
 * <p>(C) Copyright <b> WEG CRITICAL POWER </b> - Todos os direitos reservados.</p>
 * @see http://www.weg.net
 */

#include "_includes.h"
#include "PWM.H"
#include "GLOBALS.H"
#include "RSERIAL.H"
#include "MODBUS.H"

// Tipos válidos no compilador XC8
// bit                1  bit
// char               8  bits
// short ou int       16 bits
// short long         24 bits
// long               32 bits


    bit _teste = 0;

    void Calibra_ADs(void);

int main(int argc, char** argv) 
/**@brief Início do funcionamento do sistema.<br>
 * Configura o sistema e executa o loop principal.
 * @date 09/08/14
 * @param
 *      argc Número de argumentos
 *      argv Ponteiro para a lista de argumentos
 * @return EXIT_SUCCESS (quando finalizada)
 * @author Ricardo Estefano Rosa (restefano)
 */
{
    #define c_TEMPOSOFTSTART 1000
    #define c_TEMPOPWM 200
    #define c_PWM_MAX 310

    unsigned int conta_tempo = 0;
    unsigned char conta_tempo2 = 0;

    unsigned char passo_maqPrincipal = 0;
    enum PMP
    {
        PMPInicio,
        PMPPrecarga,
        PMPSoftstart,
        PMPMalha
    };

    Configura_IO();
    Identifica_Modulo();    // Deve estar logo após a configuração do I/O
    Configura_INT();
    Calibra_ADs();
    Configura_UART1();
    Configura_PWM();
    Sinaliza_Inicio();

    passo_maqPrincipal = PMPInicio;

    // LOOP PRINCIPAL
    while (1)
    {

        // Rotinas executadas constantemente
        ProtocoloModbus();
        Atualiza_HoldingRegisters();

        // Máquina de estados principal
        switch (passo_maqPrincipal)
        {
            case PMPInicio: // Passo inicial
                conta_tempo = 0;
                _passou_1ms = 0;
                Liga_RLE();
                passo_maqPrincipal = PMPPrecarga;
                break;

            case PMPPrecarga: // Passo Pré-Carga
                // Pisca OP amarelo enquanto está em precarga
                if (_sinal)
                {
                    Liga_LED_OP_VD();
                    Liga_LED_OP_VM();
                }
                else
                {
                    Desliga_LED_OP_VD();
                    Desliga_LED_OP_VM();
                }
                // Computa debounce de 500ms para finalizar pré-carga
                if (_passou_1ms)
                {
                    _passou_1ms = 0;
#ifdef SEMPRECARGA
#else
                    if (!_PCOK) 
                    {
                        conta_tempo = 0;
                    }
                    else
#endif
                    {
                        conta_tempo++;
                        if (conta_tempo >= 500) // 500ms de debounce...
                        {
                            Liga_RPC();
                            conta_tempo = 0;
                            _passou_1ms = 0;
                            passo_maqPrincipal = PMPSoftstart;
                        }
                    }
                        //TODO:Implementar timeout da pré-carga
                }
                break;

            case PMPSoftstart: // Realiza o Softstart
                if (_passou_1ms)
                {
                    _passou_1ms = 0;
                    conta_tempo++;

                    //  Pisca o LED verde durante o softstart
                    Desliga_LED_OP_VM();
                    _LED_OP_VD = _sinal;

                    // Ajusta o pulso a cada c_TEMPOSOFTSTART milissegundos
                    if (conta_tempo >= c_TEMPOSOFTSTART)
                    {
                        conta_tempo = 0;

                        if ((ATS_lido < ATS_maximoS)&&(ATS_lido > ATS_minimoS))
                        {
                            Liga_RLC(); //Comentado para testes igbt
                            passo_maqPrincipal = PMPMalha;
                        }
                        else
                        {
                            // Procura colocar ATS dentro da faixa com o PWM
                            PWM_calc = PWM_atual;
                            if (ATS_lido > ATS_maximoS) PWM_calc--;
                            if (ATS_lido < ATS_minimoS) PWM_calc++;

                            // Limita a redução de pulso a zero (evita rollover)
                            if (PWM_calc > 370) PWM_calc = 0;
                            // Limita o aumento do pulso a 100 (estoura por timeout)
                            if (PWM_calc > 100) PWM_calc = 100;
                            PWM_atual = PWM_calc;
                            Altera_PWM(PWM_atual,0);
                        }

                        //TODO:Implementar timeout do soft start

                    }
                }
                break;

            case PMPMalha: // Malha principal de controle. Modo NORMAL !
                if (_passou_1ms)
                {
                    _passou_1ms = 0;
                    conta_tempo++;
                    conta_tempo2++;

                    // Sinaliza operação normal com o LED verde
                    Desliga_LED_OP_VM();
                    Liga_LED_OP_VD();

                    if (conta_tempo >= c_TEMPOPWM) // Atualizações a cada 
                                                   // c_TEMPOPWM ms
                    {
                        conta_tempo = 0;

                        if ((ATS_lido > ATS_maximo)||(ATS_lido < ATS_minimo))
                        {
                            PWM_calc = PWM_atual;
                            if (ATS_lido > ATS_maximo) PWM_calc--;
                            if (ATS_lido < ATS_minimo) PWM_calc++;
                            if (PWM_calc > 370) PWM_calc = 0;
                            if (PWM_calc > c_PWM_MAX) PWM_calc = c_PWM_MAX;
                            PWM_atual = PWM_calc;
                            Altera_PWM(PWM_atual,0);
                        }
                   }

                }
                break;
            default:
                
                break;
        }
    }

    return (0);
}

void Calibra_ADs(void)
/**@brief Realiza a calibração dos A/Ds<br>
 * Realiza a média de 32 leituras para considerar o valor como offset
 * @date 26/03/15
 * @author Ricardo Estefano Rosa (restefano)
 */
{
    // Calibração dos A/Ds
    unsigned char conta_media = 0;
    unsigned char acumula_ATS = 0;
    unsigned short long acumula_ACS = 0;

    while (conta_media < 32)
    {
        if (_passou_1ms)
        {
            _passou_1ms = 0;
            acumula_ATS += ATS_raw;
            acumula_ACS += ACS_raw;
            conta_media++;
        }
    }
    calib_ATS = acumula_ATS >> 5;
    calib_ACS = acumula_ACS >> 5;
}