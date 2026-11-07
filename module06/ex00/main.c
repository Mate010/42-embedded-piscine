/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrossi <mgrossi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 11:07:02 by mgrossi           #+#    #+#             */
/*   Updated: 2026/05/01 17:38:46 by mgrossi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <avr/io.h>
# include <util/delay.h>
#include "uart.h"

/*
	https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf
	https://files.seeedstudio.com/wiki/Grove-AHT20_I2C_Industrial_Grade_Temperature_and_Humidity_Sensor/AHT20-datasheet-2020-4-16.pdf
*/



void ft_putnbr(uint16_t n)
{
    if (n >= 10)
    {
		ft_putnbr(n / 10);
    }
    uart_tx((n % 10) + '0');
}


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
void i2c_stop(void)
{
	/*
	TWSTO : TWSTA is replaced by TWSTO, ends the communication
	*/	
	
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);

}

int main(void)
{
	uint8_t status;
	uart_init();
	i2c_init();
	/*
	 	page : 227
			TWSR : gives the status of the operation between master and slave.
			TWDR : gives the data of the operation between master and slave.
	*/
	while(1)
	{
		i2c_start();
		status = TWSR & 0xf8;
		ft_putnbr(status);
		uart_print_str("\r\n");
		i2c_stop();
		_delay_ms(50);
	}
}
