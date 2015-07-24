/**
 * @file MODBUS.c
 * @brief Implementação do protocolo MODBUS
 * @author estefano
 * @date 8 de Janeiro de 2015
 * <p>(C) Copyright <b> WEG CRITICAL POWER </b> - Todos os direitos reservados.</p>
 * @see http://www.weg.net
 */

#include "PINAGEM.h"
#include "MODBUS.h"
#include "RSERIAL.h"

    extern unsigned short int PWM_atual;
    extern unsigned int ATI_lido;
    extern unsigned int ATS_lido;
    extern unsigned int ACS_lido;
    extern unsigned int ATI_lido;
    extern unsigned int ATS_alvo;
    extern unsigned int ATS_maximo;
    extern unsigned int ATS_minimo;

    extern unsigned char endereco;

    extern unsigned char qbytesS1E;


    #define c_TAMANHOBUFFERMODBUS 255
    unsigned char passo_maqSerial = 0;
    unsigned char proximo_maqSerial = 0;
    unsigned char conta_recebidos = 0;
    unsigned char erro_modbus = 0;
    unsigned char bufferModbus[c_TAMANHOBUFFERMODBUS];

    #define c_TAMANHOHOLDINGREGISTERS 255
    unsigned short int holding_Registers[255];

//  Temporização serial

    extern bit _timeout_modbus;
    extern unsigned char conta_modbus;

void ProtocoloModbus(void)
/**@brief Máquina para tratamento da comunicação Modbus<br>
 * @date 17/10/14
 * @author Ricardo Estefano Rosa (restefano)
 */
{
    unsigned short int crc_recebido = 0;
    unsigned short int crc_calculado = 0;
    unsigned short int posicao = 0;
    unsigned char quantidade = 0;
    unsigned char contador = 0;
    unsigned char temp = 0;

    enum PMS // Passos da máquina serial
    {
        PMSinicio_maqSerial,
        PMSrecebe_pacote,
        PMSdecodifica_pacote,
        PMSrecebe_A, PMSrecebe_B, PMSrecebe_C,
        PMSenvia_A, PMSenvia_B, PMSenvia_C,
        PMSverificaLRC,
        PMSenvia_erroLRC, PMSenvia_erroTIMEOUT,
        PMSaguarda_timeout, PMSretorna_erro
    };
    enum ERRO_MODBUS
    {
        c_ERRO_MODBUS_TAMANHOINSUFICIENTE,
        c_ERRO_MODBUS_ENDERECO,
        c_ERRO_MODBUS_CRC,
        c_ERRO_MODBUS_FUNCAONAOSUPORTADA,
        c_ERRO_MODBUS_DADOSINVALIDOS,
        c_ERRO_MODBUS_TAMANHOMAXIMOEXCEDIDO,
        c_ERRO_MODBUS_ENDERECOINVALIDO
    };

        switch (passo_maqSerial)
        {
            case PMSinicio_maqSerial:
                if (RCSTA1bits.OERR == 1)
                {
                    RCSTA1bits.CREN = 0;
                    RCSTA1bits.CREN = 1;
                };
                if (RCSTA1bits.FERR == 1)
                {
                    temp = Recebe_Byte_S1();
                };
                // Volta ao modo recepçao se acabou de transmitir
                if (TXSTA1bits.TRMT == 1)
                {
                    Desliga_DI();
                }


                if (qbytesS1E > 0)
                {
                    conta_recebidos = 0;
                    Atualiza_HoldingRegisters;
                    ArmaTimeoutModbus();
                    passo_maqSerial = PMSrecebe_pacote;
                }
                break;

            case PMSrecebe_pacote: // Recebe o pacote até timeout ou maximo
                // Verifica se recebeu algo
                if (qbytesS1E > 0)
                {
                    // Recebeu. Adiciona ao buffer
                    ArmaTimeoutModbus();
                    bufferModbus[conta_recebidos] = Recebe_Byte_S1();
                    conta_recebidos++;
                    if (conta_recebidos >= c_TAMANHOBUFFERMODBUS)
                    {
                        // Decodifica o que veio (poderia já dar erro aqui)
                        erro_modbus = c_ERRO_MODBUS_TAMANHOMAXIMOEXCEDIDO;
                        passo_maqSerial = PMSretorna_erro;
                    }
                }
                // Verifica se houve timeout (fim de pacote)
                if (_timeout_modbus)
                {
                    // Houve. Decodifica o pacote
                    passo_maqSerial = PMSdecodifica_pacote;
                }
                break;

            case PMSdecodifica_pacote: // Valida o pacote
                // Verifica se o tamanho do pacote é válido (>=4)
                if (conta_recebidos < 4)
                {
                    erro_modbus = c_ERRO_MODBUS_TAMANHOINSUFICIENTE;
                    passo_maqSerial = PMSretorna_erro;
                    break;
                }
                // Verifica se o endereço de dispositivo é o meu
                if (bufferModbus[0] != endereco)
                {
                    erro_modbus = c_ERRO_MODBUS_ENDERECO;
                    passo_maqSerial = PMSretorna_erro;
                    break;
                }
                // Verifica se o CRC é válido
                crc_recebido = bufferModbus[conta_recebidos-1]<<8;
                crc_recebido |= bufferModbus[conta_recebidos-2];
                crc_calculado = CRC16(&bufferModbus,conta_recebidos-2);
                if (crc_recebido != crc_calculado)
                {
                    erro_modbus = c_ERRO_MODBUS_CRC;
                    passo_maqSerial = PMSretorna_erro;
                }
                // Verifica se a função é suportada, se os dados estão presentes
                // e comanda a resposta adequada
                switch (bufferModbus[1])
                {
                    case 0x03: // READ HOLDING REGISTERS

                        // Verifica se os dados estão presentes
                        if (conta_recebidos != 8)
                        {
                            erro_modbus = c_ERRO_MODBUS_DADOSINVALIDOS;
                            passo_maqSerial = PMSretorna_erro;
                            break;
                        }
                        // Verifica se a solicitação é válida
                        // (INICIO+QUANTIDADE) < MAXIMO ???
                        posicao = bufferModbus[3]+bufferModbus[5];
                        if ((posicao > 255) || bufferModbus[5] > 100)
                        {
                            erro_modbus = c_ERRO_MODBUS_ENDERECOINVALIDO;
                            passo_maqSerial = PMSretorna_erro;
                            break;
                        }
                        // Monta o pacote de resposta
                        posicao = bufferModbus[3];
                        quantidade = bufferModbus[5];
                        contador = 0;

                        bufferModbus[0]=endereco;
                        bufferModbus[1]=0x03;
                        bufferModbus[2]=(quantidade<<1);
                        while (contador < quantidade)
                        {
                            bufferModbus[3+(contador<<1)]=
                                    holding_Registers[posicao]>>8;
                            bufferModbus[4+(contador<<1)]=
                                    holding_Registers[posicao]&0x00FF;
                            contador++;
                            posicao++;
                        }
                        posicao = CRC16(&bufferModbus,(quantidade<<1) + 3);
                        bufferModbus[4+(contador<<1)]=(posicao>>8);
                        bufferModbus[3+(contador<<1)]=(posicao&0x00FF);
                        Envia_BlocoRAM_S1(&bufferModbus,5+(contador<<1));
                        passo_maqSerial = PMSinicio_maqSerial;
                        break;

                    default:
                        erro_modbus = c_ERRO_MODBUS_FUNCAONAOSUPORTADA;
                        passo_maqSerial = PMSretorna_erro;
                        break;
                }
                break;
            case PMSretorna_erro: // Valida o pacote
                passo_maqSerial = 0;
                switch (erro_modbus)
                {
//                    case c_ERRO_MODBUS_CRC:
//                        break;
//                    case c_ERRO_MODBUS_ENDERECO:
//                        break;
//                    case c_ERRO_MODBUS_FUNCAONAOSUPORTADA:
//                        break;
//                    case c_ERRO_MODBUS_TAMANHOINSUFICIENTE:
//                        break;
//                    case c_ERRO_MODBUS_DADOSINVALIDOS:
//                        break;
//                    case c_ERRO_MODBUS_TAMANHOMAXIMOEXCEDIDO:
//                        break;
//                    case c_ERRO_MODBUS_ENDERECOINVALIDO;
//                        break;
                    default:
                        passo_maqSerial = PMSinicio_maqSerial;
                }
                break;
            default:
                passo_maqSerial = PMSinicio_maqSerial;
                break;
        }

}

