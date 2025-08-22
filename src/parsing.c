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

t_command	*parse_args(char *input, char **envp)
{
	t_command	*cmd;
	char		**argv;

	cmd = (t_command *)ft_calloc(1, sizeof(t_command));
	// error handling
	argv = ft_split(input, ' ');
	cmd->argv = argv;
	cmd->envp = envp;
	return (cmd);
}
