/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrossi <mgrossi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 11:07:02 by mgrossi           #+#    #+#             */
/*   Updated: 2026/04/15 19:02:15 by mgrossi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>

/*
	https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf
*/


// Used to spend CPU time
void	ft_delay_ms(uint16_t ms)
{
	for (uint16_t i = 0; i < ms; i++) // Each loop lasts 1 ms 
		for (volatile uint16_t j = 0; j < 1000; j++); // spends 1 ms per batch of 1000 iterations
}

int	main(void)
{
	DDRB |= (1 << PB1);

	while (1)
	{
		PORTB ^= (1<<PB1);
		ft_delay_ms(500);
	}
}
