/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrossi <mgrossi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 11:07:02 by mgrossi           #+#    #+#             */
/*   Updated: 2026/04/17 18:01:09 by mgrossi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
/*
	https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf
		page 132 : 16.9.3 Fast PWM Mode
		page 140 : Compare Output Mode, non-PWM / COM1A0/COM1B0 
		To see how to set TCCR in compare mode:
		page 141 : Table 16-3. Compare Output Mode, Phase Correct and Phase and Frequency Correct PWM
		page 143 : Table 16-5. Clock Select Bit Description
*/
int main(void)
{
    DDRB |= (1 << PB1); // PB1 as output 

	/* 
	ICR1 : sets the "TOP" value of Timer 1 in Fast PWM mode
	TOP = 62499 (frequency 1Hz)
    F_CPU / Prescaler / Frequency - 1
    16.000.000 / 256 / 1 - 1 = 62499 
	gives the max value in PWM mode (used with WGM13 and WGM12)
	*/
    ICR1 = 62499;
    
	
	/*
		OCR1A : 
		linked to CTC (WGM12)
		switches the LED from 1 to 0, but the counter goes up to ICR1;
	*/
	OCR1A = ICR1 / 10;

	/*
	TCCR1A : Timer/Counter1 Control Register 1A (controls the output behavior)
		COM1A1 : Compare Output Mode (COM) / 1 refers to Timer1 / A refers to channel A of Timer1 / 1 means it is bit 1 (so it will start on)	
		WGM11 : used for PWM
		WGM12 : used for PWM
		WGM13 : used for PWM
	*/
    TCCR1A = (1 << COM1A1) | (1 << WGM11); // TCCR1A = 0b10000010;
	/*
		CS12 : setting CS12 to 1 enables a prescaler of 256 (this means the Timer1 clock is divided by 256) 

	*/
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS12); //TCCR1B = 0b00011100;

    while (1)
	{
	}
}
