//Código Realizado por: lrkRocky
//Este código imprime estrofas de canciones utilizando tareas y semaforos en Free RTOS

/* FreeRTOS kernel includes. */		//	Practica RTOS parte 3
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"					//Libreria agregada para semaforos mutex

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Task priorities. */
#define hello_task_PRIORITY (configMAX_PRIORITIES - 1)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void hello_task(void *pvParameters);
static void task_b(void *pvParameters);
static void task_c(void *pvParameters);
static void task_d(void *pvParameters);

SemaphoreHandle_t xSemaphore_chi;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Application entry point.
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();						//
    BOARD_InitDebugConsole();
    xSemaphore_chi;
												//Crea tarea con caracteristicas indicadas.
    xTaskCreate(hello_task,						//Nombre de función que se debe cumplir (proceso).
    		"Hello_task A", 						//Función que se debe cumplir (proceso).
    		configMINIMAL_STACK_SIZE + 150, 		//Min 90, no. de bytes/stack
			NULL, 								//
			4,				//prioridad de tareas, max 5 (5 reservado, menor numero mayor prioridad)
			NULL);								//
    xTaskCreate(task_b,						//Nombre de función que se debe cumplir (proceso).
    		"Task_B", 						//Función que se debe cumplir (proceso).
    		configMINIMAL_STACK_SIZE + 150, 		//Min 90, no. de bytes/stack
			NULL, 								//
			3,				//prioridad de tareas, max 5 (5 reservado, menor numero mayor prioridad)
			NULL);								//
    xTaskCreate(task_c,						//Nombre de función que se debe cumplir (proceso).
       		"Task_C", 						//Función que se debe cumplir (proceso).
       		configMINIMAL_STACK_SIZE + 150, 		//Min 90, no. de bytes/stack
   			NULL, 								//
   			2,				//prioridad de tareas, max 5 (5 reservado, menor numero mayor prioridad)
   			NULL);
    xTaskCreate(task_d,						//Nombre de función que se debe cumplir (proceso).
			"Task_d", 						//Función que se debe cumplir (proceso).
			configMINIMAL_STACK_SIZE + 150, 		//Min 90, no. de bytes/stack
			NULL, 								//
			1,				//prioridad de tareas, max 5 (5 reservado, menor numero mayor prioridad)
			NULL);

    vTaskStartScheduler();						//Sistema operativo RTOS toma el control del sistema para organizar tiempos y tareas.
    for (;;)
        ;
}


/Nombre de función que se debe cumplir (proceso).
    		"Hello_task A", 						//Función que se debe cumplir (proceso).
    		configMINIMAL_STACK_SIZE + 150, 		//Min 90, no. de bytes/stack
			NULL, 								//
			4,				//prioridad de tareas, max 5 (5 reservado, menor numero mayor prioridad)
			NULL);								//
/*!
 * @brief Task responsible for printing of "Hello world." message.
 */

static void hello_task(void *pvParameters)		//Proceso de tarea
{
	xSemaphore_chi = xSemaphoreCreateMutex();

    for (;;)
    {

    	PRINTF("Que te perdone Dios, yo no lo voy a hacer \r\n");
	   vTaskDelay(1000);

    	xSemaphoreTake(xSemaphore_chi, portMAX_DELAY);
    	PRINTF("Asereje-ja-deje \r\n");
	   vTaskDelay(1000);
	   PRINTF("Ella se dejo y yo me la lleve \r\n");
	   vTaskDelay(1000);
		xSemaphoreGive(xSemaphore_chi); //portMAX_DELAY

		vTaskSuspend(NULL);						//Suspende tarea
    }
}

static void task_b(void *pvParameters)		//Proceso de tarea
{
    for (;;)
    {

        PRINTF("Los perdi a los dos y a la misma vez \r\n");
	   vTaskDelay(1000);						//Función bloqueante, 1000 = 1s(no exacto)
	   PRINTF("Hacemo' el boogie \r\n");
	   vTaskDelay(1000);
	   PRINTF("Chatita del alma de hablar sin mentiras \r\n");
	   vTaskDelay(1000);
	   vTaskSuspend(NULL);						//Suspende tarea
    }
}

static void task_c(void *pvParameters)		//Proceso de tarea
{
    for (;;)
    {
    	PRINTF("Ya veo que todo era mentira cuando ella me decia \r\n");
		vTaskDelay(1000);						//Función bloqueante, 1000 = 1s(no exacto)
		PRINTF("Hacemo' el wigiridi, ah  \r\n");
		vTaskDelay(1000);
		PRINTF("Espere mucho tiempo pa' ver si cambiabas \r\n");
		vTaskDelay(1000);
        vTaskSuspend(NULL);						//Suspende tarea
    }
}

static void task_d(void *pvParameters)		//Proceso de tarea
{
    for (;;)
    {
    	PRINTF("Que se iva pa' Puerto Rico a vacaciones con su amiga \r\n");
		vTaskDelay(1000);						//Función bloqueante, 1000 = 1s(no exacto)
		PRINTF("Ha llegado el momento \r\n");
		vTaskDelay(1000);
		PRINTF("Y tu que ni me miras \r\n");
		vTaskDelay(1000);
        vTaskSuspend(NULL);						//Suspende tarea
    }
}

