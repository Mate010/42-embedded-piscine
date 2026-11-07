/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrossi <mgrossi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 11:07:02 by mgrossi           #+#    #+#             */
/*   Updated: 2026/04/30 16:57:17 by mgrossi          ###   ########.fr       */
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


uint8_t hex_to_value(char c)
{
	if (c >= '0' && c <= '9') // converts the hexadecimal digit to a value (0 to 9)
		return (c - '0');
	else if (c >= 'a' && c <= 'f') // converts the hexadecimal digit to a value (10 to 15)
		return (c - 'a' + 10);
	else if (c >= 'A' && c <= 'F')// converts the hexadecimal digit to a value (10 to 15)
		return (c - 'A' + 10);
	return (0);
}



uint8_t read_potentiometer()
{
	
	ADMUX = (1 << REFS0) | (1 << ADLAR);
 /*
	ADCSRA : //
		ADSC : starts a new voltage measurement
	ADCH : holds the 8 bits of the result
 */

	ADCSRA |= (1 << ADSC);

	while (ADCSRA & (1 << ADSC)) // check that ADCSRA is still 1, because when ADCSRA finishes it automatically goes back to 0
	{
	}
	return (ADCH);
}

uint8_t read_temp()
{
	/*
	p : 257
		ADMUX : ADC Multiplexer Selection Register
			REFS0 : gives 5V to the LDR
			ADLAR : left-adjusts the ADC result
			MUX0 : enables the LDR (temperature sensor)
	*/
	
	
	ADMUX = (1 << REFS0) | (1 << ADLAR) | (1 << MUX0);
    
	
	ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC))
    {
    }
    return (ADCH);
}

uint8_t read_lumiere()
{
	/*
	p : 257
		ADMUX : ADC Multiplexer Selection Register
			REFS0 : gives 5V to the LDR
			ADLAR : left-adjusts the ADC result
			MUX1 : enables the LDR (light sensor)
	*/
	
	
	ADMUX = (1 << REFS0) | (1 << ADLAR) | (1 << MUX1);
    
	
	ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC))
    {
    }
    return (ADCH);
}


void init_adc()
{
	/*
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
	uint8_t val_capt_lumiere;
	uint8_t val_capt_temp;
	char next[3] = {'\r', '\n', '\0'};
	uart_init();
	init_adc();
	
	while(1)
	{
		val_potentiometer = read_potentiometer();
		// read the bit value in hex  
		// >> 4 reads the left bits 
		// 0x0F reads the right bits with a mask
		char str_potentiometer[5] = {bits_to_hex_char(val_potentiometer >> 4), bits_to_hex_char(val_potentiometer & 0x0F), ',', ' ', '\0'};
		uart_print_str(str_potentiometer);

		val_capt_temp = read_temp();
		char str_temp[5] = {bits_to_hex_char(val_capt_temp >> 4), bits_to_hex_char(val_capt_temp & 0x0F), ',', ' ', '\0'};
		uart_print_str(str_temp);
		
		val_capt_lumiere = read_lumiere();
		char str_lumiere[5] = {bits_to_hex_char(val_capt_lumiere >> 4), bits_to_hex_char(val_capt_lumiere & 0x0F), ',', ' ', '\0'};
		uart_print_str(str_lumiere);
		uart_print_str(next);
		
		_delay_ms(20);
	}
}