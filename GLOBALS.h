/**
 * @file GLOBALS.h
 * @brief Definições de variáveis globais
 * @author estefano
 * @date 18 de Setembro de 2014
 * <p>(C) Copyright <b> WEG CRITICAL POWER </b> - Todos os direitos reservados.</p>
 * @see http://www.weg.net
 */

#ifndef GLOBALS_H
#define	GLOBALS_H

#include <xc.h>
#include "RSERIAL.H"

    unsigned char endereco = 1; // TODO: Endereço do equipamento no barramento

//  Globais relacionadas à serial

    unsigned char qbytesS1E;
    unsigned char qbytesS1S;

    unsigned char ponteiroS1E;
    unsigned char ponteiroS1S;

    unsigned char ponteiroS1Ei;
    unsigned char ponteiroS1Si;

    unsigned char bufferS1E[tamanho_bufferS1E];
    unsigned char bufferS1S[tamanho_bufferS1S];

//  -------------

    unsigned short int id_modulo = 0;

//  -------------
//  Máquina de A/Ds

    bit _ler_ATI = 0;
    unsigned int ATI_lido = 0;
    unsigned int ACS_lido = 0;
    unsigned int ATS_lido = 0;

//  Calibração dos A/Ds
    unsigned int calib_ATS = 0;
    unsigned int calib_ACS = 0;
    unsigned int ACS_raw = 0;
    unsigned int ATS_raw = 0;

//  Controle

    unsigned short int PWM_atual = 0;
    unsigned short int PWM_calc = 0; // PWM durante os cálculos
    unsigned short int tempo_PWM = 100;

   bit _em_SOFTSTART; // Avisa que está realizando softstart
   unsigned short int tempo_softstart = 10; // Tempo em ms para alargar o pulso

   bit _ATS_ok = 0;

//  Referências no Softstart
    unsigned int ATS_maximoS = 821;
    unsigned int ATS_minimoS = 795;

//  Referências básicas
    unsigned int ATS_maximo = 821;
    unsigned int ATS_minimo = 795;

    unsigned int ATS_alvo = 808;   // ALVO

    unsigned short int ACS_limite = 0;

    unsigned short int intervalo_PWM = 0;
    unsigned short int conta_intervalo_PWM = 0;

    typedef struct
    {
        unsigned short int DELTAV;
        unsigned short int DELTAE;
    }NIVEIS;

//  Sinalizações
    unsigned short int conta_sinal = 0; // Contador de sinalização (500ms)
    bit _sinal = 0; // Indica o semiciclo da sinalização
    bit _passouMSegSinal = 0; // Indica que passou meio segundo para sinalização

//  Temporizações

    unsigned short int conta_ms = 0; // Postscaler do Timer 0 para 1ms
    bit _passou_1ms = 0; // Informa que passou 1ms

//  Temporização serial

    bit _timeout_modbus = 0;
    unsigned char conta_modbus = 0;

//  Proteções

    bit _inibePWM = 0;

#define c_SOBRECORRENTE 20

#endif	/* GLOBALS_H */

