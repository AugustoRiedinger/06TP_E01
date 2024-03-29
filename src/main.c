/********************************************************************************
  * @file    main.c
  * @author  G. Garcia & A. Riedinger
  * @version 0.1
  * @date    04-10-21
  * @brief   Recepción de datos a través del protocolo RS232 desde una PC
  * 		 por un cable RS232-USB.

  * SALIDAS:
  	  *	LCD  	Conexion Estandar TPs
  * ENTRADAS:
  	  * RS232 	RX - PA3
********************************************************************************/

/*------------------------------------------------------------------------------
LIBRERIAS:
------------------------------------------------------------------------------*/
#include "mi_libreria.h"

/*------------------------------------------------------------------------------
DEFINICIONES:
------------------------------------------------------------------------------*/
/*Parámetros de configuración del TIM3 para refresco del LCD:*/
#define Freq 	 4
#define TimeBase 200e3

/*Pines del RX UART:*/
#define RX_Port	GPIOA
#define RX		GPIO_Pin_3

/*Velocidad de trabajo del UART:*/
#define BaudRate 9600

/*Maximo tamaño de palabra:*/
#define MaxDataBits 8

/*------------------------------------------------------------------------------
VARIABLES GLOBALES:
------------------------------------------------------------------------------*/
/*Definicion de los pines del LCD:*/
LCD_2X16_t LCD_2X16[] = {
			// Name  , PORT ,   PIN      ,         CLOCK       ,   Init
			{ TLCD_RS, GPIOC, GPIO_Pin_10, RCC_AHB1Periph_GPIOC, Bit_RESET },
			{ TLCD_E,  GPIOC, GPIO_Pin_11, RCC_AHB1Periph_GPIOC, Bit_RESET },
			{ TLCD_D4, GPIOC, GPIO_Pin_12, RCC_AHB1Periph_GPIOC, Bit_RESET },
			{ TLCD_D5, GPIOD, GPIO_Pin_2,  RCC_AHB1Periph_GPIOD, Bit_RESET },
			{ TLCD_D6, GPIOF, GPIO_Pin_6,  RCC_AHB1Periph_GPIOF, Bit_RESET },
			{ TLCD_D7, GPIOF, GPIO_Pin_7,  RCC_AHB1Periph_GPIOF, Bit_RESET }, };

/*Variable para almacenamiento de datos recibidos:*/
char  CharRec[MaxDataBits];

/*Variable para concatenar los caracteres recibidos en una cadena:*/
/*Tamaño es la cantidad maxima de caracteres a recibir (1000) por el maximo de palabra mas margen de seguridad:*/
char  DataString[1200*MaxDataBits];

/*Variable para contar los caracteres:*/
int   StringLength = 0;

/*Variable para contar el tiempo de lectura:*/
float OpTime = 0;

int main(void)
{
/*------------------------------------------------------------------------------
CONFIGURACION DEL MICRO:
------------------------------------------------------------------------------*/
	SystemInit();

	/*Inicializacion del DISPLAY LCD:*/
	INIT_LCD_2x16(LCD_2X16);

	/*Inicializacion del puerto serie en el pin:*/
	INIT_USART_RX(RX_Port, RX, BaudRate);

	/*Inicialización del TIM3 para refresco del LCD:*/
	INIT_TIM3();
	SET_TIM3(TimeBase, Freq);

/*------------------------------------------------------------------------------
BUCLE PRINCIPAL:
------------------------------------------------------------------------------*/
    while(1)
    {
    	/*Si se recibe un dato en el puerto serie:*/
		if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) != RESET) {
			/*Se guarda lo recibido:*/
			CharRec[0] = USART_ReceiveData(USART2);

			/*Se concatena el caracter recibido con los anteriores para formar la cadena:*/
			strcat(DataString,CharRec);
		}

		/*Calculo de la cantidad de caracteres en la cadena:*/
		StringLength = strlen(DataString);

		/*Calculo del tiempo de operacion:*/
		OpTime = (float) StringLength / BaudRate;
    }
}
/*------------------------------------------------------------------------------
INTERRUPCIONES:
------------------------------------------------------------------------------*/
/*Interrupción por agotamiento de cuenta del TIM3 cada 250mseg (4 Hz):*/
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET) {
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);

		/*Buffers para almacenamiento de datos:*/
		char BufferStringLength[1200*MaxDataBits];
		char BufferCharRec[MaxDataBits];
		char BufferOpTime[BufferLength];

		/*Refresco del LCD: */
		CLEAR_LCD_2x16(LCD_2X16);

		/*Copiar datos a los buffers para imprimir:*/
		sprintf(BufferCharRec, "%c", CharRec[0]);
		sprintf(BufferStringLength, "%d", StringLength);
		sprintf(BufferOpTime, "%.2f", OpTime);

		/*Mensaje para indicar el ultimo caracter leido:*/
		PRINT_LCD_2x16(LCD_2X16, 0, 0, "Ultimo char: ");
		PRINT_LCD_2x16(LCD_2X16, 13, 0, BufferCharRec);

		/*Mensaje para indicar la cantidad de caracteres leidos:*/
		PRINT_LCD_2x16(LCD_2X16, 0, 1, "Cant:");
		PRINT_LCD_2x16(LCD_2X16, 5, 1, BufferStringLength);

		/*Mensaje para indicar el tiempo de operacion.*/
		PRINT_LCD_2x16(LCD_2X16, 10, 1, "T:");
		PRINT_LCD_2x16(LCD_2X16, 12, 1, BufferOpTime);
	}
}
