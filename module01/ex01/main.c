/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrossi <mgrossi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 11:07:02 by mgrossi           #+#    #+#             */
/*   Updated: 2026/04/18 21:28:30 by mgrossi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
/*
	https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf
		page 140 : Compare Output Mode, non-PWM / COM1A0/COM1B0 
		To see how to set TCCR in compare mode:
		page 141 : Table 16-3. Compare Output Mode, Phase Correct and Phase and Frequency Correct PWM
		page 143 : Table 16-5. Clock Select Bit Description
*/



int	main(void)
{
	DDRB |= (1 << PB1);
	// set the LED connected to the pin (OC1A) -> 1 refers to Timer1 / A refers to channel A of Timer1

	// linked to CTC (WGM12)
	OCR1A = 31249;
	// Toggle for 0.5s, LED blink 1Hz
    // (16MHz (16 000)/ 256 * 0.5) - 1 // Microcontroller clock frequency / Prescaler * desired period 0.5s - we start at 0, not 1
	// 31249 is the number of timer clock cycles.

	/*
		TCCR1A : Timer/Counter1 Control Register 1A (controls the output behavior)
		COM1A0 : Compare Output Mode (COM) / 1 refers to Timer1 / A refers to channel A of Timer1 / 0 means it is bit 0
		with this configuration it is in Toggle mode, it flips 0 to 1. 
	*/
	TCCR1A = (1 << COM1A0); 
	
	/*
		TCCR1B : Timer/Counter Control Register 1 B (controls the clock source)
		WGM12 : setting WGM12 to 1 enables CTC (Clear Timer on Compare Match)
			this resets the counter to 0 when it reaches the value stored in OCR1A (31249)
		CS12 : setting CS12 to 1 enables a prescaler of 256 (this means the Timer1 clock is divided by 256) 
	*/
	TCCR1B = (1 << WGM12) | (1 << CS12);
	while (1)
	{
	}
}
