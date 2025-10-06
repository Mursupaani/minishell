/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 11:29:56 by anpollan          #+#    #+#             */
/*   Updated: 2025/10/06 15:55:20 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_exit_val_from_args(t_command *cmd, t_shell *shell, int *val);

//NOTE: OK!
void	exit_builtin(t_command *cmd, t_shell *shell)
{
	int	val;
	int	error;

	if (!shell)
		return ;
	// printf("exit\n");
	if (cmd && cmd->argv[1])
	{
		error = get_exit_val_from_args(cmd, shell, &val);
		if (error)
			return ;
	}
	else
		val = shell->last_exit_status;
	free_memory_at_exit(shell);
	exit((uint8_t)val);
}

void	free_memory_at_exit(t_shell *shell)
{
	rl_clear_history();
	if (!shell)
		return ;
	if (shell->input)
		free(shell->input);
	if (shell->command_arena)
		arena_free(&shell->command_arena);
	if (shell->session_arena)
		arena_free(&shell->session_arena);
	free(shell);
}

int	error_exit_and_free_memory(t_shell *shell)
{
	if (!shell)
		exit(EXIT_FAILURE);
	free_memory_at_exit(shell);
	exit(EXIT_FAILURE);
}

static int	get_exit_val_from_args(t_command *cmd, t_shell *shell, int *val)
{
	int	error;

	*val = ft_atoi_safe(cmd->argv[1], &error);
	if (error)
	{
		ft_fprintf(STDERR_FILENO,
			"minishell: exit: %s: numeric argument required\n", cmd->argv[1]);
		*val = 2;
		return (0);
	}
	if (cmd->argv[2])
	{
		ft_fprintf(STDERR_FILENO, "minishell: exit: too many arguments\n");
		shell->last_exit_status = 1;
		return (1);
	}
	return (error);
}
