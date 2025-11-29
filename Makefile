# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: guisanto <guisanto@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/26 17:35:25 by guisanto          #+#    #+#              #
#    Updated: 2025/11/29 16:20:22 by guisanto         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = cc

# Include directory (including libft)
INCLUDES_DIR = library/libft

# Source files
SRCS = src/destroy.c \
       src/forks.c \
       src/init.c \
       src/monitor.c \
       src/philo.c \
       src/routine.c \
       src/time.c

# Libft directory and library
LIBFT_DIR = library/libft
LIBFT = $(LIBFT_DIR)/libft.a

CFLAGS = -Wall -Wextra -Werror -g -pthread -I $(INCLUDES_DIR)

# Objects in obj/ folder
OBJS = $(SRCS:src/%.c=obj/%.o)

# Terminal colors
GREEN = \033[0;32m
YELLOW = \033[0;33m
RED = \033[0;31m
RESET = \033[0m

# Compile source files into obj/
obj/%.o: src/%.c
	@mkdir -p obj
	@$(CC) $(CFLAGS) -c $< -o $@

# Default rule
all: $(NAME)
	@echo "$(GREEN)✔ philo compiled successfully!$(RESET)"

# Build binary
$(NAME): $(OBJS) $(LIBFT)
	@echo "$(YELLOW)Compiling philo...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)
	@echo "$(GREEN)✔ philo is ready!$(RESET)"

# Compile libft if needed
$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

# Clean object files
clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@rm -f $(OBJS)
	@$(MAKE) clean -C $(LIBFT_DIR)
	@echo "$(GREEN)✔ Objects cleaned!$(RESET)"

# Clean objects and binary
fclean: clean
	@echo "$(RED)Cleaning binary...$(RESET)"
	@rm -f $(NAME)
	@$(MAKE) fclean -C $(LIBFT_DIR)
	@echo "$(GREEN)✔ Everything cleaned!$(RESET)"

# Rebuild everything
re: fclean all
	@echo "$(YELLOW)♻ Full rebuild complete!$(RESET)"

.PHONY: all clean fclean re