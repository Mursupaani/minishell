/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_working_directory.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 15:59:58 by anpollan          #+#    #+#             */
/*   Updated: 2025/08/29 16:08:25 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_working_directory(t_shell *shell)
{
	char	*pwd;

	if (!getcwd(NULL, 0))
	{
		printf("%s\n", hash_table_get(shell->env_table, "PWD"));
		return ;
	}
	pwd = get_current_directory(shell);
	if (!pwd)
	{
		ft_fprintf(STDERR_FILENO, "minishell: pwd: ");
		ft_fprintf(STDERR_FILENO, "failed to get current directory\n");
		shell->last_exit_status = 1;
	}
	shell->last_exit_status = 0;
	printf("%s\n", pwd);
}

char	*get_current_directory(t_shell *shell)
{
	char	*pwd;

	pwd = arena_alloc(shell->command_arena, PWD_BUFFER);
	if (!pwd)
		return (NULL);
	if (!getcwd(pwd, PWD_BUFFER))
	{
		perror(strerror(errno));
		shell->last_exit_status = 1;
		return (NULL);
	}
	return (pwd);
}
