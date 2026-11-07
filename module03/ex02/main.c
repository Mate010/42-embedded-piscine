/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrossi <mgrossi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 11:07:02 by mgrossi           #+#    #+#             */
/*   Updated: 2026/04/25 21:28:00 by mgrossi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <avr/io.h>
# include <util/delay.h>
# include <avr/interrupt.h>

/*
	https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf
*/
/*
	PD5 : red
	PD3 : blue
	PD6 : green
*/



void init_rgb()
{
    // PD3, PD5, PD6 as outputs: red, blue, green LED
    DDRD |= (1 << PD3) | (1 << PD5) | (1 << PD6);
    

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
	OCR0A = g;
	OCR0B = r;
	OCR2B = b;
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

int	main(void)
{
	uint8_t pos = 0; 
	init_rgb();
	
	while (1)
	{
		wheel(pos++);
		_delay_ms(10);
	}
}
