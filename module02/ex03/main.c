/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrossi <mgrossi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 11:07:02 by mgrossi           #+#    #+#             */
/*   Updated: 2026/04/21 18:20:45 by mgrossi          ###   ########.fr       */
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
		RXCIE0 : enables receive interrupts (local)
		
		
	*/
	UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);

	/*	
		p 203	
		enables the 8N1 frame format (0 00000000 1)
	*/
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
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

__attribute__((signal, used)) // It is a macro, for the compiler
void	USART_RX_vect(void) // like the uart_rx function (receives chars), except it does not block the program 
{
	unsigned char c = 0;
	c = UDR0;
	if (c >= 'a' && c <= 'z')
	{
		c -= 32;
	}
	else if(c >= 'A' && c <= 'Z')
	{
		c += 32;
	}
	uart_tx(c);
}

int	main(void)
{
	uart_init();

	SREG |= (1<<7); // (global) enables global interrupts p77 : 12.1 Interrupt Vectors in ATmega48A and ATmega48PA
	while (1)
	{
	}
}
