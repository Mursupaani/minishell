/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables_from_input.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 13:54:02 by anpollan          #+#    #+#             */
/*   Updated: 2025/10/06 13:54:45 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_word(char **inputv, char *expanded, t_shell *shell);

char	*expand_variables_from_input(char *input, t_shell *shell)
{
	char	**inputv;
	char	*expanded;

	if (!input || !shell)
		return (NULL);
	inputv = split_input(input, shell);
	if (!inputv)
		return (NULL);
	free(input);
	expanded = ft_strdup("");
	handle_word(inputv, expanded, shell);
	return (expanded);
}

static void	handle_word(char **inputv, char *expanded, t_shell *shell)
{
	char	*temp;
	int		i;

	i = 0;
	temp = NULL;
	while (inputv[i])
	{
		if (inputv[i][0] == '$' && inputv[i][1] != '\0')
		{
			temp = hash_table_get(shell->env_table, &inputv[i][1]);
			if (temp)
				temp = ft_strjoin(expanded, temp);
			else
				temp = ft_strjoin(expanded, "");
		}
		else
			temp = ft_strjoin(expanded, inputv[i]);
		free(expanded);
		expanded = ft_strdup(temp);
		free(temp);
		i++;
	}
}
