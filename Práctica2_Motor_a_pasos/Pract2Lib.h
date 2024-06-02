/*
 * Pract2Lib.h
 *
 *  Created on: 6 sep. 2023
 *      Author: lrkRocky
 */

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
#define bait (1<<8)						//Bit utilizado en Mux de SCGC5 para habilitar GPIO
										//Recibe nombre de "bait" como referencia a palabra "byte" de 8 bits.

void EnableClocks();
void EnablePinsPD();					//Declaración de funciones utilizadas para activación o habilitación
void SetPinsOutPD();					// de relojes, perifericos, pines y configuraciones iniciales.
void SetPinsInPD();

void Sequence( unsigned int seq1, unsigned int seq2, unsigned int seq3, unsigned int seq4);
void retardo(unsigned char t);			//Declaración de funciones utilizadas para procesos secundarios
										//	y sus parámetros de entrada.
#ifndef PRACT2LIB_H_
#define PRACT2LIB_H_
#endif /* PRACT2LIB_H_ */
