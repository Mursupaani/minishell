/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external_command.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 17:10:23 by anpollan          #+#    #+#             */
/*   Updated: 2025/10/10 18:37:08 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*find_file_from_path(char *filename, t_shell *shell);
static char	*try_paths(char *filename, char **path_dirs, t_shell *shell);
static bool	is_file_path(char *arg);

void	execute_external_command(t_command *cmd, t_shell *shell)
{
	char	*executable_path;

	if (!shell || !cmd)
		exit(EXIT_FAILURE);
	if (cmd->redirections)
	{
		if (execute_redirection(cmd->redirections, cmd, shell) != 0)
			exit(shell->last_exit_status);
	}
	find_non_empty_argument(cmd);
	if (is_file_path(cmd->argv[0]))
		executable_path = cmd->argv[0];
	else
		executable_path = find_file_from_path(cmd->argv[0], shell);
	if (!executable_path)
		exit(shell->last_exit_status);
	if (!check_file_type_and_permissions(executable_path, shell))
		exit(shell->last_exit_status);
	if (execve(executable_path, cmd->argv, shell->env_array))
	{
		shell->last_exit_status = 127;
		ft_fprintf(STDERR_FILENO,
			"minishell: %s: %s\n", strerror(errno), cmd->argv[0]);
	}
	exit(shell->last_exit_status);
}

char	*find_file_from_path(char *filename, t_shell *shell)
{
	if (!filename)
	{
		ft_fprintf(STDERR_FILENO, "minishell: command '' not found\n");
		shell->last_exit_status = 127;
		return (NULL);
	}
	shell->path_dirs = ft_split_arena(hash_table_get(
				shell->env_table, "PATH"), ':', shell->command_arena);
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
		if (access(full_path, F_OK) == 0)
			return (full_path);
		path_dirs++;
	}
	ft_fprintf(STDERR_FILENO, "%s: command not found\n", filename);
	shell->last_exit_status = 127;
	return (NULL);
}

bool	check_file_type_and_permissions(char *filepath, t_shell *shell)
{
	struct stat	stats;

	if (stat(filepath, &stats) == -1)
	{
		shell->last_exit_status = 127;
		ft_fprintf(STDERR_FILENO, "minishell: %s: %s\n",
			filepath, strerror(errno));
		return (false);
	}
	if (stats.st_mode & S_IFDIR)
	{
		ft_fprintf(STDERR_FILENO, "minishell: %s: Is a directory\n", filepath);
		shell->last_exit_status = 126;
		return (false);
	}
	if (access(filepath, X_OK) == -1)
	{
		ft_fprintf(STDERR_FILENO, "minishell: %s: %s\n",
			filepath, strerror(errno));
		shell->last_exit_status = 126;
		return (false);
	}
	return (true);
}

static bool	is_file_path(char *arg)
{
	if (!arg)
		return (false);
	if (arg[0] == '.' || arg[0] == '/')
		return (true);
	while (*arg)
	{
		if (*arg == '/')
			return (true);
		arg++;
	}
	return (false);
}
