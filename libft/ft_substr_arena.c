/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr_arena.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 15:13:04 by magebreh          #+#    #+#             */
/*   Updated: 2025/08/23 12:18:36 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr_empty_arena(t_arena *arena)
{
	char	*res;

	res = arena_alloc(arena, 1);
	if (!res)
		return (NULL);
	res[0] = '\0';
	return (res);
}

char	*ft_substr_arena(
	char const *s, unsigned int start, size_t len, t_arena *arena)
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
		return (ft_substr_empty_arena(arena));
	available = (s_len - start);
	if (len < available)
		copy_len = len;
	else
		copy_len = available;
	res = arena_alloc(arena, copy_len + 1);
	if (!res)
		return (NULL);
	ft_strlcpy(res, s + start, copy_len + 1);
	return (res);
}
