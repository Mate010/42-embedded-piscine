/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrossi <mgrossi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 11:07:02 by mgrossi           #+#    #+#             */
/*   Updated: 2026/05/02 12:34:29 by mgrossi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <avr/io.h>
# include <util/delay.h>
#include "uart.h"
#include "i2c.h"

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




void	first_write()
{
	/*
		The first bit says who we are talking to
	
		i2c_write(0x38 << 1); We send the AHT20 address -> 0x38,
		the last bit at the end is 0, so we are in write mode.

		i2c_write(0xAC); triggers the measurement, required parameter p: 8
		i2c_write(0x33); required parameter p: 8 (5.4 Sensor reading process)
		i2c_write(0x00); required parameter p: 8 (5.4 Sensor reading process)

		_delay_ms(80); required parameter p: 8 (5.4 Sensor reading process)


	*/
		i2c_start();
		i2c_write(0x38 << 1);
		i2c_write(0xAC);
		i2c_write(0x33); 
		i2c_write(0x00); 
		i2c_stop();
		_delay_ms(80); 
}

int main(void)
{
	char c;
	uart_init();
	i2c_init();
	
	while(1)
	{
		first_write();
		i2c_start();
		i2c_write((0x38 << 1) | 1); // send the address and say we are going to read
		for (int i = 0; i < 6; i++)
		{
			c = i2c_ack();
			print_hex_value(c);
			uart_tx(' ');
		}
		c = i2c_nack();
		print_hex_value(c);
		uart_print_str("\r\n");
		i2c_stop();
		_delay_ms(50);
	}
}
