/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrossi <mgrossi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 11:07:02 by mgrossi           #+#    #+#             */
/*   Updated: 2026/04/26 15:50:34 by mgrossi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <avr/io.h>
# include <util/delay.h>

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
    TCCR0B = (1 << CS01) | (1 << CS00); // Frequency divided by 64

	// Timer 2 : Fast PWM, 
	// WGM21 : enables fast PWM
	// WGM20 : enables fast PWM
	// COM2B1 : clears the pin to 0 when timer2 matches OCR2B
	// P : 117
	// CS21 : prescaler 
    TCCR2A = (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
    TCCR2B = (1 << CS22); // Frequency divided by 64
}


void uart_init()
{
	/*
		ubrr = (F_CPU / prescale / UART_BAUDRATE) - 1 <- (in theory)	
	*/
	uint16_t ubrr = F_CPU / (16 * 115200);

	/*
	baud rate (communication speed): 
		p204 : 20.11.5 UBRRnL and UBRRnH – USART Baud Rate Registers 
		UBRR0H : USART Baud Rate Register 0 High
		UBRR0L : USART Baud Rate Register 0 Low
			splits the 16-bit ubrr value into two 8-bit registers
	*/
	UBRR0H = (uint16_t)(ubrr >> 8);
	UBRR0L = (uint16_t)ubrr;
	/*	
		p 203	
		enables the 8N1 frame format (0 00000000 1)
	*/
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
	/*
		UCSR0B : UCSRnB – USART Control and Status Register n B / 
		TXEN0 : Transmitter Enable n / Enables transmission
		RXEN0 : Receiver Enable n / Enables reception
	*/
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0);
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

// Receives a char
char uart_rx(void)
{
	while (!(UCSR0A & (1 << RXC0)))
		;
	return (UDR0);
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

// Sends a string
void uart_print_str(const char* str)
{
	while(*str)
	{
		uart_tx(*str++);
	}	
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
uint8_t rgb_parsing(char *buffer)
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t i = 1;
	
	if (buffer [0] != '#' || buffer[7] != '\0')
		return (0);


	// check that the buffer only contains hexadecimal chars
	while (i <= 6) 
	{

		if (!((buffer[i] >= '0' && buffer[i] <= '9') || 
			(buffer[i] >= 'a' && buffer[i] <= 'f') || 
			(buffer[i] >= 'A' && buffer[i] <= 'F'))) 
		{
			return (0); 
		}

		i++;
	}
	/*
	 if r = #A3 
	  uint8_t r = 0000 0000;
	hex_to_value : 
	  r = A -> 10 and 3 -> 3

	buffer[1]) << 4 
		<< 4  : same as multiplying by 2 to the power of 4 (so by 16), as used for hexadecimal
	  
	*/
	r = (hex_to_value(buffer[1]) << 4) + hex_to_value(buffer[2]);
	g = (hex_to_value(buffer[3]) << 4) + hex_to_value(buffer[4]);
	b = (hex_to_value(buffer[5]) << 4) + hex_to_value(buffer[6]);

	set_rgb(r, g, b);
	return(1);
}

void	read_input()
{
	char buffer[8]; // 8 is the max number of bits
	char c;
	uint8_t i = 0;

	while (1)
	{
		c = uart_rx(); // receive the char
		if (c == '\n' || c == '\r') // when the Enter key is found 
		{
			buffer[i] = '\0';
			uart_print_str("\r\n");
			if(!rgb_parsing(buffer))
			{
				uart_print_str("Error: format\r\n");
			}
			return;
		}

		else if (( c == '\b' || c == 127)) // if we want to delete a character
		{
			if (i > 0)
			{
				i--;
				uart_print_str("\b \b"); // \b is a backspace 
			}
		}

		else if (c == 27) // if Escape (27) is pressed, do nothing
		{

		}
		else if (i < 8 - 1) // Check there is room left (8 total - 1 for the '\0')
		{
			buffer[i++] = c;
			uart_tx(c); // echo the character back to the terminal
		}
	}
	
}

int	main(void)
{
	init_rgb();
	uart_init();
	uart_print_str("Test apres uart \r\n");
	uart_print_str("Send #RRGGBB\r\n");
	while (1)
	{
		read_input();
	}
}
