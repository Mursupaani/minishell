/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:40:00 by magebreh          #+#    #+#             */
/*   Updated: 2025/09/18 16:09:43 by magebreh         ###   ########.fr       */
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

int ft_isspace(char c)
{
    return ((c >= 9 && c <= 13) || c == 32);
}

static size_t ft_count_words(char const *s, char c)
{
	size_t	words;
	int		in_word;

	words = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && !in_word)
		{
			in_word = 1;
			words++;
		}
		else if (*s == c)
			in_word = 0;
		s++;
	}
	return (words);
}

static char	*ft_extract_word(char const *s, size_t start, size_t len, t_arena *arena)
{
	char	*word;
	size_t	i;

	word = arena_alloc(arena, len + 1);
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = s[start + i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

char **ft_split_arena(char const *s, char c, t_arena *arena)
{
	char	**result;
	size_t	word_count;
	size_t	i;
	size_t	start;
	size_t	word_idx;

	if (!s)
		return (NULL);
	word_count = ft_count_words(s, c);
	result = arena_alloc(arena, (word_count + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	i = 0;
	word_idx = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			start = i;
			while (s[i] && s[i] != c)
				i++;
			result[word_idx] = ft_extract_word(s, start, i - start, arena);
			if (!result[word_idx])
				return (NULL);
			word_idx++;
		}
		else
			i++;
	}
	result[word_idx] = NULL;
	return (result);
}

int is_builtin_command(char *cmd_name)
{
    return (ft_strncmp(cmd_name, "echo", 5) == 0 ||
            ft_strncmp(cmd_name, "cd", 3) == 0 ||
            ft_strncmp(cmd_name, "pwd", 4) == 0 ||
            ft_strncmp(cmd_name, "export", 7) == 0 ||
            ft_strncmp(cmd_name, "unset", 6) == 0 ||
            ft_strncmp(cmd_name, "env", 4) == 0 ||
            ft_strncmp(cmd_name, "exit", 5) == 0);
}

int is_parent_only_builtin(char *cmd_name)
{
    return (ft_strncmp(cmd_name, "cd", 3) == 0 ||
            ft_strncmp(cmd_name, "export", 7) == 0 ||
            ft_strncmp(cmd_name, "unset", 6) == 0 ||
            ft_strncmp(cmd_name, "exit", 5) == 0);
}
