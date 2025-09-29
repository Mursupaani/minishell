/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external_command.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 17:10:23 by anpollan          #+#    #+#             */
/*   Updated: 2025/09/29 17:23:02 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*try_paths(char *filename, char **path_dirs, t_shell *shell);
static bool	check_file_type_and_permissions(char *filepath);

void	execute_external_command(t_command *cmd, t_shell *shell)
{
	char	*executable_path;

	if (cmd->argv[0][0] == '.' || cmd->argv[0][0] == '/')
		executable_path = cmd->argv[0];
	else
		executable_path = find_file_from_path(cmd->argv[0], shell);
	if (!executable_path)
		return ;
	if (cmd->redirections)
		execute_redirection(cmd->redirections, cmd, shell);
	if (!check_file_type_and_permissions(executable_path))
	if (execve(executable_path, cmd->argv, shell->env_array))
	{
		shell->last_exit_status = 127;
		ft_fprintf(STDERR_FILENO,
			"minishell: %s: %s\n", strerror(errno), cmd->argv[0]);
	}
}

char	*find_file_from_path(char *filename, t_shell *shell)
{
	if (!filename)
		return (NULL);
	shell->path_dirs = ft_split_arena(
		hash_table_get(shell->env_table, "PATH"), ':', shell->command_arena);
	if (!shell->path_dirs)
		return (NULL);
	return (try_paths(filename, shell->path_dirs, shell));
}

static char	*try_paths(char *filename, char **path_dirs, t_shell *shell)
{
	char	*full_path;

	if (!filename || !path_dirs || !shell)
		return (NULL);
	while (*path_dirs)
	{
		full_path = ft_strjoin_arena(*path_dirs, "/", shell->command_arena);
		full_path = ft_strjoin_arena(full_path, filename, shell->command_arena);
		// FIXME:  Need to check different permissions?
		if (access(full_path, F_OK) == 0)
			return (full_path);
		path_dirs++;
	}
	ft_fprintf(STDERR_FILENO, "%s: command not found\n", filename);
	shell->last_exit_status = 127;
	return (NULL);
}

static bool	check_file_type_and_permissions(char *filepath)
{
	if (!filepath)
		return (false);

	return (true);
}
