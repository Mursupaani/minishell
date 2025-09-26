/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_arena.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 10:40:34 by anpollan          #+#    #+#             */
/*   Updated: 2025/08/14 10:40:51 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_arena	*arena_init(size_t	capacity)
{
	t_arena	*arena;

	if (!capacity)
		return (NULL);
	arena = (t_arena *)ft_calloc(1, sizeof(t_arena));
	if (!arena)
		return (NULL);
	arena->data = (unsigned char *)ft_calloc(1, capacity);
	if (!arena->data)
	{
		free(arena);
		return (NULL);
	}
	arena->capacity = capacity;
	return (arena);
}

void	*arena_alloc(t_arena *arena, size_t size)
{
	void	*ptr;

	if (!arena || !size)
		return (NULL);
	if (arena->size + size > arena->capacity)
	{
		while (arena->next)
			arena = arena->next;
		if (size > arena->capacity)
			arena->next = arena_init(size);
		else
			arena->next = arena_init(arena->capacity);
		if (!arena->next)
			return (NULL);
		arena = arena->next;
	}
	ptr = (void *)arena->data + arena->size;
	arena->size += size;
	return (ptr);
}

void	arena_reset(t_arena *arena)
{
	if (!arena)
		return ;
	if (arena->next)
		arena_free(&arena->next);
	ft_memset(arena->data, 0, arena->capacity);
	arena->size = 0;
}

void	arena_free(t_arena **arena)
{
	t_arena	*temp;

	if (!*arena)
		return ;
	while (*arena)
	{
		if ((*arena)->data)
		{
			free((*arena)->data);
			(*arena)->data = NULL;
		}
		temp = (*arena)->next;
		free(*arena);
		*arena = NULL;
		*arena = temp;
	}
}
