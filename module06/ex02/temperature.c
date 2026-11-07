/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   temperature.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrossi <mgrossi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 13:22:17 by mgrossi           #+#    #+#             */
/*   Updated: 2026/05/02 15:11:44 by mgrossi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h" 
#include "uart.h"
#include <stdlib.h>

void raw_calcuation(float *hum, float *temp)
{

	uint8_t data[7];
	uint32_t hum_raw;
	uint32_t temp_raw;
	
	i2c_write((0x38 << 1) | 1);
	for (int i = 0; i < 6; i++)
	{
		data[i] = i2c_ack(); 
	}
	data[6] = i2c_nack();

	/*
		this is equivalent to: 
		hum_raw = ((uint32_t)data[1] * 4096) + ((uint32_t)data[2] * 16) + ((uint32_t)data[3] / 16);

		we combine the first two bytes and half of the third, hence the division of data[3]

		temp_raw : 
		we take the other half of byte 3 and add data 4 and 5
	*/
	hum_raw = ((uint32_t)data[1] << 12) | ((uint32_t)data[2] << 4) | ((uint32_t)data[3] >> 4);
	temp_raw = (((uint32_t)data[3] & 0x0F) << 16) | ((uint32_t)data[4] << 8) | ((uint32_t)data[5]);
	

	/*
		page : 8 / calculation
	*/
	*hum = ((float)hum_raw * 100.0) / 1048576.0;
	*temp = ((float)temp_raw * 200) / 1048576.0 - 50.0;
}


void average_calculation(float hum,float temp,float *hum_average,float *temp_average)
{
	static float	temp_hist[3] = {0, 0, 0};
	static float	hum_hist[3] = {0, 0, 0};
	static int	index = 0;
	static int	count = 0;
	/*
		static so it does not restart from 0 on each call
	*/
	
	hum_hist[index] = hum;
	temp_hist[index] = temp;
	
	index = (index + 1) % 3; // the modulo is there to wrap the index around
	if (count < 3)
		count++;

	*hum_average = 0;
	*temp_average = 0;
	for (int i = 0; i < count; i++)
	{
		*hum_average += hum_hist[i];
		*temp_average += temp_hist[i];		
	}
		
	*hum_average /= count;
	*temp_average /= count;
	
}

void print_data(float hum_average,float temp_average)
{
	    char    buf[16];

	/*
		p : 2
			humidity has a resolution of 0.024, so our precision level is 0.1%
			temperature has a resolution of 0.01, so our precision level is 0.01%
	*/
    uart_print_str("Temperature: ");
    dtostrf(temp_average, 0, 2, buf);
    uart_print_str(buf);
    
	uart_print_str(" C, Humidity: ");
    dtostrf(hum_average, 0, 1, buf);
    uart_print_str(buf);
    uart_print_str("%");
    uart_print_str("\n");
	uart_print_str("\r");
}
