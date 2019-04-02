# Informações Gerais

O mcp3909 é um CI medidor de energia que possui interface de comunicação SPI e saídas que geram pulsos proporcionais ao valor de energia ativa.

A pasta mcpLib.X contém o projeto desenvolvido com a IDE MPLAB X, utilizando o compilador XC8. Foi utilizado o microcontrolador PIC18F2620, com cristal de 12Mhz e PLL ativo, para gerar um clock de 48Mhz.

# Sobre o código

O código inicializa a comunicação com o CI e, a cada 1 segundo, realiza a leitura de 250 amostras (valores de ch0 e ch1 gerados pelo CI). Após a leitura, os valores são enviados através da USART do microcontrolador, para que possam ser visualizados em um PC. Um led pisca a cada leitura realizada.

No arquivo main.c está o loop infinito e uma função com rotinas executadas antes de iniciar o loop infinito;

No arquivo init.h estão as configurações do microcontrolador, #defines e variáveis globais;

No arquivo serial.h estão rotinas referentes à interface serial (USART) do microcontrolador.

No arquivo mcp3909.h estão as funções e rotinas para comunicação com MCP3909, que estão detalhadas a seguir.

# Sobre a biblioteca MCP3909.h
O MCP3909 possui três modos serial, no entanto, esta versão inicial só contempla o modo Dual Channel Output. 
Na parte superior do arquivo estão os #defines que relacionam os pinos do microcontrolador com os pinos do MCP3909.

As funções disponíveis:

1)	void <b>mcpSetGain</b>(unsigned char ganho)

Determina o ganho do canal de corrente (CH0), conforme o valor indicado como parâmetro de entrada da função.

ganho = 0: CH0 Gain = 01 | Tensão máxima = 470mV;

ganho = 1: CH0 Gain = 02 | Tensão máxima = 235mV;

ganho = 2: CH0 Gain = 08 | Tensão máxima = 60mV;

ganho = 3: CH0 Gain = 16 | Tensão máxima = 30mV.

(Mais detalhes na página 19 do datasheet).

2)	unsigned char <b>mcpGetGanho</b>(void)

Retorna o valor do ganho definido através da função mcpSetGain().

3)	void <b>mcpInitSPI</b>(unsigned char modo)

Inicializa a comunicação serial no modo definido como parâmetro de entrada (variável do tipo unsigned char).

Modo = PRE_HPF1 : Dual Channel Output Pre HPF1;

Modo = POST_HPF1: Dual Channel Output Post HPF1.

(Mais detalhes na página 25 do datasheet).

4)	void <b>mcpRead</b>(int array_ch0[], int array_ch1[], unsigned int qntAmostras)

Realiza uma quantidade específica de leituras e armazena os valores em dois vetores. A função recebe como parâmetro de entrada dois ponteiros (indicando os vetores para armazenamento) e a quantidade de leituras que devem ser realizadas.

