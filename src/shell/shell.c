/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:40:00 by magebreh          #+#    #+#             */
/*   Updated: 2025/10/20 17:54:19 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_shell	*shell_static_init(void)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->last_exit_status = 0;
	shell->heredoc_counter = 0;
	shell->stdin_fd = STDIN_FILENO;
	shell->stdout_fd = STDOUT_FILENO;
	shell->tmp_dir = "/tmp";
	shell->env_table = NULL;
	shell->env_array = NULL;
	shell->path_dirs = NULL;
	shell->input = NULL;
	return (shell);
}

static int	shell_dynamic_init(t_shell *shell, char **env)
{
	shell->session_arena = arena_init(SESSION_ARENA_SIZE);
	if (!shell->session_arena)
	{
		cleanup_shell_partial(shell, 1);
		return (0);
	}
	shell->command_arena = arena_init(COMMAND_ARENA_SIZE);
	if (!shell->command_arena)
	{
		cleanup_shell_partial(shell, 2);
		return (0);
	}
	shell->env_table = populate_env_from_envp(env, shell->session_arena);
	if (!shell->env_table)
	{
		cleanup_shell_partial(shell, 3);
		return (0);
	}
	shell->env_array = env_array_from_hashtable(shell);
	if (!shell->env_array)
	{
		cleanup_shell_partial(shell, 3);
		return (0);
	}
	return (1);
}

t_shell	*shell_init(char **env)
{
	t_shell	*shell;

	shell = shell_static_init();
	if (!shell)
		return (NULL);
	if (!shell_dynamic_init(shell, env))
		return (NULL);
	return (shell);
}
