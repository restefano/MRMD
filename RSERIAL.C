/**
 * @file RSERIAL.c
 * @brief Rotinas para automação da USART
 * @author estefano
 * @date 18 de Setembro de 2014
 * <p>(C) Copyright <b> WEG CRITICAL POWER </b> - Todos os direitos reservados.</p>
 * @see http://www.weg.net
 */

#include <xc.h>
#include <PIC18F46K80.H>
#include "RSERIAL.h"
#include "PINAGEM.h"

    extern unsigned char qbytesS1E;
    extern unsigned char qbytesS1S;

    extern unsigned char ponteiroS1E;
    extern unsigned char ponteiroS1S;

    extern unsigned char ponteiroS1Ei;
    extern unsigned char ponteiroS1Si;

    extern unsigned char bufferS1E[];
    extern unsigned char bufferS1S[];


//==============
//==============
// API SERIAL 1
//==============
//==============

void Configura_UART1(void)
{

//  Abre a porta serial 1 com 19200 bps

    _DI_TRIS = 0; // Para uso do 485
    TXSTA1 = 0b10000100; // 8 bits, ativa.
    RCSTA1 = 0b10010000; // 8 bits, ativa.
    BAUDCON1 = 0b00000000; // Todos os recursos desabilitados.
    SPBRG1 = 207; // 19200bps
    Reinicia_S1();

}

void Reinicia_S1(void)
{

    ponteiroS1E = 0;
    ponteiroS1S = 0;
    qbytesS1E = 0;
    qbytesS1S = 0;
}

void Envia_BlocoROM_S1(const unsigned char *romstring)
{
    unsigned char i = 0;
    unsigned char j = 0;
    while(romstring[i]!=0)
    {
        j = romstring[i];
        Envia_Byte_S1(romstring[i]);
        i++;
    }
}

void Envia_BlocoRAM_S1(unsigned char *origem, unsigned char nchars)
//EX:Envia_Bloco_S1(&meuTexto,36);
{
    unsigned char i;
    for (i=0;i<nchars;i++) 
    {
        Envia_Byte_S1(origem[i]);
    }    
}    
    
void Recebe_Bloco_S1(unsigned char *destino, unsigned char nchars) 
//EX:Recebe_Bloco_S1(&meuTexto,10) 
{
    unsigned char i = 0;
    while (nchars > 0 && qbytesS1E > 0)
    {
        destino[i] = Recebe_Byte_S1();
        i++;
        nchars--;
    }    
}    

void Envia_Byte_S1(unsigned char caractere) 
//EX:Envia_Byte_Serial('a');
//EX:Envia_Byte_Serial(255);
{
    PIE1bits.TX1IE = 0;                         // Nao estoura TXIE aqui dentro

    ponteiroS1S++;
    if (ponteiroS1S == tamanho_bufferS1S)       // Verifica se passou do tamanho do buffer
    {
        ponteiroS1S = 0;                        // Passou. Começa do zero
    }    
   
    bufferS1S[ponteiroS1S]=caractere;           // Coloca o unsigned char no buffer
    qbytesS1S++;                                // Avisa que tem mais um byte
    TXSTAbits.TXEN = 1;

    Liga_DI();                                  // Habilita transmissao 485

    PIE1bits.TX1IE = 1;                         // Habilita a interrupçao
    INTCONbits.GIEL = 1;
    
}    

unsigned char Recebe_Byte_S1(void) 
{
    if (qbytesS1E > 0)                              // Recebe somente se houver algo no buffer
    {
        ponteiroS1E++;
        if (ponteiroS1E == tamanho_bufferS1E)       // Verifica se estourou
        {
            ponteiroS1E = 0;                        // Está. Recomeça do zero
        }    
        qbytesS1E--;
        return(bufferS1E[ponteiroS1E]);
    }    
    else
    {
    return (0);
    }
}    

    