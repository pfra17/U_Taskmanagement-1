/*
 * U_Taskmanagement.c
 *
 * Created: 20.03.2018 18:32:07
 * Author : chaos
 */ 

//#include <avr/io.h>
#include "avr_compiler.h"
#include "pmic_driver.h"
#include "TC_driver.h"
#include "clksys_driver.h"
#include "sleepConfig.h"
#include "port_driver.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
#include "stack_macros.h"

#include "mem_check.h"

#include "init.h"
#include "utils.h"
#include "errorHandler.h"
#include "NHD0420Driver.h"
#include "ButtonHandler.h"




extern void vApplicationIdleHook( void );
void vButtonManager(void *pvParameter);
void vDisplayManager(void *pvParameter);
void vLed1Handler(void *pvParameter);
void vLed2Handler(void *pvParameter);
void vLed3Handler(void *pvParameter);
void vLed4Handler(void *pvParameter);
void vLed5Handler(void *pvParameter);
void vLed6Handler(void *pvParameter);
void vLed7Handler(void *pvParameter);
void vLed8Handler(void *pvParameter);
void vLoadKiller(void *pvParameter);


TaskHandle_t led1Task;
TaskHandle_t led2Task;
TaskHandle_t led3Task;
TaskHandle_t led4Task;
TaskHandle_t led5Task;
TaskHandle_t led6Task;
TaskHandle_t led7Task;
TaskHandle_t led8Task;
TaskHandle_t loadkillerTask;

double loadcounter = 0;

void vApplicationIdleHook( void )
{	
	portENTER_CRITICAL();
	loadcounter++;
	portEXIT_CRITICAL();
}

