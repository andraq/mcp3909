
#include <stdio.h>

//#define INTERRUPCAO_SERIAL  1

#ifdef INTERRUPCAO_SERIAL

#define TAM_BUFFER_SERIAL   30
#define BYTE_FIM            10

char bufferSerial[TAM_BUFFER_SERIAL];
char contBuffer;
bit  f_serialRcv;

#endif

// Transmite byte --------------------------------------------------------------
void putch(char data){
    while(!TXIF)    // Check Buffer
        continue;   // Wait till ready
    TXREG = data;   // Send data
}

// Inicializa Serial -----------------------------------------------------------
void inicializa_RS232(long velocidade,int modo)
{
    RCSTA = 0X90; //habilita porta serial,recepção de 8 bit em modo continuo,assincrono.
    unsigned int valor;
        if(modo == 1){      //modo = 1 ,modo alta velocidade
            
            TXSTA = 0x24;   //modo assincrono,trasmissao 8 bits.
            valor =(unsigned int)(((_XTAL_FREQ/velocidade)-16)/16);
        }
        else{               //modo = 0 ,modo baixa velocidade
            TXSTA = 0x20;   //modo assincrono,trasmissao 8 bits.
            valor =(unsigned int)(((_XTAL_FREQ/velocidade)-64)/64);
        }
    SPBRG = valor;
    RCIE = 1;               //habilita interrupção de recepção
    TXIE = 0;               // Interrupção de transmissão desativada
}

// INTERRUPÇÃO SERIAL ----------------------------------------------------------
#ifdef INTERRUPCAO_SERIAL

void serialEnable(void){
    contBuffer=0;
    f_serialRcv=0;
    RCIE=1;
}

void serialDisable(){
    RCIE=0;
}

void trataSerial(void){

}
#endif