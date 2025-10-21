/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:40:00 by magebreh          #+#    #+#             */
/*   Updated: 2025/10/20 19:12:41 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	is_builtin_command(char *cmd_name)
{
	return (ft_strncmp(cmd_name, "echo", 5) == 0 || ft_strncmp(cmd_name, "cd",
			3) == 0 || ft_strncmp(cmd_name, "pwd", 4) == 0
		|| ft_strncmp(cmd_name, "export", 7) == 0 || ft_strncmp(cmd_name,
			"unset", 6) == 0 || ft_strncmp(cmd_name, "env", 4) == 0
		|| ft_strncmp(cmd_name, "exit", 5) == 0);
}

int	is_parent_only_builtin(char *cmd_name)
{
	return (ft_strncmp(cmd_name, "cd", 3) == 0 || ft_strncmp(cmd_name, "export",
			7) == 0 || ft_strncmp(cmd_name, "unset", 6) == 0
		|| ft_strncmp(cmd_name, "exit", 5) == 0);
}
