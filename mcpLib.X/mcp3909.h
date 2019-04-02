/*
 Copyright (c) 2019 Andr� Aquino

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 Dispon�vel em: https://github.com/andraq/mcp3909
 
 */

// Relacionar os pinos do microcontrolador com os pinos do MCP -----------------

#define MCLR        PORTBbits.RB0
#define SCK         PORTBbits.RB1
#define MOSI        PORTBbits.RB2
#define CS          PORTBbits.RB3
#define MISO        PORTCbits.RC3

#define MCLR_TIPO   TRISBbits.TRISB0
#define SCK_TIPO    TRISBbits.TRISB1
#define MOSI_TIPO   TRISBbits.TRISB2
#define CS_TIPO     TRISBbits.TRISB3
#define MISO_TIPO   TRISCbits.TRISC3

#define G0          PORTBbits.RB5
#define G1          PORTBbits.RB4

#define G0_TIPO   TRISBbits.TRISB5
#define G1_TIPO   TRISBbits.TRISB4

//-----------------------------------------------------------------------------

#define IN  1
#define OUT 0

#define PRE_HPF1    0xA4
#define POST_HPF1   0xAC

// Vari�veis Globais ----------------------------------------------------------
int mcp_ch0, mcp_ch1;
unsigned char mcp_ganho;

/* ---------------------------------------------------------------------------
 * FUN��O: mcpSetGanho
 * DESCRI��O: Determina o ganho do canal de corrente (CH0), conforme p�g19 do
 *            datasheet.
 * ENTRADA: Valor do ganho, conforme apresentado abaixo.
 *            0 : CH0 Gain = 01 | Tens�o m�xima = 470mV
 *            1 : CH0 Gain = 02 | Tens�o m�xima = 235mV
 *            2 : CH0 Gain = 08 | Tens�o m�xima = 60mV
 *            3 : CH0 Gain = 16 | Tens�o m�xima = 30mV
 * RETORNO: Vazio
 * --------------------------------------------------------------------------*/
void mcpSetGain(unsigned char ganho){
    
    G0_TIPO = OUT;
    G1_TIPO = OUT;
    
    switch(ganho){
        case 0: G0 = 0;
                G1 = 0;
                mcp_ganho=0;
                break;
                
        case 1: G0 = 1;
                G1 = 0;
                mcp_ganho=1;
                break;
                
        case 2: G0 = 0;
                G1 = 1;
                mcp_ganho=2;
                break;
                
        case 3: G0 = 1;
                G1 = 1;
                mcp_ganho=3;
                break;
    }
}

/* ---------------------------------------------------------------------------
 * FUN��O: mcpGetGanho
 * DESCRI��O: Retorna o valor do ganho definido atrav�s da fun��o mcpSetGanho.
 * ENTRADA: Vazio
 * RETORNO: Valor do ganho definido.
 * --------------------------------------------------------------------------*/
unsigned char mcpGetGanho(void){
    return mcp_ganho;
}

/* ---------------------------------------------------------------------------
 * FUN��O: mcpInitSPI
 * DESCRI��O: Inicia a comunica��o serial no modo inofrmado pelo valor de 
 *            entrada da fun��o.
 * ENTRADA: Modo da Serial (ver p�g. 25 do datasheet)
 *          A4 : Dual Channel Output Pre HPF1.
 *          AC : Dual Channel Output Post HPF1.
 * RETORNO: Vazio
 * --------------------------------------------------------------------------*/
