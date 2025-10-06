/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quick_sort_string_array.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 11:23:39 by anpollan          #+#    #+#             */
/*   Updated: 2025/10/06 12:14:17 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	partition(char **str_arr, int start, int end);
static void	swap_strings(char **str1, char **str2);

void	quick_sort_string_array(char **str_arr, int start, int end)
{
	int	pivot;

	if (start < end)
	{
		pivot = partition(str_arr, start, end);
		quick_sort_string_array(str_arr, start, pivot - 1);
		quick_sort_string_array(str_arr, pivot + 1, end);
	}
}

static int	partition(char **str_arr, int start, int end)
{
	char	*pivot;
	int		store_i;
	int		i;

	pivot = str_arr[end];
	store_i = start - 1;
	i = start;
	while (i < end)
	{
		if (ft_strcmp(str_arr[i], pivot) < 0)
		{
			store_i++;
			swap_strings(&str_arr[i], &str_arr[store_i]);
		}
		i++;
	}
	swap_strings(&str_arr[store_i + 1], &str_arr[end]);
	return (store_i + 1);
}

static void	swap_strings(char **str1, char **str2)
{
	char	*temp;

	temp = *str1;
	*str1 = *str2;
	*str2 = temp;
}
