/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fprint_hex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 12:45:46 by magebreh          #+#    #+#             */
/*   Updated: 2025/09/25 16:28:39 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_utoa_hex(uintptr_t n, int is_uppercase)
{
	char			*res;
	unsigned int	len;
	const char		*base_lower;
	const char		*base_upper;
	const char		*base;

	len = 0;
	base_lower = "0123456789abcdef";
	base_upper = "0123456789ABCDEF";
	if (is_uppercase == UPPERCASE)
		base = base_upper;
	else
		base = base_lower;
	len = count_digits(n);
	res = malloc(len + 1);
	if (!res)
		return (NULL);
	res[len] = '\0';
	while (len-- > 0)
	{
		res[len] = base[n % 16];
		n /= 16;
	}
	return (res);
}

static int	print_hex(va_list *args, int is_uppercase, int file)
{
	int				i;
	uintptr_t		n;
	char			*buff;

	i = 0;
	n = va_arg(*args, unsigned int);
	if (n == 0)
	{
		write(file, "0", 1);
		return (1);
	}
	buff = ft_utoa_hex(n, is_uppercase);
	while (buff[i])
		write(file, &buff[i++], 1);
	free(buff);
	return (i);
}

int	fprint_hex_lower(va_list *args, int file)
{
	return (print_hex(args, LOWERCASE, file));
}

int	fprint_hex_upper(va_list *args, int file)
{
	return (print_hex(args, UPPERCASE, file));
}
