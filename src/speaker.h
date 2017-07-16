/*
 * speaker.h
 *
 *  Created on: Sep 29, 2015
 *      Author: agupta87
 */

#ifndef SRC_SPEAKER_H_
#define SRC_SPEAKER_H_

	//2 global variables
	//flag just checks if a button is pressed or not
	//freqTone is used to change the tone of the button
	extern unsigned long flag;
	extern unsigned long freqTone;

	//different sounds for different buttons, therefore different frequencies
	#define toneSELECT SYSTICK_FREQUENCY / (2 * 415)
	#define toneUP SYSTICK_FREQUENCY / (2 * 700)
	#define toneDOWN SYSTICK_FREQUENCY / (2 * 100)
	#define toneLEFT SYSTICK_FREQUENCY / (2 * 250)
	#define toneRIGHT SYSTICK_FREQUENCY / (2 * 1000)

	//Task method
	extern void SpeakerTask(void*);

#endif /* SRC_SPEAKER_H_ */
