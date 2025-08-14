/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_int.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 12:45:42 by magebreh          #+#    #+#             */
/*   Updated: 2025/07/11 19:17:02 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	print_int(va_list *args)
{
	int		n;
	char	*str;
	int		bytes;
	int		i;

	i = 0;
	bytes = 0;
	n = va_arg(*args, int);
	str = ft_itoa(n);
	if (!str)
		return (-1);
	while (str[i])
		i++;
	bytes = write(1, str, i);
	free(str);
	return (bytes);
}
