/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrossi <mgrossi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 11:07:02 by mgrossi           #+#    #+#             */
/*   Updated: 2026/04/13 18:58:53 by mgrossi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>

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
(PINx only lets you read the state of the pin)


source : 	page 84 : 14. I/O-Ports // about AVRs and their dedicated registers
			page 88 : C Code Example // 
			page 100, 14.4 Register Description // describes the AVR microcontroller configuration
*/
int	main(void)
{
	DDRB |= (1 << PB0);
	PORTB |= (1 << PB0);
	while (1)
	{
	}
}
