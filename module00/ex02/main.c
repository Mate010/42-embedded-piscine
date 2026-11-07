/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrossi <mgrossi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 11:07:02 by mgrossi           #+#    #+#             */
/*   Updated: 2026/04/14 11:52:16 by mgrossi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

/*
	https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf
*/
/*
DDRx (direction register)
	DDRB |= (1 << PB0); (output: the pin sends a signal)
		Turn an LED on / off
	DDRB &= ~(1 << PB1); (input: the pin receives a signal)
		Read whether it is pressed or not
PORTB |= (1 << PB0);
	turns on pin 0 of port B (which turns on the light)

PIND & (1 << PD2)
	only lets you read the state of the pin

source : 	page 84 : 14. I/O-Ports // about AVRs and their dedicated registers
			page 88 : C Code Example // 
			page 100, 14.4 Register Description // describes the AVR microcontroller configuration
*/
int	main(void)
{
	DDRB |= (1 << PB0); // The LED sends signals (output)
	DDRD &= ~(1 << PD2); // The button receives signals (input)
	while (1)
	{
        if (!(PIND & (1 << PD2))) // Only check the state of button PD2, whether it is active or not
        {
            PORTB |= (1 << PB0);   // Turn the LED on
        }
        else
        {
            PORTB &= ~(1 << PB0);  // Turn the LED off
        }
		_delay_ms(10); // Wait 10 ms to avoid bouncing.
	}
}
