/**
 * @file    Práctica4_melodia_Project.c
 * @brief   Application entry point.
 *
 * Realizado por lrkRocky
 * Este programa reproduce una melodia en buzzer.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
/* TODO: insert other definitions and declarations here. */
#define PD0 0
#define PD1 1
#define PD2 2
#define PD3 3                   //Numeros de pines asignados a su nombre común.
#define PD4 4
#define PD5 5
#define POut (1 << PD0)         //Bits de pines asignados a sus nombres propuestos.
#define PD_clock (1 << 12u)     //Bit de registro SCGC5 que habilita reloj de periferico D.
#define bait (1<<8)             ///Bit utilizado en Mux de SCGC5 para habilitar GPIO
                                //  Recibe nombre de "bait" como referencia a palabra "byte" de 8 bits.
#define Sib3  233
#define Si3  247
#define Do4  266				//Define cada nota con su valor de frecuencia
#define Re4  294
#define Mi4  2
#define Fa4  349
#define Sol4 392
#define La4  440
#define Lab4 415
#define Do5  253
#define Re5  587
#define Mi5  659
#define Fa5  698
#define Sol5  784
#define Lab5  831
#define La5  880
#define Do6  1046
#define Re6  1175

#define Negra		450			//Define los valores de tiempo de cada nota en ms
#define Corchea 	200
#define Semicorchea 105

void EnableClocks();
void EnablePinsPD();           	//Declaración de funciones utilizadas para activación o habilitación
void SetPinsOutPD();           	// de relojes, perifericos, pines y configuraciones iniciales.

void retardo(float t);           //Declaración de funciones utilizadas para procesos secundarios
void  play(uint32_t note, uint32_t time);

/*Desarrollo de funciones */
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
    GPIOD->PDDR|= (1 << 6);
}

void retardo(float t)                //Función de retardo.
{                                       //Declara variable t como parametro de entrada en
                                        //  donde se indican las repeticiones del retardo
    uint32_t Load = 0x095FU;            //Asigna valor para cargar al temporizador SysTick y obtener 50us
    unsigned int reps;                  //Se declara variable para repetir retardo de 50us.
    for (reps= 0; reps < t ; reps++){   //Estructura de iteración para repetir y facilitar retardos base de 50us en retardo.
        uint32_t i = 0;                 //Declaración de variable utilizada para leer bandera de SysTick
        SysTick->CTRL = 0;              //Apaga bandera y resetea bits reservados de CTRL en SysTick.
        SysTick->LOAD = Load;           //Se le carga la cantidad definida previamente a LOAD para la duración de Systick en retardo base.
        SysTick->CTRL |= 0x04U;         //Se indica tomar señal de reloj de procesador con bit de clksource
        SysTick->CTRL |= 0x01U;         //Habilita Systick como contador con bit de enable en CTRL.
        while(i==0){                    //Estructura de iteración para mantener mientras la bandera esté apagada.
            i = SysTick->CTRL;          //Lee estado de registro CTRL de Systick y almacena en variable i.
            i &= (1<<16);               //Toma el valor del bit de bandera en lectura previa y la asigna como nuevo valor a variable i.
                                        //Se realiza retardo base de 095F'H iteraciones con el contador descendente de Systick
                                        //  hasta lograr tiempo de 50us utilizando la ecuación N = t(SisClk)- 1 = 50us*(48Mhz)- 1
        }
    }
}

void pause(){						//Función de pausa para distinguir entre notas con una pequeña separación.
	SIM->SCGC5|=1;    				//Activa el reloj del LPTMR
	LPTMR0->CMR = 50;				//Asigna duración de retardo en ms.
	LPTMR0->PSR |=  1<<2   			//Realiza bypass a preescaler
				   | 1;  		   	//Selecciona LPO como fuente de reloj.
	LPTMR0->CSR |=   1; 			//Habilita el LPTMR como contador.
	uint32_t i = 0;					//Declara variable para bandera
	while (i!=0x80U){				//Bucle mientras bandera de LPTMR este desactivada
		i = LPTMR0->CSR; 			//guarda estado de registro CTRL en variable i
		i &= (1<<7);				//Enmascara y guarda estado de bandera de LPTMR en variable i.
	}
	LPTMR0->CSR |= 1<<7;	//Limpia bandera
}

