# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dsoriano <dsoriano@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/08 13:22:45 by carlosg2          #+#    #+#              #
#    Updated: 2025/04/08 18:15:57 by dsoriano         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address

SRCS = philo.c error.c ft_atoi.c testing.c

OBJS = $(SRCS:.c=.o)

MAKEFLAGS += --silent

%.o: SRCS
	@echo "📢 Compiling objects..."
	@cc $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS)
	@echo "📢 Compiling sources 📦"
	@cc $(CFLAGS) $(OBJS) -o $(NAME)
	@echo $(SUCCESS) "📢 sources compiled successfully ✅"

clean:
	@echo "📢 Cleaning up .o archives 🧹"
	@rm -rf $(OBJS)
	@echo $(SUCCESS) "📢 .o archives cleaned up successfully ✅"

fclean: clean
	@echo "📢 Cleaning up philo archive 🗑️"
	@rm -rf $(NAME)
	@echo $(SUCCESS) "📢 philo cleaned up successfully ✅"

re: fclean all

.PHONY: all clean fclean re
