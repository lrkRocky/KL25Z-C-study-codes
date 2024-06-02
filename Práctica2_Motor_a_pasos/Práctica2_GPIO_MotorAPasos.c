/**
 * @file    Práctica2_GPIO_MotorAPasos.c
 * @brief   Application entry point.
 * Realizado por: lrkRocky
 * Este código realiza una secuencia para motor a pasos controlada.
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
#include "Pract2Lib.h"			//Se incluye libreria "Pract2Lib.h"

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
    SetPinsInPD();						//Función que establece pines de entrada con dirección y su PDDR.

    								//Establecimiento de varibbles por utilizar y sus valores iniciales.
    uint32_t InState	 = GPIOD->PDIR;	//Variable que guarda la lectura de estado en pines de entrada.
    uint32_t Stop;						//Variable que guarda la lectura de estado en pin de paro o pausa.
    uint32_t Turning;					//Variable que guarda la lectura de estado en pin de sentido de giro.
    uint32_t Speed;						//Variable que guarda la lectura de estado en pin de velocidad.
    uint32_t pressed 	= 0;			//Variable que guarda conteo de veces presionadas en botón de velocidad.

    while(1){									//Inicia bucle infinito para repetir programa.
    	InState = GPIOD->PDIR;					//Lee periferico D con PDIR y guarda en variable InState,
    	Stop 	= (InState & PTD4);				//Guarda estado de pin PTD4 como valor en variable Stop.
    	Turning = (InState & PTD5);				//Guarda estado de pin PTD5 como valor en variable Turning.
    											//Lógica inversa por conf. pull up de interruptores.
    	if(Stop == PTD4){						//Si el botón de pausa no esta presionado (tras leerlo):
    		if(Turning == PTD5){
    			Sequence(0x01,0x03, 0x06, 0x0C);	//Realiza secuencia 1,3,6,C'H (full step)  para motor a pasos
			}										//	en caso de tener botón de sentido sin presionar.
			else{
				Sequence(0x0C,0x06, 0x03, 0x01);	//Realiza secuencia C'H,3,1,  (full step) para motor a pasos
			}										//	en caso de tener botón de sentido presionado.
    	}
    	else{									//Si el boton de pausa está presionado:
    		while(Stop != PTD4){				//Mantiene leyendo repetitivamente si el botón esta presionado.
    			InState = GPIOD ->PDIR;
    			Stop 	= (InState & PTD4);
    		}									//Cuando se deja de presionar el botón de pausa, continua el programa.
    	}										//Regresa a inicio de buble infinito principal de programa.

    }											//Fin de bucle principal del programa.

    /* Force the counter to be placed into memory. */
    volatile static int i = 0 ;
    /* Enter an infinite loop, just incrementing a counter. */

    return 0 ;									//Retorna valor 0 en función principal.
}												//Fin de función principal.
