/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 12:11:43 by magebreh          #+#    #+#             */
/*   Updated: 2025/08/05 11:36:01 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static int	handle_overflow(long result, int sign, int *err)
{
	if (result * sign > 2147483647)
	{
		*err = 1;
		return (2147483647);
	}
	if (result * sign < -2147483648)
	{
		*err = 1;
		return (-2147483648);
	}
	return (0);
}

int	ft_atoi_safe(const char *str, int *err)
{
	int		i;
	int		sign;
	long	result;
	int		overflow_val;

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
			return (overflow_val);
		i++;
	}
	if (str[i] != '\0' && str[i] != '\n')
		return (*err = 1, 0);
	return ((int)(result * sign));
}
