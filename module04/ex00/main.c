/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrossi <mgrossi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 11:07:02 by mgrossi           #+#    #+#             */
/*   Updated: 2026/04/28 13:50:08 by mgrossi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <avr/io.h>
# include <util/delay.h>


/*
https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf

*/


volatile uint8_t button_is_push = 0;

void init_led()
{
    DDRB |= (1 << PB1); // LED PB1 as output
}

void init_button()
{
	DDRB &= ~(1 << PB2); // Set the button as input
	PORTD |= (1 << PD2); // Enable the pull-up resistor (to avoid false bounces)
}

void init_timer1()
{
	TCCR1B |= (1 << WGM12); // enable CTC mode (clear timer on compare match)
	OCR1A = (F_CPU / 1024) / 20; // 50ms (16 000 000 / prescaler) / 
	TIMSK1 |= (1 << OCIE1A); // enables the global interrupt when it matches OCR1A
	//p 145 : 18.11.6 TIMSK2 – Timer/Counter2 Interrupt Mask Register
	
}
void init_interrupts()
{
	SREG |= (1 << 7); // Enable global interrupts
	EIMSK = (1 << INT0); // Enable local interrupts (sw1) (because we will disable them later)
	// P 81 : EIMSK – External Interrupt Mask Register
						// INT0 : sets the mode so it triggers specifically when the button is pressed 
	EICRA = (1 << ISC01); // enable falling edge mode (button press)
	// P 80 : EICRA – External Interrupt Control Register A
}

// This function runs as soon as you touch the button
__attribute__((signal, used))
void INT0_vect(void)
{
	EIMSK &= ~(1 << INT0); // disable local interrupts (so EICRA = (1 << ISC01) is no longer active)
	TCNT1 = 0; // Reset timer
	TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10); // set CTC mode (the prescaler is simply required)
}


__attribute__((signal, used))
void TIMER1_COMPA_vect(void)
{
	TCCR1B = 0; // Stop timer 1
	TCNT1 = 0; // reset counter
	if (button_is_push == 0)
	{
		PORTB ^= (1 << PB1); // toggle the LED 
		button_is_push = 1; // button is pressed
		EICRA |= (1 << ISC00); // rising edge mode is active (button released)
	}
	else
	{
		button_is_push = 0; // button not pressed 
		EICRA &= ~(1 << ISC00); // enable falling edge mode (button press)
	}
	EIFR |= (1 << INTF0); // Clear INT0 interrupt flag
    EIMSK |= (1 << INT0); // re-enable local interrupts
}

int main(void)
{
    init_led();
    init_button();
    init_timer1();
	init_interrupts();
    while (1)
    {
    }
}