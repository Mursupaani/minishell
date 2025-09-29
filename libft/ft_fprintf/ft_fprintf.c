/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 10:08:47 by magebreh          #+#    #+#             */
/*   Updated: 2025/09/25 16:23:56 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	dispatch_specifier(char spec, va_list *args, int file)
{
	int					j;
	const t_fspecifier	specifiers[] = {
	{'c', fprint_char},
	{'s', fprint_string},
	{'p', fprint_pointer},
	{'d', fprint_int},
	{'i', fprint_int},
	{'u', fprint_unsigned},
	{'x', fprint_hex_lower},
	{'X', fprint_hex_upper},
	{'\0', NULL}
	};

	j = 0;
	while (specifiers[j].specifier)
	{
		if (specifiers[j].specifier == spec)
			return (specifiers[j].handler(args, file));
		j++;
	}
	write(file, &spec, 1);
	return (1);
}

static int	handle_format(int file, const char *format, va_list *args)
{
	int		i;
	int		count;
	int		written_bytes;

	i = 0;
	count = 0;
	while (format[i])
	{
		if (format[i] == '%')
		{
			written_bytes = dispatch_specifier(format[++i], args, file);
			if (written_bytes == -1)
				return (-1);
			count += written_bytes;
		}
		else
		{
			written_bytes = write(file, &format[i], 1);
			if (written_bytes == -1)
				return (-1);
			count += written_bytes;
		}
		i++;
	}
	return (count);
}

int	ft_fprintf(int file, const char *format, ...)
{
	int		count;
	va_list	args;

	if (format == NULL)
		return (-1);
	va_start(args, format);
	count = handle_format(file, format, &args);
	va_end(args);
	return (count);
}
