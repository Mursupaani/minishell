/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_file_from_path.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 13:35:19 by anpollan          #+#    #+#             */
/*   Updated: 2025/09/11 12:13:14 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*try_paths(char *filename, char **path_dirs, t_shell *shell);

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
	perror(strerror(errno));
	return (NULL);
}
