/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 11:46:25 by magebreh          #+#    #+#             */
/*   Updated: 2025/04/23 10:57:03 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	val ;
	unsigned char	*ptr;

	i = 0;
	val = (unsigned char)c;
	ptr = (unsigned char *)s;
	while (i < n)
	{
		ptr[i] = val;
		i++;
	}
	return (s);
}
