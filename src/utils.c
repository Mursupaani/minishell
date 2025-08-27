/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:40:00 by magebreh          #+#    #+#             */
/*   Updated: 2025/08/27 19:19:32 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_str_array(char **str_array)
{
	int	i;

	if (!str_array)
		return ;
	i = 0;
	while (str_array[i])
		printf("%s\n", str_array[i++]);
}

void	cleanup_shell_partial(t_shell *shell, int level)
{
	if (level >= 3)
		arena_free(&shell->command_arena);
	if (level >= 2)
		arena_free(&shell->session_arena);
	if (level >= 1)
		free(shell);
}

char *arena_substr(char *src, size_t start, size_t len, t_arena *arena)
{
    char *substr;
    size_t i;
    
    if (!src || !arena || start >= ft_strlen(src))
        return (NULL);
    substr = arena_alloc(arena, len + 1);
    if (!substr)
        return (NULL);
    i = 0;
    while (i < len && src[start + i])
    {
        substr[i] = src[start + i];
        i++;
    }
    substr[i] = '\0';
    return (substr);
}

char *arena_strdup(const char *s, t_arena *arena)
{
	char *copy;

	if (!s || !arena)
		return (NULL);
	copy = arena_alloc(arena, ft_strlen(s) + 1);
	if (!copy)
		return (NULL);
	ft_strlcpy(copy, s, ft_strlen(s) + 1);
	return (copy);
}

char *skip_whitespace(char *pos)
{
    if(!pos)
        return (NULL);
    while (*pos && ((*pos >= 9 && *pos <= 13) || *pos == 32))
		pos++;
    return (pos);
}

int ft_is_special_char(char c)
{
    return (c == '|' || c == '<' || c == '>' || c == '"' || c == '\'');
}

static int ft_isspace(char c)
{
    return ((c >= 9 && c <= 13) || c == 32);
}
