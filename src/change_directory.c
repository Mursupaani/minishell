/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_directory.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 12:06:30 by anpollan          #+#    #+#             */
/*   Updated: 2025/08/29 16:07:37 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	try_to_change_directory(t_command *cmd, t_shell *shell);

void	change_directory(t_command *cmd, t_shell *shell)
{
	int	status;

	//FIXME: User deleted parent folders?
	if (cmd->argv[2] != NULL)
	{
		ft_fprintf(STDERR_FILENO,
			"minishell: cd: too many arguments\n");
		shell->last_exit_status = 1;
		return ;
	}
	status = try_to_change_directory(cmd, shell);
	if (status == 0)
	{
		hash_table_set(shell->env_table, "PWD",
				 get_current_directory(shell), shell->session_arena);
		update_env_table_and_arr(shell);
	}
	else if (status == -1)
	{
		ft_fprintf(STDERR_FILENO, "cd: error retrieving current directory: ");
		ft_fprintf(STDERR_FILENO, "getcwd: cannot access parent directories: ");
		ft_fprintf(STDERR_FILENO, "No such file or directory\n");
	}
}

static int	try_to_change_directory(t_command *cmd, t_shell *shell)
{
	if (!cmd || ! shell)
		return (1);
	if (cmd->argv[1] == NULL
		|| ft_strncmp(cmd->argv[1], "~", ft_strlen(cmd->argv[1])) == 0)
	{
		if (chdir(hash_table_get(shell->env_table, "HOME")) == -1)
		{
			ft_fprintf(STDERR_FILENO, "minishell: cd: %s: %s\n",
				cmd->argv[1], strerror(errno));
			shell->last_exit_status = 1;
			return (1);
		}
	}
	else if (chdir(cmd->argv[1]) == -1)
	{
		ft_fprintf(STDERR_FILENO, "minishell: cd: %s: %s\n",
			cmd->argv[1], strerror(errno));
		shell->last_exit_status = 1;
		return (1);
	}
	if (!getcwd(NULL, 0))
		return (-1);
	return (0);
}
