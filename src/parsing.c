/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 11:45:53 by anpollan          #+#    #+#             */
/*   Updated: 2025/08/22 11:46:27 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*parse_args(char *input, char **envp, t_arena *arena)
{
	t_command	*cmd;
	char		**argv;

	cmd = (t_command *)arena_alloc(arena, sizeof(t_command));
	if (!cmd)
		printf("arena fail cmd\n");
	// error handling
	argv = ft_split_arena(input, ' ', arena);
	if (!argv)
		printf("arena fail argv\n");
	cmd->argv = argv;
	cmd->envp = envp;
	return (cmd);
}
