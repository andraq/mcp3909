/*******************************************************************************
 * File           : main.c
 * Autor          : Andr� Aquino
 * Data Cria��o   : 09/07/2018
 * Projeto        : Comunica��o serial com MCP3909.
 * Descri��o      : Programa b�sico para testar a biblioteca mcp3909.h. O prog.
 *                  inicializa o mcp e, a cada 1 seg, realiza a leitura por meio
 *                  da fun��o mcp_read. Os dados lidos s�o mostrados pela serial.
 *                  Um led pisca durante 100mS a cada leitura.
 ******************************************************************************/

// Includes --------------------------------------------------------------------
#include <xc.h>
#include "init.h"
#include "mcp3909.h"
#include "serial.h"

/* ---------------------------------------------------------------------------
 * FUN��O: setup
 * DESCRI��O: Cont�m rotinas executadas antes de iniciar o loop infinito.
 * ENTRADA: Vazio
 * RETORNO: Vazio
 * --------------------------------------------------------------------------*/
void setup(void){
 
    WDTCON=0;               // Desabilita WDT
    INTCON2bits.RBPU=0;     // Habilita pullup
    
    // Inicializa Serial
    inicializa_RS232(115200,1);
    
    // Configura LEDS
    LED_TIPO  = 0;
    LED = 1;
    
    // Debug LEDS
    for(auxInt=0 ; auxInt<3 ; auxInt++){
        LED = 0;
        __delay_ms(150);
        LED = 1;
        __delay_ms(150);
    }

    printf("SERIAL-OK\r\n");

    //Inicia MCP3909
    mcpSetGain(2);
    mcpInitSPI(POST_HPF1);
    printf("GANHO MCP: %u\r\n", mcpGetGanho());
}

/* ---------------------------------------------------------------------------
 * FUN��O: main
 * DESCRI��O: Rotina principal. Chama a fun��o setup e depois entra no loop
 *            infinito. 
 * ENTRADA: Vazio
 * RETORNO: Vazio
 * --------------------------------------------------------------------------*/
 void main(void) {
    setup();
    while(1){
        
        LED=0;
        // Realiza a leitura do MCP
        mcpRead(valor_ch0, valor_ch1, QNT_AMOSTRAS);
        // Imprime valores
        printf("\r\n");
        for(auxInt=0 ; auxInt<QNT_AMOSTRAS ; auxInt++){
            printf("%d | %d\r\n", valor_ch0[auxInt], valor_ch1[auxInt]);
        }
        __delay_ms(50);
        LED=1;
        __delay_ms(950);

    }
}
