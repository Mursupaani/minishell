/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:40:00 by magebreh          #+#    #+#             */
/*   Updated: 2025/08/25 13:15:59 by magebreh         ###   ########.fr       */
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
