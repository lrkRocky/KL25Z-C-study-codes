/**
 * @file    Práctica5_Interrupciones_motor_a_pasos_Project.c
 * @brief   Application entry point.
 *  Realizado por: lrkRocky
 * Este código realiza una secuencia para motor a pasos con interrupciones.
 */

/**Notas**
 * El código está diseñado para trabajar con botones en configuración resistor pull up.
 * Todos los pines están en periferico D: 0 - 4 salidas y 5 - 6 entradas.
 *
 * El código es un control lógico de motor a pasos, con un boton para cambiar entre 4
 * 	velocidades (con pulso unitario), uno para cambiar el sentido de giro (mantener
 * 	presionado) y otro para pausar secuancia (mantener presionado).
 */

//Se incluyen librerias con las que se trabaja en el programa.

#include <stdio.h>				//Se incluye libreria <stdio.h>.
#include "board.h"				//Se incluye libreria "board.h".
#include "peripherals.h"		//Se incluyen libreria "peripherals.h".
#include "pin_mux.h"			//Se incluye libreria "pin_mux.h".
#include "clock_config.h"		//Se incluye libreria "clock_config.h"
#include "MKL25Z4.h"			//Se incluye libreria "MKL25Z4.h"
#include "fsl_debug_console.h"	//Se incluye libreria "fsl_debug_console.h"

#define PD0 0
#define PD1 1
#define PD2 2							//Numeros de pines asignados a su nombre común.
#define PD3 3
#define PTD4 (1 << 4)
#define PTD5 (1 << 5)					//Bits de pines asignados a sus nombres correspondientes.
#define PTD6 (1 << 6)
#define PDOut ((1 << PD0) | (1 << PD1) | (1 << PD2) | (1 << PD3))
										//Arreglo de pines de salida para comodidad de escritura.
#define PD_clock (1 << 12u)				//Bit de registro SCGC5 que habilita reloj de periferico D.
#define PA_clock (1 << 9u)				//Bit de registro SCGC5 que habilita reloj de periferico A.
#define bait (1<<8)						//Bit utilizado en Mux de SCGC5 para habilitar GPIO
										//Recibe nombre de "bait" como referencia a palabra "byte" de 8 bits.

void EnableClocks();
void EnablePinsPD();					//Declaración de funciones utilizadas para activación o habilitación
void SetPinsOutPD();					// de relojes, perifericos, pines y configuraciones iniciales.

void retardo(unsigned char t);			//Declaración de funciones utilizadas para procesos secundarios
										//	y sus parámetros de entrada.

//Función principal
int main(void) {							//Inicia función principal

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

    EnableClocks();						//Función que activa señal de reloj de perifericos usados.
    EnablePinsPD();						//Función que configura pines utilizados como GPIO en MUX con PCR.
    SetPinsOutPD();						//Función que establece pines de salida con dirección y su PDDR.

    								//Configuración de interrupciones
    NVIC -> ISER[0] |= 1 << 30 ;		//Habilita interrupción de GPIO puerto A
    NVIC -> ISER[0] |= 1 << 31 ;		//Habilita interrupción de GPIO puerto D
    NVIC -> IP[7] |= 3 << 22 ;			//Le baja la prioridad de interrupción  IRQ30 ante las demás del mismo registro IP7.

    PORTA-> PCR[13] |= 	1<< 8			//Habilita GPIO de pin (en mux).
    				|	3				//Configura como entrada con resistor pullup (En PE yPS).
					|  	10 << 16 ;		//Re

    PORTD-> PCR[5] |= 	1<< 8			//Habilita GPIO de pin (en mux).
        			|	3				//Configura como entrada con resistor pullup (En PE yPS).
    				|  	10 << 16 ;		//Configura activación de interrupción con flanco de bajada (En IRQC).

    while(1){									//Inicia bucle infinito para repetir programa.
    }											//Fin de bucle principal del programa.

    return 0 ;									//Retorna valor 0 en función principal.
}												//Fin de función principal.

void PORTA_IRQHandler(void)
{

	unsigned int ii = 0;
	for (ii= 0; ii <1575 ; ii++){
		unsigned int seq1 = 0x01;
		unsigned int seq2 = 0x03;						//Asigna valor a las variables que se utilizan para las secuencias lógicas
		unsigned int seq4 = 0x06;						//	del motor a pasos.
		unsigned int seq6 = 0x0C;

		GPIOD->PDOR =  (GPIOD->PDOR &~ PDOut | seq1);	//Asigna valor a pines de salida para control de motor a pasos.
		retardo(6);										//Pausa un momento con función de retardo.
		GPIOD->PDOR =  (GPIOD->PDOR &~ PDOut | seq2);
		retardo(6);
		GPIOD->PDOR =  (GPIOD->PDOR &~ PDOut | seq4);
		retardo(6);
		GPIOD->PDOR =  (GPIOD->PDOR &~ PDOut | seq6);
		retardo(6);

	}

	PORTA->ISFR = (1 << 13); 							// Limpia la bandera de interrupción del pin 13 de PORTA
}

void PORTD_IRQHandler(void)
{
	unsigned int iii = 0;
	for (iii= 0; iii <1575 ; iii++){
		unsigned int seq2 = 0x0C;						//Asigna valor a las variables que se utilizan para las secuencias lógicas
		unsigned int seq4 = 0x06;						//	del motor a pasos.
		unsigned int seq6 = 0x03;
		unsigned int seq7 = 0x01;

		GPIOD->PDOR =  (GPIOD->PDOR &~ PDOut | seq2);	//Asigna valor a pines de salida para control de motor a pasos.
		retardo(6);										//Pausa un momento con función de retardo.
		GPIOD->PDOR =  (GPIOD->PDOR &~ PDOut | seq4);
		retardo(6);
		GPIOD->PDOR =  (GPIOD->PDOR &~ PDOut | seq6);
		retardo(6);
		GPIOD->PDOR =  (GPIOD->PDOR &~ PDOut | seq7);
		retardo(6);
	}
}