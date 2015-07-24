/**
 * @file RSERIAL.h
 * @brief Header file do arquivo RSERIAL.c
 * @author estefano
 * @date 18 de Setembro de 2014
 * <p>(C) Copyright <b> WEG CRITICAL POWER </b> - Todos os direitos reservados.</p>
 * @see http://www.weg.net
 */

#ifndef RSERIAL_H
#define	RSERIAL_H

#define tamanho_bufferS1E   255
#define tamanho_bufferS1S   255

void Configura_UART1(void);
void Envia_Byte_S1(unsigned char caractere);
void Envia_BlocoRAM_S1(unsigned char * origem, unsigned char nchars);
void Envia_BlocoROM_S1(const unsigned char *romstring);
unsigned char Recebe_Byte_S1(void);
void Recebe_Bloco_S1(unsigned char * destino, unsigned char nchars);
void Reinicia_S1(void);

#endif	/* RSERIAL_H */

