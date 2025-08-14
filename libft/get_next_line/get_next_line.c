/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:53:04 by magebreh          #+#    #+#             */
/*   Updated: 2025/08/05 11:38:37 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ft_save_leftovers(char *buffer, char *leftovers)
{
	size_t	i;

	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (buffer[i] == '\n')
		i++;
	while (buffer[i])
	{
		*leftovers = buffer[i++];
		leftovers++;
	}
	*leftovers = '\0';
}

char	*ft_read_to_buffer(int fd, char *leftovers, char *temp_buff)
{
	char	*buffer;
	int		num_bytes_read;

	num_bytes_read = 1;
	if (leftovers)
		buffer = ft_strdup(leftovers);
	else
		buffer = ft_strdup("");
	if (!buffer)
		return (NULL);
	while (num_bytes_read > 0 && !ft_strchr(buffer, '\n'))
	{
		num_bytes_read = read(fd, temp_buff, BUFFER_SIZE);
		if (num_bytes_read == -1)
		{
			free(buffer);
			return (NULL);
		}
		temp_buff[num_bytes_read] = '\0';
		buffer = ft_strjoin_free(buffer, temp_buff);
		if (!buffer)
			return (NULL);
	}
	return (buffer);
}

char	*ft_strjoin_core(char *s1, char const *s2)
{
	char	*res;
	size_t	total_len;
	size_t	s1_len;
	size_t	s2_len;

	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	total_len = s1_len + s2_len;
	res = malloc(total_len + 1);
	if (!res)
	{
		free(s1);
		return (NULL);
	}
	ft_memcpy(res, s1, s1_len);
	ft_memcpy(res + s1_len, s2, s2_len);
	res[s1_len + s2_len] = '\0';
	return (res);
}

char	*ft_strjoin_free(char *s1, char const *s2)
{
	char	*res;

	if (!s1)
	{
		s1 = ft_strdup("");
		if (!s1)
			return (NULL);
	}
	if (!s2)
	{
		free(s1);
		return (NULL);
	}
	res = ft_strjoin_core(s1, s2);
	free(s1);
	return (res);
}

char	*get_next_line(int fd)
{
	static char	leftovers[BUFFER_SIZE + 1];
	char		*buffer;
	char		*line;
	char		*temp_buff;

	if (fd < 0 || fd > 1024)
		return (NULL);
	temp_buff = malloc(BUFFER_SIZE + 1);
	if (!temp_buff)
		return (NULL);
	buffer = ft_read_to_buffer(fd, leftovers, temp_buff);
	if (!buffer)
	{
		free(temp_buff);
		return (NULL);
	}
	line = ft_extract_line(buffer);
	ft_save_leftovers(buffer, leftovers);
	free(temp_buff);
	free (buffer);
	if (line == NULL || *line != 0)
		return (line);
	free(line);
	return (NULL);
}
