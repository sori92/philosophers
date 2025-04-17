# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dsoriano <dsoriano@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/08 13:22:45 by carlosg2          #+#    #+#              #
#    Updated: 2025/04/17 13:40:31 by dsoriano         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=thread

SRCS = philo.c error_parse.c threads.c eat.c aux.c time_manage.c parca.c testing.c

OBJS = $(SRCS:.c=.o)

MAKEFLAGS += --silent

all: $(NAME)

$(NAME): $(OBJS)
	@echo "📢 Compiling sources 📦"
	@cc $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "📢 sources compiled successfully ✅"

clean:
	@echo "📢 Cleaning up .o archives 🧹"
	@rm -f $(OBJS)
	@echo "📢 .o archives cleaned up successfully ✅"

fclean: clean
	@echo "📢 Cleaning up philo archive 🗑️"
	@rm -f $(NAME)
	@echo "📢 philo cleaned up successfully ✅"

re: fclean all

.PHONY: all clean fclean re
