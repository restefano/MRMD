/**
 * @file SINALIZA.h
 * @brief Header file do arquivo SINALIZA.c
 * @author estefano
 * @date 27 de Setembro de 2014
 * <p>(C) Copyright <b> WEG CRITICAL POWER </b> - Todos os direitos reservados.</p>
 * @see http://www.weg.net
 */

#ifndef SINALIZA_H
#define	SINALIZA_H

#include "PINAGEM.H"

extern bit _passouMSegSinal;

void Aguarda_Sinal(void);
void Liga_Todos_LEDPOT(void);
void Desliga_Todos_LEDPOT(void);
void Sinaliza_Inicio(void);

#endif	/* SINALIZA_H */

