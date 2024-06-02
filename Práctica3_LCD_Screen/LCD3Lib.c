/*
 * LCD3Lib.c
 *
 *  Created on: 19 sep. 2023
 *      Author: lrkRocky
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"            //Inclusión de librerias utilizadas
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "LCD3Lib.h"

uint32_t POS;
uint32_t conteo;
uint32_t d ;                        //Declaración de variables utilizadas
uint32_t sincho ;
uint32_t linea;

void EnableClocks(){
    SIM->SCGC5 |= PD_clock;         //Se activa el reloj int1erno del PUERTO D
}
void EnablePinsPD(){
    PORTD->PCR[0x0] |=bait;
    PORTD->PCR[0x1] |=bait;
    PORTD->PCR[0x2] |=bait;         //Habilitación de pines en gpio en el MUX con el registro PCR.
    PORTD->PCR[0x3] |=bait;
    PORTD->PCR[0x4] |=bait;
    PORTD->PCR[0x5] |=bait;
    PORTD->PCR[0x6] |=bait;
}
void SetPinsOutPD(){
    GPIOD->PDDR|= (1 << 0);
    GPIOD->PDDR|= (1 << 1);
    GPIOD->PDDR|= (1 << 2);
    GPIOD->PDDR|= (1 << 3);         //Establece pines de salida asignando 1 al bit del pin en su registro PDDR.
    GPIOD->PDDR|= (1 << 4);
    GPIOD->PDDR|= (1 << 5);
}

void retardo(uint32_t t)                //Función de retardo.
{                                       //Declara variable t como parametro de entrada en
                                        //  donde se indican las repeticiones del retardo
    uint32_t Load = 0x01481FFU;            //Asigna valor para cargar al temporizador SysTick y obtener 28ms
    unsigned int reps;                  //Se declara variable para repetir retardo de 28ms.
    for (reps= 0; reps < t ; reps++){   //Estructura de iteración para repetir y facilitar retardos base de 28ms en retardo.
        uint32_t i = 0;                 //Declaración de variable utilizada para leer bandera de SysTick
        SysTick->CTRL = 0;              //Apaga bandera y resetea bits reservados de CTRL en SysTick.
        SysTick->LOAD = Load;           //Se le carga la cantidad definida previamente a LOAD para la duración de Systick en retardo base.
        SysTick->CTRL |= 0x04U;         //Se indica tomar señal de reloj de procesador con bit de clksource
        SysTick->CTRL |= 0x01U;         //Habilita Systick como contador con bit de enable en CTRL.
        while(i==0){                    //Estructura de iteración para mantener mientras la bandera esté apagada.
            i = SysTick->CTRL;          //Lee estado de registro CTRL de Systick y almacena en variable i.
            i &= (1<<16);               //Toma el valor del bit de bandera en lectura previa y la asigna como nuevo valor a variable i.
                                        //Se realiza retardo base de 01481FF'H iteraciones con el contador descendente de Systick
                                        //  hasta lograr tiempo de 28ms utilizando la ecuación N = t(SisClk)- 1 = 28ms*(48Mhz)- 1
        }
    }
}

void printLCD(unsigned char linea, unsigned char *mensaje){     //Función de escritura de mensaje en display.
                                                                //Se indican el texto por escribir y la linea en la que se escribirá.
    if(linea == 1){                                             //Condicional para seleccionar linea en la que se escribirá el mensaje.
        POS = 0x80;                                             //Si se selecciona la linea 1, se asigna valor de 80'H a la posición
        }
    else{
        POS = 0xC0;                                             //Si se selecciona la linea 1, se asigna valor de C0'H a la posición
    }
    SendByte(cmd, POS);                                     //Se envia comando con la posición asignada previamente.
    while (*mensaje) {                                          //Condicional para apuntar a mensaje y escribirlo completo.
        SendByte(dato, *mensaje++);                             //Se envia letra de mensaje como dato y se incrementa el valor a puntero.
    }
}
                                                                //Función de envio de información por medio de bus de datos.
void SendByte(unsigned char datatype, unsigned char dataa){     //Se escribe el tipo de información (dato o comando) y la información
                                                                //  que se desea enviar como parametros de entrada.
    if(datatype == 0x01){                       //Condicional dependiente de tipo de dato señalado.
        GPIOD->PSOR |= RS;                      //Si es un dato (definido con valor de 1), se activa pin de RS.
    }
    else{
        GPIOD->PCOR |= RS;                      //Si es un comando (definido con valor de cmd = 0), se desactiva pin de RS.
    }
    GPIOD->PDOR |= ((dataa>>4)&0x0F);           //Envia nibble más significativo a bus de datos.
    enable();                                   //Realiza función de enable.
    GPIOD->PDOR &=~ ((dataa>>4)&0xF);           //Limpia información previamente enviada en bus de datos
    GPIOD->PDOR |= (dataa & 0x0F);              //Envia nibble menos significativo a bus de datos.
    enable();                                   //Realiza función de enable.
    GPIOD->PDOR &=~  (dataa & 0x0F);            //Limpia información previamente enviada en bus de datos
    GPIOD->PCOR |= RS;                          //Se desactiva pin de RS.
}

void enable(void)                               //Función de enable.
{
    GPIOD->PSOR |= E;                           //Se activa pin de E.
    retardo(02);                                //Realiza 2 veces retardo.
    GPIOD->PCOR |= E;                           //Desactiva pin de E utilizado.
}

void ini_lcd(){                                 //Función de inicialización de display LCD
    GPIOD->PCOR |= RS;
    GPIOD->PCOR |= E;                           //Desactiva pines de Rs y E con PCOR.
    retardo(22);                                //Realiza retardo 22 veces.
    GPIOD->PDOR |= 0x02;                        //Envia 0x02 a puerto indicando inicialización a 4 bits.
    enable();                                   //Realiza función de enable.
    GPIOD->PDOR &=~ 0x02;                       //Apaga bits utilizados en bus de datos.

    SendByte( cmd, 0x28);                       //Envia comando de function set
    SendByte( cmd, 0x28);                       //Envia comando de function set
    SendByte( cmd, 0x0F);                       //Envia comando de encendido/apagado de display.
    SendByte( cmd, 0x01);                       //Envia comando de limpiar display */
}
