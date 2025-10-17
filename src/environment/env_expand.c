/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 00:00:00 by magebreh          #+#    #+#             */
/*   Updated: 2025/10/16 17:53:02 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expand_single_variable(char **src, t_shell *shell, char *dst)
{
	char		*var_start;
	char		*value;
	size_t		var_len;
	char		var_name[256];

	(*src)++;
	if (**src == '?')
	{
		//FIXME: Not allowed to use sprintf
		dst += sprintf(dst, "%d", shell->last_exit_status);
		(*src)++;
	}
	else
	{
		var_start = *src;
		while (**src && (ft_isalnum(**src) || **src == '_')
			&& **src != '\'' && **src != '"')
			(*src)++;
		var_len = *src - var_start;
		if (var_len == 0)
			*dst++ = '$';
		else
		{
			//FIXME: Not allowed to use strncpy
			strncpy(var_name, var_start, var_len);
			var_name[var_len] = '\0';
			value = hash_table_get(shell->env_table, var_name);
			if (value)
				//FIXME: Not allowed to use sprintf
				dst += sprintf(dst, "%s", value);
		}
	}
	return (dst);
}

static void	handle_quote(char **src, char *in_quote, char quote_char)
{
	if (*in_quote == quote_char)
		*in_quote = 0;
	else if (!*in_quote)
		*in_quote = quote_char;
	(*src)++;
}

static char	*process_char(char **src, char *dst, char in_quote, t_shell *shell)
{
	if (**src == '$' && (*(*src + 1) == '"' || *(*src + 1) == '\'') && !in_quote)
		(*src)++;
	else if (**src == '$' && in_quote != '\'' && *(*src + 1) != '\0')
	{
		if (in_quote && (*(*src + 1) == '\'' || *(*src + 1) == '"'))
			*dst++ = *(*src)++;
		else
			dst = expand_single_variable(src, shell, dst);
	}
	else
		*dst++ = *(*src)++;
	return (dst);
}

char	*expand_var(char *str, t_shell *shell, t_arena *arena)
{
	char		*res;
	char		*src;
	char		*dst;
	char		in_quote;

	if (!str)
		return (NULL);
	res = arena_alloc(arena, ft_strlen(str) * 10 + 1024);
	src = str;
	dst = res;
	in_quote = 0;
	while (*src)
	{
		if (*src == '"' && in_quote != '\'')
			handle_quote(&src, &in_quote, '"');
		else if (*src == '\'' && in_quote != '"')
			handle_quote(&src, &in_quote, '\'');
		else
			dst = process_char(&src, dst, in_quote, shell);
	}
	*dst = '\0';
	return (res);
}
