/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrossi <mgrossi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 11:07:02 by mgrossi           #+#    #+#             */
/*   Updated: 2026/04/28 13:58:13 by mgrossi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <avr/io.h>
# include <util/delay.h>


/*
https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf

*/


volatile uint8_t direction = 1;

void init_led()
{
    DDRB |= (1 << PB1); // LED PB1 as output
}


void init_timer1()
{
	/*
		page : 140
		TCCR1A TCCR1B: Timer 1 control
			COM1A1 : with Fast PWM, turns on at the start of each cycle
			WGM10 : enables Fast PWM mode
			WGM12 : enables Fast PWM mode
			CS10 | CS11 : sets the prescaler to 64, lowers the speed but stays smooth
			
	*/
	TCCR1A |= (1 << COM1A1) | (1 << WGM10);
	TCCR1B |= (1 << WGM12) |  (1 << CS10) |  (1 << CS11);
	OCR1A = 0; // start the light at 0%
}

void init_timer0()
{
	/*
	page : 113
		TCCR0A : Timer 0 control
			WGM01 : ENABLES CTC MODE
			CS02 : sets the prescaler to 1024 
			CS00 : sets the prescaler to 1024

			target_frequency (510) : 255 (going up) + 255 (going down)
			
			timer speed (15625) : F_CPU (16 000 000)/ Prescaler (1024)
			
			F_target (30) : timer speed (15625) / target_frequency (510)
			
			OCR0A : F_CPU / (Prescaler * F_target)) - 1
			       	16 000 000 Hz / (1024 * 30) - 1
			
			OCR0A : 30

			TIMSK0 : enables the global interrupt when it matches OCR2A
			
	*/
	
	TCCR0A |= (1 << WGM01);
	TCCR0B |= (1 << CS02) | (1 << CS00);
	OCR0A = 30;
	TIMSK0 |= (1 << OCIE0A);
}


void init_global_interruption()
{
	SREG |= (1 << 7); // Enable global interrupts
}

/*
	Interrupt called at the end of each Timer0 count
*/
__attribute__((signal, used))
void TIMER0_COMPA_vect(void)
{
    if (direction) // increase the LED on-time 
	{
        if (OCR1A < 255)
            OCR1A++;
        else
            direction = 0;
    } 
	else // decrease the LED on-time 
	{
        if (OCR1A > 0)
            OCR1A--;
        else
            direction = 1;
    }
}

int main(void)
{
    init_led();
	init_timer0(); 
    init_timer1();
	init_global_interruption();
    while (1)
    {
    }
}
