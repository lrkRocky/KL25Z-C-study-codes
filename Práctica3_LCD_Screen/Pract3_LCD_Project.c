/**
 * @file    Pract3_LCD_Project.c
 * @brief   Application entry point.
 */
//Realizado por lrkRocky
//Este código escribe un mensaje en un display LCD 16x2 a 4 bits.

/** Notas.
 *      Salida en puerto D (4 bits menos significativos).
 *      RS en pin PD4
 *      E en pin PD5
 */

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"       //Inclusión de librerias utilizadas
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "LCD3Lib.h"

int main(void){
    BOARD_InitBootPins();
    BOARD_InitBootClocks();         //Inicializacón de relojes de perifericos.
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    uint32_t POS;
    uint32_t conteo=0;
    uint32_t d = 0;
    uint32_t sincho = 0;                //Declaración de variables utilizadas y sus valores iniciales.
    uint32_t linea;

    EnableClocks();                     //Activación de reloj interno de puerto utilizado.
    EnablePinsPD();                     //Habilitación de pines utilizados.
    SetPinsOutPD();                     //Establecimiento de pines de salida.

    GPIOD->PDOR &=~ 0xFF;               //Limpieza de  pines del puerto D.
    ini_lcd();                          //Inicialización de pantalla.

    printLCD(1, "Hola mundo 0u0");      //Impresión de mensaje en linea 1 de display LCD.
    printLCD(2, "Rogelio Meza c:");     //Impresión de mensaje en linea 2 de display LCD.
    while(1){}                          //Bucle infinito sin procesos al terminar programa principal.

    return 0 ;
}
