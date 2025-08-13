# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/11 13:06:22 by anpollan          #+#    #+#              #
#    Updated: 2025/08/11 14:07:42 by anpollan         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= minishell
SRC_DIR		= ./src/
INCL_DIR	= ./incl/
SRC 		= ./src/main.c
OBJS 		= $(SRC:%.c=%.o)
HEADER		= ./incl/minishell.h
C_FLAGS		= -Wall -Wextra -Werror -pthread -g -I$(INCL_DIR)

all: $(NAME)

$(NAME): $(OBJS)
	cc $(C_FLAGS) $(OBJS) $(MLX) $(MLXFLAGS) -o $(NAME)

%.o: %.c $(HEADER)
	cc $(C_FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME) compile_commands.json

re: fclean all

.SECONDARY: $(OBJS)

.PHONY: all clean fclean re
