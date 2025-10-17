/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 00:00:00 by magebreh          #+#    #+#             */
/*   Updated: 2025/10/16 00:00:00 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*strip_quotes(char *str, t_arena *arena)
{
	char	*res;
	char	*src;
	char	*dst;
	char	in_quote;

	if (!str)
		return (NULL);
	res = arena_alloc(arena, ft_strlen(str) + 1);
	if (!res)
		return (NULL);
	src = str;
	dst = res;
	in_quote = 0;
	while (*src)
	{
		if (*src == '"' && in_quote != '\'')
		{
			if (in_quote == '"')
				in_quote = 0;
			else if (!in_quote)
				in_quote = '"';
			src++;
		}
		else if (*src == '\'' && in_quote != '"')
		{
			if (in_quote == '\'')
				in_quote = 0;
			else if (!in_quote)
				in_quote = '\'';
			src++;
		}
		else
			*dst++ = *src++;
	}
	*dst = '\0';
	return (res);
}

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
						shell->command_arena);
				redir->target = strip_quotes(redir->target,
						shell->command_arena);
			}
			redir = redir->next;
		}
		expand_cmd(current, shell);
		current = current->next;
	}
}

static int	count_quote_aware_words(char *str)
{
	int		count;
	char	in_quote;
	bool	in_word;

	count = 0;
	in_quote = 0;
	in_word = false;
	while (*str)
	{
		if ((*str == '"' || *str == '\'') && !in_quote)
			in_quote = *str;
		else if (*str == in_quote)
			in_quote = 0;
		if (!ft_isspace(*str) && !in_word)
		{
			in_word = true;
			count++;
		}
		else if (ft_isspace(*str) && !in_quote && in_word)
			in_word = false;
		str++;
	}
	return (count);
}

static char	*extract_word(char *str, char **end)
{
	char	*start;
	char	in_quote;
	int		len;

	start = str;
	in_quote = 0;
	while (*str && (in_quote || !ft_isspace(*str)))
	{
		if ((*str == '"' || *str == '\'') && !in_quote)
			in_quote = *str;
		else if (*str == in_quote)
			in_quote = 0;
		str++;
	}
	len = str - start;
	*end = str;
	return (ft_substr(start, 0, len));
}

static char	**quote_aware_split(char *str)
{
	char	**result;
	int		count;
	int		i;

	if (!str)
		return (NULL);
	count = count_quote_aware_words(str);
	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (*str && i < count)
	{
		while (ft_isspace(*str))
			str++;
		if (*str)
			result[i++] = extract_word(str, &str);
	}
	result[i] = NULL;
	return (result);
}

static int	handle_split_expansion(t_command *cmd, t_shell *shell,
				int i, char *expanded)
{
	char	**split_result;
	char	**new_argv;
	bool	*new_expandable;
	int		split_count;
	int		total_args;
	int		k;

	split_result = quote_aware_split(expanded);
	if (!split_result || !split_result[0])
		return (0);
	split_count = 0;
	while (split_result[split_count])
		split_count++;
	total_args = 0;
	while (cmd->argv[total_args])
		total_args++;
	new_argv = arena_alloc(shell->command_arena,
			sizeof(char *) * (total_args + split_count));
	new_expandable = arena_alloc(shell->command_arena,
			sizeof(bool) * (total_args + split_count));
	ft_memcpy(new_argv, cmd->argv, sizeof(char *) * i);
	ft_memcpy(new_expandable, cmd->argv_expandable, sizeof(bool) * i);
	k = -1;
	while (++k < split_count)
	{
		new_argv[i + k] = arena_strdup(split_result[k], shell->command_arena);
		new_expandable[i + k] = false;
		free(split_result[k]);
	}
	free(split_result);
	ft_memcpy(new_argv + i + split_count, cmd->argv + i + 1,
		sizeof(char *) * (total_args - i));
	ft_memcpy(new_expandable + i + split_count, cmd->argv_expandable + i + 1,
		sizeof(bool) * (total_args - i));
	cmd->argv = new_argv;
	cmd->argv_expandable = new_expandable;
	return (split_count);
}

static bool	should_split(char *original, char *expanded)
{
	return (expanded && ft_strchr(expanded, ' ')
		&& ft_strchr(original, '$')
		&& !ft_strchr(original, '"') && !ft_strchr(original, '\''));
}

static void	handle_arg_expansion(t_command *cmd, t_shell *shell, int *i)
{
	char	*expanded;

	expanded = expand_var(cmd->argv[*i], shell, shell->command_arena);
	if (should_split(cmd->argv[*i], expanded))
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
					shell->command_arena);
		i++;
	}
}