void ArmaTimeoutModbus(void)
/**@brief Máquina para tratamento da comunicação Modbus<br>
 * @date 17/10/14
 * @author Ricardo Estefano Rosa (restefano)
 */
{
    conta_modbus = 0;
    _timeout_modbus = 0;
}

unsigned int CRC16 (unsigned char *msg, unsigned char comp)
/**@brief Cálculo do CRC16
 * @date 01/11/14
 * @param
 *      msg Ponteiro para a mensagem
 *      comp Comprimento da mensagem (sem o CRC)
 * @return CRC de 16 bits
 * @author Ricardo Estefano Rosa (restefano)
*/
{
    unsigned char crc_hi = 0xFF ; // MSB do CRC inicializado
    unsigned char crc_lo = 0xFF ; // LSB do CRC inicializado
    unsigned indice ; // Indexador das tabelas

    while (comp--) // Varre o buffer com a mensagem
    {
        indice = crc_lo ^ *msg++; // Calcula o CRC
        crc_lo = crc_hi ^ tabelaCRCHI[indice];
        crc_hi = tabelaCRCLO[indice];
    }

    return (crc_hi << 8 | crc_lo);
}

void Atualiza_HoldingRegisters(void)
/**@brief Carrega os valores nos Holding Registers
 * @date 01/11/14
 * @author Ricardo Estefano Rosa (restefano)
*/
{
    holding_Registers[0] = PWM_atual;
    holding_Registers[1] = ATI_lido;
    holding_Registers[2] = ATS_lido;
    holding_Registers[3] = ACS_lido;
    holding_Registers[4] = ATI_lido;


    holding_Registers[20] = ATS_alvo;
    holding_Registers[21] = ATS_maximo;
    holding_Registers[22] = ATS_minimo;


}

void CHARtoASC(unsigned int valor,unsigned char* destino, unsigned char ndig)
{
    unsigned char temp = 0;
    unsigned char ponteiro = 0;

    if (ndig>=3) // Tem centena
    {
        temp = valor/100;
        valor -= (temp*100);
        if (temp > 9) { temp = 9; }
        destino[ponteiro] = temp + '0';
        ponteiro++;
    }

    if (ndig>=2) // Tem dezena
    {
        temp = valor/10;
        valor -= (temp*10);
        if (temp > 9) temp = 9;
        destino[ponteiro] = temp + '0';
        ponteiro++;
    }

    //  Trata a unidade
        if (valor > 9) valor = 9;
        destino[ponteiro] = valor + '0';

}


