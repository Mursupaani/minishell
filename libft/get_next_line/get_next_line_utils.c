/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:52:52 by magebreh          #+#    #+#             */
/*   Updated: 2025/07/11 19:15:42 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_extract_line(char *s)
{
	char	*line;
	size_t	i;
	size_t	len;

	len = 0;
	if (!s || !*s)
		return (NULL);
	while (s[len] && s[len] != '\n')
		len++;
	if (s[len] == '\n')
		len++;
	line = malloc(sizeof(char) * (len + 1));
	if (!line)
		return (NULL);
	i = 0;
	while (i < len)
	{
		line[i] = s[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}
