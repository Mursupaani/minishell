/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_prompt.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 22:09:01 by anpollan          #+#    #+#             */
/*   Updated: 2025/10/06 22:12:10 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

static void	update_prompt_colors(char *cwd, t_shell *shell);

void	update_prompt(t_shell *shell, t_arena *arena)
{
	char	*cwd;
	char	*home;
	int		home_len;

	cwd = arena_alloc(shell->command_arena, PWD_BUFFER);
	if (!cwd)
		error_exit_and_free_memory(shell);
	if (!getcwd(cwd, PWD_BUFFER))
		cwd = arena_strdup(hash_table_get(shell->env_table, "PWD"), arena);
	if (!cwd)
	{
		update_prompt_colors("minishell\n% ", shell);
		return ;
	}
	home = hash_table_get(shell->env_table, "HOME");
	if (home && home[0] != '\0')
	{
		home_len = ft_strlen(home);
		if (ft_strnstr(cwd, home, home_len) == cwd)
		{
			cwd[home_len - 1] = '~';
			cwd += home_len - 1;
		}
	}
	update_prompt_colors(cwd, shell);
}

static void	update_prompt_colors(char *cwd, t_shell *shell)
{
	char	*temp;

	if (!cwd || !shell)
		return ;
	temp = ft_strjoin("\033[1;30;47m", cwd);
	if (!temp)
		error_exit_and_free_memory(shell);
	shell->prompt = ft_strjoin_arena(temp, "\033[0m\n % ", shell->command_arena);
	free(temp);
	if (!shell->prompt)
		error_exit_and_free_memory(shell);
}
