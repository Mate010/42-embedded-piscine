/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrossi <mgrossi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 11:07:02 by mgrossi           #+#    #+#             */
/*   Updated: 2026/04/22 19:05:41 by mgrossi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <avr/io.h>
# include <util/delay.h>
# include <avr/interrupt.h>

/*
	https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf
*/


/*
	PD5 : red
	PD3 : blue
	PD6 : green
*/
int	main(void)
{
	DDRD |= (1 << PD3) | (1 << PD5) | (1 << PD6) ;
	while (1)
	{
		PORTD = (1 <<PD5);
		_delay_ms(1000);
		PORTD = (1 <<PD3);
		_delay_ms(1000);
		PORTD = (1 <<PD6);
		_delay_ms(1000);
	}
}
