/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:40:00 by magebreh          #+#    #+#             */
/*   Updated: 2025/08/22 15:51:20 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_shell_partial(t_shell *shell, int level)
{
	if (level >= 3)
		arena_free(&shell->command_arena);
	if (level >= 2)
		arena_free(&shell->session_arena);
	if (level >= 1)
		free(shell);
}

t_hash_table	*populate_env_from_envp(char **envp, t_arena *arena)
{
	// TODO: Implement hash table population from environment variables
	// 1. Create hash function for environment variable keys
	// 2. Parse each envp entry (split on '=' character)
	// 3. Allocate t_env_entry structs using arena_alloc
	// 4. Store key/value pairs in hash table buckets
	// 5. Handle collision chaining with ->next pointers
	(void)envp;
	(void)arena;
	return (NULL);
}

t_shell	*shell_init(char **env)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->last_exit_status = 0;
	shell->interactive = isatty(STDIN_FILENO);
	shell->mode = MODE_PROMPT;
	shell->path_dirty = 1;
	shell->heredoc_counter = 0;
	shell->stdin_fd = STDIN_FILENO;
	shell->stdout_fd = STDOUT_FILENO;
	shell->stderr_fd = STDERR_FILENO;
	shell->tmp_dir = "/tmp";
	shell->env_table = NULL;
	shell->env_array = NULL;
	shell->path_dirs = NULL;
	shell->session_arena = arena_init(8192);
	if (!shell->session_arena)
	{
		cleanup_shell_partial(shell, 1);
		return (NULL);
	}
	shell->command_arena = arena_init(4096);
	if (!shell->command_arena)
	{
		cleanup_shell_partial(shell, 2);
		return (NULL);
	}
	shell->env_table = populate_env_from_envp(env, shell->session_arena);
	if (!shell->env_table)
	{
		cleanup_shell_partial(shell, 3);
		return (NULL);
	}
	return (shell);
}