void mcpInitSPI(unsigned char modo){

    MCLR_TIPO = OUT;
    MOSI_TIPO = OUT;
    MISO_TIPO = IN;
    SCK_TIPO  = OUT;
    CS_TIPO   = OUT;

    MCLR = 0;
    CS   = 1;
    SCK  = 0;
    
    if(modo == PRE_HPF1){

        MCLR = 1;

        CS=0;

        // 1
        SCK = 0;
        MOSI = 1;
        SCK = 1;

        //2
        SCK = 0;
        MOSI = 0;
        SCK = 1;

        //3
        SCK = 0;
        MOSI = 1;
        SCK = 1;

        //4
        SCK = 0;
        MOSI = 0;
        SCK = 1;

        //5
        SCK = 0;
        MOSI = 0;
        SCK = 1;

        //6
        SCK = 0;
        MOSI = 1;
        SCK = 1;

        //7
        SCK = 0;
        MOSI = 0;
        SCK = 1;

        //8
        SCK = 0;
        MOSI = 0;
        SCK = 1;
        SCK = 0;

        CS = 1;
    }else if(modo == POST_HPF1){

        MCLR = 1;

        CS=0;

        // 1
        SCK = 0;
        MOSI = 1;
        SCK = 1;

        //2
        SCK = 0;
        MOSI = 0;
        SCK = 1;

        //3
        SCK = 0;
        MOSI = 1;
        SCK = 1;

        //4
        SCK = 0;
        MOSI = 0;
        SCK = 1;

        //5
        SCK = 0;
        MOSI = 1;
        SCK = 1;

        //6
        SCK = 0;
        MOSI = 1;
        SCK = 1;

        //7
        SCK = 0;
        MOSI = 0;
        SCK = 1;

        //8
        SCK = 0;
        MOSI = 0;
        SCK = 1;
        SCK = 0;

        CS = 1;
    }
}

/* ---------------------------------------------------------------------------
 * FUN��O: mcpRecebe
 * DESCRI��O: Realiza a recep��o dos valores do canal de corrente (CH0)e do 
 *            canal de tens�o (CH1). Os valores s�o armazenados em vari�feis 
 *            globais.
 * ENTRADA: Vazio
 * RETORNO: Vazio
 * --------------------------------------------------------------------------*/
