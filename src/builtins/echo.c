/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 20:21:55 by anpollan          #+#    #+#             */
/*   Updated: 2025/08/29 16:08:57 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	print_argv(char **argv);
static int	check_flags(char **argv, bool *print_new_line);
static bool	arg_is_valid_flag(char *arg);

// NOTE: OK!
void	ft_echo(t_command *cmd, t_shell *shell)
{
	bool	print_new_line;
	int		offset;

	print_new_line = true;
	offset = check_flags(cmd->argv, &print_new_line);
	print_argv(&cmd->argv[offset]);
	if (print_new_line)
		printf("\n");
	shell->last_exit_status = 0;
}

static int	check_flags(char **argv, bool *print_new_line)
{
	int		i;

	i = 1;
	while (argv[i])
	{
		if (arg_is_valid_flag(argv[i]))
			*print_new_line = false;
		else
			break ;
		i++;
	}
	return (i);
}

static bool	arg_is_valid_flag(char *arg)
{
	int	i;

	if (!arg)
		return (false);
	if (ft_strncmp(arg, "-n", 2) != 0)
		return (false);
	i = 2;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

static int	print_argv(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		printf("%s", argv[i]);
		if (argv[++i])
			printf(" ");
	}
	return (0);
}
