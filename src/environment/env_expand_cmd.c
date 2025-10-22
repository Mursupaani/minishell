/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 00:00:00 by magebreh          #+#    #+#             */
/*   Updated: 2025/10/20 15:14:54 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	prepare_cmd(t_command *cmd, t_shell *shell)
{
	t_command	*current;
	t_redir		*redir;

	current = cmd;
	while (current)
	{
		redir = current->redirections;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC)
				redir->target = strip_quotes(redir->target,
						shell->command_arena);
			else
			{
				redir->target = expand_var(redir->target, shell,
						shell->command_arena, cmd);
				redir->target = strip_quotes(redir->target,
						shell->command_arena);
			}
			redir = redir->next;
		}
		expand_cmd(current, shell);
		current = current->next;
	}
}

static void	copy_split_to_argv(char **dst_argv, bool *dst_exp,
				char **split_result, t_arena *arena)
{
	int	k;

	k = 0;
	while (split_result[k])
	{
		dst_argv[k] = arena_strdup(split_result[k], arena);
		dst_exp[k] = false;
		free(split_result[k]);
		k++;
	}
	free(split_result);
}

static int	handle_split_expansion(t_command *cmd, t_shell *shell,
				int i, char *expanded)
{
	char	**split_result;
	char	**new_argv;
	bool	*new_expandable;
	int		counts[2];

	split_result = quote_aware_split(expanded);
	if (!split_result || !split_result[0])
		return (0);
	counts[0] = count_array(split_result);
	counts[1] = count_array(cmd->argv);
	new_argv = arena_alloc(shell->command_arena,
			sizeof(char *) * (counts[1] + counts[0]));
	new_expandable = arena_alloc(shell->command_arena,
			sizeof(bool) * (counts[1] + counts[0]));
	ft_memcpy(new_argv, cmd->argv, sizeof(char *) * i);
	ft_memcpy(new_expandable, cmd->argv_expandable, sizeof(bool) * i);
	copy_split_to_argv(new_argv + i, new_expandable + i, split_result,
		shell->command_arena);
	ft_memcpy(new_argv + i + counts[0], cmd->argv + i + 1,
		sizeof(char *) * (counts[1] - i));
	ft_memcpy(new_expandable + i + counts[0], cmd->argv_expandable + i + 1,
		sizeof(bool) * (counts[1] - i));
	cmd->argv = new_argv;
	cmd->argv_expandable = new_expandable;
	return (counts[0]);
}

static void	handle_arg_expansion(t_command *cmd, t_shell *shell, int *i)
{
	char	*expanded;
	char	*original;

	original = cmd->argv[*i];
	expanded = expand_var(original, shell, shell->command_arena, cmd);
	if (expanded && ft_strchr(expanded, ' ') && ft_strchr(original, '$')
		&& !ft_strchr(original, '"') && !ft_strchr(original, '\''))
	{
		*i += handle_split_expansion(cmd, shell, *i, expanded);
		(*i)--;
	}
	else if (expanded)
		cmd->argv[*i] = expanded;
}

void	expand_cmd(t_command *cmd, t_shell *shell)
{
	int		i;

	if (!cmd->argv)
		return ;
	i = 0;
	while (cmd->argv[i])
	{
		if (cmd->argv_expandable && cmd->argv_expandable[i])
			handle_arg_expansion(cmd, shell, &i);
		else
			cmd->argv[i] = expand_var(cmd->argv[i], shell,
					shell->command_arena, cmd);
		i++;
	}
}
