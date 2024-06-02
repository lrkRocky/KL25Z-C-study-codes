/*
 * Pract2Lib.c
 *
 *  Created on: 6 sep. 2023
 *      Author: lrkRocky
 */

#include <stdio.h>				//Se incluye libreria <stdio.h>.
#include "board.h"				//Se incluye libreria "board.h".
#include "peripherals.h"		//Se incluyen libreria "peripherals.h".
#include "pin_mux.h"			//Se incluye libreria "pin_mux.h".
#include "clock_config.h"		//Se incluye libreria "clock_config.h"
#include "MKL25Z4.h"			//Se incluye libreria "MKL25Z4.h"
#include "fsl_debug_console.h"	//Se incluye libreria "fsl_debug_console.h"
#include "Pract2Lib.h"			//Se incluye libreria "Pract2Lib.h"

								//Declaración de variables utilizadas en funciones de programa.
uint32_t InState;				//Variable que guarda la lectura de estado en pines de entrada.
uint32_t Speed;					//Variable que guarda la lectura de estado en pin de velocidad.
uint32_t pressed;				//Variable que guarda conteo de veces presionadas en botón de velocidad.
uint32_t tUsed;					//Variable que multiplica y controla el tiempo de retardo utilizado.

void EnableClocks(){
    SIM->SCGC5 |= PD_clock;			//Se activa el reloj interno del PUERTO D
}
void EnablePinsPD(){
    PORTD->PCR[0x0] |=bait;
    PORTD->PCR[0x1] |=bait;
    PORTD->PCR[0x2] |=bait;			//Habilitación de pines en gpio en el MUX con el registro PCR.
    PORTD->PCR[0x3] |=bait;
    PORTD->PCR[0x4] |=bait;
    PORTD->PCR[0x5] |=bait;
    PORTD->PCR[0x6] |=bait;
}
void SetPinsOutPD(){
	//Establece pines de salida asignando 1 al bit del pin en su registro PDDR.
	GPIOD->PDDR|= (1 << 0);
	GPIOD->PDDR|= (1 << 1);
	GPIOD->PDDR|= (1 << 2);
	GPIOD->PDDR|= (1 << 3);
}

void SetPinsInPD(){
	//Establece pines de entrada asignando 0 al bit del pin en su registro PDDR.
    GPIOD->PDDR&=~ (1 << 4);	//Establece el pin como entrada. Recorro número de pin.
    GPIOD->PDDR&=~ (1 << 5);	//Establece el pin como entrada. Recorro número de pin.
    GPIOD->PDDR&=~ (1 << 6);	//Establece el pin como entrada. Recorro número de pin.
}


void retardo(unsigned char t)				//Inicia la función de "retardo".
{											//Declara variable t como parametro de entrada.
	InState = GPIOD ->PDIR;					//Realiza lectura de periferico D.
	Speed 	= (InState & PTD6);				//Guarda estado de pin PTD6 como valor en la variable Speed.
	if(Speed == PTD6){}						//Si boton no está presionado solo continua programa.
	else{									//Si está presionado:
		while(Speed != PTD6){				//Se ejecuta estructura while para detener programa si sigue botón
			InState = GPIOD ->PDIR;			//	de velocidad presionado, en la que lee y guarda repetitivamente
			Speed 	= (InState & PTD6);		//	el estado del pin hasta que el botón deja de presionarse.
		}
		pressed = pressed + 1;				//Aumenta 1 a conteo de presiones de botón de velocidad.
	}
	if((pressed > 4) || (pressed == 0)){	//Si el conteo de botón presionado llega a 4, o no ha incrementado,
		pressed = 0;						//	reinicia su valor de conteo
		tUsed = t;							// y reinicia valor original de tiempo para retardo.
	}
	else{									//Si el botón de velocidad ha sido presionado
		tUsed = t - pressed;				//	modifica el multiplicador de tiempo en base a las veces presionadas.
	}

	unsigned int i;							//Declara variable i.
	for (i= 0; i <(3600*tUsed) ; i++);		//Realiza ciclo for con iteraciones para tiempo controladas con.
											//	parámetros de entrada de función y conteo de velocidad
											//	controlado por botón de velocidad.
}

void Sequence( unsigned int seq1, unsigned int seq2, unsigned int seq3, unsigned int seq4){
													//Declaración de función para salida de control de motor a pasos.
	GPIOD->PDOR =  (GPIOD->PDOR &~ PDOut | seq1);	//Asigna valor a pines de salida para control de motor a pasos.
	retardo(6);										//Pausa un momento con función de retardo.
	GPIOD->PDOR =  (GPIOD->PDOR &~ PDOut | seq2);	//Repite con 4 cantidades distintas en total.
	retardo(6);
	GPIOD->PDOR =  (GPIOD->PDOR &~ PDOut | seq3);
	retardo(6);
	GPIOD->PDOR =  (GPIOD->PDOR &~ PDOut | seq4);
	retardo(6);
}
