/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 12:37:51 by magebreh          #+#    #+#             */
/*   Updated: 2025/04/22 12:48:29 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putendl_fd(char *s, int fd)
{
	size_t	s_len;
	char	new_line;

	new_line = '\n';
	if (s)
	{
		s_len = ft_strlen(s);
		write(fd, s, s_len);
	}
	write(fd, &new_line, 1);
}