int main(void)
{
    resetReason_t reason = getResetReason();

	vInitClock();	
	vInitDisplay();
	xTaskCreate(vButtonManager, (const char *) "btManager", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
	xTaskCreate( vLed1Handler, (const char *) "led1", configMINIMAL_STACK_SIZE, NULL, 1, &led1Task);
	xTaskCreate( vLed2Handler, (const char *) "led2", configMINIMAL_STACK_SIZE, NULL, 1, &led2Task);
	xTaskCreate( vLed3Handler, (const char *) "led3", configMINIMAL_STACK_SIZE, NULL, 1, &led3Task);
	xTaskCreate( vLed4Handler, (const char *) "led4", configMINIMAL_STACK_SIZE, NULL, 1, &led4Task);
	xTaskCreate( vLed5Handler, (const char *) "led5", configMINIMAL_STACK_SIZE, NULL, 1, &led5Task);
	xTaskCreate( vLed6Handler, (const char *) "led6", configMINIMAL_STACK_SIZE, NULL, 1, &led6Task);
	xTaskCreate( vLed7Handler, (const char *) "led7", configMINIMAL_STACK_SIZE, NULL, 1, &led7Task);
	xTaskCreate( vLed8Handler, (const char *) "led8", configMINIMAL_STACK_SIZE, NULL, 1, &led8Task);
	xTaskCreate( vLoadKiller, (const char *) "loadkill", configMINIMAL_STACK_SIZE, NULL, 2, &loadkillerTask);
	xTaskCreate( vDisplayManager, (const char *) "dispmng", configMINIMAL_STACK_SIZE, NULL, 3, NULL);

	vTaskSuspend(loadkillerTask);
	
	vDisplayClear();
	vDisplayWriteStringAtPos(0,0,"FreeRTOS 10.0.1");
 	vDisplayWriteStringAtPos(1,0,"EDUBoard 1.0");
 	vDisplayWriteStringAtPos(2,0,"TaskManagement");
 	vDisplayWriteStringAtPos(3,0,"ResetReason: %d", reason);
	vTaskStartScheduler();
	return 0;
}

void vButtonManager(void *pvParameter){
	initButtons();
	for(;;) {
		updateButtons();
		if(getButtonPress(BUTTON1) == SHORT_PRESSED) {
			vTaskSuspend(led1Task);
			vTaskSuspend(led2Task);
		} else if(getButtonPress(BUTTON1) == LONG_PRESSED) {
			vTaskResume(led1Task);
			vTaskResume(led2Task);
		}
		if(getButtonPress(BUTTON2) == SHORT_PRESSED) {
			vTaskSuspend(led3Task);
			vTaskSuspend(led4Task);
			vTaskSuspend(led5Task);
			vTaskSuspend(led6Task);
			vTaskSuspend(led7Task);
			vTaskSuspend(led8Task);
		} else if(getButtonPress(BUTTON2) == LONG_PRESSED) {
			vTaskResume(led3Task);
			vTaskResume(led4Task);
			vTaskResume(led5Task);
			vTaskResume(led6Task);
			vTaskResume(led7Task);
			vTaskResume(led8Task);
		}
		if(getButtonPress(BUTTON3) == SHORT_PRESSED) {

		} else if(getButtonPress(BUTTON3) == LONG_PRESSED) {

		}
		if(getButtonPress(BUTTON4) == SHORT_PRESSED) {
			vTaskResume(loadkillerTask);
		} else if(getButtonPress(BUTTON4) == LONG_PRESSED) {
			vTaskSuspend(loadkillerTask);
		}

		vTaskDelay(10/portTICK_RATE_MS);
	}
}

void vDisplayManager(void *pvParameter) {
	vTaskDelay(100/portTICK_RATE_MS);
	for(;;) {
		vDisplayClear();
		vDisplayWriteStringAtPos(0,0,"FreeRTOS 10.0.1");
 		vDisplayWriteStringAtPos(1,0,"EDUBoard 1.0");
 		vDisplayWriteStringAtPos(2,0,"TaskManagement");
 		vDisplayWriteStringAtPos(3,0,"Load: %f", loadcounter);
		loadcounter = 0;
		vTaskDelay(1000/portTICK_RATE_MS);
	}
}

void vLoadKiller(void *pvParameter) {	
	for(;;) {
		
	}
}

void vLed1Handler(void *pvParameter) {
	PORTF.DIRSET = PIN0_bm;
	PORTF.OUTSET = 0x01;
	for(;;) {
		PORTF.OUTTGL = 0x01;
		vTaskDelay(200/portTICK_RATE_MS);
	}
}

void vLed2Handler(void *pvParameter) {
	PORTF.DIRSET = PIN1_bm;
	PORTF.OUTSET = 0x02;
	for(;;) {
		PORTF.OUTTGL = 0x02;
		vTaskDelay(190/portTICK_RATE_MS);
	}
}

void vLed3Handler(void *pvParameter) {
	PORTF.DIRSET = PIN2_bm;
	PORTF.OUTSET = 0x04;
	for(;;) {
		PORTF.OUTTGL = 0x04;
		vTaskDelay(180/portTICK_RATE_MS);
	}
}

void vLed4Handler(void *pvParameter) {
	PORTF.DIRSET = PIN3_bm;
	PORTF.OUTSET = 0x08;
	for(;;) {
		PORTF.OUTTGL = 0x08;
		vTaskDelay(170/portTICK_RATE_MS);
	}
}

void vLed5Handler(void *pvParameter) {
	PORTE.DIRSET = PIN0_bm;
	PORTE.OUTSET = 0x01;
	for(;;) {
		PORTE.OUTTGL = 0x01;
		vTaskDelay(160/portTICK_RATE_MS);
	}
}

void vLed6Handler(void *pvParameter) {
	PORTE.DIRSET = PIN1_bm;
	PORTE.OUTSET = 0x02;
	for(;;) {
		PORTE.OUTTGL = 0x02;
		vTaskDelay(150/portTICK_RATE_MS);
	}
}

void vLed7Handler(void *pvParameter) {
	PORTE.DIRSET = PIN2_bm;
	PORTE.OUTSET = 0x04;
	for(;;) {
		PORTE.OUTTGL = 0x04;
		vTaskDelay(140/portTICK_RATE_MS);
	}
}

void vLed8Handler(void *pvParameter) {
	PORTE.DIRSET = PIN3_bm;
	PORTE.OUTSET = 0x08;
	for(;;) {
		PORTE.OUTTGL = 0x08;
		vTaskDelay(130/portTICK_RATE_MS);
	}
}