void mcpRcv(void){
    
    // #1
    SCK = 1;
    SCK = 0;
    mcp_ch1 = mcp_ch1 | MISO;
    mcp_ch1 = mcp_ch1 << 1;
    
    // #2
    SCK = 1;
    SCK = 0;
    mcp_ch1 = mcp_ch1 | MISO;
    mcp_ch1 = mcp_ch1 << 1;
        
    // #3
    SCK = 1;
    SCK = 0;
    mcp_ch1 = mcp_ch1 | MISO;
    mcp_ch1 = mcp_ch1 << 1;
        
    // #4
    SCK = 1;
    SCK = 0;
    mcp_ch1 = mcp_ch1 | MISO;
    mcp_ch1 = mcp_ch1 << 1;
        
    // #5
    SCK = 1;
    SCK = 0;
    mcp_ch1 = mcp_ch1 | MISO;
    mcp_ch1 = mcp_ch1 << 1;
        
    // #6
    SCK = 1;
    SCK = 0;
    mcp_ch1 = mcp_ch1 | MISO;
    mcp_ch1 = mcp_ch1 << 1;
        
    // #7
    SCK = 1;
    SCK = 0;
    mcp_ch1 = mcp_ch1 | MISO;
    mcp_ch1 = mcp_ch1 << 1;
        
    // #8
    SCK = 1;
    SCK = 0;
    mcp_ch1 = mcp_ch1 | MISO;
    mcp_ch1 = mcp_ch1 << 1;
        
    // #9
    SCK = 1;
    SCK = 0;
    mcp_ch1 = mcp_ch1 | MISO;
    mcp_ch1 = mcp_ch1 << 1;
        
    // #10
    SCK = 1;
    SCK = 0;
    mcp_ch1 = mcp_ch1 | MISO;
    mcp_ch1 = mcp_ch1 << 1;
        
    // #11
    SCK = 1;
    SCK = 0;
    mcp_ch1 = mcp_ch1 | MISO;
    mcp_ch1 = mcp_ch1 << 1;
        
    // #12
    SCK = 1;
    SCK = 0;
    mcp_ch1 = mcp_ch1 | MISO;
    mcp_ch1 = mcp_ch1 << 1;
        
    // #13
    SCK = 1;
    SCK = 0;
    mcp_ch1 = mcp_ch1 | MISO;
    mcp_ch1 = mcp_ch1 << 1;
        
    // #14
    SCK = 1;
    SCK = 0;
    mcp_ch1 = mcp_ch1 | MISO;
    mcp_ch1 = mcp_ch1 << 1;
        
    // #15
    SCK = 1;
    SCK = 0;
    mcp_ch1 = mcp_ch1 | MISO;
    mcp_ch1 = mcp_ch1 << 1;
        
    // #16
    SCK = 1;
    SCK = 0;
    mcp_ch1 = mcp_ch1 | MISO;
    
    // #1
    SCK = 1;
    SCK = 0;
    mcp_ch0 = mcp_ch0 | MISO;
    mcp_ch0 = mcp_ch0 << 1;
        
    // #2
    SCK = 1;
    SCK = 0;
    mcp_ch0 = mcp_ch0 | MISO;
    mcp_ch0 = mcp_ch0 << 1;
        
    // #3
    SCK = 1;
    SCK = 0;
    mcp_ch0 = mcp_ch0 | MISO;
    mcp_ch0 = mcp_ch0 << 1;
        
    // #4
    SCK = 1;
    SCK = 0;
    mcp_ch0 = mcp_ch0 | MISO;
    mcp_ch0 = mcp_ch0 << 1;
        
    // #5
    SCK = 1;
    SCK = 0;
    mcp_ch0 = mcp_ch0 | MISO;
    mcp_ch0 = mcp_ch0 << 1;
        
    // #6
    SCK = 1;
    SCK = 0;
    mcp_ch0 = mcp_ch0 | MISO;
    mcp_ch0 = mcp_ch0 << 1;
        
    // #7
    SCK = 1;
    SCK = 0;
    mcp_ch0 = mcp_ch0 | MISO;
    mcp_ch0 = mcp_ch0 << 1;
        
    // #8
    SCK = 1;
    SCK = 0;
    mcp_ch0 = mcp_ch0 | MISO;
    mcp_ch0 = mcp_ch0 << 1;
        
    // #9
    SCK = 1;
    SCK = 0;
    mcp_ch0 = mcp_ch0 | MISO;
    mcp_ch0 = mcp_ch0 << 1;
        
    // #10
    SCK = 1;
    SCK = 0;
    mcp_ch0 = mcp_ch0 | MISO;
    mcp_ch0 = mcp_ch0 << 1;
        
    // #11
    SCK = 1;
    SCK = 0;
    mcp_ch0 = mcp_ch0 | MISO;
    mcp_ch0 = mcp_ch0 << 1;
        
    // #12
    SCK = 1;
    SCK = 0;
    mcp_ch0 = mcp_ch0 | MISO;
    mcp_ch0 = mcp_ch0 << 1;
        
    // #13
    SCK = 1;
    SCK = 0;
    mcp_ch0 = mcp_ch0 | MISO;
    mcp_ch0 = mcp_ch0 << 1;
        
    // #14
    SCK = 1;
    SCK = 0;
    mcp_ch0 = mcp_ch0 | MISO;
    mcp_ch0 = mcp_ch0 << 1;
        
    // #15
    SCK = 1;
    SCK = 0;
    mcp_ch0 = mcp_ch0 | MISO;
    mcp_ch0 = mcp_ch0 << 1;
        
    // #16
    SCK = 1;
    SCK = 0;
    mcp_ch0 = mcp_ch0 | MISO;
    
}

/* ---------------------------------------------------------------------------
 * FUN��O: mcpRecebe
 * DESCRI��O: Identifica quando o dado est� dispon�vel no MCP e chama a fun��o
 *            mcpRcv, para receber os valores. Os valores s�o armazenados em
 *            dois arrays, identificados atrav�s dos ponteiros informados como
 *            par�metro de entrada da fun��o.
 * ENTRADA: Ponteiro para o array onde ser�o armazenados os valores de CH0;
 *          Ponteiro para o array onde ser�o armazenados os valores de CH1;
 *          Quantidade de amostras que ser�o armazenadas nos arrays.
 * RETORNO: Vazio
 * --------------------------------------------------------------------------*/
void mcpRead(int array_ch0[], int array_ch1[], unsigned int qntAmostras){

    unsigned int contAmostra;
    static bit f_sample;
    
    contAmostra=0;
    while(contAmostra<qntAmostras){
        CS = 0;

        if(MISO){
            f_sample=1;
            mcp_ch0=0;
            mcp_ch1=0;
        }

        if(!MISO && f_sample){

            mcpRcv();
            CS = 1;

            array_ch0[contAmostra] = mcp_ch0;
            array_ch1[contAmostra] = mcp_ch1;
            contAmostra++;

            f_sample=0;
        }
    }
}