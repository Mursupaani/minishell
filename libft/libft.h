/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 14:33:26 by magebreh          #+#    #+#             */
/*   Updated: 2025/09/25 16:31:37 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <unistd.h>
# include <stdarg.h>
# include <stdint.h>
# include <stdio.h>
# include <limits.h>

# ifndef LOWERCASE
#  define LOWERCASE 0
# endif

# ifndef UPPERCASE
#  define UPPERCASE 1
# endif

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

typedef int	(*t_handler)(va_list *);
typedef struct s_specifier
{
	char		specifier;
	t_handler	handler;
}	t_specifier;

typedef int	(*t_fhandler)(va_list *, int);
typedef struct s_fspecifier
{
	char		specifier;
	t_fhandler	handler;
}	t_fspecifier;

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

typedef struct s_arena
{
	unsigned char	*data;
	size_t			capacity;
	size_t			size;
	struct s_arena	*next;
}	t_arena;

int		ft_atoi_safe(const char *str, int *err);
long	ft_atol_safe(const char *str, int *err);
int		ft_atoi_hex(const char *str, int *err);
void	ft_bzero(void *s, size_t n);
int		ft_isalnum(int s);
int		ft_isalpha(int s);
int		ft_isascii(int s);
int		ft_isdigit(int s);
int		ft_isprint(int s);
void	*ft_memchr(const void *s, int c, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	*ft_memmove(void *dest, const void *src, size_t n);
void	*ft_memset(void *s, int c, size_t n);
char	**ft_split(char const *s, char c);
char	*ft_strchr(const char *s, int c);
size_t	ft_strlcat(char *dst, const char *src, size_t dstsize);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
size_t	ft_strlen(const char *s);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strnstr(const char *haystack, const char *needle, size_t len);
char	*ft_strchr(const char *s, int c);
int		ft_tolower(int c);
int		ft_toupper(int c);
char	*ft_strrchr(const char *s, int c);
char	*ft_strtrim(char const *s1, char const *set);
char	*ft_substr(char const *s, unsigned int start, size_t len);
void	*ft_calloc(size_t count, size_t size);
char	*ft_strdup(const char *s1);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void	ft_putchar_fd(char c, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_striteri(char *s, void (*f)(unsigned int, char*));
void	ft_putnbr_fd(int n, int fd);
void	ft_putstr_fd(char *s, int fd);
char	*ft_itoa(int n);
int		count_digits(long n);
t_list	*ft_lstnew(void *content);
void	ft_lstadd_front(t_list **lst, t_list *newnode);
int		ft_lstsize(t_list *lst);
t_list	*ft_lstlast(t_list *lst);
void	ft_lstadd_back(t_list **lst, t_list *newnode);
void	ft_lstdelone(t_list *lst, void (*del)(void *));
void	ft_lstclear(t_list **lst, void (*del)(void *));
void	ft_lstiter(t_list *lst, void (*f)(void *));
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

/* ft_printf functions */
int		ft_printf(const char *format, ...);
int		print_char(va_list *args);
int		print_string(va_list *args);
int		print_int(va_list *args);
int		print_unsigned(va_list *args);
int		print_hex_upper(va_list *args);
int		print_hex_lower(va_list *args);
int		print_pointer(va_list *args);
char	*ft_utoa_hex(uintptr_t n, int is_uppercase);

// ft_fprintf functions
int		ft_fprintf(int file, const char *format, ...);
int		fprint_char(va_list *args, int file);
int		fprint_string(va_list *args, int file);
int		fprint_hex_lower(va_list *args, int file);
int		fprint_hex_upper(va_list *args, int file);
int		fprint_unsigned(va_list *args, int file);
int		fprint_pointer(va_list *args, int file);
int		fprint_int(va_list *args, int file);

/* get_next_line functions */
char	*get_next_line(int fd);
char	*ft_extract_line(char *s);
char	*ft_strjoin_free(char *s1, const char *s2);
char	*ft_read_to_buffer(int fd, char *leftovers, char *temp_buff);
void	ft_save_leftovers(char *buffer, char *leftovers);

// Memory arena functions
t_arena	*arena_init(size_t	capacity);
void	*arena_alloc(t_arena *arena, size_t size);
void	arena_reset(t_arena *arena);
void	arena_free(t_arena **arena);
char	**ft_split_arena(char const *s, char c, t_arena *arena);
char	*ft_substr_arena(
			char const *s, unsigned int start, size_t len, t_arena *arena);
char	*ft_strjoin_arena(char const *s1, char const *s2, t_arena *arena);

#endif
