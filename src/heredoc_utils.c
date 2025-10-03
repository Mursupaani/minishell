/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 11:08:58 by anpollan          #+#    #+#             */
/*   Updated: 2025/10/03 11:54:17 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_input_segments(char *input);
static int	get_segment_size(char *input);
static void	copy_segment_to_vector(char *segment, char *input, int size);
static char	**process_input(char **inputv, char *input, t_shell *shell);

char	**split_input(char *input, t_shell *shell)
{
	int		segments;
	char	**inputv;

	if (!input || !shell)
		return (NULL);
	segments = count_input_segments(input);
	inputv = arena_alloc(shell->command_arena, sizeof(char *) * (segments + 1));
	if (!inputv)
		return (NULL);
	inputv = process_input(inputv, input, shell);
	if (!inputv)
		return (NULL);
	return (inputv);
}

static char	**process_input(char **inputv, char *input, t_shell *shell)
{
	int		segment_size;
	int		i;

	if (!input || !shell)
		return (NULL);
	i = 0;
	while (*input)
	{
		segment_size = get_segment_size(input);
		inputv[i] = arena_alloc(shell->command_arena, segment_size + 1);
		if (!inputv[i])
			return (NULL);
		copy_segment_to_vector(inputv[i++], input, segment_size);
		input += segment_size;
	}
	inputv[i] = NULL;
	return (inputv);
}

static int	count_input_segments(char *input)
{
	int	segments;

	if (!input)
		return (-1);
	segments = 0;
	while (*input)
	{
		if (ft_isspace(*input))
		{
			input++;
			segments++;
			while (ft_isspace(*input))
				input++;
		}
		else if ((ft_isprint(*input) && *input != ' ') || *input > 127)
		{
			input++;
			segments++;
			while (ft_isprint(*input) && *input != ' ' && *input != '$')
				input++;
		}
	}
	return (segments);
}

static int	get_segment_size(char *input)
{
	int	segment_size;

	segment_size = 0;
	if (ft_isspace(*input))
	{
		while (ft_isspace(*input))
		{
			input++;
			segment_size++;
		}
	}
	else if ((ft_isprint(*input) && *input != ' ') || *input > 127)
	{
		input++;
		segment_size++;
		while ((ft_isprint(*input) && *input != ' ' && *input != '$')
			|| *input > 127)
		{
			input++;
			segment_size++;
		}
	}
	return (segment_size);
}

static void	copy_segment_to_vector(char *segment, char *input, int size)
{
	int	i;

	if (!segment || !input)
		return ;
	i = 0;
	while (input[i] && i < size)
	{
		segment[i] = input[i];
		i++;
	}
	segment[i] = '\0';
}
