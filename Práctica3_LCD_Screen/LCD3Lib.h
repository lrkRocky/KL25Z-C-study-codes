/*
 * LCD3Lib.h
 *
 *  Created on: 19 sep. 2023
 *      Author: lrkRocky
 */

#define PD0 0
#define PD1 1
#define PD2 2
#define PD3 3                       //Numeros de pines asignados a su nombre común.
#define PD4 4
#define PD5 5
#define DB      ((1 << PD0) | (1 << PD1) | (1 << PD2) | (1 << PD3))
                                    //Arreglo de pines de salida para comodidad de escritura en bus de datos.
#define RS (1 << PD4)               //Bits de pines asignados a sus nombres propuestos.
#define E (1 << PD5)
#define PD_clock (1 << 12u)         //Bit de registro SCGC5 que habilita reloj de periferico D.
#define bait (1<<8)                 ///Bit utilizado en Mux de SCGC5 para habilitar GPIO
                                    //  Recibe nombre de "bait" como referencia a palabra "byte" de 8 bits.
#define cmd 0x00                    //Definición de valores propuestos para control en tipo de
#define dato 0x01                   //  información enviada por bus de datos.

void EnableClocks();
void EnablePinsPD();                //Declaración de funciones utilizadas para activación o habilitación
void SetPinsOutPD();                // de relojes, perifericos, pines y configuraciones iniciales.

void retardo(uint32_t t);           //Declaración de funciones utilizadas para procesos secundarios
void enable(void);                  //  y sus parámetros de entrada.
void ini_lcd();
void SendByte(unsigned char datatype, unsigned char dataa);
void printLCD(unsigned char linea, unsigned char *mensaje);

#ifndef LCD3LIB_H_
#define LCD3LIB_H_
#endif /* LCD3LIB_H_ */
