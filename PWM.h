/**
 * @file PWM.H
 * @brief Header das funções relacionadas ao PWM
 * @author Ricardo Estefano Rosa (restefano)
 * @date 09/08/14
 * <p>(C) Copyright <b> WEG CRITICAL POWER </b> - Todos os direitos reservados.</p>
 * @see http://www.weg.net
 */

#ifndef CONFIGPERIF_H
#define	CONFIGPERIF_H

#include <PIC18F46K80.H>
#include <xc.h>

void Configura_PWM();
void Altera_PWM(unsigned int dutycycle, unsigned char na_int);

#endif	/* CONFIGPERIF_H */

