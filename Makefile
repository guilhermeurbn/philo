# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: guisanto <guisanto@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/26 17:35:25 by guisanto          #+#    #+#              #
#    Updated: 2025/11/27 16:36:47 by guisanto         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = cc

CFLAGS = -Wall -Wextra -Werror -pthread

OBJDIR = obj

SRCS = src/destroy.c \
		src/forks.c \
	   	src/init.c \
	   	src/monitor.c \
	   	src/philo.c \
	   	src/routine.c \
	   	src/time.c \

OBJS = $(SRCS:src/%.c=obj/%.o)

$(OBJDIR):
	mkdir -p $(OBJDIR)

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJDIR) $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re