void  play(uint32_t note, uint32_t time){				//Función para reproducción de nota con tiempo indicados.
	uint32_t Freq;
	uint32_t Freq2;										//Declaración de variables utilizadas
	uint32_t reps2;
	uint32_t timeInrepsNeed;
	Freq =  ((1.0/note)/0.00005);						//Cálculo de retardos necesarios para lograr la frecuencia de nota indicada.
	Freq2 =  Freq/2;									//Cálculo de retardos necesarios para lograr el doble de frecuencia de nota indicada.
	timeInrepsNeed = (time/1000.0)*note;				//Cálculo de repeticiones necesarias de reproducir la frecuencia de nota
														//	indicada para lograr la duración de tiempo (ms) indicada.
	 for(reps2 = 0; reps2 < timeInrepsNeed ; reps2++){	//reps 4 desire time
	   GPIOD-> PSOR |= POut;							//Activa pin de salida.
	   retardo(Freq2);									//Realiza retardo para la mitad del periodo de frecuencia de la nota.
	   GPIOD-> PCOR |= POut;							//Desactiva pin de salida.
	   retardo(Freq2);									//Realiza retardo para la mitad del periodo de frecuencia de la nota.
	 }
	 pause();											//Realiza retardo de pausa para separar notas (0.05s).
}

														//Proceso principal, función principal
int main(void) {
    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    EnableClocks();                     //Activación de reloj interno de puerto utilizado.
    EnablePinsPD();                     //Habilitación de pines utilizados.
    SetPinsOutPD();                     //Establecimiento de pines de salida.

	while(1){
		play(Re4, Semicorchea);			//Reproduce la nota indicada en el tiempo de nota indicado.
		play(Re4, Semicorchea);			//sintaxis de función: play(nota, tiempo);
		play(Re5, (3*Semicorchea));
		play(La4, (3*Semicorchea));
		play(Lab4, Semicorchea);
		play(Sol4, (Semicorchea));		//Compás 1
		retardo(2100);					//Silencio con duración de nota semicorchea
		play(Fa4, Corchea);
		play(Re4, Semicorchea);
		play(Fa4, Semicorchea);
		play(Sol4, Semicorchea);

		play(Do4, Semicorchea);
		play(Do4, (3*Semicorchea));
		play(Re5, Corchea);
		play(La4, (3*Semicorchea));
		play(Lab4, Semicorchea);
		play(Sol4, (Semicorchea));		//Compás 2
		retardo(2100);					//Silencio con duración de nota semicorchea
		play(Fa4, Corchea);
		play(Re4, Semicorchea);
		play(Fa4, Semicorchea);
		play(Sol4, Semicorchea);


		play(Si3, Semicorchea);
		play(Si3, Semicorchea);
		play(Re5, (3*Semicorchea));
		play(La4, (3*Semicorchea));
		play(Lab4, Semicorchea);
		play(Sol4, (Semicorchea));		//Compás 3
		retardo(2100);					//Silencio con duración de nota semicorchea
		play(Fa4, Corchea);
		play(Re4, Semicorchea);
		play(Fa4, Semicorchea);
		play(Sol4, Semicorchea);

		play(Sib3, Semicorchea);		//Nota, tiempo (s)
		play(Sib3, Semicorchea);
		play(Re5, (3*Semicorchea));
		play(La4, (3*Semicorchea));
		play(Lab4, Semicorchea);
		play(Sol4, (Semicorchea));		//Compás 4
		retardo(2100);					//Silencio con duración de nota semicorchea
		play(Fa4, Corchea);
		play(Re4, Semicorchea);
		play(Fa4, Semicorchea);
		play(Sol4, Semicorchea);

		play(Fa5, Corchea);
		play(Fa5, Semicorchea);
		play(Fa5, Semicorchea);
		play(Fa5, (3*Semicorchea));
		play(Fa5, Corchea);				//Compas 5
		play(Re5, Corchea);
		play(Re5, (5*Semicorchea));

		play(Fa5, Corchea);				//compás 6
		play(Fa5, Semicorchea);
		play(Fa5, Semicorchea);
		retardo(2100);					//Silencio con duración de nota semicorchea
		play(Sol5, (3*Semicorchea));
		play(Lab5, Corchea);
		play(Sol5, (Semicorchea/2));
		play(Lab5, (Semicorchea/2));
		play(Sol5, (Semicorchea/2));
		play(Fa5, Semicorchea);
		play(Re5, Semicorchea);
		play(Fa5, Semicorchea);
		play(Sol5, Semicorchea);

		play(Fa5, Corchea);				//Compás 7
		play(Fa5, Semicorchea);
		play(Fa5, Semicorchea);
		retardo(2100);					//Silencio con duración de nota semicorchea
		play(Sol5, (3*Semicorchea));
		play(Lab5, Semicorchea);
		retardo(2100);					//Silencio con duración de nota semicorchea
		play(La5, (3*Semicorchea));
		play(Do6, Corchea);
		play(La5, (3*Semicorchea));

		play(Re6, (3*Semicorchea));
		play(Re6, (3*Semicorchea));
		play(Re6, Semicorchea);			//Comás 8
		play(La5, Semicorchea);
		play(Re6, Semicorchea);
		play(Do6, (Negra + Semicorchea));

		retardo((2100*4));				//Silencio con duración de nota negra
	}

    return 0 ;
}
