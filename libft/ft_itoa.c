/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 16:33:20 by magebreh          #+#    #+#             */
/*   Updated: 2025/04/22 12:28:45 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	count_digits(long n)
{
	int	count;

	count = 0;
	if (n == 0)
		return (1);
	while (n)
	{
		n /= 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	long	num;
	int		sign;
	int		len;
	char	*res;

	sign = (n < 0);
	num = n;
	if (sign)
		num = -num;
	len = count_digits(num) + sign;
	res = malloc(len + 1);
	if (!res)
		return (NULL);
	res[len] = '\0';
	while (len-- > 0)
	{
		res[len] = (num % 10) + '0';
		num /= 10;
		if (len == 0 && sign)
			res[0] = '-';
	}
	return (res);
}
