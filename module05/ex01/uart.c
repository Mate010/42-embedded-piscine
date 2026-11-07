/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uart.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrossi <mgrossi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 15:34:54 by mgrossi           #+#    #+#             */
/*   Updated: 2026/04/28 15:41:23 by mgrossi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

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

void uart_tx(char c)
{
	// Wait for the transmit buffer (UDR0) to be empty 
    // Loop while the UDRE0 bit is 0
    while (!(UCSR0A & (1 << UDRE0)))
		;
    // Put the character in the UDR0 register to send it
	UDR0 = c;
}


// Receives a char
char uart_rx(void)
{
	while (!(UCSR0A & (1 << RXC0)))
		;
	return (UDR0);
}


void uart_print_str(const char* str)
{
	while(*str)
	{
		uart_tx(*str);
		str++;
	}	
}
