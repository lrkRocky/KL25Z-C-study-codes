
//Realizado por lrkRocky
//Este código hace una secuancia de colores con el led
//	interno de la tarjeta KL25z.

//Se incluyen librerias con las que se trabaja en el programa.

#include <stdio.h>				//Se incluye libreria <stdio.h>.
#include "board.h"				//Se incluye libreria "board.h".
#include "peripherals.h"		//Se incluyen libreria "peripherals.h".
#include "pin_mux.h"			//Se incluye libreria "pin_mux.h".
#include "clock_config.h"		//Se incluye libreria "clock_config.h"
#include "MKL25Z4.h"			//Se incluye libreria "MKL25Z4.h"
#include "fsl_debug_console.h"	//Se incluye libreria "fsl_debug_console.h"

//Se definen macros y sus valores.
#define PB 10					//Asigna el valor de1 0 al macro PB.
#define PD 12					//Asigna el valor de 12 al macro 12

#define CR 18					//Asigna el valor de 18 al macro CR.
#define CG 19					//Asigna el valor de 19 al macro CG.
#define CB 1					//Asigna el valor de 01 al macro CB.
#define bait 8					//Asigna el valor de 88 al macro bait.

void retardo(unsigned char t)		//Inicia la función de "retardo".
{										//Declara variable t como parametro de entrada.
	unsigned int i;						//Declara variable i.
	for (i= 0; i <(36000*t) ; i++);		//Realiza ciclo for con iteraciones para tiempo.
}									//Fin de función "retardo".

//Funciones de color RGB
void colorRed()						//Inicia función de color Rojo "colorRed".
{
								//Se habilitan los colores a traves de los PCOR
								//	y desactivan con PSOR a traves del GPIO.

	GPIOB->PCOR|=(1<<CR);				//Activa el color rojo.
	GPIOB->PSOR|=(1<<CG);				//Desacriva el color verde.
	GPIOD->PSOR|=(1<<CB);				//Desactiva el color azul.
	retardo(100);						//Realiza función de retardo con t = 100.
}									//Fin de función de color rojo.
void colorGreen()					//Función de color verde "colorGreen".
{
	GPIOB->PSOR|=(1<<CR);				//Desactiva el color rojo.
	GPIOB->PCOR|=(1<<CG);				//Activa el color verda.
	GPIOD->PSOR|=(1<<CB);				//Desactiva el color azul.
	retardo(100);						//Realiza la función de retardo con t = 100
}									//Fin de función de color verde.
void colorBlue()					//Función de color azul "colorBlue".
{
	GPIOB->PSOR|=(1<<CR);				//Desactiva el color rojo.
	GPIOB->PSOR|=(1<<CG);				//Desactiva el color verdde.
	GPIOD->PCOR|=(1<<CB);				//Activa solo color azul.
	retardo(100);						//Realiza función de retardo con t = 100.
}										//Fin de función de color azul.

//Funciones de colores combinados
void colorYellow()					//Inicia Función de color naranja "colorYellow".
{
	GPIOB->PCOR|=(1<<CR);				//Activa el color rojo.
	GPIOB->PCOR|=(1<<CG);				//Activa el color verde.
	GPIOD->PSOR|=(1<<CB);				//Desactiva el color azul.
	retardo(100);						//Realiza función de retardo con t = 100.
}									//Fin de función de color amarillo.
void colorSky()						//Inicia función de color celeste "colorSky".
{
	GPIOB->PSOR|=(1<<CR);				//Desactiva el color rojo.
	GPIOB->PCOR|=(1<<CG);				//Activa color verde.
	GPIOD->PCOR|=(1<<CB);				//Activa el color azul.
	retardo(100);						//Realiza función de retardo con t = 100.
}									//Fin de función de color celeste.
void colorPurple()					//Inicia función de color morado "colorPurple".
{
	GPIOB->PCOR|=(1<<CR);				//Activa el color rojo.
	GPIOB->PSOR|=(1<<CG);				//Desactiva color verde.
	GPIOD->PCOR|=(1<<CB);				//Activa el color azul.
	retardo(100);						//Realiza función de retardo con t = 100.
}									//Fin de función de color morado.
void colorWhite()					//Inicia función de color blanco "colorWhite".
{
	GPIOB->PCOR|=(1<<CR);				//Activa el color rojo.
	GPIOB->PCOR|=(1<<CG);				//Activa el color verde.
	GPIOD->PCOR|=(1<<CB);				//Activa el color azul.
	retardo(100);						//Realiza función de retardo con t = 100.
}									//Fin de función de color blanco.
//Función principal
int main(void) {						//Inicia función principal

    /* Init board hardware. */
    BOARD_InitBootPins();					//Realiza función que inicializa los pines de la tarjeta.
    BOARD_InitBootClocks();					//Realiza función que inicializa los relojes de la tarjeta
    BOARD_InitBootPeripherals();			//Realiza función que inicializa los periféricos de la tarjeta.
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL	//comprueba si el periférico de consola de
    										//	depuración FSL ya ha sido inicializado
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();				//Inicializa el periférico de consola
    										// 	de depuración FSL en caso de no estarlo.
#endif

    SIM->SCGC5|=(1<<PB);			// Activa el reloj para el puerto B.
    SIM->SCGC5|=(1<<PD);			// Activa el reloj para el puerto D
    								//se configuran los pines de los LED RGB mediante
    								//	la modificación de los registros PCR
    PORTB->PCR[18]|=(001<<bait);	//Se configura pin de  LED rojo.
    PORTB->PCR[19]|=(001<<bait);	//Se configura pin de  LED verde.
    PORTD->PCR[1]|=(001<<bait);		//Se configura pin de  LED azul.
    								//Se configura la dirección de los pines como salidas
    								//	en el registro PDDR
    GPIOB->PDDR|=(1<<CR);			//Establece el pin del LED rojo como salida.
    GPIOB->PDDR|=(1<<CG);			//Establece el pin del LED verde como salida.
    GPIOD->PDDR|=(1<<CB);			//Establece el pin del LED azul como salida.


    while(1){						//Inicia bucle para secuencia de colores.
    							//Funciones de colores para crear secuencia.
    	colorRed();					//Función de color rojo "colorRed".
		colorGreen();				//Función de color verde "colorGreen".
		colorBlue();				//Función de color azul "colorBlue".
		colorWhite();				//Función de color blanco "colorWhite".
		colorYellow();				//Función de color amarillo "colorYellow".
		colorSky();					//Función de color turqueza "colorSky".
		colorPurple();				//Función de color turqueza "colorPurple".
    }								//Fin de bucle.

    /* Force the counter to be placed into memory. */
    volatile static int i = 0 ;
    /* Enter an infinite loop, just incrementing a counter. */

    return 0 ;						//Retorna valor 0 en función principal.
}								//Fin de función principal.