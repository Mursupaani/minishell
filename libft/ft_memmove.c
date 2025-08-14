/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 19:36:19 by magebreh          #+#    #+#             */
/*   Updated: 2025/04/22 12:44:22 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t				i;
	const unsigned char	*src_bytes;
	unsigned char		*dest_bytes;

	src_bytes = (unsigned char *)src;
	dest_bytes = (unsigned char *)dest;
	if (dest < src)
	{
		i = 0;
		while (i < n)
		{
			dest_bytes[i] = src_bytes[i];
			i++;
		}
	}
	else
	{
		i = n;
		while (i)
		{
			dest_bytes[i - 1] = src_bytes[i - 1];
			i--;
		}
	}
	return (dest);
}
