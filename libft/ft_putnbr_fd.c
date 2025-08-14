/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 12:44:56 by magebreh          #+#    #+#             */
/*   Updated: 2025/08/05 11:37:45 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	char	digits[11];
	int		i;
	long	nbr;

	nbr = n;
	i = 0;
	if (nbr == 0)
		ft_putchar_fd('0', fd);
	if (nbr < 0)
	{
		ft_putchar_fd('-', fd);
		nbr = -nbr;
	}
	while (nbr > 0)
	{
		digits[i++] = '0' + (nbr % 10);
		nbr /= 10;
	}
	while (i > 0)
		ft_putchar_fd(digits[--i], fd);
}
