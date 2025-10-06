/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 12:11:43 by magebreh          #+#    #+#             */
/*   Updated: 2025/10/06 17:48:12 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <limits.h>

static int	parse(const char *str, int *i)
{
	int	sign;

	sign = 1;
	while (str[*i] == ' ' || (str[*i] >= 9 && str[*i] <= 13))
		(*i)++;
	if (str[*i] == '-' || str[*i] == '+')
	{
		if (str[*i] == '-')
			sign = -1;
		(*i)++;
	}
	return (sign);
}

static int64_t	handle_overflow(int64_t result, int sign, int *err)
{
	result *= sign;
	if (sign > 0)
	{
		if (result > LONG_MAX)
		{
			*err = 1;
			return (LLONG_MAX);
		}
	}
	else
	{
		if (result < LONG_MIN)
		{
			*err = 1;
			return (LLONG_MIN);
		}
	}
	return (result);
}

int64_t	ft_atol_safe(const char *str, int *err)
{
	int		i;
	int		sign;
	int64_t	result;
	int64_t	overflow_val;

	i = 0;
	result = 0;
	*err = 0;
	if (!str)
		return (*err = 1, 0);
	sign = parse(str, &i);
	if (!(str[i] >= '0' && str[i] <= '9'))
		return (*err = 1, 0);
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		overflow_val = handle_overflow(result, sign, err);
		if (*err)
			return ((long int)overflow_val);
		i++;
	}
	if (str[i] != '\0' && str[i] != '\n')
		return (*err = 1, 0);
	return ((long int)(result * sign));
}
