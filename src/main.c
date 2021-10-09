/********************************************************************************
  * @file    main.c
  * @author  G. Garcia & A. Riedinger
  * @version 0.1
  * @date    04-10-21
  * @brief   Envío de datos a través del protocolo RS232 a una PC
  * 		 por un cable RS232-USB..

  * SALIDAS:
  	  *	LCD  	Conexion Estandar TPs
	  * RS232 	TX - PCXX
  * ENTRADAS:
  	  * UserBut PC13
********************************************************************************/

/*------------------------------------------------------------------------------
LIBRERIAS:
------------------------------------------------------------------------------*/
#include "mi_libreria.h"

/*------------------------------------------------------------------------------
DEFINICIONES:
------------------------------------------------------------------------------*/
/*Pines del RX UART:*/
#define RX_Port	GPIOA
#define RX		GPIO_Pin_3

/*Velocidad de transmision:*/
#define BaudRate 9600

/*------------------------------------------------------------------------------
VARIABLES GLOBALES:
------------------------------------------------------------------------------*/
/*Definicion de los pines del LCD: */
LCD_2X16_t LCD_2X16[] = {
			// Name  , PORT ,   PIN      ,         CLOCK       ,   Init
			{ TLCD_RS, GPIOC, GPIO_Pin_10, RCC_AHB1Periph_GPIOC, Bit_RESET },
			{ TLCD_E,  GPIOC, GPIO_Pin_11, RCC_AHB1Periph_GPIOC, Bit_RESET },
			{ TLCD_D4, GPIOC, GPIO_Pin_12, RCC_AHB1Periph_GPIOC, Bit_RESET },
			{ TLCD_D5, GPIOD, GPIO_Pin_2,  RCC_AHB1Periph_GPIOD, Bit_RESET },
			{ TLCD_D6, GPIOF, GPIO_Pin_6,  RCC_AHB1Periph_GPIOF, Bit_RESET },
			{ TLCD_D7, GPIOF, GPIO_Pin_7,  RCC_AHB1Periph_GPIOF, Bit_RESET }, };

/*Variable para almacenamiento de datos recibidos:*/
char Data;

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

	/*Inicializacion UserLED_Green de prueba:*/
	INIT_DO(GPIOB, GPIO_Pin_0);

/*------------------------------------------------------------------------------
BUCLE PRINCIPAL:
------------------------------------------------------------------------------*/
    while(1)
    {
    	/*Dato recibido:*/
		if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) != RESET)
		{
			/*Se guarda lo recibido en la varibale Data:*/
			Data = USART_ReceiveData(USART2);

			/*Se prende un LED para indicar que se recibió un dato:*/
			GPIO_SetBits(GPIOB, GPIO_Pin_0);
		}
    }
}
/*------------------------------------------------------------------------------
INTERRUPCIONES:
------------------------------------------------------------------------------*/
