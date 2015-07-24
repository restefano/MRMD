/**
 * @file INT.h
 * @brief Header file do arquivo INT.c
 * @author estefano
 * @date 27 de Setembro de 2014
 * <p>(C) Copyright <b> WEG CRITICAL POWER </b> - Todos os direitos reservados.</p>
 * @see http://www.weg.net
 */

#ifndef INT_H
#define	INT_H

//  Temporizações

    extern unsigned short int conta_ms; // Postscaler do Timer 0 para 1ms
    extern bit _passou_1ms; // Informa que passou 1ms

//  Temporização serial

    extern bit _timeout_modbus;
    extern unsigned char conta_modbus;


//  Sinalizações

extern unsigned short int conta_sinal;
extern bit _sinal;
extern bit _passouMSegSinal;

//  Máquina de A/Ds

extern bit _ler_ATI;
extern bit _ATI_lido;
extern bit _ler_ATS_ACS;
extern unsigned int ATI_lido;
extern unsigned int ACS_lido;
extern unsigned int ACS_raw;
extern unsigned int ATS_lido;
extern unsigned int ATS_raw;
extern unsigned int calib_ATS;
extern unsigned int calib_ACS;


//  Serial

extern unsigned char qbytesS1E;
extern unsigned char ponteiroS1Ei;
extern unsigned char ponteiroS1Si;
extern unsigned char qbytesS1S;
extern unsigned char bufferS1S[];
extern unsigned char bufferS1E[];


void Configura_INT(void);
void interrupt VETOR_INTHIGH(void);
void interrupt low_priority VETOR_INTLOW(void);


#endif	/* INT_H */

