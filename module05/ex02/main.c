/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrossi <mgrossi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 11:07:02 by mgrossi           #+#    #+#             */
/*   Updated: 2026/04/30 17:00:14 by mgrossi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <avr/io.h>
# include <util/delay.h>
# include "uart.h"

/*
https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf

*/

// takes 4 bits and converts them to a char 
// char bits_to_hex_char(uint8_t data)
// {
// 	data &= 0x0F; // apply a mask that hides the left bits and keeps only the right ones 0000
// 	// with the mask, data can go up to 15 max, like hexadecimal
// 	if (data < 10)
// 		return ('0' + data);
// 	else
// 		return ('A' + (data - 10));
// }


void ft_putnbr(uint16_t n)
{
    if (n >= 10)
    {
		ft_putnbr(n / 10);
    }
    uart_tx((n % 10) + '0');
}


uint16_t read_potentiometer()
{
	
	ADMUX = (1 << REFS0);
 /*
	ADCSRA : //
		ADSC : starts a new voltage measurement
	ADC : holds the 16 bits of the result
 */

	ADCSRA |= (1 << ADSC);

	while (ADCSRA & (1 << ADSC)) // check that ADCSRA is still 1, because when ADCSRA finishes it automatically goes back to 0
	{
	}
	return (ADC);
}

uint16_t read_temp()
{
	/*
	p : 257
		ADMUX : ADC Multiplexer Selection Register
			REFS0 : gives 5V to the LDR
			ADLAR : left-adjusts the ADC result (removed here so it does not break the result)
			MUX0 : enables the LDR (temperature sensor)
	*/
	ADMUX = (1 << REFS0) | (1 << MUX0);
    
	
	ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC))
    {
    }
    return (ADC);
}

uint16_t read_lumiere()
{
	/*
	p : 257
		ADMUX : ADC Multiplexer Selection Register
			REFS0 : gives 5V to the LDR
			ADLAR : left-adjusts the ADC result
			MUX1 : enables the LDR (light sensor)
	*/
	
	
	ADMUX = (1 << REFS0) | (1 << MUX1);
    
	
	ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC))
    {
    }
    return (ADC);
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
	ADMUX = 0000;
	ADMUX = (1 << REFS0);
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

int main(void)
{
	/*
	P : 256 
	We switch from 8 to 16 bits because of ADLAR mode (set to 0), which lets us read the 10 available bits; thanks to that we can print it in decimal with putnbr 
	*/
	uint16_t val_potentiometer;
	uint16_t val_capt_lumiere;
	uint16_t val_capt_temp;
	uart_init();
	init_adc();
	
	while(1)
	{
		val_potentiometer = read_potentiometer();
		ft_putnbr(val_potentiometer);
		uart_tx(' ');
		
		val_capt_temp = read_temp();
		ft_putnbr(val_capt_temp);
		uart_tx(' ');
		
		val_capt_lumiere = read_lumiere();
		ft_putnbr(val_capt_lumiere);
		uart_tx(' ');
		
		uart_tx('\r');
		uart_tx('\n');
		uart_tx(' ');
		_delay_ms(20);
	}
}