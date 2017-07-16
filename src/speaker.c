/*
 *  Filename: speaker.c
 *  Created and last modified on: September 29, 2015 for EECS 388 Lab 3
 *  Reused without change on: December 8, 2015, for EECS 388 Final Project
 *  Author: Arjan Gupta
 */

#include "speaker.h"
#include "system.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

unsigned long flag; //redefining the buttons flag to be used here
static unsigned long target = 0; //this is the target time for SpeakerExec
//static const unsigned long period = SYSTICK_FREQUENCY / (2 * 440); (Don't need this here)
unsigned long freqTone = SYSTICK_FREQUENCY / (2 * 440); //initial tone (doesn't matter)

void SpeakerTask(void *pvParameters) {

	// Enable GPIO Port H and configure it to drive the speaker buzzer
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);

	// The Speaker is attached to H<1> (Port H pin 1) and H<0> (Port H pin 0) som they must be set as output
	GPIOPinTypeGPIOOutput(GPIO_PORTH_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	//For pull-up resistors
	GPIOPadConfigSet( GPIO_PORTH_BASE,
	GPIO_PIN_1 | GPIO_PIN_0,
	GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

	//Set H<1> to LOW and H<0> to HIGH
	GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0x01);

	while (1) {
		/*
		 *  Toggle the LED.
		 */
		if (flag == 1) {
			GPIOPinWrite(GPIO_PORTH_BASE,
			GPIO_PIN_0 | GPIO_PIN_1,
					GPIOPinRead(GPIO_PORTH_BASE, GPIO_PIN_0 | GPIO_PIN_1)
							^ 0x03);
		}

		//	Advance next execution time for the LED task
		vTaskDelay(freqTone);
	}

}

