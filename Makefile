# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: guisanto <guisanto@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/16 12:22:15 by guisanto          #+#    #+#              #
#    Updated: 2025/10/16 12:22:34 by guisanto         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Nome do programa final (o binário gerado)
NAME = pipex

CC = cc

# Diretório de includes (onde estão os arquivos de cabeçalho)
INCLUDES_DIR = includes

# Lista de arquivos de código fonte .c
SRCS = main.c \

# Diretório onde a biblioteca libft está localizada
LIBFT_DIR = libft

LIBFT = $(LIBFT_DIR)/libft.a

CFLAGS = -Wall -Werror -Wextra -g -I $(INCLUDES_DIR)

# Geração dos arquivos objetos a partir dos fontes .c
OBJS = $(SRCS:.c=.o)

# Regra padrão que vai gerar o programa final (push_swap)
# Esta regra será executada quando você rodar 'make' sem argumentos
all: $(NAME)

# Regra para criar o binário final (push_swap)
# Aqui, o binário é criado ao compilar os arquivos objetos e a libft.a
$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o pipex

# Regra para compilar a biblioteca libft, caso ela ainda não tenha sido compilada
# Esta regra é chamada automaticamente antes de compilar o programa principal, se necessário
$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)


# Regra para compilar qualquer arquivo .c em um arquivo objeto .o
# A regra é aplicada a todos os arquivos .c que não possuem um arquivo .o correspondente
%.o: %.c
	@$(CC) $(CFLAGS) -c -o $@ $^

# Regra para limpar os arquivos objetos (.o) gerados durante a compilação
# Quando 'make clean' for executado, os arquivos .o serão removidos
clean:
	@rm -f $(OBJS)
	@$(MAKE) clean -C $(LIBFT_DIR)

# Regra para limpar arquivos objetos e o binário final (push_swap)
# Quando 'make fclean' for executado, além dos objetos, o binário gerado será removido
fclean: clean
	@rm -f $(NAME)
	@$(MAKE) fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re  #o Makefile ignora nomes coincidirem com arquivos