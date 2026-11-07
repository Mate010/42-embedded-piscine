/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i2c.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrossi <mgrossi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 17:41:18 by mgrossi           #+#    #+#             */
/*   Updated: 2026/05/02 12:45:16 by mgrossi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
# include "uart.h"

void i2c_init(void)
{
	/*
		page: 222 / 225
			computes the bit rate
			SCL = F_CPU / (16 + 2 * TWBR * prescaler)
			100000 = 16000000 / (16 + 2 * TWBR)
			
			TWBR : register where the speed is set
			SCL : Serial Clock, sets the pace, the standard is 100000Hz

			TWBR = 72;

		page: 241
			TWSR : TWI Status Register
				TWPS1 : clear this bit to set the prescaler to 1
				TWPS0 : clear this bit to set the prescaler to 1
	*/
	TWBR = 72;
	TWSR &= ~((1 << TWPS1) | (1<< TWPS0));
}

// starts the communication
void i2c_start(void)
{
	/*
	P: 239 / 225
		TWCR : 	TWI Control Register (I2C control register)
			TWINT : Interrupt flag, automatically set back to 1 when the module has finished sending a signal
			TWSTA : makes us the master, sends a signal to start the communication
			TWEN : enables the I2C interface (TWI)
	*/
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT))) // while TWINT is 0, wait for it to be set back to 1
		;
}



void i2c_write(unsigned char data)
{
	/*
		TWDR : the data is placed here to be sent on the bus
			data : 
		TWCR : 	TWI Control Register (I2C control register)
			TWINT : Interrupt flag, automatically set back to 1 when the module has finished sending a signal
			TWEN : enables the I2C interface (TWI)
	*/
	
	
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)))
		;
}
// void i2c_read(void)
// {
	
// }

void print_hex_value(uint8_t c)
{
	uint8_t high = c >> 4;
	uint8_t low = c & 0x0F;

	if (high <= 9)
		uart_tx('0' + high);
	else if ( high >= 10 && high <= 15)
		uart_tx('A' + high - 10);

	if (low <= 9)
		uart_tx('0' + low);
	else if ( low >= 10 && low <= 15)
		uart_tx('A' + low - 10);
}


void i2c_stop(void)
{
	/*
	TWSTO : TWSTA is replaced by TWSTO, ends the communication
	*/	
	
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);

}


// receives a byte from the slave and tells it to continue
uint8_t i2c_ack(void)
{
	/*
		TWCR : TWI Control Register (I2C control register)
			TWINT : Interrupt flag, automatically set back to 1 when the module has finished sending a signal
			TWEA : tells the slave to keep sending another byte
			TWEN : enables the I2C interface (TWI)
		TWDR : holds the received byte
	*/
	
	TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)))
		;
	return (TWDR);
}

// receives a byte from the slave but does not ask it to continue
uint8_t i2c_nack(void)
{
	// there is no TWEA, so the slave does not continue 
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)))
		;
	return (TWDR);
}