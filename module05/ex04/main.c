/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrossi <mgrossi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 11:07:02 by mgrossi           #+#    #+#             */
/*   Updated: 2026/04/30 17:16:44 by mgrossi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <avr/io.h>
# include <util/delay.h>
# include "uart.h"

/*
https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf

*/

void init_rgb()
{
    // PD3, PD5, PD6 as outputs: red, blue, green LED
    DDRD |= (1 << PD3) | (1 << PD5) | (1 << PD6);
   
	
	// LEDs as outputs
	DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);
	
	
	
	
	// P : 113 / 115 
    // Timer 0 : Fast PWM,
	// COM0A1 : clears the pin to 0 when timer0 matches OCR0A 
	// COM0B1 : clears the pin to 0 when timer0 matches OCR0B
	// WGM01 : enables fast PWM
	// WGM00 : enables fast PWM
	// P : 117
	// CS21 : prescaler 
    TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);
    TCCR0B = (1 << CS01); // Frequency divided by 8
    

	// Timer 2 : Fast PWM, 
	// WGM21 : enables fast PWM
	// WGM20 : enables fast PWM
	// COM2B1 : clears the pin to 0 when timer2 matches OCR2B
	// P : 117
	// CS21 : prescaler 	
    TCCR2A = (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
    TCCR2B = (1 << CS21); // Frequency divided by 8
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
	if (r == 0)
	{
		TCCR0A &= ~(1 << COM0B1); // disconnect the LED  
		PORTD &= ~(1 << PD5); // Force the pin to 0
	}
	else
	{	
		TCCR0A |= (1 << COM0B1);
		OCR0B = r;
	}

	if (g == 0)
	{
		TCCR0A &= ~(1 << COM0A1);
		PORTD &= ~(1 << PD6);
	}
	else
	{
		TCCR0A |= (1 << COM0A1);
		OCR0A = g;
	}

	if (b == 0)
	{
		TCCR2A &= ~(1 << COM2B1);
		PORTD &= ~(1 << PD3);
	}
	else
	{
		TCCR2A |= (1 << COM2B1);
		OCR2B = b;
	}
}


uint8_t read_potentiometer()
{
	
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
void	led_value(uint8_t pos)
{
	if (pos < 63)
	{
		PORTB = 0;
	}
	else if (( pos >= 63) && ( pos < 127))
	{
		PORTB = (1 << PB0);
	}
	else if (( pos >= 127) && ( pos < 191))
	{
		PORTB = (1 << PB0) | (1 << PB1);
	}
	else if (( pos >= 191) && ( pos < 255))
	{
		PORTB = (1 << PB0) | (1 << PB1) | (1 << PB2);
	}
	else if (pos == 255)
	{
		PORTB = (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);
	}
}


void wheel(uint8_t pos) 
{
	pos = 255 - pos;
	if (pos < 85) // goes from red to blue
	{
		set_rgb(255 - pos * 3, 0, pos * 3);
	} 
	else if (pos < 170) // goes from blue to green
	{
		pos = pos - 85;
		set_rgb(0, pos * 3, 255 - pos * 3);
	} 
	else // goes from green to red
	{
		pos = pos - 170;
		set_rgb(pos * 3, 255 - pos * 3, 0);
	}
}

void init_potentiometer()
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
	uint8_t val;
	uart_init();
	init_potentiometer();
	init_rgb();
	
	while(1)
	{
		val = read_potentiometer();

		wheel(val);
		led_value(val);
		_delay_ms(20);
	}
}