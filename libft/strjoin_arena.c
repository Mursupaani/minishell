/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strjoin_arena.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 14:14:53 by anpollan          #+#    #+#             */
/*   Updated: 2025/09/10 14:16:54 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin_arena(char const *s1, char const *s2, t_arena *arena)
{
	size_t	s1_len;
	size_t	s2_len;
	char	*res;

	if (!s1 || !s2)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	res = arena_alloc(arena, s1_len + s2_len + 1);
	if (!res)
		return (NULL);
	ft_strlcpy(res, s1, s1_len + 1);
	ft_strlcat(res, s2, s1_len + s2_len + 1);
	return (res);
}
