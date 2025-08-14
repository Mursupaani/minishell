/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 15:13:04 by magebreh          #+#    #+#             */
/*   Updated: 2025/04/24 18:48:12 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr_empty(void)
{
	char	*res;

	res = malloc(1);
	if (!res)
		return (NULL);
	res[0] = '\0';
	return (res);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*res;
	size_t	s_len;
	size_t	copy_len;
	size_t	available;

	s_len = 0;
	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (ft_substr_empty());
	available = (s_len - start);
	if (len < available)
		copy_len = len;
	else
		copy_len = available;
	res = malloc(copy_len + 1);
	if (!res)
		return (NULL);
	ft_strlcpy(res, s + start, copy_len + 1);
	return (res);
}
