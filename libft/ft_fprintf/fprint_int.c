/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fprint_int.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 12:45:42 by magebreh          #+#    #+#             */
/*   Updated: 2025/09/25 16:21:22 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	fprint_int(va_list *args, int file)
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
	bytes = write(file, str, i);
	free(str);
	return (bytes);
}
