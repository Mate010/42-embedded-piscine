/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrossi <mgrossi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 11:07:02 by mgrossi           #+#    #+#             */
/*   Updated: 2026/04/21 18:10:50 by mgrossi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/*
https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf
 184 : USART Initialization
 202 : UMSELn Bits Settings
*/
void uart_init()
{
	/*
		ubrr = (F_CPU / prescale / UART_BAUDRATE) - 1 <- (in theory)	
	*/
	uint16_t ubrr = (F_CPU / 16 / 115200);

	/*
	baud rate (communication speed): 
		p204 : 20.11.5 UBRRnL and UBRRnH – USART Baud Rate Registers 
		UBRR0H : USART Baud Rate Register 0 High
		UBRR0L : USART Baud Rate Register 0 Low
			splits the 16-bit ubrr value into two 8-bit registers
	*/
	UBRR0H = (ubrr << 8);
	UBRR0L =  ubrr;

	/*
	
		UCSR0B : UCSRnB – USART Control and Status Register n B / 
		TXEN0 : Transmitter Enable n / Enables transmission
		RXEN0 : Receiver Enable n / Enables reception
		
	*/
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);

	/*	
		p 203	
		enables the 8N1 frame format (0 00000000 1)
	*/
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}



void timer1_init()
{
	// linked to CTC (WGM12)
	OCR1A = 31249;
	// Toggle for 0.5s, LED blink 1Hz
    // (16MHz (16 000)/ 1024 * 0.5) - 1 // Microcontroller clock frequency / Prescaler * desired period 0.5s - we start at 0, not 1
	// 31249 is the number of timer clock cycles.

	// enable interrupts on the timer
	TIMSK1 = (1 << OCIE1A); // p144 : Timer/Counter1 Interrupt Mask Register
	
	// CTC mode + Prescaler 1024
	TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); // Prescale 1024
}


// Sends a char 
void uart_tx(char c)
{
	// Wait for the transmit buffer (UDR0) to be empty 
    // Loop while the UDRE0 bit is 0
    while (!(UCSR0A & (1 << UDRE0)))
		;
    // Put the character in the UDR0 register to send it
	UDR0 = c;
}

void uart_print_str(const char* str)
{
	while(*str)
	{
		uart_tx(*str);
		str++;
	}	
}


__attribute__((signal, used)) // It is a macro, for the compiler
void	TIMER1_COMPA_vect(void) // runs when the timer matches 
{
	uart_print_str("Hello World!\r\n");
}

int	main(void)
{
	uart_init();
	timer1_init();

	SREG |= (1<<7); // enables global interrupts p77 : 12.1 Interrupt Vectors in ATmega48A and ATmega48PA
	while (1)
	{
	}
}
