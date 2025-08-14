/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_hex.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 11:35:00 by magebreh          #+#    #+#             */
/*   Updated: 2025/08/05 11:33:46 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static int	parse(const char *str, int *i)
{
	while (str[*i] == ' ' || (str[*i] >= 9 && str[*i] <= 13))
		(*i)++;
	if (str[*i] == '0' && (str[*i + 1] == 'x' || str[*i + 1] == 'X'))
	{
		*i += 2;
		return (1);
	}
	return (0);
}

static int	hex_char_to_value(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	else if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	else if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	return (-1);
}

int	ft_atoi_hex(const char *str, int *err)
{
	int		i;
	long	result;
	int		hex_value;

	i = 0;
	result = 0;
	*err = 0;
	if (!str)
		return (*err = 1, 0);
	if (!parse(str, &i))
		return (*err = 1, 0);
	if (hex_char_to_value(str[i]) == -1)
		return (*err = 1, 0);
	while (hex_char_to_value(str[i]) != -1)
	{
		hex_value = hex_char_to_value(str[i]);
		result = result * 16 + hex_value;
		if (result > 0xFFFFFF)
			return (*err = 1, 0);
		i++;
	}
	if (str[i] != '\0' && str[i] != '\n')
		return (*err = 1, 0);
	return ((int)result);
}
