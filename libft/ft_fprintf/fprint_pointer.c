/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fprint_pointer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 12:45:27 by magebreh          #+#    #+#             */
/*   Updated: 2025/09/25 16:24:31 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	fprint_pointer(va_list *args, int file)
{
	void			*ptr;
	uintptr_t		n;
	char			*hex_str;
	int				len;

	ptr = va_arg(*args, void *);
	if (ptr == NULL)
	{
		write(file, "(nil)", 5);
		return (5);
	}
	n = (uintptr_t) ptr;
	hex_str = ft_utoa_hex(n, LOWERCASE);
	if (hex_str == NULL)
		return (-1);
	write(file, "0x", 2);
	write(file, hex_str, ft_strlen(hex_str));
	len = ft_strlen(hex_str);
	free(hex_str);
	return (2 + len);
}
