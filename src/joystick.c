/*
 *  Filename: joystick.c
 *  Created on: October 28, 2015 for Project 1, EECS 388
 *  Changes made on: December 8, 2015 for Project 2, EECS 388
 *  Author: Arjan Gupta
 */
#include "joystick.h" //header file
#include "speaker.h" //speaker header
#include "system.h" //base library
#include "rit128x96x4.h" //OLED driver
//Standard libraries
#include "stdlib.h"
#include "stdio.h"
// Stellaris driver libraries
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
//Declarations
unsigned long left; //left, right, up, down get input from joystick
unsigned long right;
unsigned long up;
unsigned long down;
unsigned long displace = 5; // Set some displacement for the star
unsigned long x = 60; //x is the x coordinate of the cursor
unsigned long y = 43; // y is the y coordinate of the cursor
unsigned long rtrue = 1;
unsigned long ltrue = 0;
unsigned long utrue = 0;
unsigned long dtrue = 0;
unsigned long game = 0; //checks if game over
unsigned long food_x = 0; //food x pos
unsigned long food_y = 0; //food y pos
unsigned long buttonDelay = 1 * TICK_R;
unsigned long timer = 0;
static unsigned long speakerDelay = 200 * TICK_R;

void JoystickTask(void* pvParameters){
	//Enable GPIO port D
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	//Configure the GPIO inputs with weak pull up and strength 2 mA
	//Initialize Pin 0
	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_0);
	GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	//Initialize Pin 1
	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_1);
	GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	//Initialize Pin 2
	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_2);
	GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	//Initialize Pin 3
	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_3);
	GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	//generate a position for the food
	food_x = 23;
	food_y = 62;
	//clear the screen
	RIT128x96x4Clear();
	//display the rules
	RIT128x96x4StringDraw("RULES:", 22, 15, 15);
	RIT128x96x4StringDraw("You must catch", 10, 25, 15);
	RIT128x96x4StringDraw("the food in 6 sec!", 10, 35, 15);
	RIT128x96x4StringDraw("Move your joystick", 10, 55, 15);
	RIT128x96x4StringDraw("to START!", 22, 65, 15);
	//wait for the user to move the joystick
	while(GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_0)&&
		  GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_1)&&
		  GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_2)&&
		  GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_3));
	//start the game
	while (1) {
		if(timer > 13000){ //end game if timed out (timer reaches 6.5 seconds)
			game=1;
		}
		flag=0;
		buttonDelay = 1 * TICK_R;
		if(game == 0){
			//	Read data from the GPIO inputs
			left = GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_0);
			up = GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_1);
			right = GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_2);
			down = GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_3);
			//	If any switch button is pushed, the value read from the corresponding GPIO<D> pins goes to 0
			if (left == 0) { //	If left is pushed, choose to reduce the x-coordinate
				ltrue= 1; rtrue = 0; utrue =0; dtrue =0;
			}
			if (right == 0) { // If right is pushed, choose to increase the x-coordinate
				ltrue= 0; rtrue = 1; utrue =0; dtrue =0;
			}
			if (up == 0) { // If up is pushed, choose to decrease the y-coordinate
				ltrue= 0; rtrue = 0; utrue =1; dtrue =0;
			}
			if (down == 0) { // If down is pushed, choose to increase the y-coordinate
				ltrue= 0; rtrue = 0; utrue =0; dtrue =1;
			}
			//keep the 'o' moving in the direction the user chose
			if(utrue){
				y -= 3;
				if(y < 4){y = 86;}//if bottom of the screen is reached, send the 'o' to the bottom
			}else if(dtrue){
				y +=3;
				if(y > 86){y = 4;}//if bottom of screen is reached, go back to the top
			}else if(rtrue){
				x += 3;
				if(x > 120){x = 2;}//go back to the left side if the right end is reached
			}else if(ltrue){
				x -= 3;
				if(x < 2){x = 120;}//go back to the right side if the left end is reached
			}
			RIT128x96x4Clear(); //	Clear the OLED screen
			RIT128x96x4StringDraw("o", x, y, 15); //print the o (the cursor)
			RIT128x96x4StringDraw("*", food_x, food_y, 15); //print the food
			//determine if food is caught
			if((x >= food_x-3 && x <= food_x+3) && (y >= food_y-3 && y <= food_y+3)){
				//if food is caught, decide new spot for the food to respawn
				food_x = (rand() % 70) + 20;
				food_y = (rand() % 50) + 20;
				timer = 0; //set timer stopwatch back to 0
				flag = 1; //set the speaker flag to 1 so a tone can be played
				buttonDelay = speakerDelay; //set how long you want the tone to be heard
				freqTone = toneLEFT; //set the nature of the tone you would like to hear
			}
		}else{
			//if timer has timed out, then clear screen and say game over
			RIT128x96x4Clear();
			RIT128x96x4StringDraw("TIMED OUT!", 30, 45, 15);
			RIT128x96x4StringDraw("GAME OVER", 30, 55, 15);
			flag = 1; //set the speaker flag to 1 so a tone can be played
			buttonDelay = speakerDelay; //set how long you want the tone to be heard
			freqTone = toneRIGHT; //set the nature of the tone you would like to hear
		}
		if(buttonDelay != speakerDelay){
			//if food has not been caught but timer has not reached 6.5 seconds yet, increment timer
			timer += buttonDelay;
		}
		//advance loop with delay time
		vTaskDelay(buttonDelay);
	}
}
