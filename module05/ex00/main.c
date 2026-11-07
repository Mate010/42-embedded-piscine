/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrossi <mgrossi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 11:07:02 by mgrossi           #+#    #+#             */
/*   Updated: 2026/04/30 16:46:05 by mgrossi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <avr/io.h>
# include <util/delay.h>
# include "uart.h"

/*
https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf

*/

// takes 4 bits and converts them to a char 
char bits_to_hex_char(uint8_t data)
{
	data &= 0x0F; // apply a mask that hides the left bits and keeps only the right ones 0000
	// with the mask, data can go up to 15 max, like hexadecimal
	if (data < 10)
		return ('0' + data);
	else
		return ('A' + (data - 10));
}

uint8_t read_potentiometer()
{
	
 /*
	ADCSRA : //
		ADSC : starts a new voltage measurement
	ADCH : holds the 8 bits of the result
 */

	ADCSRA |= (1 << ADSC); // first (dummy) conversion 

	while (ADCSRA & (1 << ADSC)) // check that ADCSRA is still 1, because when ADCSRA finishes it automatically goes back to 0
	{
	}
	return (ADCH);
}

void init_potentiometer()
{
	/*
	AVCC : Analog Voltage Common Collector
		p : 257
		ADMUX :ADC Multiplexer Selection Register
			REFS0 : selects the reference voltage (here 5V)
			ADLAR : left-adjusts the result to read the 8-bit value
		
		p : 258
		ADCSRA : ADC Control and Status Register A
			ADEN : enables the ADC module
			ADPS2 : Prescaler at 128
			ADPS1 : Prescaler at 128
			ADPS0 : Prescaler at 128
		
			we need a prescaler of 128 to be between 50 and 200kHz, so it can take a reliable measurement
		
	*/
	ADMUX = (1 << REFS0) | (1 << ADLAR);
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

int main(void)
{
	uint8_t val_potentiometer;
	uart_init();
	init_potentiometer();
	
	while(1)
	{
		val_potentiometer = read_potentiometer();
		// read the bit value in hex  
		// >> 4 reads the left bits 
		// 0x0F reads the right bits with a mask
		char str[5] = {bits_to_hex_char(val_potentiometer >> 4), bits_to_hex_char(val_potentiometer & 0x0F), '\r', '\n', '\0'};
		uart_print_str(str);
		_delay_ms(20);
	}
}